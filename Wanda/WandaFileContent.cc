//
// Created by zhanglei on 2021/3/18.
//
extern "C"
{
#include <Zend/zend_types.h>
#include <php.h>
}

#include <string>
#include <iostream>
#include "GlobalWanda.h"
#include "WandaFileContent.h"

//拦截file_get_content
void Wanda::WandaFileContent::wandaFileGetContent(zend_execute_data *execute_data, zval *return_value) {
    char *filename;
    size_t filename_len;
    zend_string *contents;
    zend_bool use_include_path = 0;
    php_stream *stream;
    zend_long offset = -1;
    zend_long maxlen = PHP_STREAM_COPY_ALL;
    zval *zcontext = NULL;

    /* Parse arguments */
    if (zend_parse_parameters_ex(ZEND_PARSE_PARAMS_QUIET, ZEND_NUM_ARGS(), "p|br!ll", &filename, &filename_len, &use_include_path, &zcontext, &offset, &maxlen) == FAILURE) {
        fileGetContentHandler(execute_data, return_value);
        return;
    }

    std::string url(filename);

    //检查url是否是空的
    if (url.empty()) {
        fileGetContentHandler(execute_data, return_value);
        return;
    }

    //检查是否含有http
    if (url.find("http") == std::string::npos) {
        fileGetContentHandler(execute_data, return_value);
        return;
    }

    if (url.find(WANDA_G(wandaFirstSplit)) != std::string::npos) {
        //找到了要拼接?
        url += (WANDA_G(wandaThenSplit) + WANDA_G(wandaParam) + "=");
    } else {
        //找不到了要拼接&
        url += (WANDA_G(wandaFirstSplit) + WANDA_G(wandaParam) + "=");
    }

    //修改要传递进去的参数
    zval *param = ZEND_CALL_ARG(execute_data, 1);
    if (param) {
        ZVAL_STRING(param, url.c_str());
    }
    fileGetContentHandler(execute_data, return_value);
}

//设置执行器
bool Wanda::WandaFileContent::executor() {
    //hook掉curl_init
    fileGetContent = static_cast<zend_function *>(
            zend_hash_str_find_ptr(CG(function_table), "file_get_contents", strlen("file_get_contents")));

    if (fileGetContent) {
        fileGetContentHandler = fileGetContent->internal_function.handler;
        fileGetContent->internal_function.handler = wandaFileGetContent;
    }

    return true;
}