<?php
function Post($url,$post='',$cookie='',$timeout=1) {
        $matches = parse_url($url);
		$p=$matches['port']?$matches['port']:80;
		$host=$matches['host'].($matches['port']?':'.$matches['port']:'');		
		$path=$matches['path'].($matches['query']?'?'.$matches['query']:'');
        $out = "POST {$path} HTTP/1.1 \r\n";
        $out .= "Accept: */* \r\n";
        $out .= "Accept-Language: zh-cn \r\n";
        $out .= "Content-Type: application/x-www-form-urlencoded \r\n";
        $out .= "User-Agent: $_SERVER[HTTP_USER_AGENT] \r\n";
        $out .= "Host: {$host} \r\n";
        $out .= 'Content-Length: '.strlen($post)."\r\n";
        $out .= "Connection: Close \r\n";
        $out .= "Cookie: $cookie \r\n\r\n";	
        $out .= $post;
        $socket = @fsockopen($host,$p,$errno,$errstr,$timeout);	
		$header = $data = "";
		if($socket){
			fwrite($socket,$out);
			while($infos = trim(fgets($socket,1024))) {
					$header.=$infos;
			}
			while(!feof($socket)) {
					$data .= fgets($socket,1024);
			}
			fclose($socket);
		}
		else{
			$data='e';
		}
        return $data;
}