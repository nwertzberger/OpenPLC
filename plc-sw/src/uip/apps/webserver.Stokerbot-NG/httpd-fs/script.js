document.write('<script type="text/javascript" src="/var.js"><\/script>'); 

function hideall()
{
	document.getElementById('main1').style.backgroundImage = "url('header.gif')";
	document.getElementById('main2').style.backgroundImage = "url('header.gif')";
	document.getElementById('main3').style.backgroundImage = "url('header.gif')";

	document.getElementById('sub2').style.display = 'none';
	document.getElementById('sub3').style.display = 'none';
}

function showOne(caller, menu)
{
	if (menu != null)
		document.getElementById(menu).style.display = 'block';
	caller.style.backgroundImage = "url('header_over.gif')";
}

function showMenu()
{
document.write('<div class="menu">');
document.write('<table border=0><tr><td><img src="stokerbotlogo.png" border=0></td><td>Firmware : NG '+FIRM_MAJOR+'.'+FIRM_MINOR+'<br>');
document.write('SystemID : '+SYSID+'<br>Hardware version: SBNG Small</td></tr></table>');
document.write('<div class="headerMenu" id="main1" style="float:left;width:200px;" onMouseOver="hideall();showOne(this, null);"><a href="index.htm">Sensor list</a></div>');
document.write('<div class="headerMenu" id="main2" style="float:left;width:200px;" onMouseOver="hideall();showOne(this, \'sub2\');">Configuration</div>');
document.write('<div class="headerMenu" id="main3" style="float:left;width:200px;" onMouseOver="hideall();showOne(this, \'sub3\');">Support</div>');
document.write('<br><br>');
document.write('<div class="subMenu" id="sub2" style="display:none">');
document.write('<a href="set_main.htm">General</a>');
document.write('<a href="set_io.htm">IO</a>');
document.write('<a href="set_alarm.htm">Alarms</a>');
document.write('<a href="set_net.htm">Network</a>');
document.write('<a href="set_lcd.htm">LCD</a>');
document.write('<a href="set_wireless.htm">Wireless</a>');
document.write('</div>');
document.write('<div class="subMenu" id="sub3" style="display:none">');
document.write('<a href="http://www.stokerbot.dk" target="_blank">Stokerbot.dk</a>');
document.write('<a href="http://www.stokerlog.dk" target="_blank">Stokerlog.dk</a>');
document.write('<a href="mailto:post@minigrossisten.dk">Email</a>');
document.write('</div>');
document.write('</div>');
}








//V3.01.A - http://www.openjs.com/scripts/jx/
jx={getHTTPObject:function(){var A=false;if(typeof ActiveXObject!="undefined"){try{A=new ActiveXObject("Msxml2.XMLHTTP")}catch(C){try{A=new ActiveXObject("Microsoft.XMLHTTP")}catch(B){A=false}}}else{if(window.XMLHttpRequest){try{A=new XMLHttpRequest()}catch(C){A=false}}}return A},load:function(url,callback,format){var http=this.init();if(!http||!url){return }if(http.overrideMimeType){http.overrideMimeType("text/xml")}if(!format){var format="text"}format=format.toLowerCase();var now="uid="+new Date().getTime();url+=(url.indexOf("?")+1)?"&":"?";url+=now;http.open("GET",url,true);http.onreadystatechange=function(){if(http.readyState==4){if(http.status==200){var result="";if(http.responseText){result=http.responseText}if(format.charAt(0)=="j"){result=result.replace(/[\n\r]/g,"");result=eval("("+result+")")}if(callback){callback(result)}}else{if(error){error(http.status)}}}};http.send(null)},init:function(){return this.getHTTPObject()}}
