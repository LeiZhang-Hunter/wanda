//
// Created by zhanglei on 2021/3/20.
//

#ifndef WANDA_WANDASWOOLECOROUTINE_H
#define WANDA_WANDASWOOLECOROUTINE_H

#include "WandaInterface.h"
/**
 * wanda的swoole协程处理器
 */
namespace Wanda {
    static void (*swooleCreatorHandler)(zend_execute_data *execute_data, zval *return_value) = nullptr;

    class WandaSwooleCoroutine : public WandaInterface {
    public:
        /**
         * 在swoole协程中加入过滤
         * @param execute_data
         * @param return_value
         */
        static void wandaCreatorHandler(zend_execute_data *execute_data, zval *return_value);

        void wandaHookDispatcher(zend_execute_data *execute_data, zval *return_value, int number);

        //获取协程id
        static long getCoroutineId();

        /**
         * 启动处理器
         * @return
         */
        bool executor() final;

        //swoole的defer函数
        zend_function *swooleCreater = nullptr;

    };
}

#endif //WANDA_WANDASWOOLECOROUTINE_H
