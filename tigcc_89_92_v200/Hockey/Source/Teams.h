// Header File
// Created 10/10/2003; 9:46:35 PM

unsigned char Starting_Pos[9][2] = {  // Three Team Formations
	{4, 6}, {13, 5}, {12, 8}, 
	{4, 6}, {12, 4}, {12, 10}, 
	{4, 6}, {13, 7}, {8, 7}, 
};

/* Most of these were taken from  http://hockey.mrsports.com/natl/nhlrosters.cfm 
	 Also, most of the names are from the 2001-2002 season */
Names Default_Western[16] = {
	{ "Anaheim", { "Jean Sebastien Giguere", "Jason York", "Adam Oates" }, 0, { Skinny, Average } }, 
	{ "Calgary", { "Roman Turek", "Jarome Iginla", "Craig Conroy" }, 1, { Fat, Fat } }, 
	{ "Chicago", { "Jocelyn Thibault", "Theo Fleury", "Alexei Zhamnov" }, 2, { Average, Average } }, 
	{ "Colorado", { "Patrick Roy", "Rob Blake", "Joe Sakic" }, 2, { Average, Fat } }, 
	{ "Columbus", { "Marc Denis", "Ray Whitney", "Mike Sillinger" }, 1, { Skinny, Fat } }, 
	{ "Dallas", { "Ron Tugnutt", "Bill Guerin", "Mike Modano" }, 0, { Fat, Fat } }, 
	{ "Detroit", { "Dominik Hasek", "Brendan Shanahan", "Sergei Fedorov" }, 1, { Skinny, Skinny } }, 
	{ "Edmonton", { "Tommy Salo", "Janne Niinimaa", "Mike Comrie" }, 2, { Fat, Average } }, 
	{ "Los Angeles", { "Felix Potvin", "Jaroslav Modry", "Jason Allison" }, 1, { Average, Average } }, 
	{ "Minnesota", { "Dwayne Roloson", "Marian Gaborik", "Jim Dowd" }, 0, { Skinny, Average } }, 
	{ "Nashville", { "Mike Dunham", "Kimmo Timonen", "Greg Johnson" }, 0, { Fat, Fat } }, 
	{ "Phoenix", { "Sean Burke", "Tony Amonte", "Daniel Briere" }, 1, { Skinny, Fat } }, 
	{ "San Jose", { "Evgeni Nabokov", "Gary Suter", "Vincent Damphousse" }, 2, { Average, Fat } }, 
	{ "St. Louis", { "Brent Johnson", "Keith Tkachuk", "Pavol Demitra" }, 1, { Skinny, Skinny } }, 
	{ "Vancouver", { "Dan Cloutier", "Todd Bertuzzi", "Markus Naslund" }, 2, { Fat, Fat } }, 
	{ "All-Stars", { "Patrick Roy", "Rob Blake", "Mike Modano" }, 0, { Average, Skinny } }, 
};

Names Default_Eastern[15] = {
	{ "Atlanta", { "Milan Hnilicka", "Dany Heatley", "Tony Hrkac" }, 1, { Fat, Fat } }, 
	{ "Boston", { "Byron Dafoe", "Glen Murray", "Brian Rolston" }, 2, { Average, Skinny } }, 
	{ "Buffalo", { "Martin Biron", "Miroslav Satan", "Curtis Brown" }, 0, { Fat, Average } }, 
	{ "Carolina", { "Arturs Irbe", "Sami Kapanen", "Ron Francis" }, 0, { Skinny, Skinny } }, 
	{ "Florida", { "Roberto Luongo", "Sandis Ozolinsh", "Kristian Huselius" }, 2, { Average, Average } }, 
	{ "Montreal", { "Jose Theodore", "Mariusz Czerkawski", "Yanic Perreault" }, 1, { Fat, Skinny } }, 
	{ "New Jersey", { "Martin Brodeur", "Brian Rafalski", "Joe Nieuwendyk" }, 1, { Average, Average } }, 
	{ "New York", { "Mike Richter", "Pavel Bure", "Eric Lindros" }, 2, { Skinny, Average } }, 
	{ "Ottowa", { "Patrick Lalime", "Daniel Alfredsson", "Radek Bonk" }, 0, { Average, Fat } }, 
	{ "Philadelphia", { "Roman Cechmanek", "Kim Johnsson", "Simon Gagne" }, 1, { Fat, Fat } }, 
	{ "Pittsburgh", { "Johan Hedberg", "Michal Rozsival", "Alexei Kovalev" }, 0, { Skinny, Average } }, 
	{ "Tampa Bay", { "Nikolai Khabibulin", "Pavel Kubina", "Brad Richards" }, 0, { Average, Fat } }, 
	{ "Toronto", { "Ed Belfour", "Bryan McCabe", "Mats Sundin" }, 2, { Skinny, Skinny } }, 
	{ "Washington", { "Olaf Kolzig", "Jaromir Jagr", "Robert Lang" }, 0, { Fat, Skinny } }, 
	{ "All-Stars", { "Nikolai Khabibulin", "Brian Leetch", "Jaromir Jagr" }, 1, { Average, Average } }, 
};

