PLINKO v1.0
--By Fisch
phishfud@elknet.net


/* -------------- IMPORTANT NOTE -------------- *\
Plinko is provided as-is!! I in no way am responsible for anything that happens to you or your calc/computer as a result of my program.  The source to Plinko is provided to help new programmers learn C.  If you need a part of Plinko's source for one of your own projects, please e-mail me and I'm sure it'll be fine.  Please do not separate this ReadMe file from Plinko.

/* -------------- Game Description -------------- *\
     If you've never watched The Price is Right before, Plinko is not that hard of a game to grasp.  It is a game of chance and anticipation.  Basically, you start off with one free plinko chip and must win as many chips out of 4 tries as you can.  For each chip, you are shown the name of a product and a 2 digit price.  You must decide which digit is in the actual retail price of the given product.  If you guess correct, you are rewarded with another plinko chip.  After 4 products with a total of 5 possible chips (you receive a freebie chip at the start), you move to the Plinko board!  The board is vertical with pegs in it and when you drop a chip down it, it bounces randomly off the pegs and lands in one of the 9 bins on the bottom.  Each bin has a different cash value.  Drop however many chips you won guessing the product prices and see if your total lands you a highscore:)  This is my first game using fopen, etc. routines to create/manipulate external data!

-- If you think that sounded confusing, just try Plinko once and you'll find that it's not that complicated and very fun!

/* -------------- Running Plinko -------------- *\
Send plinko 89.y and plinko 89.z to your ti-89 calc or VTI and type " plinko() " on the homescreen entry line.
If you have ANY problems getting Plinko to work, e-mail me and I will try to help you.

/* -------------- Game Controls -------------- *\
2nd	Chooses every option and drops your chip

Left/Right	Moves your plinko chip and changes the selected product

Up/Down	In main menu, moves your cursor

HOME	Exits the game!!! (It is my personal preference to not use ESC)

Enter	When entering a high score name, press enter to confirm

/* -------------- Thanks To: (in no order and probably missing some people) -------------- *\
Everyone who helped make TIGCC and it's extensive Library
Thomas Nussbaumer for his excellent ExtGraph
Everyone at the TICT message boards!!
Cisco
Zeljko Juric
Kevin Kofler
Sebastian Reichelt for the IDE
me
James Bond

/* -------------- To do: -------------- *\
-- As of now, there are only about 25 products, so you will get products repeated if you play for a little bit and have a good memory.  It is very easy to add more products, I just haven't done it yet...
-- I would like to add an option to be able to skip the product guessing stage and get right to the main game of plinko starting with 5 chips.
-- 92+/V200 support, (not that hard)

phishfud@elknet.net	// Please, no junk mail