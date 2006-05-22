<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html>
	<head>
		<script type="text/javascript" src="common.js"></script>
		<script type="text/javascript" src="lang_pack/english.js"></script>
		<script type="text/javascript" src="lang_pack/language.js"></script>
		<script type="text/javascript">document.write("<meta http-equiv=\"Content-Type\" content=\"application/xhtml+xml; charset=" + lang_charset.set + "\" />")</script>
		<title><% nvram_get("router_name"); %></title>
<!--	<link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style.css" />	-->
		<!--[if IE]><link type="text/css" rel="stylesheet" href="style/<% nvram_get("router_style"); %>/style_ie.css" /><![endif]-->

<!-- Botho 22/04 : css_include() and js_include() correct in a temporary way the loss of style and javascript includes -->
		<style type="text/css">
<% css_include(); %>
		</style>
		<script type="text/javascript">
<% js_include(); %>

var clk = <% get_clkfreq(); %>;
var rest_default = <% nvram_get("sv_restore_defaults"); %>;
my_tab = new getTimeOut(clk, rest_default);
var submit_button = "<% get_web_page_name(); %>";
var timer = setTimeout("message()", my_tab.wait_time * 1000);
var browserName=navigator.appName;

function to_submit()
{
	if(submit_button == ".asp")
		history.go(-1);
	else
		document.location.href =  submit_button;
}

function message()
{
	clearTimeout(timer);
	bar1.togglePause();
	setElementVisible("mess", true);
	if (browserName == "Microsoft Internet Explorer") {
		document.execCommand("Stop");
	}
	else {
		window.stop();
	}
}

function init()
{
	bar1.togglePause();
}

		</script>
	</head>
	
	<body onload="init()" onunload="clearTimeout(timer)">
		<div class="message">
			<div>
				<form name="success">
					<script type="text/javascript">Capture(success.upgrade)</script><br /><br />
					<div align="center">
						<script type="text/javascript">
							var bar1 = createBar(500,15,100,15,my_tab.scroll_count,"to_submit()");
							bar1.togglePause();
						</script>
	            	</div>
            		<div id="mess" style="display:none"><br /><br />
	            		<div style="text-align:left">
	            			<script type="text/javascript">
	            				if (rest_default == 1) {
	            					Capture(success.alert_reset);
	            				}
	            				Capture(success.alert1)
	            			</script>
	            			<ul>
	            				<li><script type="text/javascript">Capture(success.alert2)</script></li>
	            				<li><script type="text/javascript">Capture(success.alert3)</script></li>
	            			</ul>
	            		</div>
		            	<script type="text/javascript">
							document.write("<input type=\"button\" name=\"action\" value=\"" + sbutton.continu + "\" onclick=\"to_submit()\" />");
							if (browserName == "Microsoft Internet Explorer")
								document.write("<input type=\"button\" value=\"" + sbutton.clos + "\" onclick=\"opener=self;self.close();\" />");
						</script>
	            	</div>
				</form>
			</div>
		</div>
   </body>
</html>