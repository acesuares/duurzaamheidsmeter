<?
	
// close indicator van gemeente-vragen

 
if ( ${$current_code . "_datafromyear_id"} == $current_datafromyear_id ):
	
	$html_out .= "<TR HEIGHT=\"28\">";
	
	$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
	
	$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>Score</B>";
	$html_out .= "</TD>";
	
	
	$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
	$html_out .= $sorteerbalk_fontface[$spiegel];
	$html_out .= $sorteerbalk_fontcolor[$spiegel];
	$html_out .= $sorteerbalk_fontsize[$spiegel];
	$html_out .= "<B>$this_indicator_score</B>";
	$html_out .= "</TD>";
	
	
	$html_out .= "</TR>\n";
	
	// correctiefactor
	
	$query  = "SELECT ";
	$query .= " CF.correctie_factor AS correctie_factor, ";
	$query .= " CF.correctie_reden AS correctie_reden ";
	
	$query .= "FROM ";
	$query .= " correctie CF ";
	
	$query .= " WHERE ";
	$query .= " CF.gemeente_id = '$choice' ";
	$query .= " AND CF.indicator_id= '$current_indicator' ";
	
	$corr_result=mysql($WebEngine_database,$query);
	
	$corr_row = mysql_fetch_assoc($corr_result);
	
	// get values
	foreach ($corr_row as $key => $value){
		$$key=$value;
	}
	
	if ($correctie_factor != 0 ):
	
		$totaal_score += $correctie_factor;
	
		$html_out .= "<TR HEIGHT=\"28\">";
	
		$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
	
		$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= $sorteerbalk_fontsize[$spiegel];
		$html_out .= "<B>Correctiefactor</B>";
		$html_out .= "</TD>";
	
	
		$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= $sorteerbalk_fontsize[$spiegel];
		$html_out .= "<B>$correctie_factor</B>";
		$html_out .= "</TD>";
	
		$html_out .= "</TR>\n";
	
	endif;
	
	if ($correctie_reden != ""):
	
		$html_out .= "<TR>";
	
		$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
	
		$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= $sorteerbalk_fontsize[$spiegel];
		$html_out .= "<B>Toelichting: $correctie_reden</B>";
		$html_out .= "</TD>";
	
	
		$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= $sorteerbalk_fontsize[$spiegel];
		$html_out .= "&nbsp;";
		$html_out .= "</TD>";
	
		$html_out .= "</TR>\n";
	
	endif;
	
	if ($correctie_factor != 0):
	
		$html_out .= "<TR HEIGHT=\"28\">";
	
		$html_out .= t_spacer(20,$sorteerbalk_bgcolor[$spiegel]);
	
		$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= $sorteerbalk_fontsize[$spiegel];
		$html_out .= "<B>Gecorrigeerde Score</B>";
		$html_out .= "</TD>";
	
		$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
		$html_out .= " BGCOLOR=\"#".$sorteerbalk_bgcolor[$spiegel]."\">";
		$html_out .= $sorteerbalk_fontface[$spiegel];
		$html_out .= $sorteerbalk_fontcolor[$spiegel];
		$html_out .= $sorteerbalk_fontsize[$spiegel];
		$html_out .= "<B>".($correctie_factor + $this_indicator_score)."</B>";
		$html_out .= "</TD>";
	
		$html_out .= "</TR>\n";
	
	endif;
	
	// spacer
	
	$html_out .= "<TR HEIGHT=\"28\">";
	
	$html_out .= t_spacer(20,"FFFFFF");
	
	$html_out .= "<TD COLSPAN=3 ALIGN=LEFT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#FFFFFF\">";
	$html_out .= "&nbsp;";
	$html_out .= "</TD>";
	
	
	$html_out .= "<TD ALIGN=RIGHT VALIGN=CENTER";
	$html_out .= " BGCOLOR=\"#FFFFFF\">";
	$html_out .= "&nbsp;";
	$html_out .= "</TD>";
	
	
	$html_out .= "</TR>\n";

endif;
$geen_gegevens = "";



?>