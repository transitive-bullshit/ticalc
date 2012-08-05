-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------- Sumo Wrestling v0.20 -------------------------------------
----------------------------------- By Travis Fischer /Fisch\ -----------------------------------
-------------------------------------- phishfud@elknet.net --------------------------------------
---------------------------------------- January 22, 2005 ---------------------------------------
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------

OVERVIEW:
1) Introduction
2) Requirements
3) Instructions
4) Hints and Help
5) Features
6) Credits
7) Disclaimer


=============
INTRODUCTION
=============

Get ready to play a full-featured Sumo Wrestling game, complete with everything from pre-
match smack talk, to earthquake-inducing tosses, to creating your own customized wrestlers!
This program was designed with the Ti-89, Ti-89 Titanium, Ti-92+, and v200 in mind, and the 
same program should run on all three calculators.  If needed, key information is provided 
in the game itself from the main menu's Info option.  To get the most out of Sumo Wrestling, 
be sure to read through the instructions before playing.  Have fun with this game :)


=============
REQUIREMENTS
=============

1) Send the two Sumo files to your calculator
	* The file 'sumo' is the program launcher
	* The file 'sumodata' contains the compressed game code which is run by sumo()
2) Type sumo() in the Home entry line
3) Press ENTER to run the game
4) There will be a very short pause while the game is decompressing.  This is normal and 
should last one to two seconds.

Compatibility Notes:

* Sumo Wrestling needs about 125,000 bytes of free RAM to run.  To free up memory, you may 
have to archive some files.  Go to the Var-Link menu (2nd -), choose a file (or more than 
one, using F4), then press F1 to open the Manage menu and press 8, 'Archive Variable' to 
archive files.

* Sumo Wrestling should be compatible with all AMS versions 1.01 and greater, including 
the Titanium.  It should also run fine on PedroM.

* The program is designed to detect which type of calculator it is being run on, so the same 
program will run on a Ti-89, TI-89 Titanium, TI-92+, or on a v200.  One of the files should 
have a .z extension (ASM extension on calc) and the other should have a .y (ppg extension on 
calc).

* With recent versions of Ti-Connect, the .z and .y extensions will not show up on your 
computer.  This is normal; just send the two files to your calculator anyway.


=============
INSTRUCTIONS
=============

General:
--------

The object of Sumo Wrestling is to force your opponent out of the ring (dohyo).  You lose if 
you are forced out of the ring or if you fall to the ground.  A match is conducted by weakening 
your opponent with various pummeling techniques and then using one of many finishes available to 
conclude the match.  The game modes currently available allow you to play against the computer, 
practice against a 'dummy' opponent, watch the computer play itself, or play against a friend 
with multiplayer linkplay.  A tournament mode is under development.


GamePlay:
---------

Every match starts out with each wrestler performing one of three Sumo rituals.  Wrestlers will 
then move into the center, bow down, and begin wrestling once they are frowning at each other.
You can bypass the ritual introductions by pressing Enter to immediately begin the match.
Walking into your opponent will cause both wrestlers to enter the Pummel position (holding onto 
each other).  Most moves/finishes are only available from the Pummel position.  The 'Forwards' 
key refers to pressing into your opponent, and the 'Backwards' key refers to pressing away from 
your opponent.  Depending on a comparison of wrestler stats and a bit of randomness, moves can 
be Reversed.  For example, if an extremely weak wrestler attempted to Lift a very strong/heavy 
wrestler, he would begin the move, but not be able to lift all the way through.  When most 
Reversals occur, the wrestler attempting the move will 'sweat' as he tries unsuccessfully to 
perform the desired move.
Once your opponent is sufficiently weak enough, when you attempt a move, you wrestler will 
perform a Finish instead, ending the match.  There are multiple creative finishes available for 
each move and some of them are pretty funny.


Key Explanations:
Ti-89(t)  Ti-92+	Ti-v200
-------------------------------
2nd	  F1		Hand		Punch (1 possible finish)
Shift	  F5		Q		Hold while standing still, then move towards opponent to Charge
Down+2nd  Down+F1	Down+Hand	Taunt opponent (you gain Health, but are vulnerable to attack)
Up+2nd	  Up+F1		Up+Hand		Another Taunt  (you gain Health, but are vulnerable to attack)
F5	  F4		F3		Teacher Key, Quits the game Fast
	Left/Right			Moves your wrestler horizontally
APPS	  APPS		APPS		Pauses the game (tap twice to turn calculator off)
ESC	  ESC		ESC		Quits the game

Pummeling Combos:
   Ti-89(t)		Ti-92+		Ti-v200
-----------------------------------------------------
1) 2nd+Forwards		F1+Forwards	Hand+Forwards
* Push your opponent (1 possible finish)

2) 2nd+Up		F1+Up		Hand+Up
* Lift your opponent up in the air -- once there, you may carry your opponent forward by 
pressing towards your opponent.  (also, if your opponent is weak enough to be finished and you 
lift him near the middle of the ring, the coolest finish will occur) (2 possible finishes)