#if 0
typedef struct {
	char Formation;
	char Types[2];
} Names;


/* Most of these were taken from  http://hockey.mrsports.com/natl/nhlrosters.cfm 
	 Also, most of the names are from the 2001-2002 season */
Names Default_Western[16] = {
	{ 0, { Skinny, Average } }, 
	{ 1, { Fat, Fat } }, 
	{ 2, { Average, Average } }, 
	{ 2, { Average, Fat } }, 
	{ 1, { Skinny, Fat } }, 
	{ 0, { Fat, Fat } }, 
	{ 1, { Skinny, Skinny } }, 
	{ 2, { Fat, Average } }, 
	{ 1, { Average, Average } }, 
	{ 0, { Skinny, Average } }, 
	{ 0, { Fat, Fat } }, 
	{ 1, { Skinny, Fat } }, 
	{ 2, { Average, Fat } }, 
	{ 1, { Skinny, Skinny } }, 
	{ 2, { Fat, Fat } }, 
	{ 0, { Average, Skinny } }, 
};

Names Default_Eastern[15] = {
	{ 1, { Fat, Fat } }, 
	{ 2, { Average, Skinny } }, 
	{ 0, { Fat, Average } }, 
	{ 0, { Skinny, Skinny } }, 
	{ 2, { Average, Average } }, 
	{ 1, { Fat, Skinny } }, 
	{ 1, { Average, Average } }, 
	{ 2, { Skinny, Average } }, 
	{ 0, { Average, Fat } }, 
	{ 1, { Fat, Fat } }, 
	{ 0, { Skinny, Average } }, 
	{ 0, { Average, Fat } }, 
	{ 2, { Skinny, Skinny } }, 
	{ 0, { Fat, Skinny } }, 
	{ 1, { Average, Average } }, 
};

