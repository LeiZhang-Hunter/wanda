//
// Created by zhanglei on 2021/3/20.
//

#ifndef WANDA_WANDACOROUTINEHOOK_H
#define WANDA_WANDACOROUTINEHOOK_H
extern "C"
{
#include <php.h>
#include <Zend/zend_API.h>
};

#define WANDA_ORIGINAL_CLOSURE "wanda_original_closure"
#define WANDA_OLD_HOOK "setoldhook"

void load_wanda_hook();

PHP_METHOD(WandaHook, __construct);

PHP_METHOD(WandaHook, proxy);

PHP_METHOD(WandaHook, httpProxy);

PHP_METHOD(WandaHook, setOldHook);

PHP_METHOD(WandaHook, defer);

PHP_METHOD(WandaHook, __destruct);


#endif //WANDA_WANDACOROUTINEHOOK_H
