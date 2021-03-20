//
// Created by zhanglei on 2021/3/19.
//

#ifndef WANDA_WANDASWOOLEHTTPCLIENT_H
#define WANDA_WANDASWOOLEHTTPCLIENT_H

#include "WandaInterface.h"
/**
 * wanda http客户端的hook类
 */
namespace Wanda {
    static void ZEND_FASTCALL (*swooleHttpClientHandler)(zend_execute_data *execute_data, zval *return_value) = nullptr;


    class WandaSwooleHttpClient : public WandaInterface {
    public:
        //hook掉swoole协程http的client
        static void wandaSwooleHandler(zend_execute_data *execute_data, zval *return_value);

        zend_function *swooleHttpClientFunction;

        bool executor() final;
    };
}
#endif //WANDA_WANDASWOOLEHTTPCLIENT_H
