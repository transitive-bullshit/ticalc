// C Source File
// Created 5/2/2004; 2:46:25 PM
#include <tigcclib.h>
/* REVIEW for AP-Statistics Exam:
         -- Disclaimer --
   This program is strictly for 
   review purposes and was not 
   written for people to cheat 
   with.  I hereby state that I 
   can be in now way held 
   responsible for your actions.
   It is NOT this author's 
   intent to write a 'cheat-
   sheet' prog, so please don't 
   use it as one!  And, as 
   usual, I am in no way liable 
   for what happens.  The author 
   expresses NO guarantees or 
   warranties of the software 
   and/or the data files, either 
   expressed or implied.
         -- Disclaimer --
   
   By:  Me :)
*/

#define MAX_PAGES 21
#define OFFSET    11
const char assumptions[] = "Assumptions:";

const char *const info[11 * (MAX_PAGES + 1)] = {
	"1 Sample Z Interval", "\x8F known", "", "\x9A ± Z*\x8F/\xA8(n)", 
	"Z*\x8F/\xA8(n) = Margin of Error", "", 
	assumptions, "1) SRS", "2) Pop Normal", 0, 0, 
	
	"1 Sample Z Test", "\x8F known", "Ho: M = ?", "Z = (\x9A - M)/(\x8F/\xA8(n))", "", 
	"Matched Pairs (2 Sets from 1 sample)", "Ho: M1 - M2 = 0", "", 
	assumptions, "1) SRS", "2) Pop Normal", 
	
	"2 Sample Z Interval", "\x8F known and 2 Independent Samples", 
	"(\x9A""1 - \x9A""2) ± Z*\xA8(\x8F""1²/""n1 + \x8F""2²/""n2)", "", 
	assumptions, "1) SRS", "2) Pop Normal", "3) Samples Independent", 0, 0, 0, 
	
	"2 Sample Z Test", "\x8F known and 2 Independent Samples", "Ho: M1 = M2 or M1 - M2 = 0", 
	"z = ((\x9A""1-\x9A""2)-(M1-M2))/\xA8(\x8F""1²/""n1 + \x8F""2²/""n2)", "", 
	assumptions, "1) SRS", "2) Pop Normal", "3) Samples Independent", 0, 0, 
	
	"1 Sample T Interval", "\x8F is unknown", "\x9A ± t*s/\xA8(n)", "t*s/\xA8(n) \x9C Margin of Error",
	"", "Matched Pairs (2 Sets from 1 sample)", "Ho: M1 - M2 = 0", "", 
	assumptions, "1) SRS", 
"2) n < 15 : No skewness/outliers\n  15""\x9Cn\x9C""40 : Slight skewness, No outliers\n  n \x9E 40 : Okay to use", 
	
	"1 Sample T Test", "\x8F is unknown", "Ho: M = ?", "df = n - 1", "t = (\x9A - M)/(s/\xA8(n))", "", 
	assumptions, "1) SRS", 
"2) n < 15 : No skewness/outliers\n  15""\x9Cn\x9C""40 : Slight skewness, No outliers", 
	"  n \x9E 40 : Okay to use", 0, 
	
	"2 Sample T Interval", "\x8F is unknown and 2 Independent Samples", 
	"Conservative df = smaller of n1-1 and n2-1", "(\x9A""1 - \x9A""2) ± t*\xA8(s1²/""n1 + s2²/""n2)", "", 
	assumptions, "1) SRS", 
"2) n < 15 : No skewness/outliers\n  15""\x9Cn\x9C""40 : Slight skewness, No outliers", 
	"  n \x9E 40 : Okay to use", "3) Samples from 2 Distinct, Independent Pops", 0, 
	
	"2 Sample T Test", "\x8F is unknown and 2 Independent Samples", "Ho: M1 = M2 or M1 - M2 = 0", 
	"Conservative df = smaller of n1-1 and n2-1", "t = ((\x9A""1-\x9A""2)-(M1-M2))/\xA8(s1²/""n1 + s2²/""n2)", "", 
	assumptions, "1) SRS", 
"2) n < 15 : No skewness/outliers\n  15""\x9Cn\x9C""40 : Slight skewness, No outliers", 
	"  n \x9E 40 : Okay to use", "3) Samples from 2 Distinct, Independent Pops", 
	
	"General Proportions (%'s)", "p^hat = sample prop (statistic)", "p^hat = successes/trials", "", 
	"p = population proportion (paramater)", 0, 0, 0, 0, 0, 0, 
	
	"1 Sample Z-Prop Interval", "p^hat ± z*\xA8((p^hat*(1-p^hat))/""n)", 
	"Margin of Error = z*\xA8(.5*(1-.5)/""n)", "", 
	assumptions, "1) SRS", "2) Pop \x9E 10n", "3) n*p^hat \x9E 10\n  n*(1-p^hat) \x9E 10", 0, 0, 0, 
	
	"1 Sample Z-Prop Test", "Ho: P = Po", "z = (p^hat - Po)/\xA8(Po*(1-Po)/""n)", "", 
	assumptions, "1) SRS", "2) Pop \x9E 10n", "3) n*Po \x9E 10\n  n*(1-Po) \x9E 10", 0, 0, 0, 
	
	"2 Sample Z-Prop Interval", "(p^hat1-p^hat2) ± z*\xA8(p^hat1*(1-p^hat1)÷\nn1 + p^hat2*(1-p^hat2)/""n2)", "", 
	assumptions, "1) Independent SRS's", "2) Pop1 \x9E 10n1\n  Pop2 \x9E 10n2", 
	"3) n1*p^hat1, n2*p^hat2, n1*(1-p^hat1),", "n2*(1-p^hat2) all \x9E 5", 0, 0, 0, 
	
	"2 Sample Z-Prop Test", "Ho: p1 = p2 or p1 - p2 = 0", "p^hat = (x1 + x2)/(n1 + n2)", 
	"z = (p^hat1 - p^hat2)/\xA8(p^hat(1-p^hat)*(1/""n1+1/""n2))", "", 
	assumptions, "1) Independent SRS's", "2) Pop1 \x9E 10n1\n  Pop2 \x9E 10n2", 
	"3) n1*p^hat, n2*p^hat, n1*(1-p^hat),", "n2*(1-p^hat) all \x9E 5", 0, 
	
	"Categorical Counts", "List = Chi² Goodness of Fit", "", "Matrix/Table = Chi² Test", 0, 0, 0, 0, 0, 0, 0, 
	
	"Bivariate Data Plots", "y^hat = a + bx = \x80 + \x81x", "", 
	"resid = observed y - predicted y = y - y^hat", "", "SEb = s/\xA8(\x8F(x-\x9A)²)", 
	0, 0, 0, 0, 0, 
	
	"Linear-Regression T Test", "Ho: \x81 = 0", "t = (b - \x81)/SEb or (b - 0)/SEb", "", 
	assumptions, "1) Observations are independent", 
	"2) Linear Relationship - a) Scatterplot b) r-\n  value  c) resid plot d) Norm Probability Plot", 
	"3) Standard Devation (\x8F) same for all x", "  a) Residual Plot - Look for fanning", 
	"4) Response varies (y) normally for all x", 
	"  a) Modified Box Plot residuals\n  - No outliers, Symmetric", 
	
	"Random Variables", "\x8Fx² = Variance", "", "\x8Fx = Standard Deviation (\xA8variance)", 
	0, 0, 0, 0, 0, 0, 0, 
	
	"Z Normal Distribution", "normalcdf(LL, UL, M, \x8F)", "", "68-95-99.7 Rule", 0, 0, 0, 0, 0, 0, 0, 
	
	"t Distribution", "tcdf(LL, UL, df)", "", "df = n - 1", "", 
	"Higher df = More Normal = less area in tails", 0, 0, 0, 0, 0, 
	
	"Binomial Distribution", "binomcdf(n fixed trials, p, k)", "M = np and \x8F = \xA8(np(1-p))", "", 
	"Conditions:", "1) Success/Failure", "2) n is fixed", "3) p same for each trial", 
	"4) Trials are independent", 0, 0, 
	
	"Geometric Distribution", "geomcdf(p, nth trial)", "M = 1/p", "", 
	"Conditions:", "1) Success/Failure", "2) n is no trials Until success", "3) p same for each trial", 
	"4) Trials are independent", 0, 0, 
	
	"Chi² Distribution", "X²cdf(LL, UL, df)", "df = n - 1", "", "Takes only positive values", "", 
	"Always right skewed", "", "Higher df = more normal = less skewed", 0, 0, 
};//  ±  ÷  ²

