
#include "StdAfx.h"
#include "Setup.h"


#ifdef WIN32
#pragma warning(disable:4305)	// warning C4305: 'argument' : truncation from 'double' to 'float'
#endif


// Learn all command, straight from level3.cpp
void learnAll( Player *plr )
{
	static uint32 spellarray[DRUID+1][9999] =
	{
		{ 0 },		// CLASS 0
		{ 71, 2458, 355, 469, 674, 676, 750, 871, 1161, 1680, 1719, 2048, 2565, 2687, 3411, 5246, 6554, 7620, 11578, 11585, 12678, 18499, 20230, 23920, 25203, 25208, 25212, 25225, 25231, 25236, 25242, 25264, 25266, 25275, 29704, 29707, 30357, 34428, 0 },      // CLASS 1
		{ 750, 1020, 1038, 1044, 1152, 3127, 4987, 5502, 5573, 7620, 10278, 10308, 10322, 10324, 10326, 19746, 19752, 20271, 20347, 20348, 20356, 20772, 20773, 25780, 25895, 25898, 27136, 27137, 27138, 27139, 27140, 27141, 27142, 27143, 27144, 27145, 27148, 27149, 27150, 27151, 27152, 27153, 27154, 27155, 27158, 27169, 27173, 27180, 31789, 31801, 31884, 31895, 32223, 33388, 33391, 33776, 34090, 34091, 0 },      // CLASS 2
		{ 136, 674, 781, 883, 982, 1002, 1130, 1462, 1494, 1495, 1499, 1510, 1513, 1515, 1543, 1978, 2641, 2643, 2974, 3018, 3034, 3043, 3044, 3045, 3111, 3127, 3661, 3662, 5116, 5118, 5384, 6197, 6991, 13159, 13161, 13163, 13165, 13542, 13543, 13544, 13549, 13550, 13551, 13552, 13553, 13554, 13555, 13795, 13809, 13813, 14260, 14261, 14262, 14263, 14264, 14265, 14266, 14267, 14268, 14269, 14270, 14271, 14272, 14273, 14274, 14279, 14280, 14281, 14282, 14283, 14284, 14285, 14286, 14287, 14288, 14289, 14290, 14294, 14295, 14302, 14303, 14304, 14305, 14310, 14311, 14316, 14317, 14318, 14319, 14320, 14321, 14322, 14323, 14324, 14325, 14326, 14327, 15629, 15630, 15631, 15632, 19801, 19878, 19879, 19880, 19882, 19883, 19884, 19885, 20043, 20190, 20736, 25294, 25295, 25296, 27014, 27015, 27016, 27018, 27019, 27020, 27021, 27022, 27023, 27025, 27044, 27045, 27046, 34026, 34074, 34120, 34477, 34600, 36916, 0 },      // CLASS 3
		{ 674, 921, 1725, 1787, 1804, 1833, 1842, 1860, 2094, 2836, 2842, 3127, 5938, 6774, 8643, 8681, 11297, 11305, 26669, 26679, 26862, 26863, 26865, 26866, 26867, 26884, 26889, 27441, 27448, 31224, 32684, 38764, 38768, 0 },      // CLASS 4
		{ 17, 139, 453, 527, 528, 552, 586, 588, 589, 591, 592, 594, 596, 598, 600, 602, 605, 970, 976, 984, 988, 992, 996, 1004, 1006, 1243, 1244, 1245, 1706, 2006, 2010, 2052, 2053, 2054, 2055, 2060, 2061, 2096, 2767, 2791, 3747, 6060, 6063, 6064, 6065, 6066, 6074, 6075, 6076, 6077, 6078, 7128, 8092, 8102, 8103, 8104, 8105, 8106, 8122, 8124, 8129, 8131, 8192, 9472, 9473, 9474, 9484, 9485, 9578, 9579, 9592, 10874, 10875, 10876, 10880, 10881, 10888, 10890, 10892, 10893, 10894, 10898, 10899, 10900, 10901, 10909, 10911, 10912, 10915, 10916, 10917, 10927, 10928, 10929, 10933, 10934, 10937, 10938, 10941, 10942, 10945, 10946, 10947, 10951, 10952, 10953, 10955, 10957, 10960, 10961, 10963, 10964, 10965, 14914, 15261, 15262, 15263, 15264, 15265, 15266, 15267, 19289, 19291, 19292, 19293, 20770, 25210, 25213, 25217, 25218, 25221, 25222, 25233, 25235, 25308, 25314, 25315, 25316, 25363, 25364, 25367, 25368, 25372, 25375, 25379, 25380, 25384, 25389, 25429, 25431, 25435, 25450, 25596, 32375, 32379, 32546, 32996, 33076, 34433, 0 },      // CLASS 5
		{ 0 },		// CLASS 6
		{ 131, 324, 325, 332, 370, 421, 526, 529, 546, 547, 548, 556, 905, 913, 915, 930, 939, 943, 945, 959, 1064, 1535, 2008, 2484, 2645, 2860, 2870, 3599, 5394, 5675, 5730, 6041, 6196, 6363, 6364, 6365, 6375, 6377, 6390, 6391, 6392, 6495, 8004, 8005, 8008, 8010, 8012, 8017, 8024, 8027, 8030, 8033, 8038, 8042, 8044, 8045, 8046, 8050, 8052, 8053, 8056, 8058, 8071, 8075, 8134, 8143, 8154, 8155, 8160, 8161, 8166, 8170, 8177, 8181, 8184, 8190, 8227, 8232, 8235, 8249, 8498, 8499, 8512, 8835, 10391, 10392, 10395, 10396, 10406, 10407, 10408, 10412, 10413, 10414, 10427, 10428, 10431, 10432, 10437, 10438, 10442, 10447, 10448, 10456, 10462, 10463, 10466, 10467, 10468, 10472, 10473, 10478, 10479, 10486, 10495, 10496, 10497, 10526, 10537, 10538, 10585, 10595, 10600, 10601, 10605, 10613, 10622, 10623, 10627, 11314, 11315, 15107, 15111, 15112, 15207, 15208, 16339, 16341, 16342, 16355, 16356, 16362, 16387, 20608, 20609, 20610, 20776, 20777, 24398, 25357, 25359, 25361, 25391, 25396, 25420, 25422, 25423, 25439, 25442, 25448, 25449, 25454, 25457, 25464, 25469, 25472, 25489, 25500, 25505, 25508, 25509, 25525, 25528, 25533, 25546, 25547, 25557, 25560, 25563, 25567, 25570, 25574, 25577, 25908, 29228, 33736, 36936, 0 },      // CLASS 7
		{ 10, 66, 116, 118, 120, 122, 130, 143, 145, 205, 475, 543, 587, 597, 604, 759, 837, 865, 990, 1008, 1449, 1459, 1460, 1461, 1463, 1953, 2120, 2121, 2136, 2137, 2138, 2139, 2855, 2948, 3140, 3552, 5143, 5144, 5145, 5504, 5505, 5506, 6117, 6127, 6129, 6131, 6141, 6143, 7300, 7301, 7302, 7320, 7322, 8400, 8401, 8402, 8406, 8407, 8408, 8412, 8413, 8416, 8417, 8422, 8423, 8427, 8437, 8438, 8439, 8444, 8445, 8446, 8450, 8451, 8455, 8457, 8458, 8461, 8462, 8492, 8494, 8495, 10053, 10054, 10138, 10139, 10140, 10144, 10145, 10148, 10149, 10150, 10151, 10156, 10157, 10159, 10160, 10161, 10169, 10170, 10173, 10174, 10177, 10179, 10180, 10181, 10185, 10186, 10187, 10191, 10192, 10193, 10197, 10199, 10201, 10202, 10205, 10206, 10207, 10211, 10212, 10215, 10216, 10219, 10220, 10223, 10225, 10230, 12051, 12824, 12825, 12826, 22782, 22783, 25304, 25306, 25345, 27070, 27071, 27072, 27073, 27074, 27075, 27078, 27079, 27080, 27082, 27085, 27086, 27087, 27088, 27101, 27124, 27125, 27126, 27128, 27130, 27131, 28609, 28612, 30449, 30451, 30455, 30482, 32796, 33717, 33944, 33946, 37420, 38699, 45438, 0 },      // CLASS 8
		{ 126, 132, 172, 348, 603, 688, 689, 691, 693, 695, 696, 697, 698, 699, 702, 704, 705, 706, 707, 709, 710, 712, 755, 980, 1010, 1014, 1086, 1088, 1094, 1098, 1106, 1108, 1120, 1122, 1454, 1455, 1456, 1490, 1714, 1949, 2362, 2941, 3698, 3699, 3700, 5138, 5484, 5500, 5676, 5697, 5699, 5740, 5782, 5784, 6201, 6202, 6205, 6213, 6215, 6217, 6219, 6222, 6223, 6226, 6229, 6353, 6366, 6789, 7641, 7646, 7648, 7651, 7658, 7659, 8288, 8289, 11659, 11660, 11661, 11665, 11667, 11668, 11671, 11672, 11675, 11677, 11678, 11683, 11684, 11687, 11688, 11689, 11693, 11694, 11695, 11699, 11700, 11703, 11704, 11707, 11708, 11711, 11712, 11713, 11717, 11719, 11721, 11722, 11725, 11726, 11729, 11730, 11733, 11734, 11735, 11739, 11740, 17727, 17728, 17862, 17919, 17920, 17921, 17922, 17923, 17924, 17925, 17926, 17928, 17937, 18540, 18647, 20752, 20755, 20756, 20757, 23161, 25307, 25309, 25311, 27209, 27210, 27211, 27212, 27213, 27215, 27216, 27217, 27218, 27219, 27220, 27221, 27222, 27223, 27224, 27226, 27228, 27229, 27230, 27238, 27243, 27259, 27260, 28172, 28176, 28189, 28610, 29722, 29858, 29893, 30459, 30545, 30908, 30909, 30910, 32231, 0 },      // CLASS 9
		{ 0 },		// CLASS 10
		{ 99, 339, 467, 740, 768, 769, 770, 774, 778, 779, 780, 782, 783, 1058, 1062, 1066, 1075, 1079, 1082, 1126, 1178, 1430, 1735, 1822, 1823, 1824, 1850, 2090, 2091, 2637, 2782, 2893, 2908, 2912, 3025, 3029, 3137, 3627, 5177, 5178, 5179, 5180, 5186, 5187, 5188, 5189, 5195, 5196, 5201, 5209, 5211, 5215, 5217, 5221, 5225, 5229, 5232, 5234, 6756, 6778, 6780, 6783, 6785, 6787, 6793, 6795, 6798, 6800, 6807, 6808, 6809, 8903, 8905, 8907, 8910, 8914, 8918, 8921, 8924, 8925, 8926, 8927, 8928, 8929, 8936, 8938, 8939, 8940, 8941, 8946, 8949, 8950, 8951, 8955, 8972, 8983, 8992, 8998, 9005, 9490, 9492, 9493, 9634, 9745, 9747, 9749, 9750, 9752, 9754, 9756, 9758, 9821, 9823, 9827, 9829, 9830, 9833, 9834, 9835, 9839, 9840, 9841, 9845, 9846, 9849, 9850, 9852, 9853, 9856, 9857, 9858, 9862, 9863, 9866, 9867, 9875, 9876, 9880, 9881, 9884, 9885, 9888, 9889, 9894, 9896, 9898, 9901, 9904, 9907, 9908, 9910, 9912, 9913, 16914, 16952, 16954, 16958, 16961, 17401, 17402, 18657, 18658, 18960, 20484, 20719, 20739, 20742, 20747, 20748, 22568, 22570, 22812, 22827, 22828, 22829, 22842, 22895, 22896, 24248, 25297, 25298, 25299, 26978, 26979, 26980, 26981, 26982, 26983, 26984, 26985, 26986, 26987, 26988, 26989, 26990, 26992, 26993, 26994, 26995, 26996, 26997, 26998, 26999, 27000, 27001, 27002, 27003, 27005, 27006, 27008, 27012, 29166, 31018, 33357, 33745, 33763, 33786, 33943, 34090, 0 },      // CLASS 11
	};

	uint32 c = plr->getClass();
	for(uint32 i = 0; spellarray[c][i] != 0; ++i)
	{
		plr->addSpell(spellarray[c][i]);
	}

	static uint32 paladinspellarray[RACE_DRAENEI+1][2] =
	{
		{ 0 },				// RACE 0
		{ 13819, 0 },		// HUMAN
		{ 0 },				// ORC
		{ 13819, 0 },		// DWARF
		{ 0 },				// NIGHTELF
		{ 0 },				// UNDEAD
		{ 0 },				// TAUREN
		{ 0 },				// GNOME
		{ 0 },				// TROLL
		{ 34769, 0 },		// BLOODELF
		{ 13819, 0 },		// DRAENEI
	};

	static uint32 priestspellarray[RACE_DRAENEI+1][16] =
	{
		{ 0 },		// RACE 0
		{ 13896, 13908, 19236, 19238, 19240, 19241, 19242, 19243, 19271, 19273, 19274, 19275, 25437, 25441, 0 },      // HUMAN
		{ 0 },		// ORC
		{ 13908, 19236, 19238, 19240, 44041, 19241, 19242, 19243, 25437, 44043, 44044, 44045, 44046, 44047, 0 },      // DWARF
		{ 2651, 10797, 19296, 19299, 19302, 19303, 19304, 19305, 25446, 0 },      // NIGHTELF
		{ 2652, 2944, 19261, 19262, 19264, 19265, 19266, 19276, 19277, 19278, 19279, 19280, 25461, 25467, 0 },      // UNDEAD
		{ 0 },		// TAUREN
		{ 0 },		// GNOME
		{ 9035, 18137, 19281, 19282, 19283, 19284, 19285, 19308, 19309, 19310, 19311, 19312, 25470, 25477, 0 },      // TROLL
		{ 2652, 19261, 19262, 19264, 19265, 19266, 25461, 32676, 0 },      // BLOODELF
		{ 32548, 44041, 44043, 44044, 44045, 44046, 44047, 0 },      // DRAENEI
	};

	static uint32 shamanspellarray[RACE_DRAENEI+1][2] =
	{
			{ 0 },			// RACE 0
			{ 0 },			// HUMAN
			{ 2825, 0 },	// ORC
			{ 0 },			// DWARF
			{ 0 },			// NIGHTELF
			{ 0 },			// UNDEAD
			{ 2825, 0 },	// TAUREN
			{ 0 },			// GNOME
			{ 2825, 0 },	// TROLL
			{ 0 },			// BLOODELF
			{ 32182, 0 },	// DRAENEI
	};

	static uint32 magespellarray[RACE_DRAENEI+1][11] =
	{
			{ 0 },			// RACE 0
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// HUMAN
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// ORC
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// DWARF
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// NIGHTELF
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// UNDEAD
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// TAUREN
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// GNOME
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// TROLL
			{ 3563, 3566, 3567, 11417, 11418, 11420, 32267, 32272, 35715, 35717, 0 },		// BLOODELF
			{ 3561, 3562, 3565, 10059, 11416, 11419, 32266, 32271, 33690, 33691, 0 },		// DRAENEI
	};

		uint32 r = plr->getRace();
		switch(c)
		{
			case PALADIN:
				for( uint32 i = 0; paladinspellarray[r][i] != 0; ++i )
				{
					plr->addSpell( paladinspellarray[r][i] );
				}
				break;
			case PRIEST:
				for( uint32 i = 0; priestspellarray[r][i] != 0; ++i )
				{
					plr->addSpell( priestspellarray[r][i] );
				}
				break;
			case MAGE:
				for( uint32 i = 0; magespellarray[r][i] != 0; ++i )
				{
					plr->addSpell( magespellarray[r][i] );
				}
				break;
			case SHAMAN:
				for( uint32 i = 0; shamanspellarray[r][i] != 0; ++i )
				{
					plr->addSpell( shamanspellarray[r][i] );
				}
				break;
		}
	}


