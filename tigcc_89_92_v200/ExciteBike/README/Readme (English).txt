EEEEEE XX   XX  CCCC IIIIII TTTTTT EEEEEE BBBBB  IIIIII KK  KK EEEEEE      666666  888888  KK  KK
EE      XX XX  CC      II     TT   EE     BB  BB   II   KK KK  EE         66      88    88 KK KK
EEEE     XXX  CC       II     TT   EEEE   BBBB     II   KKKK   EEEE       666666   888888  KKKK
EE      XX XX  CC      II     TT   EE     BB  BB   II   KK KK  EE         66   66 88    88 KK KK
EEEEEE XX   XX  CCCC IIIIII   TT   EEEEEE BBBBB  IIIIII KK  KK EEEEEE      666666  888888  KK  KK
-------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------
----------------------------------- By Travis Fischer /Fisch\ -----------------------------------
-------------------------------------- phishfud@elknet.net --------------------------------------
--------------------------------------- February 29, 2004 ---------------------------------------
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

This game is a clone of the well-known NES Excitebike dirtbike racing game.  Gameplay in 
Excitebike 68k is exactly the same as the original, as well as the Five built-in tracks.  
This program was designed with the Ti-89, Ti-89 Titanium, Ti-92+, and v200 in mind, and the 
same program should run on all three calculators.  If needed, key information is provided 
in the game itself from the main menu's Info option.  Have fun with this game :)


=============
REQUIREMENTS
=============

Send the two Excitebike files to your calculator and type excitebk() in the Home entry line 
to start.  It does not matter if you send the 89 files to your 92+ or the V2Z (v200) Files 
to an 89 (Unless you have a Ti-89 Titanium!).  The program is designed to detect which type 
of calculator it is being run on and the same program should run on all three calculators.  
Just make sure that one of the files has a .z extension (ASM extension on calc) and the 
other has a .y (ppg extension on calc).  If you have a Ti-89 Titanium, you MUST send the 
appropriate Titanium launcher (instead of the .z file).  The Ti-89 Titanium launcher is 
incompatable with the Ti-89, Ti-92+, and v200 launchers.  Please note, that this program 
has yet to be tested on an actual Ti-89 Titanium, but in theory, it should work.


=============
INSTRUCTIONS
=============

General:
--------

Race against one to three Computer AI Bikes, a Friend, or alone in one of Five Built-In Tracks 
or a Custom, external track.  Avoid hazardous terrain and hit huge air on over twenty types of 
Terrain.  Each track has a Record best time and also a qualifying time necessary for you to 
take in the top 3 and qualify to advance on to harder races.

Default Keys (Same on all calculators):
---------------------------------------
Up			Moves the bike up on the track
Down			Moves the bike down on the track
Left			Tilts the bike backwards
Right			Tilts the bike forwards (only relevant when in the air)
APPS			Pauses the game
ESC			Quits the game
Left and Down		When held in the air, it will perform a Backflip for a Time bonus!
Right and Up		When held in the air, it will perform a Frontflip for a Time bonus!

Calculator Specific Keys:
Ti-89	Ti-92+	Ti-v200
-----------------------
2nd	F1	Hand	Uses the slower gear for your bike, but it will not overheat you (Gas key)
Shift	F5	F1	Uses the faster gear for your bike, but it can overheat you      (Turbo key)
F5	F4	F3	Teacher Key, Quits the game Fast


Modes:
------

In both Selection A and Selection B, Choose to race in any of Five built-in tracks.  If you 
finish in the top 3, you will advance on to the next Round/Track until you do not qualify.  
You may also choose the option of Loading and Racing on an external Track, which you may not 
advance from.

1) Selection A - Race alone;  It's just you vs. the track!

2) Selection B - Choose to race against One to Three computer opponents, adding another 
obstacle to an already challenging game.  Keep in mind that you will crash if your front wheel 
collides with an opponent's rear wheel, or else you'll be eating dirt.

