<?php
use Swoole\Coroutine\Http\Client;
$http = new Swoole\Http\Server('0.0.0.0', 9600);

$http->on('Request', function ($request, $response) {
    $cli = new Client('httpbin.org', 80);
    $cli->get('/get');
    $response->header('Content-Type', 'text/html; charset=utf-8');
    $response->end($cli->body);
    $cli->close();
});

$http->start();