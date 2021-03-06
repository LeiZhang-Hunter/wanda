//
// Created by zhanglei on 2021/3/19.
//

extern "C"
{
#include <Zend/zend_API.h>
}

#include <string>
#include "GlobalWanda.h"

std::string Wanda::WandaUtil::getWandaUrl(const char* url) {
    //赋值
    std::string copyUrl = url;

    std::string wandaEnvValue;

    long cid = WandaSwooleCoroutine::getCoroutineId();
    if (cid == -1) {
        wandaEnvValue = WANDA_G(wandaEnvValue);
    } else {
        auto iter = WANDA_G(wandaCoroutine).find(cid);
        if (iter != WANDA_G(wandaCoroutine).end()) {
            wandaEnvValue = iter->second;
        }
    }

    //转换
    if (copyUrl.find(WANDA_G(wandaFirstSplit)) != std::string::npos) {
        //找到了要拼接?
        copyUrl += (WANDA_G(wandaThenSplit) + WANDA_G(wandaParam) + "=" + wandaEnvValue);
    } else {
        //找不到了要拼接&
        copyUrl += (WANDA_G(wandaFirstSplit) + WANDA_G(wandaParam) + "=" + wandaEnvValue);
    }

    return copyUrl;
}