3) Multiplayer - Race against a Human opponent via the Link in a fast-paced one-on-one showdown!
     a) Fastest Time - The person with the fastest time at the end of the race wins.  Note, that 
     this may not be the first person to finish the race since you can do backflips to lower 
     your time. (default)
     b) First to Finish - The first person to cross the finish line wins, regardless of the 
     other player's time.
See the Linkplay section of this Readme for more information on the Multiplayer mode.

4) Track Editor - Build and customize your own Externally stored tracks and then race on them 
in either of the other two modes.  Editor options include Creating, Editing, and Deleting 
External Tracks.  In the Track Editor, the currently selected piece of Terrain will Blink.  
Pressing Left or Right will move your cursor in the corresponding direction at the top of the 
screen.  Press the Gas key to place a Terrain and overwrite the currently selected blinking 
terrain.  Press the Turbo key to accept a terrain choice and scroll on to the next piece of 
Track.  You can scroll backwards along a Track by pressing the Backspace key (this will Not 
erase the track, it will just scroll the screen in the opposite direction).  Be creative by 
making your tracks challenging and adding twists to them.
NOTE - In Multiplayer Mode, only the Host Calc (the one to join the game first) has to have 
the external level if you wish to race on it.


Two Player Info
----------------

Make sure that both Calcs have the Excitebike Files and that the Link cable is connected 
firmly into both Calcs and then choose the 'Multiplayer' option in the Main Menu.  The 
Calculator which Joins the game first will become the Host, which, unless you are 
interested in the source, really doesn't matter.  Multiplayer games have been extensively 
tested, but there is no way to ensure a 100% connection rate.  There is a noticeable 
speed difference in the multiplayer games because of the large amounts of data that need 
to be transferred through the link each frame.  If there is a Link Error, just try again; 
most of the time it will work.  If one calc gets stuck waiting for the other calc to join 
when the other calc has already joined, there has been an error so ESC back and try it 
again.  If the problem persists, there may be a defect in your cable/connection.  Check 
again that the link cable is firmly connected to both calcs, and if the problem still 
hasn't gone away, you may need to archive everything on your calc and reset it.  Also, 
don't think that you have to both enter the Multiplayer option at the same time because it 
really doesn't matter as long as both calcs are running the Excitebike program.  Remember 
that when racing against computer or human opponents, you will crash if your front wheel 
collides with an opponent's rear wheel.  It is Very fun to play Excitebike with a friend 
so go try it!  It is especially fun to challenge friends with your own custom-made tracks.

NOTE - In Multiplayer Mode, only the Host Calc (the one to join the game first) has to have 
the external level if you wish to race on it.  Also, you may want to check to make sure 
both calcs have the latest version of Excitebike.  Linkplay in Excitebike is backwards-
compatible, but there is a rare chance that you may encounter connection problems if both 
calcs do not have the same version of Excitebike.

Cheats
-------

After every race on Track 5 that your time is less than 50 seconds (with no cheats enabled), 
a message will be shown, notifying you of a new cheat.  You can enter cheats in the Options 
menu.  Each time you enter or reenter a cheat, you will either enable or disable it.  If 
you entered a cheat successfully, a message will be shown indicating which cheat you have 
enabled/disabled.  Cheats can be used in any game mode, but you will be unable to unlock 
new cheats if you have any turned on.


==============
HINTS and HELP
==============

If you find Anything wrong with the game, you know a way to improve Excitebike, or you are 
just lonely, your e-mail is very welcome.  Please, no Spam or Junk e-mails.  If you would 
like to reuse a major portion of the source, please contact me first.

Hints:
-----

1) By either pressing Left or Right going INTO a jump, you can greatly change the type of 
jump you will have.  Pressing Back (Left) will give a shorter, higher Jump.  Pressing 
Forwards (Right) will give a longer, lower Jump.

