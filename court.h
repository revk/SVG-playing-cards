const char *Black_path[16]={
	NULL,	// JS
	NULL,	// QS
	NULL,	// KS
	NULL,	// JH
	NULL,	// QH
	NULL,	// KH
	NULL,	// JC
	NULL,	// QC
	NULL,	// KC
	NULL,	// JD
	NULL,	// QD
	NULL,	// KD
	NULL,	// 1J
	NULL,	// 2J
	NULL,	// AS
	NULL,	// 1B
};

const char *Blue_path[16]={
	NULL,	// JS
	NULL,	// QS
	NULL,	// KS
	NULL,	// JH
	NULL,	// QH
	NULL,	// KH
	NULL,	// JC
	NULL,	// QC
	NULL,	// KC
	NULL,	// JD
	NULL,	// QD
	NULL,	// KD
	NULL,	// 1J
	NULL,	// 2J
	NULL,	// AS
	NULL,	// 1B
};

const char *Red_path[16]={
	NULL,	// JS
	NULL,	// QS
	NULL,	// KS
	NULL,	// JH
	NULL,	// QH
	NULL,	// KH
	NULL,	// JC
	NULL,	// QC
	NULL,	// KC
	NULL,	// JD
	NULL,	// QD
	NULL,	// KD
	NULL,	// 1J
	NULL,	// 2J
	NULL,	// AS
	NULL,	// 1B
};

const char *Gold_path[16]={
	NULL,	// JS
	NULL,	// QS
	NULL,	// KS
	NULL,	// JH
	NULL,	// QH
	NULL,	// KH
	NULL,	// JC
	NULL,	// QC
	NULL,	// KC
	NULL,	// JD
	NULL,	// QD
	NULL,	// KD
	NULL,	// 1J
	NULL,	// 2J
	NULL,	// AS
	NULL,	// 1B
};

const char *Stroke_path[16]={
	NULL,	// JS
	NULL,	// QS
	NULL,	// KS
	NULL,	// JH
	NULL,	// QH
	NULL,	// KH
	NULL,	// JC
	NULL,	// QC
	NULL,	// KC
	NULL,	// JD
	NULL,	// QD
	NULL,	// KD
	NULL,	// 1J
	NULL,	// 2J
	NULL,	// AS
	NULL,	// 1B
};

const char *Thin_path[16]={
	NULL,	// JS
	NULL,	// QS
	NULL,	// KS
	NULL,	// JH
	NULL,	// QH
	NULL,	// KH
	NULL,	// JC
	NULL,	// QC
	NULL,	// KC
	NULL,	// JD
	NULL,	// QD
	NULL,	// KD
	NULL,	// 1J
	NULL,	// 2J
	NULL,	// AS
	NULL,	// 1B
};

struct {int x,y,s,r,border;} pips[16][20]={
	{{910,1454,75,35},{845,1359,75,33},{780,1261,75,33}},	// JS
	{{1188,1065,90,-40},{1194,957,90,-40},{1096,967,90,-40},{1022,1053,90,-40},{918,1149,90,-40},{897,1274,90,-40}},	// QS
	{{495,1388,90,-10},{528,1249,90,-5},{839,1360,90,20},{795,1251,90,20}},	// KS
	{{546,1000,100,0},{615,1000,100,180},{883,1224,75,30},{915,1289,75,30},{954,1354,75,30},{220,1367,75,60},{155,1272,75,60},{52,1272,75,60},{96,1175,75,60}},	// JH
	{{967,812,75,30},{1013,933,75,15},{1041,1058,75,3},{1054,1184,75,5},{1068,1301,75,10}},	// QH
	{{988,1199,90,85},{958,1298,90,60},{975,1089,90,110},{922,997,90,130}},	// KH
	{{504,1272,75,0},{504,1156,75,0},{826,1357,75,0}},	// JC
	{{1017,1099,90,-45},{1125,1022,90,-45},{1229,940,90,-45}},	// QC
	{{893,1141,90,5},{927,1279,90,15},{992,1399,90,25}},	// KC
	{{650,1000,200,0,1},{894,1402,50,-26},{852,1377,50,-22},{806,1361,50,-14},{757,1350,50,-6},{706,1345,50,0},{656,1347,50,5},{607,1353,50,10},{560,1363,50,15}},	// JD
	{{650,1000,100,0,1},{580,995,100,0,1},{510,990,100,0,1}},	// QD
	{{650,1000,120,0},{570,1000,120,0},{400,1360,170,-15,1},{435,1235,150,-10,1},{453,1115,130,-5,1},{460,1018,110,0,1},{450,930,100,10,1},{1259,1000,90,60},{1173,948,90,50},{1094,880,90,40},{1038,791,90,30}},	// KD
	{},	// 1J
	{},	// 2J
	{},	// AS
	{},	// 1B
};