// Number of strings 124
asm("
names:
.word names0   - names
.word names1   - names
.word names2   - names
.word names3   - names
.word names4   - names
.word names5   - names
.word names6   - names
.word names7   - names
.word names8   - names
.word names9   - names
.word names10  - names
.word names11  - names
.word names12  - names
.word names13  - names
.word names14  - names
.word names15  - names
.word names16  - names
.word names17  - names
.word names18  - names
.word names19  - names
.word names20  - names
.word names21  - names
.word names22  - names
.word names23  - names
.word names24  - names
.word names25  - names
.word names26  - names
.word names27  - names
.word names28  - names
.word names29  - names
.word names30  - names
.word names31  - names
.word names32  - names
.word names33  - names
.word names34  - names
.word names35  - names
.word names36  - names
.word names37  - names
.word names38  - names
.word names39  - names
.word names40  - names
.word names41  - names
.word names42  - names
.word names43  - names
.word names44  - names
.word names45  - names
.word names46  - names
.word names47  - names
.word names48  - names
.word names49  - names
.word names50  - names
.word names51  - names
.word names52  - names
.word names53  - names
.word names54  - names
.word names55  - names
.word names56  - names
.word names57  - names
.word names58  - names
.word names59  - names
.word names60  - names
.word names13  - names
.word names14  - names
.word names23  - names
.word names61  - names
.word names62  - names
.word names63  - names
.word names64  - names
.word names65  - names
.word names66  - names
.word names67  - names
.word names68  - names
.word names69  - names
.word names70  - names
.word names71  - names
.word names72  - names
.word names73  - names
.word names74  - names
.word names75  - names
.word names76  - names
.word names77  - names
.word names78  - names
.word names79  - names
.word names80  - names
.word names81  - names
.word names82  - names
.word names83  - names
.word names84  - names
.word names85  - names
.word names86  - names
.word names87  - names
.word names88  - names
.word names89  - names
.word names90  - names
.word names91  - names
.word names92  - names
.word names93  - names
.word names94  - names
.word names95  - names
.word names96  - names
.word names97  - names
.word names98  - names
.word names99  - names
.word names100 - names
.word names101 - names
.word names102 - names
.word names103 - names
.word names104 - names
.word names105 - names
.word names106 - names
.word names107 - names
.word names108 - names
.word names109 - names
.word names110 - names
.word names111 - names
.word names112 - names
.word names113 - names
.word names114 - names
.word names115 - names
.word names116 - names
.word names60  - names
.word names109 - names
.word names117 - names
.word names118 - names

names0:
.asciz \"Anaheim\"
names1:
.asciz \"Jean Sebastien Giguere\"
names2:
.asciz \"Jason York\"
names3:
.asciz \"Adam Oates\"
names4:
.asciz \"Calgary\"
names5:
.asciz \"Roman Turek\"
names6:
.asciz \"Jarome Iginla\"
names7:
.asciz \"Craig Conroy\"
names8:
.asciz \"Chicago\"
names9:
.asciz \"Jocelyn Thibault\"
names10:
.asciz \"Theo Fleury\"
names11:
.asciz \"Alexei Zhamnov\"
names12:
.asciz \"Colorado\"
names13:
.asciz \"Patrick Roy\"
names14:
.asciz \"Rob Blake\"
names15:
.asciz \"Joe Sakic\"
names16:
.asciz \"Columbus\"
names17:
.asciz \"Marc Denis\"
names18:
.asciz \"Ray Whitney\"
names19:
.asciz \"Mike Sillinger\"
names20:
.asciz \"Dallas\"
names21:
.asciz \"Ron Tugnutt\"
names22:
.asciz \"Bill Guerin\"
names23:
.asciz \"Mike Modano\"
names24:
.asciz \"Detroit\"
names25:
.asciz \"Dominik Hasek\"
names26:
.asciz \"Brendan Shanahan\"
names27:
.asciz \"Sergei Fedorov\"
names28:
.asciz \"Edmonton\"
names29:
.asciz \"Tommy Salo\"
names30:
.asciz \"Janne Niinimaa\"
names31:
.asciz \"Mike Comrie\"
names32:
.asciz \"Los Angeles\"
names33:
.asciz \"Felix Potvin\"
names34:
.asciz \"Jaroslav Modry\"
names35:
.asciz \"Jason Allison\"
names36:
.asciz \"Minnesota\"
names37:
.asciz \"Dwayne Roloson\"
names38:
.asciz \"Marian Gaborik\"
names39:
.asciz \"Jim Dowd\"
names40:
.asciz \"Nashville\"
names41:
.asciz \"Mike Dunham\"
names42:
.asciz \"Kimmo Timonen\"
names43:
.asciz \"Greg Johnson\"
names44:
.asciz \"Phoenix\"
names45:
.asciz \"Sean Burke\"
names46:
.asciz \"Tony Amonte\"
names47:
.asciz \"Daniel Briere\"
names48:
.asciz \"San Jose\"
names49:
.asciz \"Evgeni Nabokov\"
names50:
.asciz \"Gary Suter\"
names51:
.asciz \"Vincent Damphousse\"
names52:
.asciz \"St. Louis\"
names53:
.asciz \"Brent Johnson\"
names54:
.asciz \"Keith Tkachuk\"
names55:
.asciz \"Pavol Demitra\"
names56:
.asciz \"Vancouver\"
names57:
.asciz \"Dan Cloutier\"
names58:
.asciz \"Todd Bertuzzi\"
names59:
.asciz \"Markus Naslund\"
names60:
.asciz \"All-Stars\"
names61:
.asciz \"Atlanta\"
names62:
.asciz \"Milan Hnilicka\"
names63:
.asciz \"Dany Heatley\"
names64:
.asciz \"Tony Hrkac\"
names65:
.asciz \"Boston\"
names66:
.asciz \"Byron Dafoe\"
names67:
.asciz \"Glen Murray\"
names68:
.asciz \"Brian Rolston\"
names69:
.asciz \"Buffalo\"
names70:
.asciz \"Martin Biron\"
names71:
.asciz \"Miroslav Satan\"
names72:
.asciz \"Curtis Brown\"
names73:
.asciz \"Carolina\"
names74:
.asciz \"Arturs Irbe\"
names75:
.asciz \"Sami Kapanen\"
names76:
.asciz \"Ron Francis\"
names77:
.asciz \"Florida\"
names78:
.asciz \"Roberto Luongo\"
names79:
.asciz \"Sandis Ozolinsh\"
names80:
.asciz \"Kristian Huselius\"
names81:
.asciz \"Montreal\"
names82:
.asciz \"Jose Theodore\"
names83:
.asciz \"Mariusz Czerkawski\"
names84:
.asciz \"Yanic Perreault\"
names85:
.asciz \"New Jersey\"
names86:
.asciz \"Martin Brodeur\"
names87:
.asciz \"Brian Rafalski\"
names88:
.asciz \"Joe Nieuwendyk\"
names89:
.asciz \"New York\"
names90:
.asciz \"Mike Richter\"
names91:
.asciz \"Pavel Bure\"
names92:
.asciz \"Eric Lindros\"
names93:
.asciz \"Ottowa\"
names94:
.asciz \"Patrick Lalime\"
names95:
.asciz \"Daniel Alfredsson\"
names96:
.asciz \"Radek Bonk\"
names97:
.asciz \"Philadelphia\"
names98:
.asciz \"Roman Cechmanek\"
names99:
.asciz \"Kim Johnsson\"
names100:
.asciz \"Simon Gagne\"
names101:
.asciz \"Pittsburgh\"
names102:
.asciz \"Johan Hedberg\"
names103:
.asciz \"Michal Rozsival\"
names104:
.asciz \"Alexei Kovalev\"
names105:
.asciz \"Tampa Bay\"
names106:
.asciz \"Nikolai Khabibulin\"
names107:
.asciz \"Pavel Kubina\"
names108:
.asciz \"Brad Richards\"
names109:
.asciz \"Toronto\"
names110:
.asciz \"Ed Belfour\"
names111:
.asciz \"Bryan McCabe\"
names112:
.asciz \"Mats Sundin\"
names113:
.asciz \"Washington\"
names114:
.asciz \"Olaf Kolzig\"
names115:
.asciz \"Jaromir Jagr\"
names116:
.asciz \"Robert Lang\"
names117:
.asciz \"Brian Leetch\"
");

#endif