2) Pull off Backflips Only on jumps that have steeper slopes.  Even though you have more 
airtime on some of the longer jumps, you will have less backwards momentum/pull to finish 
your flip.  I have found that Tall Bumps, both Monster Jumps, and the Large Bump Terrains 
are the best to perform flips on.  Backflips look sweet and are also the *key* to qualifying 
on some of the harder courses.  With each Backflip you perform, Three precious seconds are 
taken off the clock, so get flipping.

3) If in doubt when taking a jump, just let the bike fly as it wants to.  Often this will 
be a slower method of travel, but it will be an easier time landing.

4) Line your bike's angle (tilt) up with the slope of the landing area.  This will make 
your transition from obstacle to obstacle easier and faster.  If your bike's Tilt is too 
far off of the slope of the landing area, your bike will crash.  For example, if you land 
with a forward tilt on the Backwards slope of an obstacle, you will crash.  The same goes 
if you land with a backwards tilt on the Forwards slope (or top edge) of an obstacle.

5) Try to avoid getting stuck in a group of same speed opponents as they can pen you in.  
This could be the difference between qualifying, and being eliminated.  By colliding a 
bike's rear wheel with an opponent bike's front wheel, you can cause computer opponents 
to crash.  Computer bikes, however, can also do the same to you, especially if you are 
packed in among a group of them while trying to avoid obstacles.

6) When designing a course, try to mix some harder challenges with easier ones to create 
a fun course for all.

7) Run over Cool spots often so your bike will not overheat (Arrow marks on the Track).

8) It helps to learn the track's contents!  Knowing where a cool spot is or whether or not 
to take a jump will really pay off.

9) As in the original Excitebike, tapping Gas and/or Turbo while running back to your Bike 
after crashing will greatly speed up the process and get you racing much sooner.

10) If you are playing for a challenge, then multiple AI bikes are the best, but they will 
slow you down.  Every frame, the CPU has to update several bikes instead of just your one, 
so you will Not possibly be able to finish a race as fast as you would without having any 
Computer bikes.

=========
FEATURES
=========

* Fast Gameplay *exactly* like the original NES version
* Two-Player Link Races
* Pseudo-3D look and feel
* Five built-in tracks are the same as on the original
* Grayscale double buffered graphics
* Excellent, user-friendly Track Editor
* Bikes have the ability to do Back and Front flips
* Ti-89/92+/v200 Compatible (including Ti-89 Titanium)
* Hidden, unlockable cheats
* Game information/explanation included in-game
* Runs Fast and Smooth on HW1 and HW2 68k Calcs
* Customize your number of Computer opponents (Zero to Three)
* Well Commented C Source Code included


========
CREDITS
========

This program was written by me, Travis Fischer.  The graphics and tracks are all ripped 
from the original NES Excitebike.  There are in-game scrolling Credits in the Info option 
off of the Main menu.  I really have to give the biggest thanks to the Programming Message 
Board at http://pub26.ezboard.com/btichessteamhq.  I visit it often and always find quick, 
correct answers to my many questions.  If you are considering learning to program in C with 
TIGCC, then you should definitely visit that forum quite frequently.

I would like to thank:
---------------------

Thomas Nussbaumer
Everyone at TICT
The Tigcc Team
Nintendo (NES)
Open Source
Malcolm Smith
Zeljko Juric
Kevin Kofler
Sebastian Reichelt
Lionel Debroux
TechnoPlaza
Clawsonia
240-185 for the French translation
Mathiniste and Kevin Kofler for the German translation
and Cisco, my Cat
*And anyone else that I forgot

Beta Testers:
------------
Clawsonia
Angelboy
Jordan Krage
Kevin Kofler
Geoffrey Anneheim
AS
240-185
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

If you like this game, then you may also like to try Ice Hockey 68k, an extensive 3v3 Hockey 
game programmed by me.  It can be downloaded in the archives at www.ticalc.org.