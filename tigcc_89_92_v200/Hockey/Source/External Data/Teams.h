// Header File
// Created 11/12/2003; 6:43:46 AM
unsigned char Starting_Pos[9][2] = {  // Three Team Formations
	{4, 6}, {13, 5}, {12, 8}, 
	{4, 6}, {12, 4}, {12, 10}, 
	{4, 6}, {13, 7}, {8, 7}, 
};

// Each team in the Western and Eastern conference is a Names struct
typedef struct {
	char TeamName[13];
	char PlrNames[3][23];
	char Formation;
	char Types[2];
} Names;

#define Average								0
#define Fat										10
#define Skinny							 -1

/* Most of these were taken from  http://hockey.mrsports.com/natl/nhlrosters.cfm 
	 Also, most of the names are from the 2001-2002 season */
Names Western[16] = {
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

Names Eastern[15] = {
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