中文 

## 关于PHPWanda
是一个有关php隔离测试环境的扩展

作者：LeiZhang-Hunter
- 底层HOOK：无侵入，hook了底层的curl系列函数和file_get_contents
- 支持swoole协程、支持fpm，支持阻塞模式,支持swoole的Http的server和client

## 如何安装PHPWanda
```
git git@github.com:LeiZhang-Hunter/wanda.git
phpize
./configure
make
make install
```

