<?php

use Swoole\Coroutine\Http\Client;
use function Swoole\Coroutine\run;
class A{
};
$a = new A();
run(function () use ($a){
    $cli = new Client('httpbin.org', 80);
    $cli->recv(null);
    echo $cli->body;
    $cli->close();
});