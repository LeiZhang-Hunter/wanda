//
// Created by zhanglei on 2021/3/19.
//

#ifndef WANDA_WANDASWOOLESERVER_H
#define WANDA_WANDASWOOLESERVER_H
#include "WandaInterface.h"
/**
 * Wanda swooleServer的hook类
 */
namespace Wanda
{
    class WandaSwooleServer : public WandaInterface
    {
    public:
        bool executor() final;
    };
}

#endif //WANDA_WANDASWOOLESERVER_H