3) 2nd+Backwards	F1+Backwards	Hand+Backwards
* Throw opponent in the opposite direction.  After a throw, both wrestlers will have switched 
directions  (3 possible finishes)

4) Shift+Forwards	F1+Forwards	Hand+Forwards
* Shove your opponent away, out of the pummel position (1 possible finish)

5) Hold Shift		Hold F1		Hold Hand
* Only as you are reversing your opponent's throw and have wore him down to a point where you 
can finish.  This is the hardest finish to perform, but it's also one of the best.  If performed 
correctly, your opponent will throw you up in the air only to have you crash back down, squishing 
him and causing an earthquake. (1 possible finish)

6) Shift+Reversal	F1+Reversal	Hand+Reversal
* Press while reversing your opponent's throw to attempt a finish where your opponent will toss 
you way up, only to have you come crashing down on him, smushing him.  Very hard/rare move to 
perform.


Modes:
------

1) Plr vs Calc - Wrestle off against a Computer AI opponent.  Set the difficulty level of the 
AI in the Options menu.

2) Calc vs Calc - Sit back and watch the frenzy as two computer AI's go at it.  Turn up the 
speed in the Options menu for some really hectic gameplay.

3) Two Player - Wrestle against a Human opponent via the Link in a fast-paced one-on-one 
showdown!  See the Two Player Info section of this Readme for more information on this mode.

4) Coming soon:  Tournament Mode
Journey along an expansive Japanese landscape, training and fighting your way towards becoming 
the one and only Grand Yokozuna!  High Scores will only be applicable in Tournament Mode.

5) Sumo Editor - Build and customize your own Externally stored sumos and then wrestle them 
against your friend's.  It's really fun to create two wrestlers based off of real people 
and then have them wrestle either Calc vs Calc or via Linkplay.  My math teacher actually 
let me create him and another math teacher and have a grudge match in front of the class 
using the TI-89's Projector.  Editor options include Creating, Editing, and Deleting 
External Wrestlers.  You may not edit built-in wrestlers.  Using the Editor is fairly self-
explanatory.  Use the up/down keys to select different attributes of your wrestler and then 
press ENTER to change them, except for the stat bars, where you press Right/Left to adjust 
their values.  You only have a limited number of points to use for your wrestler's stats, so 
make sure you spread them out wisely.  Each wrestler has three randomly selected smack talk 
sayings, which are fully customizable (and extremely fun to create).

Stats/Attributes:
-----------------
* Strength - The most important stat, for determining reversals/finishes, and affects how much 
damage to inflict with each move
* Health - The amount of Health your wrestler will begin the match with; all wrestlers begin 
with a health stat of at least 41.  Adjusting this value will just add to that minimum towards 
the maximum health of 72.  This attribute also determines the proportionate amount of health 
a wrestler gains by taunting.
* Stamina - Factors into determining reversals and finishes
* Speed - How fast your character can move forwards/backwards.  Whichever wrestler is faster 
will gain the advantage if both wrestlers are attempting to begin movers at the same time, as 
well as AI Charging and how quickly AI characters will perform Pummel moves

Sumo Wrestling involves strategy as well as button-smashing.  Different wrestlers will fair 
better or worse against other wrestlers depending on a comparison of their stats.  Since the 
Strength attribute is extremely important for determining how much damage each blow inflicts, 
a weak, fast wrestler will not always do very well against a strong, slow wrestler.  Try to 
look for a balance in wrestler stats.

Two Player Info
----------------

Make sure that both Calcs have the Sumo Files and that the Link cable is connected 
firmly into both Calcs and then choose the 'Two Player' option in the Exhibition.  The 
Calculator which Joins the game first will become the Host, which, unless you are 
interested in the source, really doesn't matter.  Multiplayer games have been extensively 
tested, but there is no way to ensure a 100% connection rate.  There may be a noticeable 
speed difference in the multiplayer games because of the amount of data that needs to
be transferred through the link each frame.  If there is a Link Error, just try again; 
most of the time it will work.  If one calc gets stuck waiting for the other calc to join 
when the other calc has already joined, there has been an error so ESC back and try it 
again.  If the problem persists, there may be a defect in your cable/connection.  Check 
again that the link cable is firmly connected to both calcs, and if the problem still 
hasn't gone away, you may need to archive everything on your calc and reset it.  Also, 
don't think that you have to both enter the 'Two Player' option at the same time because it 
really doesn't matter.  It is Very fun to play Sumo Wrestling with a friend, so go try it!
It is especially fun to wrestle bouts between your own custom-made wrestlers.

NOTE - In Multiplayer Mode, only the person who wishes to use a particular external 
wrestler needs to have the wrestler file.  Both calcs do NOT need a copy of the external 
wrestler file.  Also, you may want to check to make sure both calcs have the latest version 
of Sumo Wrestling.  Linkplay may not be backwards-compatible.

