<?

$WebEngine_version="2.64";

$WebEngine_host="127.0.0.1:3306";

$WebEngine_database="la21nl2007";

$WebEngine_admin_user="ROEva1Oove";

$WebEngine_admin_password="PhaiH3ia";

$Webengine_include_path="/usr/local/webengine/" . $WebEngine_version . "/";

$Webengine_local_inc = $Webengine_include_path . "local.inc";

$dblink=mysql_connect($WebEngine_host,$WebEngine_admin_user,$WebEngine_admin_password);

?>