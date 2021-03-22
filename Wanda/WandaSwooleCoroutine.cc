//
// Created by zhanglei on 2021/3/20.
//
extern "C" {
#include <php.h>
#include <Zend/zend_types.h>
#include <Zend/zend_interfaces.h>
}

#include <iostream>
#include "GlobalWanda.h"
#include "WandaHook.h"
#include "WandaSwooleCoroutine.h"

extern zend_class_entry *wanda_hook_ce;

long Wanda::WandaSwooleCoroutine::getCoroutineId()
{
    //没开启协程直接返回-1
    if (!WANDA_G(swoole_enable)) {
        return -1;
    }

    zval retValue;
    zval getCidFunctionName;
    //初始化getCid的名字
    ZVAL_STRING(&getCidFunctionName, "swoole\\coroutine::getcid");

    //获取当前的协程id
    call_user_function_ex(CG(function_table), nullptr, &getCidFunctionName, &retValue, 0,
            nullptr, 0, nullptr);
    zval_ptr_dtor(&getCidFunctionName);

    if (Z_TYPE(retValue) != IS_LONG) {
        return -1;
    }

    return Z_LVAL(retValue);
}

/**
 * swoole协程的派遣处理
 * @param execute_data
 * @param return_value
 * @param number
 */
void Wanda::WandaSwooleCoroutine::wandaHookDispatcher(zend_execute_data *execute_data, zval *return_value, int number)
{
    //替换swoole的defer函数
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    //修改要传递进去的参数
    zval *param = ZEND_CALL_ARG(execute_data, number);

    //拷贝原来的参数到钩子里
    zval object;
    object_init_ex(&object, wanda_hook_ce);

    //传到代理类
    zend_call_method(&object, wanda_hook_ce, nullptr, WANDA_OLD_HOOK,
                     strlen(WANDA_OLD_HOOK), nullptr, 1, param, nullptr);

    //创建一个回调函数
    zval wandaHook;
    zval methodName;
    array_init(&wandaHook);
    ZVAL_STRING(&methodName, "proxy");
    zend_hash_index_add(HASH_OF(&wandaHook), 0, &object);
    zend_hash_index_add(HASH_OF(&wandaHook), 1, &methodName);

    zval_ptr_dtor(param);
    //替换参数
    ZVAL_DUP(param, &wandaHook);

    //继续执行原来的钩子
    swooleCreatorHandler(execute_data, return_value);

    //释放内存防止内存泄露

//    zval_ptr_dtor(param);

    zval_ptr_dtor(&wandaHook);
//    zval_ptr_dtor(param);
}
void Wanda::WandaSwooleCoroutine::wandaCreatorHandler(zend_execute_data *execute_data, zval *return_value)
{
    std::shared_ptr<WandaSwooleCoroutine> wanDaCoroutine = std::make_shared<WandaSwooleCoroutine>();
    wanDaCoroutine->wandaHookDispatcher(execute_data, return_value, 1);
}

bool Wanda::WandaSwooleCoroutine::executor() {

    //hook掉swoole的协程创建函数
    swooleCreater = static_cast<zend_function *>(
            zend_hash_str_find_ptr(CG(function_table), "swoole_coroutine_create",
                    strlen("swoole_coroutine_create")));

    if (swooleCreater) {
        swooleCreatorHandler = swooleCreater->internal_function.handler;
        swooleCreater->internal_function.handler = wandaCreatorHandler;
    }

    swooleCreater = static_cast<zend_function *>(
            zend_hash_str_find_ptr(CG(function_table), "go", strlen("go")));

    if (swooleCreater) {
        swooleCreatorHandler = swooleCreater->internal_function.handler;
        swooleCreater->internal_function.handler = wandaCreatorHandler;
    }
    return true;
}