//  \x8F is sigma
//  \xA8( is sqrt(
//  \x9A is x bar
//  \x9C is <=
//  \x9E is >=
//  \x80 is alpha
//  \x81 is beta

// Main Function
void _main(void)
{
	short page = 0, a;
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);
	INT_HANDLER interrupt5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5, DUMMY_HANDLER);
	
	do {
		clrscr();
		
		FontSetSys(F_6x8);
		printf("%s:\n", info[OFFSET*page]);
		FontSetSys(F_4x6);
		printf("\n");
		printf_xy(LCD_WIDTH - 22 - ((page > 8) << 2), 95, "(%d/%d)", page + 1, MAX_PAGES + 1);
		
		for(a = 1; a < OFFSET; a++)
			printf("%s\n", info[OFFSET*page + a]);
		
		while(_rowread(0));
		OSTimerRestart(APD_TIMER);
		while(!_rowread(0)) {
			idle();
			// Check for APD
			if(OSTimerExpired(APD_TIMER)) {
				off();
				OSTimerRestart(APD_TIMER);
			}
		}
		
		if (_keytest (RR_ESC) || _keytest (RR_F5))
			break;
		if (_keytest (RR_LEFT) && --page < 0)
			page = MAX_PAGES;
		if ((_keytest (RR_RIGHT) || _keytest (RR_ENTER)) && ++page > MAX_PAGES)
			page = 0;
		
	} while(1);
	
	while(_rowread(0));
	SetIntVec(AUTO_INT_1, interrupt1);  // Restore default interrupt handlers
	SetIntVec(AUTO_INT_5, interrupt5);
}