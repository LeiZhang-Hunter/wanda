//
// Created by zhanglei on 2021/3/18.
//

#ifndef WANDA_WANDA_H
#define WANDA_WANDA_H

#include <memory>

#include "WandaCurl.h"
#include "WandaFileContent.h"
#include "WandaSwooleServer.h"
#include "WandaSwooleHttpClient.h"

namespace Wanda {
    class ZendWanda : public WandaInterface {
    public:
        ZendWanda()
                : wandaCurl(std::make_shared<Wanda::WandaCurl>()),
                  wandaFileContent(std::make_shared<Wanda::WandaFileContent>()),
                  wandaSwooleServer(std::make_shared<Wanda::WandaSwooleServer>()),
                  wandaHttpClient(std::make_shared<Wanda::WandaSwooleHttpClient>()) {

        }

        /**
         * 执行器入口
         * @return
         */
        bool executor() final {
            wandaCurl->executor();
            wandaFileContent->executor();
            wandaSwooleServer->executor();
            wandaHttpClient->executor();
            return true;
        }

        ~ZendWanda() {

        }

    private:
        std::shared_ptr<Wanda::WandaCurl> wandaCurl;
        std::shared_ptr<Wanda::WandaFileContent> wandaFileContent;
        std::shared_ptr<Wanda::WandaSwooleServer> wandaSwooleServer;
        std::shared_ptr<Wanda::WandaSwooleHttpClient> wandaHttpClient;
    };
}

#endif //WANDA_WANDA_H
