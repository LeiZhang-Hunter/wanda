//
// Created by zhanglei on 2021/3/18.
//

#ifndef WANDA_WANDAFILECONTENT_H
#define WANDA_WANDAFILECONTENT_H

#include "WandaInterface.h"
/**
 * Wanda file_get_content的hook类
 */
namespace Wanda {

    //file_get_content的地址
    static void (*fileGetContentHandler)(zend_execute_data *execute_data, zval *return_value) = nullptr;

    class WandaFileContent : public Wanda::WandaInterface {
    public:
        bool executor() final;

        static void wandaFileGetContent(zend_execute_data *execute_data, zval *return_value);

        //curl_init的函数地址
        zend_function *fileGetContent = nullptr;
    };
}

#endif //WANDA_WANDAFILECONTENT_H
