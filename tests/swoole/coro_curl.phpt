<?php
use Swoole\Coroutine\Http\Client;
$http = new Swoole\Http\Server('0.0.0.0', 9600);

$http->on('Request', function ($request, $response) {
    $string = "httpbin.org/get";
    $ch = curl_init();
    curl_setopt($ch,CURLOPT_URL, $string);
    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    $r = curl_exec($ch);
    var_dump($r);
    curl_close($ch);

    $response->header('Content-Type', 'text/html; charset=utf-8');
    $response->end($r);
});

$http->start();