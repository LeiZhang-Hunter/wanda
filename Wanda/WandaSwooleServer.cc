//
// Created by zhanglei on 2021/3/18.
//
extern "C"
{
#include <Zend/zend_types.h>
#include <php.h>
#include <Zend/zend_interfaces.h>
}
#include <iostream>
#include "WandaSwooleCoroutine.h"
#include "WandaSwooleServer.h"
#include "WandaHook.h"

extern zend_class_entry *wanda_hook_ce;



void Wanda::WandaSwooleServer::wandaSwooleHttpServerHandler1(zend_execute_data *execute_data, zval *return_value)
{
    //替换swoole的defer函数
    zend_fcall_info fci;
    zend_fcall_info_cache fci_cache;

    //事件名字
    zval *eventName = ZEND_CALL_ARG(execute_data, 1);
    //修改要传递进去的参数
    zval *param = ZEND_CALL_ARG(execute_data, 2);

    if (!eventName) {
        swooleHttpServerOn(execute_data, return_value);
        return;
    }

    if (Z_TYPE(*eventName) != IS_STRING) {
        swooleHttpServerOn(execute_data, return_value);
        return;
    }

    //只是处理onrequest
    zend_string* eventNameString = zend_string_tolower(Z_STR(*eventName));
    if (strcasecmp(eventNameString->val, "request") != 0) {
        swooleHttpServerOn(execute_data, return_value);
        return;
    }

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
    ZVAL_STRING(&methodName, "httpproxy");
    zend_hash_index_add(HASH_OF(&wandaHook), 0, &object);
    zend_hash_index_add(HASH_OF(&wandaHook), 1, &methodName);

    zval_ptr_dtor(param);
    //替换参数
    ZVAL_DUP(param, &wandaHook);

    //继续执行原来的钩子
    swooleHttpServerOn(execute_data, return_value);

    //释放内存防止内存泄露
    zval_ptr_dtor(&wandaHook);
//    zval_ptr_dtor(param);
    return;
}

bool Wanda::WandaSwooleServer::executor() {
    //hook掉curl_init
    zend_class_entry *ce = static_cast<zend_class_entry *>(
            zend_hash_str_find_ptr(CG(class_table), "swoole\\http\\server", strlen("swoole\\http\\server")));

    if (!ce) {
        return false;
    }

    zend_function* function = static_cast<zend_function *>(zend_hash_str_find_ptr(&ce->function_table, "on",
                                                                                  strlen("on")));
    if (!function) {
        return false;
    }
    swooleHttpServerOn = function->internal_function.handler;
    function->internal_function.handler = wandaSwooleHttpServerHandler1;

    //hook掉curl_init
    ce = static_cast<zend_class_entry *>(
            zend_hash_str_find_ptr(CG(class_table), "swoole_http_server", strlen("swoole_http_server")));

    if (!ce) {
        return false;
    }
    function = static_cast<zend_function *>(zend_hash_str_find_ptr(&ce->function_table, "on",
                                                                                  strlen("on")));
    if (!function) {
        return false;
    }
    function->internal_function.handler = wandaSwooleHttpServerHandler1;

    return true;
}