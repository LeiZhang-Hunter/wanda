//
// Created by zhanglei on 2021/3/18.
//

#ifndef WANDA_WANDACURL_H
#define WANDA_WANDACURL_H

#include "WandaInterface.h"
#include "WandaUtil.h"
/**
 * WandaCurl的hook函数
 */
namespace Wanda {

    //curl_init的Handler地址
    static void (*curlInitHandler)(zend_execute_data *execute_data, zval *return_value) = nullptr;
    //curl_setopt的handler地址
    static void (*curlSetoptionHandler)(zend_execute_data *execute_data, zval *return_value) = nullptr;

    class WandaCurl : WandaInterface {
    public:
        WandaCurl()
        {
        }
        static void wandaCurlInit(zend_execute_data *execute_data, zval *return_value);

        static void wandaCurlSetoption(zend_execute_data *execute_data, zval *return_value);

        //运行执行器
        bool executor() final;
        //curl_init的函数地址
        zend_function *curlInitFunction = nullptr;
        //curl_setopt的函数地址
        zend_function *curlSetoptionFunction = nullptr;


        //复原
        ~WandaCurl()
        {
//            if (curlInitHandler) {
//                curlInitFunction->internal_function.handler = curlInitHandler;
//            }
//
//            if (curlSetoptionHandler) {
//                curlSetoptionFunction->internal_function.handler = curlSetoptionHandler;
//            }
        }
    private:


    };
}

#endif //WANDA_WANDACURL_H
