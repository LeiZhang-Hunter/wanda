<?php
use Swoole\Coroutine\Http\Client;
$http = new Swoole\Http\Server('0.0.0.0', 9600);

$http->on('Request', function ($request, $response) {
$client = new Client('127.0.0.1', 80);
    $response->header('Content-Type', 'text/html; charset=utf-8');
    $response->end('<h1>Hello Swoole. #' . rand(1000, 9999) . '</h1>');
});

$http->start();