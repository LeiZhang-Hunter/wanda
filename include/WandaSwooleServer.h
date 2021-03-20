//
// Created by zhanglei on 2021/3/19.
//

#ifndef WANDA_WANDASWOOLESERVER_H
#define WANDA_WANDASWOOLESERVER_H

#include "WandaInterface.h"
/**
 * Wanda swooleServer的hook类
 */
namespace Wanda {
    static void ZEND_FASTCALL (*swooleHttpServerOn)(zend_execute_data *execute_data, zval *return_value) = nullptr;

    class WandaSwooleServer : public WandaInterface {
    public:

        /**
         * 协程server的hook
         * @param execute_data
         * @param return_value
         */
        static void wandaSwooleHttpServerHandler1(zend_execute_data *execute_data, zval *return_value);

        /**
         * 处理swooleServer的拦截
         * @return
         */
        bool executor() final;

    };
}

#endif //WANDA_WANDASWOOLESERVER_H
