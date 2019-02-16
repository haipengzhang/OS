<?php
/*
 * Project: Magoo
 * Engineer: Makin
 * Time: 2014-6-28 23:47:15
 */
class controller extends core{
	protected $page;
	private $zpage;
	public function __construct() {
		parent::config();
	}
	#过滤IP
	protected function filter(){
		//获取客户端IP
		function get_ip(){
			$unknown = 'unknown'; 
			if (isset($_SERVER['HTTP_X_FORWARDED_FOR']) && $_SERVER['HTTP_X_FORWARDED_FOR'] && strcasecmp($_SERVER['HTTP_X_FORWARDED_FOR'], $unknown)) { 
				$ip = $_SERVER['HTTP_X_FORWARDED_FOR']; 
			} 
			elseif(isset($_SERVER['REMOTE_ADDR']) && $_SERVER['REMOTE_ADDR'] && strcasecmp($_SERVER['REMOTE_ADDR'], $unknown)) { 
				$ip = $_SERVER['REMOTE_ADDR']; 
			} 
			if (false !== strpos($ip, ',')) $ip = reset(explode(',', $ip)); 
			return $ip; 
		}
		$ip = get_ip(); // 获取IP
		if(strpos($ip,'121.42.0.')>-1){//判断IP
			$this->location($this->config->filter);
		}
	}
    public function CurlAction($url,$header=0){
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_URL, $url);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);  
        curl_setopt($ch, CURLOPT_BINARYTRANSFER, true);
        curl_setopt($ch, CURLOPT_FOLLOWLOCATION, 1);
        curl_setopt($ch, CURLOPT_HEADER,$header);
        $content=curl_exec($ch);
		$data=$header?curl_getinfo($ch):$content;
        curl_close($ch);
        return $data;
    }
	public function CurlPost($url,$post=''){
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL,$url) ;  
		curl_close($ch);
		return $content;
	}
	public function fsopen_post($url,$post='',$cookie='',$timeout=1) {
        $matches = parse_url($url);
		$p=$matches['port']?$matches['port']:80;
		$host=$matches['host'].($matches['port']?':'.$matches['port']:'');		
		$path=$matches['path'].($matches['query']?'?'.$matches['query']:'');
        $out = "POST {$path} HTTP/1.0 \r\n";
        $out .= "Accept: */* \r\n";
        $out .= "Accept-Language: zh-cn \r\n";
        $out .= "Content-Type: application/x-www-form-urlencoded \r\n";
        $out .= "User-Agent: $_SERVER[HTTP_USER_AGENT] \r\n";
        $out .= "Host: $host \r\n";
        $out .= 'Content-Length: '.strlen($post)."\r\n";
        $out .= "Connection: Close \r\n";
        $out .= "Magoo: 1 \r\n";
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
	protected function alert($str=''){
		echo "<script>alert('$str');self.location.href=document.referrer;</script>";
	}
	protected function load($path,$data=false){
		$file=$_SERVER['DOCUMENT_ROOT'].'/'.$this->config->path.'/'.$this->config->view.$path;
		is_file($file)?include $file:exit($file."\n".$this->lang()->view_exists);
	}
	#判断是否有值，可选为判断类型，第三个可选参数是是否结束程序
	protected function End($object,$type=false,$exit=true){
		$array=array('number'=>is_numeric($object),'array'=>is_array($object),'object'=>is_object($object),'bool'=>is_bool($object),'strinig'=>is_string($object));
		if($object===false||($type&&!$array[$type])){return $exit?exit('e'):false;}
		return $object;
	}
	#不适合where筛选 即不能用在模糊搜索上
	protected function get_limit_page($page,$limitnum,$rows,$maxpage=30,$desc=true){
		$ceil=ceil($rows/$limitnum);
		$zpage=$ceil<$maxpage?$ceil:$maxpage;
        if($page>$zpage||!$page){$page=1;}
        if(!preg_match('/^\d{1,3}$/',$page)){
            $this->notfound();
        }
		if(!preg_match('/\d+/', $page)){return false;}
		
		$limit=($page-1)*$limitnum;
		if($desc){
			$descnum=$rows-$limit;
			$overnum=$descnum-$limitnum+1;
			$between=($overnum>0?$overnum:1).' and '.$descnum;
		}else{
			$startnum=$limit+1;
			$overnum=$startnum+$limitnum-1;
			$between=$startnum.' and '.($overnum<$rows?$overnum:$rows);
		}
		$this->page=$page;
        $this->zpage=$zpage;
		return (object)array(
			'between'=>$between
			,'rows'=>$rows
			,'page'=>$page
			,'zpage'=>$zpage			
			,'limit'=>$limit
			,'limitnum'=>$limitnum
		);
	}
	protected function get_page($param=false){
		$url=($param?'?'.$param.'&':'?').'page';
		if(($this->page-3)>'0'){$echo.='<a href="'.$url.'=1">首页</a>';}
		if(($this->page-1)>'0'){$echo.=' <a href="'.$url.'='.($this->page-1).'">上一页</a>';}
		if(($this->page-3)>'0'){$echo.=' <a href="'.$url.'='.($this->page-3).'">'.($this->page-3).'</a>';}
		if(($this->page-2)>'0'){$echo.=' <a href="'.$url.'='.($this->page-2).'">'.($this->page-2).'</a>';}
		if(($this->page-1)>'0'){$echo.=' <a href="'.$url.'='.($this->page-1).'">'.($this->page-1).'</a>';}
		if(($this->page+1)<=$this->zpage||($this->page-1)>'0'){$echo.=' <b>'.$this->page.'</b>';}
		if(($this->page+1)<=$this->zpage){$echo.=' <a href="'.$url.'='.($this->page+1).'">'.($this->page+1).'</a>';}
		if(($this->page+2)<=$this->zpage){$echo.=' <a href="'.$url.'='.($this->page+2).'">'.($this->page+2).'</a>';}
		if(($this->page+3)<=$this->zpage){$echo.=' <a href="'.$url.'='.($this->page+3).'">'.($this->page+3).'</a>';}
		if(($this->page+1)<=$this->zpage){$echo.=' <a href="'.$url.'='.($this->page+1).'">下一页</a>';}
		return $echo;
	}
}