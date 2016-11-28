<?php
// licensed under GPL v2. (C) 2008 Ace Suares, Willemstad
	/*
		Copyright (C) 2008 Ace Suares, Gravenstraat 4, Otrobanda, Willemstad, Curaçao, Nehterlands Antilles
		http://www.suares.com
		
		This program is free software; you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation; either version 2 of the License, or
		(at your option) any later version.
		
		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.
		
		You should have received a copy of the GNU General Public License
		along with this program; if not, write to the Free Software
		Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	*/
// internationalize phone and faxnumbers
	if ( $lang == "nl" ):
		function make_inter( $number ){
			return $number;
		}
	else:
		function make_inter( $number ){
			if ( $number[ 0 ] == "0" ):
				$number = substr_replace( $number, "+31-", 0, 1 );
			endif;
			return $number;
		}
	endif;	
		
// translations and strings
	$translations = array();
	
	$translations[ "nl" ][ "Duurzaamheidsmeter" ] = "Duurzaamheidsmeter";
	$translations[ "en" ][ "Duurzaamheidsmeter" ] = "Sustainability Benchmark";

	$translations[ "nl" ][ "ja" ] = "ja";
	$translations[ "en" ][ "ja" ] = "yes";
	
	$translations[ "nl" ][ "nee" ] = "nee";
	$translations[ "en" ][ "nee" ] = "no";
	
	$translations[ "nl" ][ "onbekend" ] = "onbekend";
	$translations[ "en" ][ "onbekend" ] = "unknown";

	$translations[ "nl" ][ "home" ] = "Terug naar de Hoofdsite";
	$translations[ "en" ][ "home" ] = "Back to the Main Site";
	
	$translations[ "nl" ][ "views" ] = array( "uitleg", "gemeente", "provincie", "antwoorden", "ranglijst", "vragenlijst" );
	$translations[ "en" ][ "views" ] = array( "explanation", "municipality", "province", "answers", "rank", "questionaire" );
	
	$translations[ "nl" ][ "tooltip" ][ "uitleg" ] = "Korte introductie over het onderwerp";
	$translations[ "nl" ][ "tooltip" ][ "gemeente" ] = "Overzicht van prestaties van gemeenten via een kaart of een zoekfunctie op naam";
	$translations[ "nl" ][ "tooltip" ][ "provincie" ] = "Een kaart de gemiddelde prestatie van gemeenten per provincie";
	$translations[ "nl" ][ "tooltip" ][ "antwoorden" ] = "Per vraag een kaart van de antwoorden op dit thema";
	$translations[ "nl" ][ "tooltip" ][ "ranglijst" ] = "De totaalscores van gemeenten gerangschikt van hoog naar laag";
	$translations[ "nl" ][ "tooltip" ][ "vragenlijst" ] = "Met deze vragenlijst kunt u de gegevens per gemeente invoeren";
	
	$translations[ "en" ][ "tooltip" ][ "explanation" ] = "Korte introductie over het onderwerp";
	$translations[ "en" ][ "tooltip" ][ "municipality" ] = "Overzicht van prestaties van gemeenten via een kaart of een zoekfunctie op naam";
	$translations[ "en" ][ "tooltip" ][ "province" ] = "Een kaart de gemiddelde prestatie van gemeenten per provincie";
	$translations[ "en" ][ "tooltip" ][ "answers" ] = "Per vraag een kaart van de antwoorden op dit thema";
	$translations[ "en" ][ "tooltip" ][ "rank" ] = "De totaalscores van gemeenten gerangschikt van hoog naar laag";
	$translations[ "en" ][ "tooltip" ][ "questionaire" ] = "Met deze vragenlijst kunt u de gegevens per gemeente invoeren";
	
	$translations[ "nl" ][ "default_view" ] = "uitleg";
	$translations[ "en" ][ "default_view" ] = "explanation";
	
	$translations[ "nl" ][ "kiesgemeente" ] = "Kies hier een gemeente of klik op de kaart...";
	$translations[ "en" ][ "kiesgemeente" ] = "Choose a municipality or click on the map...";
	
	$translations[ "nl" ][ "kiesgemeente-invul" ] = "Kies een gemeente...";
	$translations[ "en" ][ "kiesgemeente-invul" ] = "Choose a municipality...";
	
	$translations[ "nl" ][ "kieskaart" ] = "Kies een vraag om de kaart te zien...";
	$translations[ "en" ][ "kieskaart" ] = "Choose a question to view the map...";
	
	$translations[ "nl" ][ "verwerk" ] = "verwerk";
	$translations[ "en" ][ "verwerk" ] = "process";
	
	$translations[ "nl" ][ "gemeente" ] = "gemeente";
	$translations[ "en" ][ "gemeente" ] = "municipality";

	$translations[ "nl" ][ "Gemeente" ] = "Gemeente";
	$translations[ "en" ][ "Gemeente" ] = "Municipality";
	
	$translations[ "nl" ][ "vraag" ] = "vraag";
	$translations[ "en" ][ "vraag" ] = "question";
	
	$translations[ "nl" ][ "uitleg" ] = "uitleg";
	$translations[ "en" ][ "uitleg" ] = "explanation";
	
	$translations[ "nl" ][ "provincie" ] = "provincie";
	$translations[ "en" ][ "provincie" ] = "province";
	
	$translations[ "nl" ][ "adres" ] = "adres";
	$translations[ "en" ][ "adres" ] = "address";
	
	$translations[ "nl" ][ "telefoonnummer" ] = "telefoon";
	$translations[ "en" ][ "telefoonnummer" ] = "phone";
	
	$translations[ "nl" ][ "faxnummer" ] = "fax";
	$translations[ "en" ][ "faxnummer" ] = "telefax";
	
	$translations[ "nl" ][ "e-mail" ] = "e-mail";
	$translations[ "en" ][ "e-mail" ] = "e-mail";
	
	$translations[ "nl" ][ "website" ] = "website";
	$translations[ "en" ][ "website" ] = "website";
	
	$translations[ "nl" ][ "inwoners" ] = "inwoners";
	$translations[ "en" ][ "inwoners" ] = "inhabitants";
	
	$translations[ "nl" ][ "contactpersoon" ] = "contactpersoon";
	$translations[ "en" ][ "contactpersoon" ] = "contact person";
	
	$translations[ "nl" ][ "geen" ] = "geen";
	$translations[ "en" ][ "geen" ] = "none";

	$translations[ "nl" ][ "indicator" ] = "indicator";
	$translations[ "en" ][ "indicator" ] = "indicator";

	$translations[ "nl" ][ "datafromyear" ] = "gegevens uit";
	$translations[ "en" ][ "datafromyear" ] = "data from";
	
	$translations[ "nl" ][ "score" ] = "score";
	$translations[ "en" ][ "score" ] = "score";
	
	$translations[ "nl" ][ "nr." ] = "nr.";
	$translations[ "en" ][ "nr." ] = "nr.";
	
	$translations[ "nl" ][ "antwoord" ] = "antwoord";
	$translations[ "en" ][ "antwoord" ] = "answer";
	
	$translations[ "nl" ][ "antwoorden" ] = "antwoorden";
	$translations[ "en" ][ "antwoorden" ] = "answers";

	$translations[ "nl" ][ "introductie" ] = "introductie";
	$translations[ "en" ][ "introductie" ] = "introduction";

	$translations[ "nl" ][ "correctiefactor" ] = "correctiefactor";
	$translations[ "en" ][ "correctiefactor" ] = "correction factor";
	
	$translations[ "nl" ][ "vragenlijst gemeente" ] = "antwoordenlijst gemeente";
	$translations[ "en" ][ "vragenlijst gemeente" ] = "questionaire for municipality";
	
	$translations[ "nl" ][ "Reden voor correctie" ] = "Reden voor correctie";
	$translations[ "en" ][ "Reden voor correctie" ] = "Reason for the correction";
	
	$translations[ "nl" ][ "deed niet mee met de  vragenlijst" ] = "deed niet mee met de  vragenlijst";
	$translations[ "en" ][ "deed niet mee met de  vragenlijst" ] = "did not participate in the questionaire";
	
	$translations[ "nl" ][ "gemiddelde score" ] = "gemiddelde<BR>score";
	$translations[ "en" ][ "gemiddelde score" ] = "average<BR>score";

	$translations[ "nl" ][ "deelname" ] = "deelname<BR>gemeenten";
	$translations[ "en" ][ "deelname" ] = "participation<BR>municipalities";

	$translations[ "nl" ][ "ranglijst" ] = "ranglijst";
	$translations[ "en" ][ "ranglijst" ] = "rank";
	
	$translations[ "nl" ][ "vragenlijst" ] = "vragenlijst";
	$translations[ "en" ][ "vragenlijst" ] = "questionaire";
	


?>