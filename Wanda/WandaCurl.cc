//
// Created by zhanglei on 2021/3/18.
//

extern "C"
{
#include <Zend/zend_types.h>
#include <php.h>
}

#include <string>

#include "GlobalWanda.h"
#include "WandaCurl.h"

//存在地址替换掉地址的值
void Wanda::WandaCurl::wandaCurlInit(zend_execute_data *execute_data, zval *return_value) {

    //修改要传递进去的参数
    if (!curlInitHandler) {
        return;
    }
    int number = EX_NUM_ARGS();
    //查看是否有url参数
    if (number != 1) {
        curlInitHandler(execute_data, return_value);
        return;
    }

    //修改要传递进去的参数
    zval *param = ZEND_CALL_ARG(execute_data, 1);

    if (Z_TYPE(*param) != IS_STRING) {
        curlInitHandler(execute_data, return_value);
        return;
    }

    std::string copyUrl = WANDA_G(util)->getWandaUrl(Z_STRVAL(*param));


    if (param) {
        ZVAL_STRING(param, copyUrl.c_str());
    }
    curlInitHandler(execute_data, return_value);
}

//设置curl的选项
void Wanda::WandaCurl::wandaCurlSetoption(zend_execute_data *execute_data, zval *return_value) {
    //修改要传递进去的参数
    if (!curlSetoptionHandler) {
        return;
    }

    zval *zid, *zvalue;
    zend_long options;

    ZEND_PARSE_PARAMETERS_START(3, 3)
            Z_PARAM_RESOURCE(zid)
            Z_PARAM_LONG(options)
            Z_PARAM_ZVAL(zvalue)
    ZEND_PARSE_PARAMETERS_END();

    if (options != 10002) {
        curlSetoptionHandler(execute_data, return_value);
        return;
    }

    std::string copyUrl = WANDA_G(util)->getWandaUrl(Z_STRVAL(*zvalue));

    //修改要传递进去的参数
    zval *param = ZEND_CALL_ARG(execute_data, 3);
    if (param) {
        ZVAL_STRING(param, copyUrl.c_str());
    }
    curlSetoptionHandler(execute_data, return_value);
}

bool Wanda::WandaCurl::executor() {
    //hook掉curl_init
    curlInitFunction = static_cast<zend_function *>(
            zend_hash_str_find_ptr(CG(function_table), "curl_init", strlen("curl_init")));

    if (curlInitFunction) {
        curlInitHandler = curlInitFunction->internal_function.handler;
        curlInitFunction->internal_function.handler = wandaCurlInit;
    }

    //hook掉curl_setoption
    curlSetoptionFunction = static_cast<zend_function *>(
            zend_hash_str_find_ptr(CG(function_table), "curl_setopt", strlen("curl_setopt")));

    if (curlSetoptionFunction) {
        curlSetoptionHandler = curlSetoptionFunction->internal_function.handler;
        curlSetoptionFunction->internal_function.handler = wandaCurlSetoption;
    }

    return true;
}