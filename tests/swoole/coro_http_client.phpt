<?php

use Swoole\Coroutine\Http\Client;
use function Swoole\Coroutine\run;

run(function () {
    $cli = new Client('httpbin.org', 80);
    $cli->get('/get');
    echo $cli->body;
    $cli->close();
});