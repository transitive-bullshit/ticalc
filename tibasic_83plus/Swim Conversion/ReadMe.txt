------ Swim Conversion ------
By:  Travis Fischer (Fisch2)
phishfud@elknet.net
March 14, 2005


OVERVIEW:
1) Introduction
2) Requirements
3) Instructions
4) Credits
5) Disclaimer


=============
INTRODUCTION
=============

This is a swim conversion utility tool which will convert boys or girls race times from 
yards to meters or from meters to yards.  You may find it useful for converting times 
between different pool types.  My math teacher, Mr. Butters (who is also a swim coach), 
thought this program would be useful and asked me to port and soup-up his TI-82 version 
to make it easier to access.  I really don't know much about these conversions or all of 
the potential uses for them since I'm personally not into swimming -- I'm a wrestler :-)
Anyway, one thing I am good at is coding, so I hope you find this program well done and 
useful.


==============
REQUIREMENTS
==============

1) Send the SWIMCONV.83p file to your calculator
2) Run the SWIMCONV program (listed under the PRGM key)

Compatibility Notes:

* Supported calculators include:  TI-83, TI-83+, TI-83+ Silver Edition, and the TI-84 Plus/SE
* This program is written in TI-Basic and is compatible with MirageOS


=============
INSTRUCTIONS
=============

Running the program will bring you to the main menu screen, where you will have the option of 
choosing the type of conversion you would like to perform.  The '->' means 'convert to', so 
"BOYS -> METERS" would mean that you want to convert a boys yard pool time to meters.  After 
selecting the type of conversion, you must pick the event (use the arrow keys to scroll events 
and the Enter key or the 2nd key to confirm).  Below is a list of available events to choose 
from.  While at the Pick Event screen, you may press Clear to return to the Main Menu or 
choose the 'Exit' option to exit the program.  After selecting an event, you will be prompted 
for the race time in the format 'Minutes:Seconds.hundredths'.  The interpretation/parsing of 
how you enter the time is fairly leniant, so you can enter 0:48.2 or 48.2.  To find the ':' 
colon symbol, press ALPHA+'.'  (aka press the period key with ALPHA turned on).  If the colon 
is emitted, the program will assume the time entered is in seconds.  Also, it is not necessary 
to include the hundredths colum, so entering a time such as 48 would be the same as entering 
48.0 .  The resulting time, when converted according to your specifications, will be displayed 
below in the same 'Minutes:Seconds.hundredths' format.

Available Events:
200MED  (200 Medley)			200FR  (200 Free)
200IM  (200 Individual Medley)		50FR  (50 Free)
100FL  (100 Fly)			100FR  (100 Free)
500FR  (500 Free)			200RE  (200 Relay)
100BK  (100 Backstroke)			100BR  (100 Breaststroke)
400RE  (400 Relay)

Note:  It is recommended that you do not break the program's execution by pressing ON to exit.
This program modifies several system/graph variables, and though no harm will be done by 
exiting manually, you are better off letting the program clean up regularly, especially if you 
are an inexperienced calculator user.


========
CREDITS
========

This program was written by Travis Fischer (Fisch2).  A set of TI-82 TI-Basic programs written 
by Mr. Butters was used for the basic program outline, conversion factors, and formulas.  I 
would also like to thank you for downloading this program.  If you're a TI-Basic developer, 
I'd suggest checking out the source; though it doesn't look fancy, parts of the program are 
actually fairly advanced (specifically the parsing for Time input and converting the resulting 
string into accessible numbers).


===========
DISCLAIMER
===========

This program may be distributed by any other organization for non-commercial use only.  You 
are free to re-use any part of the sourcecode in any of your Free projects.  I hope that the 
source code will help someone to learn TI-Basic, but just remember before copying any of the 
source to give credit where credit is due.  This program has been tested, but in case it 
causes something harmful, I am in no way Liable for what happens.  The author expresses NO 
guarantees or warranties of the software and/or the data files, either expressed or implied.  
I can in no way guarantee the accuracy of this conversion tool!  If you find any other bugs, 
please feel free to e-mail me at phishfud@elknet.net.