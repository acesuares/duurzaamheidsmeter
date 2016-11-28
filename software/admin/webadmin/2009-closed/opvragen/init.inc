<?

$WebEngine_version="2.64";

$WebEngine_host="localhost:/var/run/mysqld/mysqld.sock";

$WebEngine_database="la21nl2009";

$WebEngine_admin_user="RORub5io5u";

$WebEngine_admin_password="zu6Aeyiu";

$Webengine_include_path="/usr/local/webengine/" . $WebEngine_version . "/";

$Webengine_local_inc = $Webengine_include_path . "local.inc";

$dblink=mysql_connect($WebEngine_host,$WebEngine_admin_user,$WebEngine_admin_password);

?>