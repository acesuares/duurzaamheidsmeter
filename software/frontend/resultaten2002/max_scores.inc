<?
	
// We need the maximum score for each indicator to identify what 100% is

$max_query  = "SELECT ";
$max_query .= " sum(V.score) AS max_score, ";
$max_query .= " I.title as indicator, ";
$max_query .= " I.id as indicator_id, ";
$max_query .= " I.plaatje as plaatje ";
$max_query .= " FROM ";
$max_query .= " vraag V, indicator I ";
$max_query .= " WHERE ";
$max_query .= " V.active='Y' AND ";
$max_query .= " V.indicator_id = I.id AND ";
$max_query .= " I.id <> 8 AND "; // HOME
$max_query .= " I.id <> 10"; // extra water
$max_query .= " GROUP BY V.indicator_id";

$totaal_max_score = 0;
$max_score_result=mysql($WebEngine_database,$max_query);
while($max_score_row = mysql_fetch_array($max_score_result)):
	$indicator             = stripslashes($max_score_row["indicator"]);
	$max_score[$indicator] = stripslashes($max_score_row["max_score"])+0; 
	$totaal_max_score += $max_score[$indicator];
	$score_gif[$indicator] = "/images/" . basename(stripslashes($max_score_row["plaatje"])); 
endwhile;

?>
