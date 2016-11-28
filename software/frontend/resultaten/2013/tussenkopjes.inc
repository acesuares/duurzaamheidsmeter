<?php
 $tmp_kopje = explode(' ', $row->name);
 $tmp_kopje = substr($tmp_kopje[0], -2);
 if ($tmp_kopje != $prev_tmp_kopje) {
  $prev_tmp_kopje =  $tmp_kopje;
 $out .= "<tr>\n";
 $out .= "<td colspan='5' class=\"vl-tussenkopje\">";
 $out .= $tussenkopjes[$indicators[ $onderwerp ]->code][$tmp_kopje];
 $out .= "</td>\n";
 $out .= "</tr>\n";
}
?>
