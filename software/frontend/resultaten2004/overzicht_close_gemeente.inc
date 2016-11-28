<?

//$corrected_totaal_score += $corrected_score;
// we need to close the previous gemeente
$html_out .= "&nbsp;</TD>";
$html_out .= t_spacer( 5, $regel_bgcolor );
if ( $spiegel != "indicator" ):
	$totaal_score_procent = round ( ( $totaal_score_procent / 3), 1 );
endif;

$html_out .= "<TD WIDTH=\"50\" ALIGN=\"RIGHT\" VALIGN=\"CENTER\"";
$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
$html_out .= $t_fontface[$spiegel];
$html_out .= $t_fontcolor[$spiegel];
$html_out .= $t_fontsize[$spiegel];
$html_out .= sprintf( "%01.1f%%", $totaal_score_procent );
$html_out .= "</TD>";

$html_out .= t_spacer(5,$regel_bgcolor);

$html_out .= "<TD WIDTH=\"110\" ALIGN=\"LEFT\" VALIGN=\"CENTER\"";
$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
if ( $spiegel != "provincie"):
	$html_out .= "<A HREF=\"$PHP_SELF?spiegel=provincie&choice=$previous_provincie_id\"";
	$html_out .= " STYLE=\"text-decoration: none\">";
endif;
$html_out .= $t_fontface[$spiegel];
$html_out .= $t_fontcolor[$spiegel];
$html_out .= $t_fontsize[$spiegel];
$html_out .= "<B>$previous_provincie</B>";
if ( $spiegel != "provincie"):
	$html_out .= "</A>";
endif;
$html_out .= "</TD>";

$html_out .= t_spacer(5,$regel_bgcolor);

$html_out .= "<TD WIDTH=\"90\" ALIGN=\"RIGHT\" VALIGN=\"CENTER\"";
$html_out .= " BGCOLOR=\"#$regel_bgcolor\">";
$html_out .= $t_fontface[$spiegel];
$html_out .= $t_fontcolor[$spiegel];
$html_out .= $t_fontsize[$spiegel];
$html_out .= "<B>".number_format ($previous_inwoneraantal, 0, ",", ".")."</B>";
$html_out .= "</TD>";

$html_out .= t_spacer(10,$regel_bgcolor);

$html_out .= "</TR>";
$html_out .= "</TABLE>";

// we have succesfully outputted this line, let's
// flip colours
$flipcolor_counter++;
$totaal_score_procent = 0;
?>