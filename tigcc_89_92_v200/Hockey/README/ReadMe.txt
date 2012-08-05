/* ------------------------ *\
      Hockey 68k v2.75
	 By Fisch
     (Travis Fischer)

    fisch0920@yahoo.com
/* ------------------------ *\
Original Release Date: October 23, 2003
Current Release Date: December 31, 2005

OVERVIEW:
0) Updating Notes
1) Introduction
2) Requirements
3) Instructions
4) Hints and Help
5) Features
6) Credits
7) Disclaimer

==============
UPDATING NOTES
==============

If you are updating from a previous version of Ice Hockey (below v2.70):

1) There is no longer a need for the external graphics file 'hockgfx'.  If it is still 
located on your calculator, you may delete it.  All data which used to be located in this 
file has been moved back into the main program as a result of 15kb worth of optimizations!

2) Linkplay in this version has been heavily optimized/updated, so it is incompatible with 
the linkplay of previous versions.  Before playing multiplayer, first make sure that both 
calcs have the most recent version of Ice Hockey (should be v2.75).  Linkplay in v2.75 
should be more efficient and stable.

3) Any configuration settings you had saved in an older version of Ice Hockey will be erased, 
but it is very easy to reset them.  Edited, external teams from previous versions will work 
fine with any version of Ice Hockey.

=============
INTRODUCTION
=============

I've always loved the game of Hockey and thought it would be fun to make a game of it.  This 
game was entirely created with TIGCC and needs approximately 80,000 bytes of available RAM 
for the game to run.  This program was designed with the Ti-89, Ti-92+ and v200 in mind and 
the same program should run on all three calculators.  To run the game on a Ti-89 Titanium, 
you must send the appropriate version, located in a separate folder marked 'Ti-89 Titanium'.
This is a 3v3 Hockey game with no penalties.  If needed, key information is provided in the 
game itself from the main menu's Info option.  Have fun with this game :)


=============
REQUIREMENTS
=============

1) Send the two hockey files to your calculator
	* The file 'hockey' is the program launcher
	* The file 'hoekcyzip' contains the compressed game code which is run by hockey()
2) Type hockey() in the Home entry line
3) Press ENTER to run the game
4) There will be a very short pause while the game is decompressing.  This is normal and 
should last one to two seconds.

Compatibility Notes:

*Ice Hockey 68k needs about 85,000 bytes of free RAM to run.  To free up memory, you may 
have to archive some files.  Go to the Var-Link menu (2nd -), choose a file (or more than 
one, using F4), then press F1 to open the Manage menu and press 8, 'Archive Variable' to 
archive files.

* Ice Hockey 68k should be compatible with all AMS versions 1.01 and greater, including 
the Titanium 3.00.  It should also run fine on PedroM.

* The program is designed to detect which type of calculator it is being run on, so the same 
program will run on a Ti-89, TI-89 Titanium, TI-92+, or on a v200.  One of the files should 
have a .z extension (ASM extension on calc) and the other should have a .y (ppg extension on 
calc).


=============
INSTRUCTIONS
=============

General:
--------

During the game, the player that you control will have a hand pointing at it.  The Player who has 
control of the puck will have a large circle surrounding the Player.  Games are 3v3 with one 
Goalie on each team.  Each team is either in the Western of Eastern conference.  These are the 
real NHL's teams and Rosters.  Each team also has a type of formation.  With some practice, 
you should find which formations do well against others and which do not.  Each team then has 
three Players located at the top of the screen.  The first Player will always be a goalie.  There 
are then three other types of players, each with their own unique strengths and weaknesses.  

Player Types:
-------------

1) Goalie     - Goalies will always stay in front of their goal and react to the action 
rather than taking part in it.  The Goalie's attributes are the same as an Average Plr.

2) Fat        - This Players are slow and will easily be outskated by skinnier players.  
However, they have a very hard shot and have a higher chance at winning the puck in a 
scramble.  I have found that the Fat Players are harder to play with, so if you get pretty 
good at Hockey, try playing with the Fat guys on Hard Difficulty.

3) Average    - This Player is an average of the Fat and Skinny Players.  Average Players 
have no strengths and that is their weakness.

4) Skinny     - These Players are very fast skaters, but have a softer shot.  They are also 
less likely to win the puck in a scramble.

There are four main Modes of Play:
-----------------------------------

1) Exhibition - Hone your skills against a computer opponent or a friend in a single game.  
Choosing the Calc vs Calc option will allow you to watch the Computer A.I. play itself.

2) Tournament - Battle your way to the top of an eight team tournament.  To win, you must 
beat three computer teams successively.

3) ShootOut   - Practice against the computer or a friend in a Five-Round 1 on 1 Shootout.  
In a ShootOut, you have Five seconds to score.  If the score is tied after five rounds of 
alternating shots, the ShootOut will go into Sudden Death!
Choosing the Calc vs Calc option will allow you to watch the Computer A.I. play itself.

4) Practice   - Make up plays or just skate around and face no opposition.

(default)
Ti-89 Keys:
------------
2nd			Shoots
Shift			Passes
Diamond*		Changes Plrs on defense
APPS			Pauses the game
ESC			Quits the game
F5			Teacher Key, Quits the game Fast

Ti-92+ Keys:
--------------
F1			Shoots
F5			Passes
F2*			Changes Plrs on defense
APPS			Pauses the game
ESC			Quits the game
F4			Teacher Key, Quits the game Fast

Ti-v200 Keys:
--------------
HAND			Shoots
F1			Passes
Q*			Changes Plrs on defense
APPS			Pauses the game
ESC			Quits the game
F3			Teacher Key, Quits the game Fast

*Diamond on the Ti-89, F2 on the Ti-92+, and Q on the Ti-v200 will automatically change 
control to the closest Plr to the Puck on defense.  If you double tap the change key, 
control will go to your Goalie.

Hockey allows for the user to redefine the game's key layout in the Options Menu.

Two Player Info
----------------

A Multiplayer option is available in the Exhibition and ShootOut Modes.  Make sure that 
both Calcs have the Hockey Files and that the Link cable is connected firmly into both 
Calcs and then choose the 'two player' option in either the Exhibition or ShootOut Menu.  
The Calculator which Joins the game first will become the Host, which, unless you are 
interested in the source, really doesn't matter.  Multiplayer games have been extensively 
tested, but there is no way to ensure a 100% connection rate.  There is a noticeable 
speed difference in the multiplayer games because of the large amounts of data that need 
to be transferred through the link each frame.  If there is a Link Error, just try again; 
however, this has never happened in my tests so the link functions should be working 
pretty well.  It is Very fun to play Hockey with a friend!

Note:  Ice Hockey 68k v2.70 (and above)'s Link code is incompatible with previous versions 
(changes were made to make the linkplay more optimized and stable)

Note:  Currently, there is no check to make sure that both calcs have the same hockteam 
file, so playing multiplayer with a customized team will not work as expected.  To play 
multiplayer with customized teams, make sure that both calculators have the same 
hockteam file.


==============
HINTS and HELP
==============

If you find Anything wrong with the game, you know a way to improve Hockey, or you are just 
lonely, your e-mail is very welcome.  Please, no Spam or Junk e-mails.  If you would like 
to reuse a major portion of the source, please contact me first.

Hints:
-----

1) Winning the Faceoffs is critical to winning the game.  Try out each of the (3) formations 
to see which one you do the best with.  I find that it is easier to, instead of heading 
straight for the puck, skate back a bit and then take the puck from whoever wins it in the 
scramble.

2) Practice and come up with different ways to score.  One sweet way to score is to skate 
all of the way around the other player's goal and come out on the other side to school the 
goalie big time!

3) On defense, if your player is behind the other team's player who controls the puck, 
unless they are a skinny player, the chances of them catching up are slim.  Following 
behind a player does nothing, so try to move your current player away from the puck so 
your goalie will be the closest player to the puck.  On Hard Difficulty, the Computer 
Players will score on your computer goalie an average of 8/10 times by aiming for the 
corners of the net, so taking control of your goalie to prevent a computer's 
'smart shot' is a must.

4) Find unique ways to school players on the other team.  Maybe cut back and then skate 
on an angle away from a defensive player.  It's pretty sweet to play multiplayer and 
leave your friend in the dust.

5) In ShootOuts against the Computer, I find it is easier to bring out your goalie fast, 
before the Computer player has the chance to get off a shot.

6) The farther away you are when you pass the puck, the less accurate your pass will be.


=========
FEATURES
=========

* First non-text Hockey game ever on a Ti-Calc
* Multidirectional Scrolling Rink
* 31 Official NHL Teams, (each with their own Roster, Style, and Player Types), Two being All-Star Teams
* Strong A.I.
* Two-Player Linking Option for Exhibition and ShootOut Modes!!!
* Grayscale
* Two Difficulty Levels
* Set Game Time anywhere from 1 to 15 Minutes in length
* 4 Modes of Play (Exhibition, ShootOut, Tournament, Practice)
* Graphics from Ice Hockey for the NES
* Hidden Secrets
* Runs Fast and Smooth on HW1 and HW2 68k Calcs
* Customizable Game Speed
* Ti-89/92+/v200 Compatible (Titanium compatibility through a separate binary)
* 4 types of Players, (Goalie, Skinny, Average, Fat), each with their own strengths/weaknesses
* Well Commented C Source Code included
* Redefinition of Keys


========
CREDITS
========

This program was written by me, Travis Fischer.  Most of the graphics were ripped from 
the game Ice Hockey for the NES, made by Nintendo.  There are in-game scrolling Credits 
in the Info option off of the Main menu.  I really have to give the biggest thanks to 
the Programming Message Board at http://pub26.ezboard.com/btichessteamhq.  I visit it 
often and always find quick, correct answers to my many questions.  If you are considering
learning to program in C with TIGCC, then you should definitely visit that forum quite
frequently.

I would like to thank:
---------------------

Thomas Nussbaumer
Everyone at TICT
The Tigcc Team
Apogee Software
Nintendo
Jesystems
Open Source
Mr. Mobius
Malcolm Smith
Zeljko Juric
Kevin Kofler
Sebastian Reichelt
Greg Dietsche
Scott Noveck
Lionel Debroux
TechnoPlaza
Clawsonia
and Cisco, my Cat
*And anyone else that I forgot

Beta Testers:
------------
Clawsonia
Deadbird
Malcolm Smith
Kevin Kofler
Stoopidguy
Adrian Bader
Digital
Jordan Krage
Lionel Debroux
Shadow
Henry Barrow
Dan Barr
Jesse Frey

===========
DISCLAIMER
===========

This program may be distributed by any other organization for non-commercial use only.  You 
are free to re-use any part of the sourcecode in any of your FREE projects.  I hope that this 
game may help some people to learn C.  This game has been extensively tested, but in case it 
causes something harmful, I am in no way Liable for what happens.  The author expresses NO 
guarantees or warranties of the software and/or the data files, either expressed or implied.  
If you find any other bugs, please feel free to e-mail me at fisch0920@yahoo.com.

Other games programmed by Travis Fischer (available in the archives at www.ticalc.org):
* Excitebike 68k, a clone of the well-known NES Excitebike dirtbike racing game
* Sumo Wrestling, a full-featured Sumo Wrestling game, complete with everything from pre-match 
smack talk, to earthquake-inducing tosses, to creating your own customized wrestlers!