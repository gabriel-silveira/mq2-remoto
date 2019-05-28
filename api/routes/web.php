<?php

$router->get('/', function () use ($router) {
    return $router->app->version();
});

$router->group(['prefix' => 'alert'], function () use ($router) {
    $router->get('/', function ()    {
        $res = \DB::connection('mysql')->select("SELECT alert FROM `mq2_alert` WHERE id = 1");
        return ['status'=>$res[0]->alert];
    });

    $router->get('/{status}', function ($status) {
        // atualiza status
        $affRows = \DB::connection('mysql')
            ->table('mq2_alert')
            ->where('id', 1)
            ->update([
                'alert' => $status
            ]);
        return ['updated'=>$affRows];
    });
});