Cheats
-------

There are cheats hidden in the game :)


==============
HINTS and HELP
==============

If you find Anything wrong with the game, you know a way to improve Sumo Wrestling, or you 
are just lonely, your e-mail is very welcome.  Please, no Spam or Junk e-mails.  If you 
would like to reuse a major portion of the source, please contact me first.  Please give 
credit where credit is due.

Hints/Tips:
----------

1) The best defense is a good offense.  Try to Charge at your opponent right away, get a 
few pummel moves in, and them throw them away before they have time to react.

2) Every time you taunt your opponent, your health will increase proportionately, according 
to your wrestler's health stat.  When you are taunting, however, you have no way of 
defending yourself against attack.  The best time to taunt, therefore, is when your opponent 
cannot fight back (like right after you've thrown him or if they are teetering on the ring's 
edge).  This is also the strategy the AI uses to determine when to taunt.

3) Quickness and button smashing is a major part of winning matches, but strategy is also 
very important as well.  Certain wrestlers will fair better or worse against other wrestlers 
based on their attributes.

4) Try to unlock all of the finishes!  Some of the coolest ones cause Earthquakes :)

5) Punching, especially on the hard difficulty setting, is not very effective in wearing 
down an opponent's health.  Punches are very useful for blocking charges.

6) Time Limit, Speed, Difficulty, and Distortion options are available in the Options menu.  
Try mixing their values around to get the most out of the possible gameplay options.  If you 
can whoop the computer, then try turning the speed way up (15).  If you want to wrestle 
in slow-motion, set the speed to 1.  The screen Distortion option will make the ring and the 
wrestlers look wavy/dreamy.  It will slow down gameplay a bit, though, so I recommend turning 
the speed up to 15 when playing with a Distorted screen.  You should definitely try it out 
because you'll never believe these graphics were possible on your calc!

7) Use the Practice mode to get a feel for the game's basic gameplay and to get used to the 
different button combinations.  The Computer AI will not fight back in Practice mode.

8) Many more finishes are available if your opponent is on the edge of the ring.  The same is 
true for your opponent who will be trying to push you out of the ring, so defensively, try to 
stay in the center, and offensively, try to get your opponent near the edges.

=========
FEATURES
=========

* Intense button-smashing, yet strategic Gameplay
* Easy pick-up-and-play style
* Two-Player Link Matches
* Excellently crisp, Grayscale double buffered graphics
* Sixteen built-in wrestlers are all official, top-ranked Japanese Sumo Wrestlers
* User-friendly Wrestler Editor built-in
* A wide variety of moves and creative finishes
* Sophisticated AI opponents with adjustable difficulty level
* Ti-89/92+/v200 Compatible (including Ti-89 Titanium)
* Currently four modes of gameplay (plr vs calc, calc vs calc, plr vs plr, practice)
* Hidden secrets???
* Game information/explanation included in-game
* Runs Fast and Smooth on HW1 and HW2 68k Calcs
* Over 11,000 lines of source code (printed out, that's about 180 pages of paper)
* Fully customizable gameplay
* Well Commented/Structured C Source Code included


========
CREDITS
========

This program was written by me, Travis Fischer.  Most graphics are ripped from a Japanese Sumo 
Wrestling game on the original NES.  There are in-game scrolling credits in the Information 
option off of the Main menu.  I really have to give the biggest thanks to the Programming 
Message Board at http://pub26.ezboard.com/btichessteamhq.  I visit it often and always find 
quick, correct answers to my many questions.  If you are considering learning to program in C 
with TIGCC, then you should definitely visit that forum quite frequently.  I have put about a 
year into this game's development, so if you liked this game, I would really appreciate it if 
you gave it a rating in the archives of www.ticalc.org.
Thanks :)

I would like to thank:
---------------------

TICT
The Tigcc Team
Nintendo (NES)
Open Source
TechnoPlaza
Kevin Kofler (kevin.kofler@chello.at)
Lionel Debroux
240-185 (naPO) for the French translation
Squale92
and Cisco, my Cat
*And anyone else that I forgot

Beta Testers:
------------
Lionel Anton
mhubi
Kevin Kofler

and Me, of course
(Cisco has not tested the game, due to his lack of apposable thumbs)


===========
DISCLAIMER
===========

This program may be distributed by any other organization for non-commercial use only.  You 
are free to re-use any part of the sourcecode in any of your Free projects.  I hope that this 
game may help someone to learn C, but just remember to give credit where credit is due.  This 
game has been extensively tested, but in case it causes something harmful, I am in no way 
Liable for what happens.  The author expresses NO guarantees or warranties of the software 
and/or the data files, either expressed or implied.  If you find any other bugs, please feel 
free to e-mail me at phishfud@elknet.net.

Other games programmed by Travis Fischer (available in the archives at www.ticalc.org):
* Ice Hockey 68k, an extensive 3v3 Hockey game
* Excitebike 68k, a clone of the well-known NES Excitebike dirtbike racing game