void PlayerFirstEnter(Player * pPlayer)
{
/*
	Look at arcemu-optional.conf for starting level:
	<Optional    StartingLevel = "1"
	Set it to 70:
	<Optional    StartingLevel = "70"
*/
	pPlayer->SetUInt32Value(PLAYER_FIELD_COINAGE,200000000); // Set gold
	learnAll( pPlayer );

/*
	// Shaman Totems
	uint32 PlayerClass = pPlayer->getClass();
	switch (PlayerClass)
	{
		case SHAMAN:
		{
			Item * pItem = objmgr.CreateItem(5178, pPlayer);
			pPlayer->GetItemInterface()->AddItemToFreeSlot(pItem); //Air Totem
			pItem = objmgr.CreateItem(5177, pPlayer);
			pPlayer->GetItemInterface()->AddItemToFreeSlot(pItem); //Water Totem
			pItem = objmgr.CreateItem(5176, pPlayer);
			pPlayer->GetItemInterface()->AddItemToFreeSlot(pItem); //Fire Totem
			pItem = objmgr.CreateItem(5175, pPlayer);
			pPlayer->GetItemInterface()->AddItemToFreeSlot(pItem); //Earth Totem
			delete pItem;
		}
		break;
	}
*/

}

void PlayerFirstCreate(Player * pPlayer)
{
	pPlayer->SetUInt32Value(UNIT_FIELD_HEALTH, pPlayer->GetUInt32Value(UNIT_FIELD_MAXHEALTH)); // Health

	// Explore all maps *wink*
	for (uint8 i=0; i<64; i++)
	{
	pPlayer->SetFlag(PLAYER_EXPLORED_ZONES_1+i,0xFFFFFFFF);
	}

	// Taximask
	for (uint8 i=0; i<8; i++)
	pPlayer->SetTaximask(i, 0xFFFFFFFF);

}

void SetupInstant70(ScriptMgr * mgr)
{
	mgr->register_hook(SERVER_HOOK_EVENT_ON_FIRST_ENTER_WORLD, (void*)PlayerFirstEnter);
	mgr->register_hook(SERVER_HOOK_EVENT_ON_CHARACTER_CREATE, (void*)PlayerFirstCreate);
}
