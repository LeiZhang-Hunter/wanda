//
// Created by zhanglei on 2021/3/19.
//
extern "C"
{
#include <php.h>
#include <Zend/zend_API.h>
#include <Zend/zend_string.h>
#include <Zend/zend_modules.h>
}

#include "GlobalWanda.h"
#include "ZendWanda.h"

bool Wanda::ZendWanda::startFpmMode()
{
    WANDA_G(fpm_mode) = true;
    return true;
}

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

    if (running) {
        return true;
    }

    //加载fpm的启动参数
    if (WANDA_G(fpm_mode)) {
        if (!loadFpmParam()) {
            return false;
        }
    }

    zend_string* extension_name = zend_string_init("swoole", strlen("swoole"), 0);

    //检查swoole扩展是否存在
    if (zend_hash_exists(&module_registry, extension_name)) {
        WANDA_G(swoole_enable) = true;
        //hook coro
        wandaSwooleCoroutine->executor();

        //hook swoole server
        wandaSwooleServer->executor();

        //hook httpClient
        wandaHttpClient->executor();
    }

    //hook curl
    wandaCurl->executor();

    //hook file_get_contents
    wandaFileContent->executor();

    running = true;
    return true;
}