//
// Created by zhanglei on 2021/3/19.
//

#ifndef WANDA_WANDASWOOLEHTTPCLIENT_H
#define WANDA_WANDASWOOLEHTTPCLIENT_H
#include "WandaInterface.h"
/**
 * wanda http客户端的hook类
 */
namespace Wanda
{
    class WandaSwooleHttpClient : public WandaInterface
    {
    public:
        bool executor() final;
    };
}
#endif //WANDA_WANDASWOOLEHTTPCLIENT_H
