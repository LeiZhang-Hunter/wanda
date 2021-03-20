//
// Created by zhanglei on 2021/3/18.
//

#ifndef PHPSCITER_GLOBALWANDA_H
#define PHPSCITER_GLOBALWANDA_H

#include <memory>
#include <unordered_map>
#include "WandaUtil.h"
#include "ZendWanda.h"

typedef struct _zend_wanda_globals {
    bool swoole_enable = false;
    bool fpm_mode = false;
    std::string wandaEnvValue = "default";
    std::string wandaParam = "WandaVirtualEnv";
    //首字符
    std::string wandaFirstSplit = "?";
    //后续分隔符
    std::string wandaThenSplit = "&";
    std::shared_ptr<Wanda::ZendWanda> wanda;
    std::shared_ptr<Wanda::WandaUtil> util;
    //记录协程里的wanda环境信息
    std::unordered_map<long, std::string> wandaCoroutine;
} zend_wanda_globals;

extern ZEND_DECLARE_MODULE_GLOBALS(wanda)

#ifdef ZTS
# define WANDA_G(v) TSRMG(wanda_globals_id, zend_wanda_globals *, v)
#else
# define WANDA_G(v) (wanda_globals.v)
#endif

#endif //PHPSCITER_GLOBALWANDA_H
