//
// Created by zhanglei on 2021/3/18.
//

extern "C"
{
#include <Zend/zend_types.h>
#include <php.h>
}

#include <iostream>
#include "GlobalWanda.h"
#include "WandaSwooleHttpClient.h"

void Wanda::WandaSwooleHttpClient::wandaSwooleHandler(zend_execute_data *execute_data, zval *return_value) {
    char *path = nullptr;
    size_t path_len = 0;

    ZEND_PARSE_PARAMETERS_START(1, 1)
            Z_PARAM_STRING(path, path_len)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    std::string copyUrl = WANDA_G(util)->getWandaUrl(path);
    //修改要传递进去的参数
    zval *param = ZEND_CALL_ARG(execute_data, 1);
    if (param) {
        ZVAL_STRING(param, copyUrl.c_str());
    }
    swooleHttpClientHandler(execute_data, return_value);
}

void Wanda::WandaSwooleHttpClient::wandaSwoolePostHandler(zend_execute_data *execute_data, zval *return_value) {
    char *path = nullptr;
    size_t path_len = 0;
    zval *post_data;

    ZEND_PARSE_PARAMETERS_START(2, 2)
            Z_PARAM_STRING(path, path_len)
            Z_PARAM_ZVAL(post_data)
    ZEND_PARSE_PARAMETERS_END_EX(RETURN_FALSE);

    std::string copyUrl = WANDA_G(util)->getWandaUrl(path);
    //修改要传递进去的参数
    zval *param = ZEND_CALL_ARG(execute_data, 1);
    if (param) {
        ZVAL_STRING(param, copyUrl.c_str());
    }
    swooleHttpClientPostHandler(execute_data, return_value);
}

bool Wanda::WandaSwooleHttpClient::executor() {
    //由于swoole只有一个协程客户端所以我们只要简单的hook一个就可以了
    zend_class_entry *ce = static_cast<zend_class_entry *>(
            zend_hash_str_find_ptr(CG(class_table), "swoole\\coroutine\\http\\client",
                                   strlen("swoole\\coroutine\\http\\client")));

    //没找到这个扩展，不要再继续处理了
    if (!ce) {
        return false;
    }
    zend_function *function = static_cast<zend_function *>(zend_hash_str_find_ptr(&ce->function_table, "get",
                                                                                  strlen("get")));
    if (!function) {
        return false;
    }

    swooleHttpClientHandler = function->internal_function.handler;
    function->internal_function.handler = wandaSwooleHandler;

    function = static_cast<zend_function *>(zend_hash_str_find_ptr(&ce->function_table, "post",
                                                                                  strlen("post")));
    if (!function) {
        return false;
    }

    swooleHttpClientPostHandler = function->internal_function.handler;
    function->internal_function.handler = wandaSwoolePostHandler;
    return true;
}