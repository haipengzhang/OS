<?php
/*
 * Project: Web Get
 * Engineer: Makin
 * Time: 2014-5-5 10:09:38
 * 
 */
class WebGet{
    public $content;
    public $headerhost;
    public $cookie;
    public function CurlGet($url){
        $this->headerhost?$header[]='Host: '.$this->headerhost:false;
        $this->cookie?$header[]='Cookie: '.$this->cookie:false;
        $header[]='User-Agent:Mozilla/5.0 (Windows NT 8.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/34.0.1847.131 Safari/537.36';
        $ch = curl_init($url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true) ; // 获取数据返回  
        curl_setopt($ch, CURLOPT_BINARYTRANSFER, true) ; // 在启用 CURLOPT_RETURNTRANSFER 时候将获取数据返回
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1); //可以跟随跳转之后抓取
        curl_setopt($ch, CURLOPT_HTTPHEADER, $header);
        $this->content=curl_exec($ch);
        curl_close($ch);
    }
    public function PreGetOne($pattern,$subject=false){
        preg_match('/'.$pattern.'/isU',  $subject?$subject:$this->content,$return);
        return $return;
    }
    public function PreGetAll($pattern,$subject=false){
        preg_match_all('/'.$pattern.'/isU',$subject?$subject:$this->content,$return);
        return $return;
    }
    public function GbkUtf8($string) {
        return iconv('GBK','UTF-8//IGNORE',$string);
    }
    public function EndStr1($string,$needle) {
        return substr(strrchr($string,$needle),1);
    }
}