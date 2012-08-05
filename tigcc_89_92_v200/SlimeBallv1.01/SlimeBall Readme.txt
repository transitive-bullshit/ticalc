

	 #### ###     #               ####        ###   ###               ###          ###  ###   
	#       #                     #   #         #     #                 #         #   #   #   
	#       #   ###    # #   ###  #   #  ###    #     #         #   #   #         #   #   #   
	 ###    #     #   # # # #   # ####      #   #     #         #   #   #         #   #   #   
	    #   #     #   # # # ##### #   #  ####   #     #         #   #   #         #   #   #   
	    #   #     #   # # # #     #   # #   #   #     #          # #    #         #   #   #   
	####  ##### ##### # # #  #### ####   #### ##### #####         #   #####   #    ###  ##### 



Started : 04/02/2004
by Jean-Francois Geyelin and Travis Fischer

...:::History:::...

Note:not all of these version where released.
04/23/2004 v0.80: can be played in 3 modes: vs. A.I.; vs. human (no link) ; vs. human (link).
04/30/2004 v0.90: The game is pretty much done: you can disconnect the cable without making freeze any of the Ti89. Score added. New explosions, (inspired by "Alien Invasion" from Jason Ho). When you quit a game, you go back to the main menu. Made new A.I.
05/08/2004 v0.92: Lot's of beta testing, some changes concerning the link code. There is a bug I cannot correct: sometimes the Ti89 turns off it's self. Also, when playing with 2 calc, there sometimes is a synchronization (due to a loss of data), which causes a big slowdown; I have no idea how to correct this! I still release it because I won't be coding for some time (the end of year exam are coming!).
05/20/2004 v0.93: There now is an arrow showing which slime you are when either playing alone or trough the link. After a loss, you are now "teleported" to a initial position and the "round" number is displayed. Also, the explosions have changed. The source (with some comments) is included. I corrected an error: I was redirecting AUTO_INT 2 instead of AUTO_INT 1! I hope I will be able to stop programming and focus on the exam wich have already started!!!!
06/05/2004 v0.94: Phewww, changed LOTS OF STUFF. Travis Fischer made me stop using trigo functions to slow down the prgm (with a pokeIO(0x600005,0b11111) instead). Also, he told me about the use of "static inline"-> program size diminution. Lionel helped me use ExtGraph 2.00 instead of v1.02 (boy is this library fast!). I Added a preference menu (this was a real pain in the a**): one can change such things as gravity, the background, how the wall moves. I also "organized" my code with prototyping and A HUGE amount of defines (so I might start using long and bitshifting instead of floats'). And damn, I didnt do too good at my Baccalauréat!
06/10/2004 v0.95: Added a new background picture. Background picture are now compressed with horizontal RLE thanks to Image Studio. The decompression routine was taken from Ice Hockey 68k. In-game messages look better now: they are "animated". Tweaked a couple things to reduce the program's size. Sasume (Jackiechan68k) made me an assembly routine allowing me to draw much faster the wall in the middle. At the start of link games, one of the calc transmits the preferences to the other one-> both calcs have the same preferences. Also, the receiving calcs check if both calcs are using the same version of slimeball (i.e. have the same "magic").
06/11/2004 v0.96: I created a header where all the strings are. I did this because cerede2000 suggested I should do a french version... which he helped me do: he translated the header. I changed the vertical speed of the wall: it wasn't fair before because the wall was always up on one side. (thanks for Genocyd for this remark). Added some comments to the source, and indented it. I now use TIGCC beta 10. This is a public release.
06/25/2004 v0.97: This ENORMOUS update has been made by Travis Fischer. He changed the whole link code (and that's really something) and added a menu to change the speed of the game. On my side, I changed some stuff Lionel Debroux told me about. Sorry for the time I took to upload the program. BTW, if you feel like translating slimeball in an other language...
07/01/2004 v0.98: Reduced the size by using some of Kevin Kofler's Backgammon routines. You can now use the numbers instead of the arrows to move the slime.
10/08/2004 v0.99: Forgot a line of code in v0.98 (OSLinkOpen...). Don't disconnect the cable while playing otherwise one calc will crash.
10/16/2004 v1.00: !!!!! THIS IS NOT THE FINAL RELEASE, BUT IT'S DARN CLOSE !!!!!  New stuff: Big optimizations by Travis Fischer, and some smaller one's by myself; one hell of a good idea by a "fan" : nearly-zero gravity maihem (I simply love this mode!); very funny bug corrected (happened when you pressed [2ND] and an other key); added a pause button : [F5]. 	Note: I am very happy about the way this game turned out.
11/13/2004 v1.01: This is the "I_dare_you_find_a_bug" version. Corrected 2 things: the movements with the arrows and the "pause bug" (the wall kept moving even during the pause).

======================================================================================================

...:::About this program:::...

You need around 25000 free bytes to play Slimeball.

SlimeBall works on the TI89 titanium, (and of course on the 89,92+ and v200)
This program is similar to the java game "Slimeball" avaible at: www.slimeball.tk/
I discovered this game with Slimeball for Ti83+. I thank the author (Joakim A-R) of the 83+ version for helping me do the "A.I.".

KEYS:
When vs AI or vs an other player in link mode, move with the arrows [LEFT] [RIGHT] [UP] or with the numbers [4] [6] [8].
When vs an oher player on the same calc, one player uses the arrows, and the other uses the numbers.
Press [F5] to pause when not playing in link mode.
Press [ESC] to return to the main menu.


======================================================================================================

...:::Tools that where used:::...

TIGCC 0.95.
http://tigcc.ticalc.org/

E X T G R A P H    v 2 . 0 0    Beta 3
(TestCollide16, GraySprite16_MASK_R, GraySprite8_MASK_R, GraySprite8_OR_R, DrawGrayRect2B, FastCopyScreen_R, ClearGrayScreen2B, FastDrawGrayLine2B)
http://tict.ticalc.org/

Image Studio v2.1
http://cullen.macross2000.org

Vti Version 2.5 beta 5 (aka "the Ultimate TI Calculator Emulator")

CalcCapture 1.0

SPRITE MAKER
http://www.ticalc.org/archives/files/fileinfo/208/20808.html

SpritED v1.55uF
www.damnsoft.org

http://entries.the5k.org/364/font.html

======================================================================================================

...:::People:::...

Email: Jfgeyelin@gmail.com
Website: http://oragestudio.free.fr

Credits to people who helped the most (random order!):
Joakim A-R
Lionel Debroux
Kevin Kofler
Julien Richard-Foy

Other thanks:
Cerede2000 for the translation
Vincent Corfdir
The dude who thought about the zero-gravity mode
Samuel Stearley