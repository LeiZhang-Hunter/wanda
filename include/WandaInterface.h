//
// Created by zhanglei on 2021/3/19.
//

#ifndef WANDA_WANDAINTERFACE_H
#define WANDA_WANDAINTERFACE_H

/**
 * 抽象接口类
 */
namespace Wanda
{
    class WandaInterface
    {
    public:
        WandaInterface()
        {

        }
        virtual bool executor() = 0;

        ~WandaInterface()
        {

        }
    };
}
#endif //WANDA_WANDAINTERFACE_H
