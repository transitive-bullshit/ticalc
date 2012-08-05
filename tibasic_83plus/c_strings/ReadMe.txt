/******************************************************************************
*
* file name:       c_strings.c
* version:         1.0
* initial date:    8/12/2004
* authors:         phishfud@elknet.net (Travis Fischer - Fisch2)
* contributors:    TICT (Thomas Nussbaumer/Lionel Debroux)
* description:     Converts an array of strings into assembly format
*
******************************************************************************/

------
ABOUT:
------
C_strings is a DOS utility which will convert an array of ordinary C strings into 
an optimized, assembly format.  Most of the time, when programs have large arrays 
of strings (like for an info screen or a game's storyline), they are stored by 
TIGCC using a .long offset (4 bytes) to their actual position.  By using a .word 
offset (2 bytes) and retrieving the string addresses with inline assembly, programs 
can save hundreds of bytes!  I have used this method in all of my major programs 
(Ice Hockey 68k, Excitebike 68k, Sumo Wrestling).  The amount of space you'll save 
by using the assembly format depends on the number of strings in the array.  Smaller 
arrays of strings (approx less than 10 strings) will likely be better off using the 
C method, while larger arrays of strings will benefit greatly by converting them to 
assembly format.

There are three types of assembly code the program can generate to access the 
strings.  You may access the strings using inline assembly (smaller and faster), an 
assembly function (easier to use and smaller if you access the strings multiple 
times), or a 'master' assembly function which takes the string to access as an 
argument (best if you use two or more assembly-formatted strings).  If you have any 
questions, feel free to mail me a question at phishfud@elknet.net.

Strings which are included more than once in the C array will only appear once in 
the assembly array since the program will optimize their usage.  An example input 
file has been included.  Input files do not have to be in .txt form.  You may call 
c_strings without any arguments and the Usage will be printed as below.

Usage: c_strings [-i|-f|-f2] <infile> <outfile> (array_name)
                  -i         ... include inline version of function to access strings
                  -f         ... include regular function to access the strings
                  -f2        ... include master function to access the strings, which
                                 takes the string to access as an argument
                  infile     ... file including C strings to convert
                  outfile    ... output filename (use - as name for output to stdout)
                  array_name ... optional name of array to generate

                  Converts a C string array into its optimized assembly
                  form including the necessary functions to access them.

           NOTE:  If you only need to access the strings a few times, using an
                  inline version of the function to access them will be faster
                  and possibly smaller, but will also be harder to use.
           NOTE:  Each string Must be surrounded by quotes.

           Ex.    c_strings -f in.txt out.txt

-------
THANKS:
-------
Big thanks to Lionel Debroux, who first showed me this method, explained it to me, and 
even showed me examples of its usage in TICT projects.

* everyone at TIGCC for their great compiler!
* Dev-Cpp makers
* the TICT/TIGCC forum