------ Slope Fields ------
By:  Travis Fischer (Fisch2)
phishfud@elknet.net
April 8, 2005


OVERVIEW:
1) Introduction
2) Requirements
3) Instructions
4) Credits
5) Disclaimer


=============
INTRODUCTION
=============

This program will graph the slope field for a given dy/dx equation, allow you to modify the viewing 
window, display the slope at a given (x,y) point, and overlay the original (antiderivative) equation 
over the slope field, so you can see the relationship better.  Useful for Calculus AB, BC, and up.  
Unlike many other slope field programs, this one:  has a variable frequency for how often/large 
the slope marks are (set as a grid on the x/y axis tick marks), overcomes "divide by zero" errors
 encountered with equations like -x/y, is speed-optimized, and comes with a variety of options.


==============
REQUIREMENTS
==============

1) Send the SLOPEFLD.83p file to your calculator
2) Run the SLOPEFLD program (listed under the PRGM key)

Compatibility Notes:

* Supported calculators include:  TI-83, TI-83+, TI-83+ Silver Edition, and the TI-84 Plus/SE
* This program is written in TI-Basic and is compatible with MirageOS


=============
INSTRUCTIONS
=============

From the main screen, you see the following options:

Enter dY/dX EQ - Input the equation of the derivative you want to display the slope field for.  If 
there is already an equation entered, it will be shown as well.  dY/dX may use X and Y variables 
without worrying about dividing by zero errors.  Equation must be entered before entering any 
other menu item (except for modifying the Window).

View Slope Field - Display the slope field of dY/dX using the current window settings, and the 
current xscl and yscl as the step for each slope tick-mark.  After the slope field is displayed, 
you will be able to scroll around the field; press Enter to return to the Main Menu.

Window - Edit the current window settings.  Press Up/Down to select a setting, enter to enable 
editing, and then type in what your desired setting.  I have found these window settings to be 
the best for viewing most functions:  xmin=-5, xmax=5, xscl=1, ymin=-5, ymax=5, yscl=1.

Value - Input a coordinate pair (x, then y) and view the slope at that point (which is just dY/dX 
evaluated at the specified (x,y)).  If possible, the slope will be displayed in fraction form.

Graph orig EQ - Input the equation of the original function (antiderivative), to be plotted over the 
slope field.  This is useful for seeing the relationship between slopes (dY/dX) and the original 
equation (y=).  The original equation must be solved for y and be in the form of y=f(x).
Note:  This will overwrite y6.

Exit - Cleanup and exit the program.

========
CREDITS
========

This program was written by Travis Fischer (Fisch2).    I would also like to thank you for 
downloading this program.  If you're a TI-Basic developer, I'd suggest checking out the source; 
though it doesn't look fancy, it is well written for a basic program.  If you think it's a bit bloated, 
well, it's optimized mostly for speed (four different drawing functions depending on whether 
an X variable is present, a Y variable is present, both are present, or no variables are present).


===========
DISCLAIMER
===========

This program may be distributed by any other organization for non-commercial use only.  You 
are free to re-use any part of the sourcecode in any of your Free projects.  I hope that the 
source code will help someone to learn TI-Basic, but just remember before copying any of the 
source to give credit where credit is due.  This program has been tested, but in case it 
causes something harmful, I am in no way Liable for what happens.  The author expresses NO 
guarantees or warranties of the software and/or the data files, either expressed or implied.  
I can in no way guarantee the accuracy of this program and am not responsible if it produces 
invalid or unwanted results!  If you find any other bugs, please feel free to e-mail me at 
phishfud@elknet.net.