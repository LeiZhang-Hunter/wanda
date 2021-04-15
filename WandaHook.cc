//
// Created by zhanglei on 2021/3/20.
//
#include <Zend/zend_API.h>

#include "WandaHook.h"
#include "GlobalWanda.h"
const zend_function_entry wanda_hooks_method[] =
        {
                PHP_ME(WandaHook, __construct,  NULL, ZEND_ACC_PUBLIC | ZEND_ACC_CTOR)
                PHP_ME(WandaHook, __destruct,   NULL, ZEND_ACC_PUBLIC | ZEND_ACC_DTOR)
                PHP_ME(WandaHook, setOldHook,      NULL, ZEND_ACC_PUBLIC)
                PHP_ME(WandaHook, proxy,      NULL, ZEND_ACC_PUBLIC)
                PHP_ME(WandaHook, httpProxy,      NULL, ZEND_ACC_PUBLIC)
                PHP_ME(WandaHook, defer,     NULL, ZEND_ACC_PUBLIC)

                {
                        NULL, NULL, NULL
                }
        };

zend_class_entry *wanda_hook_ce;

PHP_METHOD(WandaHook, __construct)
{

}

PHP_METHOD(WandaHook, setOldHook)
{
    //将闭包进行复制
    zval* hook;
    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_ZVAL(hook)
    ZEND_PARSE_PARAMETERS_END();

    //设置代理属性
    zend_update_property(wanda_hook_ce, getThis(), WANDA_ORIGINAL_CLOSURE,
            strlen(WANDA_ORIGINAL_CLOSURE), hook);
}

/**
 * 代理swoole协程的hook，在这创建defer回收资源，
 * @param execute_data
 * @param return_value
 */
PHP_METHOD(WandaHook, proxy)
{
    //协程id
    long coroutineId = Wanda::WandaSwooleCoroutine::getCoroutineId();
    if (coroutineId == -1) {
        return;
    }

    WANDA_G(wandaCoroutine)[coroutineId] = WANDA_G(wandaEnvValue);

    //创建defer
    zval function_name;
    ZVAL_STRING(&function_name, "defer");

    zval* originalHook = zend_read_property(wanda_hook_ce, getThis(),
            WANDA_ORIGINAL_CLOSURE, strlen(WANDA_ORIGINAL_CLOSURE), 0, nullptr);

    //组成hook defer的名字
    zval wandaHookDefer;
    zval retValue;
    array_init(&wandaHookDefer);
    zval_add_ref(getThis());
    zend_hash_index_add(HASH_OF(&wandaHookDefer), 0, getThis());
    zend_hash_index_add(HASH_OF(&wandaHookDefer), 1, &function_name);
    zval realParams[1];
    realParams[0] = wandaHookDefer;

    //注入Wanda的defer
    call_user_function(CG(function_table), nullptr, &function_name,
            &retValue,
            1, realParams);

    if (originalHook && (Z_TYPE(*originalHook) != IS_NULL)) {
        int paramCount = ZEND_CALL_NUM_ARGS(execute_data);
        zval return_result;
        zval* args = ZEND_CALL_VAR_NUM(execute_data, 0);
        //执行原有的逻辑
        call_user_function_ex(EG(function_table), nullptr, originalHook,&return_result,
                              paramCount,
                           args, 0, nullptr);
    }


    zval_ptr_dtor(&wandaHookDefer);
}

/**
 * 代理swoole协程的hook，在这创建defer回收资源，
 * @param execute_data
 * @param return_value
 */
PHP_METHOD(WandaHook, httpProxy)
{
    //协程id
    long coroutineId = Wanda::WandaSwooleCoroutine::getCoroutineId();

    //检查get参数是否有分离的参数
    zval* requestObject = ZEND_CALL_ARG(execute_data, 1);
    if (Z_TYPE(*requestObject) == IS_OBJECT) {
        zval* getObject = zend_read_property(Z_OBJCE(*requestObject), requestObject, "get", strlen("get"), 0, nullptr);
        if (getObject && (Z_TYPE(*getObject) == IS_ARRAY)) {
            zval* wandaValue = zend_hash_str_find(HASH_OF(getObject), WANDA_G(wandaParam).c_str(), WANDA_G(wandaParam).length());
            if (wandaValue && (Z_TYPE(*wandaValue) == IS_STRING)) {
                if (coroutineId != -1) {
                    WANDA_G(wandaCoroutine)[coroutineId] = Z_STRVAL(*wandaValue);
                } else {
                    WANDA_G(wandaEnvValue) = Z_STRVAL(*wandaValue);
                }
            }
        }
    }

    if (coroutineId != -1) {
        WANDA_G(wandaCoroutine)[coroutineId] = WANDA_G(wandaEnvValue);

        //创建defer
        zval function_name;
        ZVAL_STRING(&function_name, "defer");


        //组成hook defer的名字
        zval wandaHookDefer;
        zval retValue;
        array_init(&wandaHookDefer);
        zval_add_ref(getThis());
        zend_hash_index_add(HASH_OF(&wandaHookDefer), 0, getThis());
        zend_hash_index_add(HASH_OF(&wandaHookDefer), 1, &function_name);
        zval realParams[1];
        realParams[0] = wandaHookDefer;

        //注入Wanda的defer
        call_user_function(CG(function_table), nullptr, &function_name,
                           &retValue,
                           1, realParams);

        zval_ptr_dtor(&wandaHookDefer);
    }

    zval* originalHook = zend_read_property(wanda_hook_ce, getThis(),
                                            WANDA_ORIGINAL_CLOSURE, strlen(WANDA_ORIGINAL_CLOSURE), 0, nullptr);

    if (originalHook && (Z_TYPE(*originalHook) != IS_NULL)) {
        int paramCount = ZEND_CALL_NUM_ARGS(execute_data);
        zval return_result;
        zval* args = ZEND_CALL_VAR_NUM(execute_data, 0);
        //执行原有的逻辑
        call_user_function_ex(EG(function_table), nullptr, originalHook,&return_result,
                              paramCount,
                              args, 0, nullptr);
    }
}

/**
 * 回收资源
 * @param execute_data
 * @param return_value
 */
PHP_METHOD(WandaHook, defer)
{
    //协程id
    long coroutineId = Wanda::WandaSwooleCoroutine::getCoroutineId();
    if (coroutineId == -1) {
        RETURN_TRUE
    }
    auto iterator = WANDA_G(wandaCoroutine).find(coroutineId);
    if (iterator != WANDA_G(wandaCoroutine).end()) {
        WANDA_G(wandaCoroutine).erase(iterator);
    }
    RETURN_TRUE
}

/**
 * 析构修复
 * @param execute_data
 * @param return_value
 */
PHP_METHOD(WandaHook, __destruct)
{

}

void load_wanda_hook()
{
    zend_class_entry wanda_hook_entry;

    INIT_CLASS_ENTRY(wanda_hook_entry, "WandaHook", wanda_hooks_method);

    wanda_hook_ce = zend_register_internal_class_ex(&wanda_hook_entry, nullptr);

    //更新属性用来存储旧有的钩子
    zend_declare_property_null(wanda_hook_ce, ZEND_STRL(WANDA_ORIGINAL_CLOSURE),
            ZEND_ACC_PRIVATE TSRMLS_CC);
}