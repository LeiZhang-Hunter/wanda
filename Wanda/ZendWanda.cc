//
// Created by zhanglei on 2021/3/19.
//
extern "C"
{
#include <php.h>
#include <Zend/zend_API.h>
#include <Zend/zend_string.h>
}

#include "GlobalWanda.h"
#include "ZendWanda.h"

bool Wanda::ZendWanda::loadFpmParam() {
    //获取get参数
    //initialize $_GET $_POST  $_REQUEST $_SERVER
    if (PG(auto_globals_jit)) {
        if (zend_is_auto_global_str("_GET", sizeof("_GET") - 1) == FAILURE) {
            return false;
        }
    }

    HashTable * data = HASH_OF(&PG(http_globals)[TRACK_VARS_GET]);
    if (!data) {
        return false;
    }
    //查找全局上的值
    zval *wandaEnvValue = zend_hash_str_find(data, WANDA_G(wandaParam).c_str(),
                                             WANDA_G(wandaParam).length());

    //查找wandaEnvValue
    if (!wandaEnvValue) {
        return false;
    }

    if (Z_TYPE(*wandaEnvValue) != IS_STRING) {
        return false;
    }

    WANDA_G(wandaEnvValue) = Z_STRVAL(*wandaEnvValue);
    return true;
}

bool Wanda::ZendWanda::executor() {

    //加载fpm的启动参数
    if (fpmMode) {
        if (!loadFpmParam()) {
            return false;
        }
    }

    if (running) {
        return true;
    }

    wandaCurl->executor();
    wandaFileContent->executor();
    wandaSwooleServer->executor();
    wandaHttpClient->executor();

    running = true;
    return true;
}