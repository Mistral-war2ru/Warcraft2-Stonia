#include <windows.h>
#include <stdint.h>
#include <stdio.h>
#include "patch.h"//Fois patcher
#include "defs.h"//some addresses and constants
#include "names.h"//some constants

WORD m_screen_w = 640;
WORD m_screen_h = 480;

byte a_custom = 0;//custom game
byte reg[4] = { 0 };//region
int* unit[1610];//array for units
int* unitt[1610];//temp array for units
int units = 0;//number of units in array
byte ua[255] = { 255 };//attackers array
byte ut[255] = { 255 };//targets array     only unit ua[i] can deal damage to unit ut[i]
bool first_step = false;//first step of trigger
bool ai_fixed = false;//ai fix
bool saveload_fixed = false;//saveload ai break bug fix

struct Vizs
{
    byte x = 0;
    byte y = 0;
    byte p = 0;
    byte s = 0;
};
Vizs vizs_areas[2000];
int vizs_n = 0;

//-----sound
void* def_name = NULL;
void* def_sound = NULL;
void* def_name_seq = NULL;
void* def_sound_seq = NULL;

char k1_name[] = "Stonia\\kirka\\k1.wav\x0";
void* k1_sound = NULL;
char k2_name[] = "Stonia\\kirka\\k2.wav\x0";
void* k2_sound = NULL;
char k3_name[] = "Stonia\\kirka\\k3.wav\x0";
void* k3_sound = NULL;
char k4_name[] = "Stonia\\kirka\\k4.wav\x0";
void* k4_sound = NULL;
char* k_names[] = { k1_name ,k2_name,k3_name,k4_name };
void* k_sounds[] = { k1_sound,k2_sound,k3_sound,k4_sound };

char rs1_name[] = "Stonia\\pistol\\p1.wav\x0";
void* rs1_sound = NULL;
char rs2_name[] = "Stonia\\pistol\\p2.wav\x0";
void* rs2_sound = NULL;
char rs3_name[] = "Stonia\\pistol\\p3.wav\x0";
void* rs3_sound = NULL;
char rs4_name[] = "Stonia\\pistol\\p4.wav\x0";
void* rs4_sound = NULL;
char rs5_name[] = "Stonia\\pistol\\p5.wav\x0";
void* rs5_sound = NULL;
char rs6_name[] = "Stonia\\pistol\\p6.wav\x0";
void* rs6_sound = NULL;
char rs7_name[] = "Stonia\\pistol\\p7.wav\x0";
void* rs7_sound = NULL;
char* rs_names[] = { rs1_name ,rs2_name,rs3_name,rs4_name,rs5_name,rs6_name,rs7_name };
void* rs_sounds[] = { rs1_sound,rs2_sound,rs3_sound,rs4_sound,rs5_sound,rs6_sound,rs7_sound };

char c1_name[] = "Stonia\\canon\\c1.wav\x0";
void* c1_sound = NULL;
char c2_name[] = "Stonia\\canon\\c2.wav\x0";
void* c2_sound = NULL;
char* c_names[] = { c1_name ,c2_name };
void* c_sounds[] = { c1_sound,c2_sound };

char hy1_name[] = "Stonia\\speech\\hero\\hyessr1.wav\x0";
void* hy1_sound = NULL;
char hy2_name[] = "Stonia\\speech\\hero\\hyessr2.wav\x0";
void* hy2_sound = NULL;
char hy3_name[] = "Stonia\\speech\\hero\\hyessr3.wav\x0";
void* hy3_sound = NULL;
char hw1_name[] = "Stonia\\speech\\hero\\hwhat1.wav\x0";
void* hw1_sound = NULL;
char hw2_name[] = "Stonia\\speech\\hero\\hwhat2.wav\x0";
void* hw2_sound = NULL;
char hw3_name[] = "Stonia\\speech\\hero\\hwhat3.wav\x0";
void* hw3_sound = NULL;
char hp1_name[] = "Stonia\\speech\\hero\\hpisd1.wav\x0";
void* hp1_sound = NULL;
char hp2_name[] = "Stonia\\speech\\hero\\hpisd2.wav\x0";
void* hp2_sound = NULL;
char hp3_name[] = "Stonia\\speech\\hero\\hpisd3.wav\x0";
void* hp3_sound = NULL;
char* h_names[] = { hp1_name ,hp2_name,hp3_name,hw1_name ,hw2_name,hw3_name,hy1_name ,hy2_name,hy3_name };
void* h_sounds[] = { hp1_sound,hp2_sound,hp3_sound,hw1_sound,hw2_sound,hw3_sound,hy1_sound,hy2_sound,hy3_sound };

char ay1_name[] = "Stonia\\speech\\armored\\ayessr1.wav\x0";
void* ay1_sound = NULL;
char ay2_name[] = "Stonia\\speech\\armored\\ayessr2.wav\x0";
void* ay2_sound = NULL;
char ay3_name[] = "Stonia\\speech\\armored\\ayessr3.wav\x0";
void* ay3_sound = NULL;
char ay4_name[] = "Stonia\\speech\\armored\\ayessr4.wav\x0";
void* ay4_sound = NULL;
char aw1_name[] = "Stonia\\speech\\armored\\awhat1.wav\x0";
void* aw1_sound = NULL;
char aw2_name[] = "Stonia\\speech\\armored\\awhat2.wav\x0";
void* aw2_sound = NULL;
char aw3_name[] = "Stonia\\speech\\armored\\awhat3.wav\x0";
void* aw3_sound = NULL;
char aw4_name[] = "Stonia\\speech\\armored\\awhat4.wav\x0";
void* aw4_sound = NULL;
char ap1_name[] = "Stonia\\speech\\armored\\apissd1.wav\x0";
void* ap1_sound = NULL;
char ap2_name[] = "Stonia\\speech\\armored\\apissd2.wav\x0";
void* ap2_sound = NULL;
char ap3_name[] = "Stonia\\speech\\armored\\apissd3.wav\x0";
void* ap3_sound = NULL;
char ar_name[] = "Stonia\\speech\\armored\\aready.wav\x0";
void* ar_sound = NULL;
char* a_names[] = { ap1_name ,ap2_name,ap3_name,ar_name,aw1_name ,aw2_name,aw3_name,aw4_name,ay1_name ,ay2_name,ay3_name,ay4_name };
void* a_sounds[] = { ap1_sound,ap2_sound,ap3_sound,ar_sound,aw1_sound,aw2_sound,aw3_sound,aw4_sound,ay1_sound,ay2_sound,ay3_sound,ay4_sound };

char wy1_name[] = "Stonia\\speech\\worker\\wyessr1.wav\x0";
void* wy1_sound = NULL;
char wy2_name[] = "Stonia\\speech\\worker\\wyessr2.wav\x0";
void* wy2_sound = NULL;
char wy3_name[] = "Stonia\\speech\\worker\\wyessr3.wav\x0";
void* wy3_sound = NULL;
char wy4_name[] = "Stonia\\speech\\worker\\wyessr4.wav\x0";
void* wy4_sound = NULL;
char ww1_name[] = "Stonia\\speech\\worker\\wwhat1.wav\x0";
void* ww1_sound = NULL;
char ww2_name[] = "Stonia\\speech\\worker\\wwhat2.wav\x0";
void* ww2_sound = NULL;
char ww3_name[] = "Stonia\\speech\\worker\\wwhat3.wav\x0";
void* ww3_sound = NULL;
char ww4_name[] = "Stonia\\speech\\worker\\wwhat4.wav\x0";
void* ww4_sound = NULL;
char wp1_name[] = "Stonia\\speech\\worker\\wpissd1.wav\x0";
void* wp1_sound = NULL;
char wp2_name[] = "Stonia\\speech\\worker\\wpissd2.wav\x0";
void* wp2_sound = NULL;
char wp3_name[] = "Stonia\\speech\\worker\\wpissd3.wav\x0";
void* wp3_sound = NULL;
char wp4_name[] = "Stonia\\speech\\worker\\wpissd4.wav\x0";
void* wp4_sound = NULL;
char wp5_name[] = "Stonia\\speech\\worker\\wpissd5.wav\x0";
void* wp5_sound = NULL;
char wp6_name[] = "Stonia\\speech\\worker\\wpissd6.wav\x0";
void* wp6_sound = NULL;
char wp7_name[] = "Stonia\\speech\\worker\\wpissd7.wav\x0";
void* wp7_sound = NULL;
char wr_name[] = "Stonia\\speech\\worker\\wready.wav\x0";
void* wr_sound = NULL;
char wd_name[] = "Stonia\\speech\\worker\\wrkdon.wav\x0";
void* wd_sound = NULL;
char wde_name[] = "Stonia\\speech\\worker\\wrkdone.wav\x0";
void* wde_sound = NULL;
char* w_names[] = { ww1_name,ww2_name,ww3_name,ww4_name,ww1_name,ww3_name,wp1_name,wp2_name,wp3_name,wp4_name,wp5_name,wp6_name,wp7_name,wp7_name,wy1_name,wy2_name,wy3_name,wy4_name,wde_name,wd_name,wr_name };
void* w_sounds[] = { ww1_sound,ww2_sound,ww3_sound,ww4_sound,ww1_sound,ww2_sound,wp1_sound,wp2_sound,wp3_sound,wp4_sound,wp5_sound,wp6_sound,wp7_sound,wp7_sound,wy1_sound,wy2_sound,wy3_sound,wy4_sound,wde_sound,wd_sound,wr_sound };

char dh1_name[] = "Stonia\\speech\\dwarf\\help1.wav\x0";
void* dh1_sound = NULL;
char dh2_name[] = "Stonia\\speech\\dwarf\\help2.wav\x0";
void* dh2_sound = NULL;
char dd_name[] = "Stonia\\speech\\dwarf\\dead.wav\x0";
void* dd_sound = NULL;
char da_name[] = "Stonia\\speech\\dwarf\\assembly.wav\x0";
void* da_sound = NULL;
char dh_name[] = "Stonia\\speech\\dwarf\\house.wav\x0";
void* dh_sound = NULL;
char* d_names[] = { dh1_name ,dh2_name,dd_name,da_name,dh_name };
void* d_sounds[] = { dh1_sound,dh2_sound,dd_sound,da_sound,dh_sound };

char my1_name[] = "Stonia\\speech\\mercenary\\myessir1.wav\x0";
void* my1_sound = NULL;
char my2_name[] = "Stonia\\speech\\mercenary\\myessir2.wav\x0";
void* my2_sound = NULL;
char my3_name[] = "Stonia\\speech\\mercenary\\myessir3.wav\x0";
void* my3_sound = NULL;
char my4_name[] = "Stonia\\speech\\mercenary\\myessir4.wav\x0";
void* my4_sound = NULL;
char mw1_name[] = "Stonia\\speech\\mercenary\\mwhat1.wav\x0";
void* mw1_sound = NULL;
char mw2_name[] = "Stonia\\speech\\mercenary\\mwhat2.wav\x0";
void* mw2_sound = NULL;
char mw3_name[] = "Stonia\\speech\\mercenary\\mwhat3.wav\x0";
void* mw3_sound = NULL;
char mw4_name[] = "Stonia\\speech\\mercenary\\mwhat4.wav\x0";
void* mw4_sound = NULL;
char mp1_name[] = "Stonia\\speech\\mercenary\\mpissed1.wav\x0";
void* mp1_sound = NULL;
char mp2_name[] = "Stonia\\speech\\mercenary\\mpissed2.wav\x0";
void* mp2_sound = NULL;
char mp3_name[] = "Stonia\\speech\\mercenary\\mpissed3.wav\x0";
void* mp3_sound = NULL;
char mp4_name[] = "Stonia\\speech\\mercenary\\mpissed4.wav\x0";
void* mp4_sound = NULL;
char mp5_name[] = "Stonia\\speech\\mercenary\\mpissed5.wav\x0";
void* mp5_sound = NULL;
char mp6_name[] = "Stonia\\speech\\mercenary\\mpissed6.wav\x0";
void* mp6_sound = NULL;
char mp7_name[] = "Stonia\\speech\\mercenary\\mpissed7.wav\x0";
void* mp7_sound = NULL;
char mr_name[] = "Stonia\\speech\\mercenary\\mready.wav\x0";
void* mr_sound = NULL;
char* m_names[] = { mw1_name,mw2_name,mw3_name,mw4_name,mw1_name,mw3_name,mp1_name,mp2_name,mp3_name,mp4_name,mp5_name,mp6_name,mp7_name,mp7_name,my1_name,my2_name,my3_name,my4_name,wde_name,wd_name,mr_name };
void* m_sounds[] = { mw1_sound,mw2_sound,mw3_sound,mw4_sound,mw1_sound,mw2_sound,mp1_sound,mp2_sound,mp3_sound,mp4_sound,mp5_sound,mp6_sound,mp7_sound,mp7_sound,my1_sound,my2_sound,my3_sound,my4_sound,wde_sound,wd_sound,mr_sound };

char aly1_name[] = "Stonia\\speech\\alchemist\\alyessr1.wav\x0";
void* aly1_sound = NULL;
char aly2_name[] = "Stonia\\speech\\alchemist\\alyessr2.wav\x0";
void* aly2_sound = NULL;
char aly3_name[] = "Stonia\\speech\\alchemist\\alyessr3.wav\x0";
void* aly3_sound = NULL;
char alw1_name[] = "Stonia\\speech\\alchemist\\alwhat1.wav\x0";
void* alw1_sound = NULL;
char alw2_name[] = "Stonia\\speech\\alchemist\\alwhat2.wav\x0";
void* alw2_sound = NULL;
char alw3_name[] = "Stonia\\speech\\alchemist\\alwhat3.wav\x0";
void* alw3_sound = NULL;
char alp1_name[] = "Stonia\\speech\\alchemist\\alpissd1.wav\x0";
void* alp1_sound = NULL;
char alp2_name[] = "Stonia\\speech\\alchemist\\alpissd2.wav\x0";
void* alp2_sound = NULL;
char alp3_name[] = "Stonia\\speech\\alchemist\\alpissd3.wav\x0";
void* alp3_sound = NULL;
char alr_name[] = "Stonia\\speech\\alchemist\\already.wav\x0";
void* alr_sound = NULL;
char* al_names[] = { alp1_name ,alp2_name,alp3_name,alr_name,alw1_name,alw2_name,alw3_name,aly1_name ,aly2_name,aly3_name };
void* al_sounds[] = { alp1_sound,alp2_sound,alp3_sound,alr_sound,alw1_sound,alw2_sound,alw3_sound,aly1_sound,aly2_sound,aly3_sound };

char fy1_name[] = "Stonia\\speech\\flyer\\fyessr1.wav\x0";
void* fy1_sound = NULL;
char fy2_name[] = "Stonia\\speech\\flyer\\fyessr2.wav\x0";
void* fy2_sound = NULL;
char fw_name[] = "Stonia\\speech\\flyer\\fwhat.wav\x0";
void* fw_sound = NULL;
char fr_name[] = "Stonia\\speech\\flyer\\fready.wav\x0";
void* fr_sound = NULL;
char* f_names[] = { fr_name,fw_name,fy1_name,fy2_name };
void* f_sounds[] = { fr_sound,fw_sound,fy1_sound,fy2_sound };

char secret1_name[] = "Stonia\\speech\\secret\\a1.wav\x0";
void* secret1_sound = NULL;
char secret2_name[] = "Stonia\\speech\\secret\\a2.wav\x0";
void* secret2_sound = NULL;
char secret3_name[] = "Stonia\\speech\\secret\\e1.wav\x0";
void* secret3_sound = NULL;
char secret4_name[] = "Stonia\\speech\\secret\\e2.wav\x0";
void* secret4_sound = NULL;
char secret5_name[] = "Stonia\\speech\\secret\\v1.wav\x0";
void* secret5_sound = NULL;
char secret6_name[] = "Stonia\\speech\\secret\\v2.wav\x0";
void* secret6_sound = NULL;
char secret7_name[] = "Stonia\\speech\\secret\\v3.wav\x0";
void* secret7_sound = NULL;
char* secret_names[] = { secret1_name,secret2_name,secret3_name,secret4_name,secret5_name,secret6_name,secret7_name };
void* secret_sounds[] = { secret1_sound,secret2_sound,secret3_sound,secret4_sound,secret5_sound,secret6_sound,secret7_sound };

char dialog1_name[] = "Stonia\\dialog\\dialog_gedsun1.wav\x0";
void* dialog1_sound = NULL;
char dialog2_name[] = "Stonia\\dialog\\dialog_gedsun2.wav\x0";
void* dialog2_sound = NULL;
char dialog3_name[] = "Stonia\\dialog\\dialog_gedsun3.wav\x0";
void* dialog3_sound = NULL;
char dialog4_name[] = "Stonia\\dialog\\dialog_gedsun4.wav\x0";
void* dialog4_sound = NULL;
char dialog5_name[] = "Stonia\\dialog\\dialog_gedsun5.wav\x0";
void* dialog5_sound = NULL;
char dialog6_name[] = "Stonia\\dialog\\dialog_gedsun6.wav\x0";
void* dialog6_sound = NULL;
char dialog7_name[] = "Stonia\\dialog\\dialog_gedsun7.wav\x0";
void* dialog7_sound = NULL;
char dialog8_name[] = "Stonia\\dialog\\dialog_gedsun8.wav\x0";
void* dialog8_sound = NULL;
char dialog9_name[] = "Stonia\\dialog\\dialog_gedsun9.wav\x0";
void* dialog9_sound = NULL;
char dialog10_name[] = "Stonia\\dialog\\dialog_gedsun10.wav\x0";
void* dialog10_sound = NULL;
char dialog11_name[] = "Stonia\\dialog\\dialog_gedsun11.wav\x0";
void* dialog11_sound = NULL;
char dialog12_name[] = "Stonia\\dialog\\dialog_gedsun12.wav\x0";
void* dialog12_sound = NULL;
char dialog13_name[] = "Stonia\\dialog\\dialog_gedsun13.wav\x0";
void* dialog13_sound = NULL;
char dialog14_name[] = "Stonia\\dialog\\dialog_gedsun14.wav\x0";
void* dialog14_sound = NULL;
char dialog15_name[] = "Stonia\\dialog\\dialog_gedsun15.wav\x0";
void* dialog15_sound = NULL;
char dialog16_name[] = "Stonia\\dialog\\dialog_gedsun16.wav\x0";
void* dialog16_sound = NULL;
char dialog17_name[] = "Stonia\\dialog\\dialog_gedsun17.wav\x0";
void* dialog17_sound = NULL;
char dialog18_name[] = "Stonia\\dialog\\dialog_gedsun18.wav\x0";
void* dialog18_sound = NULL;
char dialog19_name[] = "Stonia\\dialog\\dialog_gedsun19.wav\x0";
void* dialog19_sound = NULL;
char dialog20_name[] = "Stonia\\dialog\\dialog_gedsun20.wav\x0";
void* dialog20_sound = NULL;
char dialog21_name[] = "Stonia\\dialog\\dialog_elder1.wav\x0";
void* dialog21_sound = NULL;
char dialog22_name[] = "Stonia\\dialog\\dialog_elder2.wav\x0";
void* dialog22_sound = NULL;
char dialog23_name[] = "Stonia\\dialog\\dialog_elder3.wav\x0";
void* dialog23_sound = NULL;
char dialog24_name[] = "Stonia\\dialog\\dialog_elder4.wav\x0";
void* dialog24_sound = NULL;
char dialog25_name[] = "Stonia\\dialog\\dialog_garuh1.wav\x0";
void* dialog25_sound = NULL;
char dialog26_name[] = "Stonia\\dialog\\dialog_garuh2.wav\x0";
void* dialog26_sound = NULL;
char dialog27_name[] = "Stonia\\dialog\\dialog_garuh3.wav\x0";
void* dialog27_sound = NULL;
char dialog28_name[] = "Stonia\\dialog\\dialog_mage1.wav\x0";
void* dialog28_sound = NULL;
char dialog29_name[] = "Stonia\\dialog\\dialog_mage2.wav\x0";
void* dialog29_sound = NULL;
char dialog30_name[] = "Stonia\\dialog\\dialog_mage3.wav\x0";
void* dialog30_sound = NULL;
char dialog31_name[] = "Stonia\\dialog\\dialog_book.wav\x0";
void* dialog31_sound = NULL;
char* dialog_names[] = { dialog1_name,dialog2_name,dialog3_name,dialog4_name,dialog5_name,dialog6_name,dialog7_name,dialog8_name,dialog9_name,dialog10_name,
dialog11_name,dialog12_name,dialog13_name,dialog14_name,dialog15_name,dialog16_name,dialog17_name,dialog18_name,dialog19_name,dialog20_name,dialog21_name,
dialog22_name,dialog23_name,dialog24_name,dialog25_name,dialog26_name,dialog27_name,dialog28_name,dialog29_name,dialog30_name,dialog31_name };
void* dialog_sounds[] = { dialog1_sound,dialog2_sound,dialog3_sound,dialog4_sound,dialog5_sound,dialog6_sound,dialog7_sound,dialog8_sound,dialog9_sound,dialog10_sound,
dialog11_sound,dialog12_sound,dialog13_sound,dialog14_sound,dialog15_sound,dialog16_sound,dialog17_sound,dialog18_sound,dialog19_sound,dialog20_sound,dialog21_sound,
dialog22_sound,dialog23_sound,dialog24_sound,dialog25_sound,dialog26_sound,dialog27_sound,dialog28_sound,dialog29_sound,dialog30_sound,dialog31_sound };

void sound_play_from_file(int oid, DWORD name, void* snd)//old id
{
    def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * oid);
    def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);//save default
    patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)name);
    patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)snd);
    ((void (*)(WORD))F_WAR2_SOUND_PLAY)(oid);//war2 sound play
    snd = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);
    patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)def_sound);
    patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)def_name);//restore default
}

void sound_play_dialog(int id)
{
    sound_play_from_file(0, (DWORD)dialog_names[id], dialog_sounds[id]);
}
//-----sound

void show_message(byte time, char* text)
{
    ((void (*)(char*, int, int))F_MAP_MSG)(text, 15, time * 10);//original war2 show msg func
}

int get_val(int adress, int player)
{
    return (int)(*(WORD*)(adress + player * 2));//player*2 cause all vals is WORD
}

bool cmp_args(byte m, byte v, byte c)
{//compare bytes
    bool f = false;
    switch (m)
    {
    case CMP_EQ:f = (v == c); break;
    case CMP_NEQ:f = (v != c); break;
    case CMP_BIGGER_EQ:f = (v >= c); break;
    case CMP_SMALLER_EQ:f = (v <= c); break;
    case CMP_BIGGER:f = (v > c); break;
    case CMP_SMALLER:f = (v < c); break;
    default: f = false; break;
    }
    return f;
}

bool cmp_args2(byte m, WORD v, WORD c)
{//compare words
    bool f = false;
    switch (m)
    {
    case CMP_EQ:f = (v == c); break;
    case CMP_NEQ:f = (v != c); break;
    case CMP_BIGGER_EQ:f = (v >= c); break;
    case CMP_SMALLER_EQ:f = (v <= c); break;
    case CMP_BIGGER:f = (v > c); break;
    case CMP_SMALLER:f = (v < c); break;
    default: f = false; break;
    }
    return f;
}

bool cmp_args4(byte m, int v, int c)
{//comapre 4 bytes (for resources)
    bool f = false;
    switch (m)
    {
    case CMP_EQ:f = (v == c); break;
    case CMP_NEQ:f = (v != c); break;
    case CMP_BIGGER_EQ:f = (v >= c); break;
    case CMP_SMALLER_EQ:f = (v <= c); break;
    case CMP_BIGGER:f = (v > c); break;
    case CMP_SMALLER:f = (v < c); break;
    default: f = false; break;
    }
    return f;
}

void lose(bool t)
{
    if (t == true)
    {
        char buf[] = "\x0";//if need to show table
        PATCH_SET((char*)LOSE_SHOW_TABLE, buf);
    }
    else
    {
        char buf[] = "\x3b";
        PATCH_SET((char*)LOSE_SHOW_TABLE, buf);
    }
	if (!first_step)
	{
		char l[] = "\x2";
        PATCH_SET((char*)(ENDGAME_STATE + (*(byte*)LOCAL_PLAYER)), l);
		((void (*)())F_LOSE)();//original lose func
	}
	else
	{
		patch_setdword((DWORD*)VICTORY_FUNCTION, (DWORD)F_LOSE);
	}
}

void win(bool t)
{
    if (t == true)
    {
        char buf[] = "\xEB";//if need to show table
        PATCH_SET((char*)WIN_SHOW_TABLE, buf);
    }
    else
    {
        char buf[] = "\x74";
        PATCH_SET((char*)WIN_SHOW_TABLE, buf);
    }
	if (!first_step)
	{
		char l[] = "\x3";
        PATCH_SET((char*)(ENDGAME_STATE + (*(byte*)LOCAL_PLAYER)), l);
		((void (*)())F_WIN)();//original win func
	}
	else
	{
		patch_setdword((DWORD*)VICTORY_FUNCTION, (DWORD)F_WIN);
	}
}

void tile_remove_trees(int x, int y)
{
    ((void (*)(int, int))F_TILE_REMOVE_TREES)(x, y);
}

void tile_remove_rocks(int x, int y)
{
    ((void (*)(int, int))F_TILE_REMOVE_ROCKS)(x, y);
}

void tile_remove_walls(int x, int y)
{
    ((void (*)(int, int))F_TILE_REMOVE_WALLS)(x, y);
}

bool stat_byte(byte s)
{//chech if unit stat is 1 or 2 byte
    bool f = (s == S_DRAW_X) || (s == S_DRAW_Y) || (s == S_X) || (s == S_Y) || (s == S_HP)
        || (s == S_INVIZ) || (s == S_SHIELD) || (s == S_BLOOD) || (s == S_HASTE)
        || (s == S_AI_SPELLS) || (s == S_NEXT_FIRE)
        || (s == S_LAST_HARVEST_X) || (s == S_LAST_HARVEST_Y)
        || (s == S_BUILD_PROGRES) || (s == S_BUILD_PROGRES_TOTAL)
        || (s == S_RESOURCES) || (s == S_ORDER_X) || (s == S_ORDER_Y)
        || (s == S_RETARGET_X1) || (s == S_RETARGET_Y1) || (s == S_RETARGET_X2) || (s == S_RETARGET_Y2);
    return !f;
}

bool cmp_stat(int* p, int v, byte pr, byte cmp)
{
    //p - unit
    //v - value
    //pr - property
    //cmp - compare method
    bool f = false;
    if (stat_byte(pr))
    {
        byte ob = v % 256;
        char buf[3] = { 0 };
        buf[0] = ob;
        buf[1] = *((byte*)((uintptr_t)p + pr));
        if (cmp_args(cmp, buf[1], buf[0]))
        {
            f = true;
        }
    }
    else
    {
        if (cmp_args2(cmp, *((WORD*)((uintptr_t)p + pr)), (WORD)v))
        {
            f = true;
        }
    }
    return f;
}

void set_stat(int* p, int v, byte pr)
{
    if (stat_byte(pr))
    {
        char buf[] = "\x0";
        buf[0] = v % 256;
        PATCH_SET((char*)((uintptr_t)p + pr), buf);
    }
    else
    {
        char buf[] = "\x0\x0";
        buf[0] = v % 256;
        buf[1] = (v / 256) % 256;
        PATCH_SET((char*)((uintptr_t)p + pr), buf);
    }
}

void unit_convert(byte player, int who, int tounit, int a)
{
    //original war2 func converts units
    ((void (*)(byte, int, int, int))F_UNIT_CONVERT)(player, who, tounit, a);
}

void unit_create(int x, int y, int id, byte owner, byte n)
{
    while (n > 0)
    {
        n -= 1;
        int* p = (int*)(UNIT_SIZE_TABLE + 4 * id);//unit sizes table
        ((void (*)(int, int, int, byte, int*))F_UNIT_CREATE)(x, y, id, owner, p);//original war2 func creates unit
        //just call n times to create n units
    }
}

void unit_kill(int* u)
{
    ((void (*)(int*))F_UNIT_KILL)(u);//original war2 func kills unit
}

void unit_remove(int* u)
{
    byte f = *((byte*)((uintptr_t)u + S_FLAGS3));
    f |= SF_HIDDEN;
    set_stat(u, f, S_FLAGS3);
    unit_kill(u);//hide unit then kill
}

void unit_cast(int* u)//unit autocast
{
    ((void (*)(int*))F_AI_CAST)(u);//original war2 ai cast spells
}

void bullet_create(WORD x, WORD y, byte id)
{
    int* b = ((int* (*)(WORD, WORD, byte))F_BULLET_CREATE)(x, y, id);
}

void bullet_create_unit(int* u, byte b)
{
    WORD x = *((WORD*)((uintptr_t)u + S_DRAW_X));
    WORD y = *((WORD*)((uintptr_t)u + S_DRAW_Y));
    bullet_create(x + 16, y + 16, b);
}

void bullet_create8_around_unit(int* u, byte b)
{
    WORD ux = *((WORD*)((uintptr_t)u + S_DRAW_X));
    WORD uy = *((WORD*)((uintptr_t)u + S_DRAW_Y));
    WORD x = ux + 16;
    WORD y = uy + 16;
    if ((b == B_LIGHT_FIRE) || (b == B_HEAVY_FIRE))y -= 8;
    if ((b == B_LIGHTNING) || (b == B_COIL))
    {
        x += 16;
        y += 16;
    }
    WORD xx = x;
    WORD yy = y;
    bullet_create(xx + 48, yy, b);//right
    bullet_create(xx, yy + 48, b);//down
    bullet_create(xx + 32, yy + 32, b);//right down
    if (xx <= 32)xx = 0;
    else xx -= 32;
    bullet_create(xx, yy + 32, b);//left down
    if (yy <= 32)yy = 0;
    else yy -= 32;
    bullet_create(xx, yy, b);//left up
    xx = x;
    bullet_create(xx + 32, yy, b);//right up
    yy = y;
    if (xx <= 48)xx = 0;
    else xx -= 48;
    bullet_create(xx, yy, b);//left
    xx = x;
    if (yy <= 48)yy = 0;
    else yy -= 48;
    bullet_create(xx, yy, b);//up
}

void set_region(int x1, int y1, int x2, int y2)
{
    if (x1 < 0)x1 = 0;
    if (x1 > 127)x1 = 127;
    if (y1 < 0)y1 = 0;
    if (y1 > 127)y1 = 127;
    if (x2 < 0)x2 = 0;
    if (x2 > 127)x2 = 127;
    if (y2 < 0)y2 = 0;
    if (y2 > 127)y2 = 127;
    reg[0] = x1 % 256;
    reg[1] = y1 % 256;
    reg[2] = x2 % 256;
    reg[3] = y2 % 256;
}

bool in_region(byte x, byte y, byte x1, byte y1, byte x2, byte y2)
{
    //dnt know why but without this big monstrous ussless code gam crash 
    byte tmp;
    tmp = x % 256;
    x = tmp;
    tmp = y % 256;
    y = tmp;
    tmp = x1 % 256;
    x1 = tmp;
    tmp = y1 % 256;
    y1 = tmp;
    tmp = x2 % 256;
    x2 = tmp;
    tmp = y2 % 256;
    y2 = tmp;
    if (x < 0)x = 0;
    if (x > 127)x = 127;
    if (y < 0)y = 0;
    if (y > 127)y = 127;
    if (x1 < 0)x1 = 0;
    if (x1 > 127)x1 = 127;
    if (y1 < 0)y1 = 0;
    if (y1 > 127)y1 = 127;
    if (x2 < 0)x2 = 0;
    if (x2 > 127)x2 = 127;
    if (y2 < 0)y2 = 0;
    if (y2 > 127)y2 = 127;
    if (x2 < x1)
    {
        tmp = x1;
        x1 = x2;
        x2 = tmp;
    }
    if (y2 < y1)
    {
        tmp = y1;
        y1 = y2;
        y2 = tmp;
    }
    //just check if coords inside region
    return ((x >= x1) && (y >= y1) && (x <= x2) && (y <= y2));
}

bool check_unit_dead(int* p)
{
    bool dead = false;
    if (p)
    {
        if ((*((byte*)((uintptr_t)p + S_FLAGS3))
            & (SF_DEAD | SF_DIEING | SF_UNIT_FREE)) != 0)
            dead = true;
    }
    else
        dead = true;
    return dead;
}

bool check_unit_complete(int* p)//for buildings
{
    bool f = false;
    if (p)
    {
        if ((*((byte*)((uintptr_t)p + S_FLAGS3)) & SF_COMPLETED) != 0)//flags3 last bit
            f = true;
    }
    else
        f = false;
    return f;
}

bool check_unit_hidden(int* p)
{
    bool f = false;
    if (p)
    {
        if ((*((byte*)((uintptr_t)p + S_FLAGS3)) & SF_HIDDEN) != 0)//flags3 4 bit
            f = true;
    }
    else
        f = true;
    return f;
}

bool check_unit_preplaced(int* p)
{
    bool f = false;
    if (p)
    {
        if ((*((byte*)((uintptr_t)p + S_FLAGS3)) & SF_PREPLACED) != 0)//flags3
            f = true;
    }
    else
        f = false;
    return f;
}

bool check_unit_near_death(int* p)
{
    bool dead = false;
    if (p)
    {
        if (((*((byte*)((uintptr_t)p + S_FLAGS3)) & SF_DIEING) != 0)
            && ((*((byte*)((uintptr_t)p + S_FLAGS3)) & (SF_DEAD | SF_UNIT_FREE)) == 0))
            dead = true;
    }
    else
        dead = true;
    return dead;
}

bool check_peon_loaded(int* p, byte r)
{
    bool f = false;
    if (p)
    {
        if (r == 0)
        {
            if (((*((byte*)((uintptr_t)p + S_PEON_FLAGS)) & PEON_LOADED) != 0)
                && ((*((byte*)((uintptr_t)p + S_PEON_FLAGS)) & PEON_HARVEST_GOLD) != 0))
                f = true;
        }
        if (r == 1)
        {
            if (((*((byte*)((uintptr_t)p + S_PEON_FLAGS)) & PEON_LOADED) != 0)
                && ((*((byte*)((uintptr_t)p + S_PEON_FLAGS)) & PEON_HARVEST_LUMBER) != 0))
                f = true;
        }
        if (r == 2)
        {
            if (((*((byte*)((uintptr_t)p + S_PEON_FLAGS)) & PEON_LOADED) != 0))
                f = true;
        }
    }
    return f;
}

void find_all_units(byte id)
{
	//CAREFUL with this function - ALL units get into massive 
    //even if their memory was cleared already
    //all units by id will go in array
    units = 0;
    int* p = (int*)UNITS_MASSIVE;//pointer to units
    p = (int*)(*p);
    int k = *(int*)UNITS_NUMBER;
    while (k > 0)
    {
        bool f = *((byte*)((uintptr_t)p + S_ID)) == (byte)id;
        if (f)
        {
            unit[units] = p;
            units++;
        }
        p = (int*)((int)p + 0x98);
        k--;
    }
}

void find_all_alive_units(byte id)
{
    //all units by id will go in array
    units = 0;
    for (int i = 0; i < 16; i++)
    {
        int* p = (int*)(UNITS_LISTS + 4 * i);//pointer to units list for each player
        if (p)
        {
            p = (int*)(*p);
            while (p)
            {
                bool f = *((byte*)((uintptr_t)p + S_ID)) == (byte)id;
                if (id == ANY_BUILDING)
                    f = *((byte*)((uintptr_t)p + S_ID)) >= U_FARM;//buildings
                if (id == ANY_MEN)
                    f = *((byte*)((uintptr_t)p + S_ID)) < U_FARM;//all nonbuildings
                if (id == ANY_UNITS)
                    f = true;//all ALL units
                if (id == ANY_BUILDING_2x2)//small buildings
                {
                    byte sz = *((byte*)UNIT_SIZE_TABLE + *((byte*)((uintptr_t)p + S_ID)) * 4);
                    f = sz == 2;
                }
                if (id == ANY_BUILDING_3x3)//med buildings
                {
                    byte sz = *((byte*)UNIT_SIZE_TABLE + *((byte*)((uintptr_t)p + S_ID)) * 4);
                    f = sz == 3;
                }
                if (id == ANY_BUILDING_4x4)//big buildings
                {
                    byte sz = *((byte*)UNIT_SIZE_TABLE + *((byte*)((uintptr_t)p + S_ID)) * 4);
                    f = sz == 4;
                }
                if (f)
                {
                    if (!check_unit_dead(p))
                    {
                        unit[units] = p;
                        units++;
                    }
                }
                p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void sort_complete()
{
    //only completed units stay in array
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (check_unit_complete(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_in_region()
{
    //only units in region stay in array
    int k = 0;
    WORD x = 0, y = 0;
    for (int i = 0; i < units; i++)
    {
        x = *((WORD*)((uintptr_t)unit[i] + S_DRAW_X)) / 32;
        y = *((WORD*)((uintptr_t)unit[i] + S_DRAW_Y)) / 32;
        if (in_region((byte)x, (byte)y, reg[0], reg[1], reg[2], reg[3]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_not_in_region()
{
    //only units not in region stay in array
    int k = 0;
    WORD x = 0, y = 0;
    for (int i = 0; i < units; i++)
    {
        x = *((WORD*)((uintptr_t)unit[i] + S_DRAW_X)) / 32;
        y = *((WORD*)((uintptr_t)unit[i] + S_DRAW_Y)) / 32;
        if (!in_region((byte)x, (byte)y, reg[0], reg[1], reg[2], reg[3]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_target_in_region()
{
    //only units that have order coords in region stay in array
    int k = 0;
    byte x = 0, y = 0;
    for (int i = 0; i < units; i++)
    {
        x = *((byte*)((uintptr_t)unit[i] + S_ORDER_X));
        y = *((byte*)((uintptr_t)unit[i] + S_ORDER_Y));
        if (in_region(x, y, reg[0], reg[1], reg[2], reg[3]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_stat(byte pr, int v, byte cmp)
{
    //only units stay in array if have property compared to value is true
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (cmp_stat(unit[i], v, pr, cmp))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_hidden()
{
    //only not hidden units stay in array
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!check_unit_hidden(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_self(int* u)
{
    //unit remove self from array
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!(unit[i] == u))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_preplaced()
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!check_unit_preplaced(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_near_death()
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (check_unit_near_death(unit[i]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_attack_can_hit(int* p)
{
    //only units stay in array that *p can attack them
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        int a = 0;
        a = ((int(*)(int*, int*))F_ATTACK_CAN_HIT)(p, unit[i]);//attack can hit original war2 function
        if (a != 0)
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_attack_can_hit_range(int* p)
{
    //only units stay in array that *p can attack them and have passable terrain in attack range
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        int a = 0;
        a = ((int(*)(int*, int*))F_ATTACK_CAN_HIT)(p, unit[i]);//attack can hit
        if (a != 0)
        {
            byte id = *((byte*)((uintptr_t)unit[i] + S_ID));
            byte szx = *(byte*)(UNIT_SIZE_TABLE + 4 * id);
            byte szy = *(byte*)(UNIT_SIZE_TABLE + 4 * id + 2);
            byte idd = *((byte*)((uintptr_t)p + S_ID));
            byte rng = *(byte*)(UNIT_RANGE_TABLE + idd);
            byte ms = *(byte*)MAP_SIZE;
            byte xx = *((byte*)((uintptr_t)unit[i] + S_X));
            byte yy = *((byte*)((uintptr_t)unit[i] + S_Y));
            if (xx < rng)xx = 0;
            else xx -= rng;
            if (yy < rng)yy = 0;
            else yy -= rng;
            byte cl = *((byte*)((uintptr_t)p + S_MOVEMENT_TYPE));//movement type
            WORD mt = *(WORD*)(GLOBAL_MOVEMENT_TERRAIN_FLAGS + 2 * cl);//movement terrain flags

            bool f = false;
            for (int x = xx; (x < szx + xx + rng * 2 + 1) && (x < 127); x++)
            {
                for (int y = yy; (y < szy + yy + rng * 2 + 1) && (x < 127); y++)
                {
                    int aa = 1;
                    if ((cl == 0) || (cl == 3))//land and docked transport
                    {
                        aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt);//original war2 func if terrain passable with that movement type
                    }
                    if ((x % 2 == 0) && (y % 2 == 0))//air and water
                    {
                        if ((cl == 1) || (cl == 2))
                        {
                            aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt);
                        }
                    }
                    if (aa == 0)f = true;
                }
            }
            if (f)
            {
                unitt[k] = unit[i];
                k++;
            }
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_peon_loaded(byte r)
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (check_peon_loaded(unit[i], r))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void sort_peon_not_loaded(byte r)
{
    int k = 0;
    for (int i = 0; i < units; i++)
    {
        if (!check_peon_loaded(unit[i], r))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void set_stat_all(byte pr, int v)
{
    for (int i = 0; i < units; i++)
    {
        set_stat(unit[i], v, pr);//set stat to all units in array
    }
}

void kill_all()
{
    for (int i = 0; i < units; i++)
    {
        unit_kill(unit[i]);//just kill all in array
    }
    units = 0;
}

void remove_all()
{
    for (int i = 0; i < units; i++)
    {
        unit_remove(unit[i]);//just kill all in array
    }
    units = 0;
}

void cast_all()
{
    for (int i = 0; i < units; i++)
    {
        unit_cast(unit[i]);//casting spells
    }
    units = 0;
}

void damag(int* p, byte n1, byte n2)
{
    WORD hp = *((WORD*)((uintptr_t)p + S_HP));//unit hp
    WORD n = n1 + 256 * n2;
    if (hp > n)
    {
        hp -= n;
        set_stat(p, hp, S_HP);
    }
    else
    {
        set_stat(p, 0, S_HP);
        unit_kill(p);
    }
}

void damag_all(byte n1, byte n2)
{
    for (int i = 0; i < units; i++)
    {
        damag(unit[i], n1, n2);
    }
}

void heal(int* p, byte n1, byte n2)
{
    byte id = *((byte*)((uintptr_t)p + S_ID));//unit id
    WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * id);//max hp
    WORD hp = *((WORD*)((uintptr_t)p + S_HP));//unit hp
    WORD n = n1 + 256 * n2;
    if (hp < mhp)
    {
        hp += n;
        if (hp > mhp)
            hp = mhp;//canot heal more than max hp
        set_stat(p, hp, S_HP);
    }
}

void heal_all(byte n1, byte n2)
{
    for (int i = 0; i < units; i++)
    {
        heal(unit[i], n1, n2);
    }
}

void peon_load(int* u, byte r)
{
    byte f = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
    if (!(f & PEON_LOADED))
    {
        if (r == 0)
        {
            f |= PEON_LOADED;
            f |= PEON_HARVEST_GOLD;
            set_stat(u, f, S_PEON_FLAGS);
            ((void (*)(int*))F_GROUP_SET)(u);
        }
        else
        {
            f |= PEON_LOADED;
            f |= PEON_HARVEST_LUMBER;
            set_stat(u, f, S_PEON_FLAGS);
            ((void (*)(int*))F_GROUP_SET)(u);
        }
    }
}

void peon_load_all(byte r)
{
    for (int i = 0; i < units; i++)
    {
        peon_load(unit[i], r);
    }
}

void viz_area(byte x, byte y, byte pl, byte sz)
{
    int Vf = F_VISION2;
    switch (sz)
    {
    case 0:Vf = F_VISION2; break;
    case 1:Vf = F_VISION2; break;
    case 2:Vf = F_VISION2; break;
    case 3:Vf = F_VISION3; break;
    case 4:Vf = F_VISION4; break;
    case 5:Vf = F_VISION5; break;
    case 6:Vf = F_VISION6; break;
    case 7:Vf = F_VISION7; break;
    case 8:Vf = F_VISION8; break;
    case 9:Vf = F_VISION9; break;
    default: Vf = F_VISION2; break;
    }
    for (byte i = 0; i < 8; i++)
    {
        if (((1 << i) & pl) != 0)
        {
            ((void (*)(WORD, WORD, byte))Vf)(x, y, i);
        }
    }
}

void viz_area_add(byte x, byte y, byte pl, byte sz)
{
    if ((vizs_n >= 0) && (vizs_n <= 255))
    {
        vizs_areas[vizs_n].x = x;
        vizs_areas[vizs_n].y = y;
        vizs_areas[vizs_n].p = pl;
        vizs_areas[vizs_n].s = sz;
        vizs_n++;
    }
}

void viz_area_all(byte pl, byte sz)
{
    for (int i = 0; i < units; i++)
    {
        byte x = *((byte*)((uintptr_t)unit[i] + S_X));
        byte y = *((byte*)((uintptr_t)unit[i] + S_Y));
        viz_area_add(x, y, pl, sz);
    }
}

void give(int* p, byte owner)
{
    ((void (*)(int*, byte, byte))F_CAPTURE)(p, owner, 1);//original capture unit war2 func
    *(byte*)(RESCUED_UNITS + 2 * owner) -= 1;//reset number of captured units
}

void give_all(byte o)
{
    for (int i = 0; i < units; i++)
    {
        give(unit[i], o);
    }
}

bool unit_move(byte x, byte y, int* unit)
{
    if (x < 0)return false;
    if (y < 0)return false;//canot go negative
    byte mxs = *(byte*)MAP_SIZE;//map size
    if (x >= mxs)return false;
    if (y >= mxs)return false;//canot go outside map
    if (check_unit_hidden(unit))return false;//if unit not hidden
    byte cl = *((byte*)((uintptr_t)unit + S_MOVEMENT_TYPE));//movement type
    WORD mt = *(WORD*)(GLOBAL_MOVEMENT_TERRAIN_FLAGS + 2 * cl);//movement terrain flags

    int aa = 1;
    if ((cl == 0) || (cl == 3))//land and docked transport
    {
        aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt);//original war2 func if terrain passable with that movement type
    }
    if ((x % 2 == 0) && (y % 2 == 0))//air and water
    {
        if ((cl == 1) || (cl == 2))
        {
            aa = ((int (*)(int, int, int))F_XY_PASSABLE)(x, y, (int)mt);
        }
    }
    if (aa == 0)
    {
        ((void (*)(int*))F_UNIT_UNPLACE)(unit);//unplace
        set_stat(unit, x, S_X);
        set_stat(unit, y, S_Y);//change real coords
        set_stat(unit, x * 32, S_DRAW_X);
        set_stat(unit, y * 32, S_DRAW_Y);//change draw sprite coords
        ((void (*)(int*))F_UNIT_PLACE)(unit);//place
        return true;
    }
    return false;
}

void move_all(byte x, byte y)
{
    sort_stat(S_ID, U_FARM, CMP_SMALLER);//non buildings
    sort_stat(S_ANIMATION, 2, CMP_EQ);//only if animation stop
    for (int i = 0; i < units; i++)
    {
        int xx = 0, yy = 0, k = 1;
        bool f = unit_move(x, y, unit[i]);
        xx--;
        while ((!f) & (k < 5))//goes in spiral like original war2 (size 5)
        {
            while ((!f) & (yy < k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                yy++;
            }
            while ((!f) & (xx < k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                xx++;
            }
            while ((!f) & (yy > -k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                yy--;
            }
            while ((!f) & (xx >= -k))
            {
                f = unit_move(x + xx, y + yy, unit[i]);
                xx--;
            }
            k++;
        }
    }
}

void give_order(int* u, byte x, byte y, byte o)
{
    byte id = *((byte*)((uintptr_t)u + S_ID));
    if (id < U_FARM)
    {
        char buf[] = "\x0";
        bool f = ((o >= ORDER_SPELL_VISION) && (o <= ORDER_SPELL_ROT));
        if (f)
        {
            buf[0] = o;
            PATCH_SET((char*)GW_ACTION_TYPE, buf);
        }
        int* tr = NULL;
        for (int i = 0; i < 16; i++)
        {
            int* p = (int*)(UNITS_LISTS + 4 * i);//pointer to units list for each player
            if (p)
            {
                p = (int*)(*p);
                while (p)
                {
                    if (p!=u)
                    {
                        if (!check_unit_dead(p) && !check_unit_hidden(p))
                        {
                            byte xx = *(byte*)((uintptr_t)p + S_X);
                            byte yy = *(byte*)((uintptr_t)p + S_Y);
                            if ((abs(x - xx) <= 2) && (abs(y - yy) <= 2))
                            {
                                if (f)
                                {
                                    byte idd = *(byte*)((uintptr_t)p + S_ID);
                                    if (idd < U_FARM)
                                    {
                                        bool trf = true;
                                        if (o == ORDER_SPELL_ARMOR)
                                        {
                                            WORD ef = *(WORD*)((uintptr_t)p + S_SHIELD);
                                            trf = ef == 0;
                                        }
                                        if (o == ORDER_SPELL_BLOODLUST)
                                        {
                                            WORD ef = *(WORD*)((uintptr_t)p + S_BLOOD);
                                            trf = ef == 0;
                                        }
                                        if (o == ORDER_SPELL_HASTE)
                                        {
                                            WORD ef = *(WORD*)((uintptr_t)p + S_HASTE);
                                            trf = (ef == 0) || (ef > 0x7FFF);
                                        }
                                        if (o == ORDER_SPELL_SLOW)
                                        {
                                            WORD ef = *(WORD*)((uintptr_t)p + S_HASTE);
                                            trf = (ef == 0) || (ef <= 0x7FFF);
                                        }
                                        if (o == ORDER_SPELL_POLYMORPH)
                                        {
                                            trf = idd != U_CRITTER;
                                        }
                                        if (o == ORDER_SPELL_HEAL)
                                        {
                                            WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * idd);
                                            WORD hp = *((WORD*)((uintptr_t)p + S_HP));
                                            trf = hp < mhp;
                                        }
                                        if (trf)
                                        {
                                            WORD efi = *(WORD*)((uintptr_t)p + S_INVIZ);
                                            trf = efi == 0;
                                        }
                                        if (trf)
                                            tr = p;
                                    }
                                }
                                else
                                    tr = p;
                            }
                        }
                    }
                    p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
                }
            }
        }
        bool aoe = (o == ORDER_SPELL_VISION) || (o == ORDER_SPELL_EXORCISM) || (o == ORDER_SPELL_FIREBALL) ||
            (o == ORDER_SPELL_BLIZZARD) || (o == ORDER_SPELL_EYE) || (o == ORDER_SPELL_RAISEDEAD) ||
            (o == ORDER_SPELL_DRAINLIFE) || (o == ORDER_SPELL_WHIRLWIND) || (o == ORDER_SPELL_RUNES) ||
            (o == ORDER_SPELL_ROT) || (o == ORDER_MOVE) || (o == ORDER_PATROL) ||
            (o == ORDER_ATTACK_AREA) || (o == ORDER_ATTACK_WALL) || (o == ORDER_STAND) ||
            (o == ORDER_ATTACK_GROUND) || (o == ORDER_ATTACK_GROUND_MOVE) || (o == ORDER_DEMOLISH) ||
            (o == ORDER_HARVEST) || (o == ORDER_RETURN) || (o == ORDER_UNLOAD_ALL) || (o == ORDER_STOP);

        if (o != ORDER_ATTACK_WALL)
        {
            int ord = *(int*)(ORDER_FUNCTIONS + 4 * o);//orders functions
            if (!aoe && (tr != NULL) && (tr != u))
                ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(u, 0, 0, tr, ord);//original war2 order
            if (aoe)
                ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(u, x, y, NULL, ord);//original war2 order
        }
        else
        {
            byte oru = *(byte*)((uintptr_t)u + S_ORDER);
            if (oru!=ORDER_ATTACK_WALL)
            {
                int ord = *(int*)(ORDER_FUNCTIONS + 4 * ORDER_STOP);//orders functions
                ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(u, 0, 0, NULL, ord);//original war2 order
            }
            set_stat(u, ORDER_ATTACK_WALL, S_NEXT_ORDER);
            set_stat(u, x, S_ORDER_X);
            set_stat(u, y, S_ORDER_Y);
        }

        if (f)
        {
            buf[0] = 0;
            PATCH_SET((char*)GW_ACTION_TYPE, buf);
        }
    }
}

void order_all(byte x, byte y, byte o)
{
    for (int i = 0; i < units; i++)
    {
        give_order(unit[i], x, y, o);
    }
}

bool check_ally(byte p1, byte p2)
{
    //check allied table
    return ((*(byte*)(ALLY + p1 + 16 * p2) != 0) && (*(byte*)(ALLY + p2 + 16 * p1) != 0));
}

bool slot_alive(byte p)
{
    return (get_val(ALL_BUILDINGS, p) + get_val(ALL_UNITS, p)) != 0;//no units and buildings
}

void ally(byte p1, byte p2, byte a)
{
    //set ally bytes in table
    *(byte*)(ALLY + p1 + 16 * p2) = a;
    *(byte*)(ALLY + p2 + 16 * p1) = a;
    ((void (*)())F_RESET_COLORS)();//orig war2 func reset colors of sqares around units
}

void ally_one_sided(byte p1, byte p2, byte a)
{
    //set ally bytes in table
    *(byte*)(ALLY + p1 + 16 * p2) = a;
    ((void (*)())F_RESET_COLORS)();//orig war2 func reset colors of sqares around units
}

bool check_opponents(byte player)
{
    //check if player have opponents
    bool f = false;
    byte o = C_NOBODY;
    for (byte i = 0; i < 8; i++)
    {
        if (player != i)
        {
            if (slot_alive(i) && !check_ally(player, i))//if enemy and not dead
                f = true;
        }
    }
    return f;
}

void viz(int p1, int p2, byte a)
{
    //set vision bits
    byte v = *(byte*)(VIZ + p1);
    if (a == 0)
        v = v & (~(1 << p2));
    else
        v = v | (1 << p2);
    *(byte*)(VIZ + p1) = v;

    v = *(byte*)(VIZ + p2);
    if (a == 0)
        v = v & (~(1 << p1));
    else
        v = v | (1 << p1);
    *(byte*)(VIZ + p2) = v;
}

void viz_one_sided(int p1, int p2, byte a)
{
    //set vision bits
    byte v = *(byte*)(VIZ + p1);
    if (a == 0)
        v = v & (~(1 << p2));
    else
        v = v | (1 << p2);
    *(byte*)(VIZ + p1) = v;
}

void comps_vision(bool v)
{
    //comps can give vision too
    if (v)
    {
        char o[] = "\x0";
        PATCH_SET((char*)COMPS_VIZION, o);
        char o2[] = "\x90\x90";
        PATCH_SET((char*)COMPS_VIZION2, o2);
        char o3[] = "\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char*)COMPS_VIZION3, o3);
    }
    else
    {
        char o[] = "\xAA";
        PATCH_SET((char*)COMPS_VIZION, o);
        char o2[] = "\x84\xC9";
        PATCH_SET((char*)COMPS_VIZION2, o2);
        char o3[] = "\xF\x85\x8C\x0\x0\x0";
        PATCH_SET((char*)COMPS_VIZION3, o3);
    }
}

void change_res(byte p, byte r, byte k, int m)
{
    int a = GOLD;
    int* rs = (int*)a;
    DWORD res = 0;
    bool s = false;
    if (p >= 0 && p <= 8)//player id
    {
        switch (r)//select resource and add or substract it
        {
        case 0:
            a = GOLD + 4 * p;
            s = false;
            break;
        case 1:
            a = LUMBER + 4 * p;
            s = false;
            break;
        case 2:
            a = OIL + 4 * p;
            s = false;
            break;
        case 3:
            a = GOLD + 4 * p;
            s = true;
            break;
        case 4:
            a = LUMBER + 4 * p;
            s = true;
            break;
        case 5:
            a = OIL + 4 * p;
            s = true;
            break;
        default:break;
        }
        if (r >= 0 && r <= 5)
        {
            rs = (int*)a;//resourse pointer
            if (s)
            {
                if (*rs > (int)(k * m))
                    res = *rs - (k * m);
                else
                    res = 0;//canot go smaller than 0
            }
            else
            {
                if (*rs <= (256 * 256 * 256 * 32))
                    res = *rs + (k * m);
            }
            patch_setdword((DWORD*)a, res);
        }
    }
}

void add_total_res(byte p, byte r, byte k, int m)
{
    int a = GOLD_TOTAL;
    int* rs = (int*)a;
    DWORD res = 0;
    if (p >= 0 && p <= 8)//player id
    {
        switch (r)//select resource and add or substract it
        {
        case 0:
            a = GOLD_TOTAL + 4 * p;
            break;
        case 1:
            a = LUMBER_TOTAL + 4 * p;
            break;
        case 2:
            a = OIL_TOTAL + 4 * p;
            break;
        default:break;
        }
        if (r >= 0 && r <= 2)
        {
            rs = (int*)a;//resourse pointer
            if (*rs <= (256 * 256 * 256 * 32))
                res = *rs + (k * m);
            patch_setdword((DWORD*)a, res);
        }
    }
}

void set_res(byte p, byte r, byte k1, byte k2, byte k3, byte k4)
{
    //as before but dnt add or sub res, just set given value
    char buf[4] = { 0 };
    int a = 0;
    if (p >= 0 && p <= 8)
    {
        switch (r)
        {
        case 0:
            a = GOLD + 4 * p;
            break;
        case 1:
            a = LUMBER + 4 * p;
            break;
        case 2:
            a = OIL + 4 * p;
            break;
        default:break;
        }
        if (r >= 0 && r <= 2)
        {
            buf[0] = k1;
            buf[1] = k2;
            buf[2] = k3;
            buf[3] = k4;
            PATCH_SET((char*)a, buf);
        }
    }
}

bool cmp_res(byte p, byte r, byte k1, byte k2, byte k3, byte k4, byte cmp)
{
    //compare resource to value
    int a = GOLD;
    int* rs = (int*)a;
    if (p >= 0 && p <= 8)
    {
        switch (r)
        {
        case 0:
            a = GOLD + 4 * p;
            break;
        case 1:
            a = LUMBER + 4 * p;
            break;
        case 2:
            a = OIL + 4 * p;
            break;
        default:break;
        }
        if (r >= 0 && r <= 2)
        {
            rs = (int*)a;
            return cmp_args4(cmp, *rs, k1 + 256 * k2 + 256 * 256 * k3 + 256 * 256 * 256 * k4);
        }
    }
    return false;
}

int empty_false(byte) { return 0; }//always return false function
int empty_true(byte) { return 1; }//always return true function
void empty_build(int id)
{
    ((void (*)(int))F_TRAIN_UNIT)(id);//original build unit func
}
void empty_build_building(int id)
{
    ((void (*)(int))F_BUILD_BUILDING)(id);//original build func
}
void empty_build_research(int id)
{
    ((void (*)(int))F_BUILD_RESEARCH)(id);
}
void empty_build_research_spell(int id)
{
    ((void (*)(int))F_BUILD_RESEARCH_SPELL)(id);
}
void empty_build_upgrade_self(int id)
{
    ((void (*)(int))F_BUILD_UPGRADE_SELF)(id);
}
void empty_cast_spell(int id)
{
    ((void (*)(int))F_CAST_SPELL)(id);
}

void empty_upgrade_th1(int id)
{
    int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
    set_stat(u, 0, S_AI_AIFLAGS);
    empty_build_upgrade_self(id);
}
void empty_upgrade_th2(int id)
{
    int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
    set_stat(u, 1, S_AI_AIFLAGS);
    empty_build_upgrade_self(id);
}

int empty_research_swords(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SWORDS)(id); }//0 or 1
int empty_research_shield(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SHIELD)(id); }//0 or 1
int empty_research_cat(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_CAT)(id); }//0 or 1
int empty_research_arrows(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_ARROWS)(id); }//0 or 1
int empty_research_ships_at(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SHIPS_AT)(id); }//0 or 1
int empty_research_ships_def(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SHIPS_DEF)(id); }//0 or 1
int empty_research_ranger(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_RANGER)(id); }
int empty_research_scout(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SCOUT)(id); }
int empty_research_long(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_LONG)(id); }
int empty_research_marks(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_MARKS)(id); }
int empty_research_spells(byte id) { return ((int (*)(int))F_CHECK_RESEARCH_SPELL)(id); }
//00444410
int empty_upgrade_th(byte id) { return ((int (*)(int))F_CHECK_UPGRADE_TH)(id); }//0 or 1
int empty_upgrade_tower(byte id) { return ((int (*)(int))F_CHECK_UPGRADE_TOWER)(id); }//0 or 1
int empty_spell_learned(byte id) { return ((int (*)(int))F_CHECK_SPELL_LEARNED)(id); }

int _2tir() 
{ 
    if ((get_val(TH2, *(byte*)LOCAL_PLAYER) != 0) || (get_val(TH3, *(byte*)LOCAL_PLAYER) != 0))
        return 1;
    else
        return 0;
}

int _3tir()
{
    if (get_val(TH3, *(byte*)LOCAL_PLAYER) != 0)
        return 1;
    else
        return 0;
}

int _2tir_L()
{
    if ((get_val(TH2, *(byte*)LOCAL_PLAYER) != 0) || (get_val(TH3, *(byte*)LOCAL_PLAYER) != 0))
    {
        if (get_val(LUMBERMILL, *(byte*)LOCAL_PLAYER) != 0)
            return 1;
        else
            return 0;
    }
    else
        return 0;
}

int empty_res_meds(byte id)
{
    int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
    bool f = false;
    byte d = *((byte*)((uintptr_t)u + S_ID));
    byte pf = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
    if (d == U_STRONGHOLD)
    {
        if (pf != 0)f = true;
    }
    if (d == U_FORTRESS)f = true;
    if (f)return empty_research_ships_at(id);
    else return 0;
}

int empty_res_hammers(byte id)
{
    int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
    bool f = false;
    byte d = *((byte*)((uintptr_t)u + S_ID));
    byte pf = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
    if (d == U_STRONGHOLD)
    {
        if (pf == 0)f = true;
    }
    if (d == U_FORTRESS)f = true;
    if (f)return empty_research_ships_def(id);
    else return 0;
}

int empty_flyer(byte id)
{
    bool f = false;
    if ((*(byte*)LEVEL_OBJ == LVL_HUMAN2) || (*(byte*)LEVEL_OBJ == LVL_HUMAN3))
    {
        int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
        byte d = *((byte*)((uintptr_t)u + S_ID));
        byte pf = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
        if (d == U_STRONGHOLD)
        {
            if (pf != 0)f = true;
        }
        if (d == U_FORTRESS)f = true;
    }
    if (f)return 1;
    else return 0;
}

int get_marks()
{
    if (*(byte*)(GB_MARKS + *(byte*)LOCAL_PLAYER))
        return 1;
    else
        return 0;
}

void repair_cat(bool b)
{
    //peon can repair unit if it have transport flag OR catapult flag
    if (b)
    {
        char r1[] = "\xeb\x75\x90\x90\x90";//f6 c4 04 74 14
        PATCH_SET((char*)REPAIR_FLAG_CHECK2, r1);
        char r2[] = "\x66\xa9\x04\x04\x74\x9c\xeb\x86";
        PATCH_SET((char*)REPAIR_CODE_CAVE, r2);
    }
    else
    {
        char r1[] = "\xf6\xc4\x4\x74\x14";
        PATCH_SET((char*)REPAIR_FLAG_CHECK2, r1);
    }
}

void trigger_time(byte tm)
{
    //war2 will call victory check function every 200 game ticks
    char ttime[] = "\xc8";//200 default
    ttime[0] = tm;
    PATCH_SET((char*)TRIG_TIME, ttime);
}

void manacost(byte id, byte c)
{
    //spells cost of mana
    char mana[] = "\x1";
    mana[0] = c;
    PATCH_SET((char*)(MANACOST + 2 * id), mana);
}

void upgr(byte id, byte c)
{
    //upgrades power
    char up[] = "\x1";
    up[0] = c;
    PATCH_SET((char*)(UPGRD + id), up);
}

byte get_upgrade(byte id, byte pl)
{
    int a = GB_ARROWS;
    switch (id)
    {
    case ARROWS:a = GB_ARROWS; break;
    case SWORDS:a = GB_SWORDS; break;
    case ARMOR:a = GB_SHIELDS; break;
    case SHIP_DMG:a = GB_BOAT_ATTACK; break;
    case SHIP_ARMOR:a = GB_BOAT_ARMOR; break;
    case SHIP_SPEED:a = GB_BOAT_SPEED; break;
    case CATA_DMG:a = GB_CAT_DMG; break;
    default:a = GB_ARROWS; break;
    }
    return *(byte*)(a + pl);
}

void set_upgrade(byte id, byte pl, byte v)
{
    int a = GB_ARROWS;
    switch (id)
    {
    case ARROWS:a = GB_ARROWS; break;
    case SWORDS:a = GB_SWORDS; break;
    case ARMOR:a = GB_SHIELDS; break;
    case SHIP_DMG:a = GB_BOAT_ATTACK; break;
    case SHIP_ARMOR:a = GB_BOAT_ARMOR; break;
    case SHIP_SPEED:a = GB_BOAT_SPEED; break;
    case CATA_DMG:a = GB_CAT_DMG; break;
    default:a = GB_ARROWS; break;
    }
    char buf[] = "\x0";
    buf[0] = v;
    PATCH_SET((char*)(a + pl), buf);
    ((void (*)())F_STATUS_REDRAW)();//status redraw
}

int upgr_check_swords(byte b)
{
    byte u = get_upgrade(SWORDS, *(byte*)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))return 1;
    if ((b == 1) && (u == 1))return 1;
    if ((b == 2) && (u >= 2))return 1;
    return 0;
}

int upgr_check_shields(byte b)
{
    byte u = get_upgrade(ARMOR, *(byte*)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))return 1;
    if ((b == 1) && (u == 1))return 1;
    if ((b == 2) && (u >= 2))return 1;
    return 0;
}

int upgr_check_boat_attack(byte b)
{
    byte u = get_upgrade(SHIP_DMG, *(byte*)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))return 1;
    if ((b == 1) && (u == 1))return 1;
    if ((b == 2) && (u >= 2))return 1;
    return 0;
}

int upgr_check_boat_armor(byte b)
{
    byte u = get_upgrade(SHIP_ARMOR, *(byte*)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))return 1;
    if ((b == 1) && (u == 1))return 1;
    if ((b == 2) && (u >= 2))return 1;
    return 0;
}

int upgr_check_arrows(byte b)
{
    byte u = get_upgrade(ARROWS, *(byte*)LOCAL_PLAYER);
    if ((b == 0) && (u == 0))return 1;
    if ((b == 1) && (u == 1))return 1;
    if ((b == 2) && (u >= 2))return 1;
    return 0;
}

void upgr_check_replace(bool f)
{
    if (f)
    {
        char buf[] = "\xC3";//ret
        patch_ljmp((char*)UPGR_CHECK_FIX_SWORDS, (char*)upgr_check_swords);
        PATCH_SET((char*)(UPGR_CHECK_FIX_SWORDS + 5), buf);
        patch_ljmp((char*)UPGR_CHECK_FIX_SHIELDS, (char*)upgr_check_shields);
        PATCH_SET((char*)(UPGR_CHECK_FIX_SHIELDS + 5), buf);
        patch_ljmp((char*)UPGR_CHECK_FIX_SHIPS_AT, (char*)upgr_check_boat_attack);
        PATCH_SET((char*)(UPGR_CHECK_FIX_SHIPS_AT + 5), buf);
        patch_ljmp((char*)UPGR_CHECK_FIX_SHIPS_DEF, (char*)upgr_check_boat_armor);
        PATCH_SET((char*)(UPGR_CHECK_FIX_SHIPS_DEF + 5), buf);
        patch_ljmp((char*)UPGR_CHECK_FIX_ARROWS, (char*)upgr_check_arrows);
        PATCH_SET((char*)(UPGR_CHECK_FIX_ARROWS + 5), buf);
    }
    else
    {
        char buf2[] = "\x33\xC0\x33\xC9\xA0\x18";//back
        PATCH_SET((char*)UPGR_CHECK_FIX_SWORDS, buf2);
        PATCH_SET((char*)UPGR_CHECK_FIX_SHIELDS, buf2);
        PATCH_SET((char*)UPGR_CHECK_FIX_SHIPS_AT, buf2);
        PATCH_SET((char*)UPGR_CHECK_FIX_SHIPS_DEF, buf2);
        PATCH_SET((char*)UPGR_CHECK_FIX_ARROWS, buf2);
    }
}

void center_view(byte x, byte y)
{
    ((void (*)(byte, byte))F_MINIMAP_CLICK)(x, y);//original war2 func that called when player click on minimap
}

PROC g_proc_00451054;
void count_add_to_tables_load_game(int* u)
{
    byte l = *(byte*)LOCAL_PLAYER;
    byte id = *((byte*)((uintptr_t)u + S_ID));
    if (id == U_PIGFARM)
    {
        WORD fd = *(WORD*)(FOOD_LIMIT + 2 * l);
        if (fd < 200)fd++;//add 1 more food limit from farm
        *(WORD*)(FOOD_LIMIT + 2 * l) = fd;
    }
    if (saveload_fixed)
    {
        byte f = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
        byte ff = f | AI_PASSIVE;
        set_stat(u, ff, S_AI_AIFLAGS);
        ((void (*)(int*))g_proc_00451054)(u);//original
        set_stat(u, f, S_AI_AIFLAGS);
    }
    else
        ((void (*)(int*))g_proc_00451054)(u);//original
}

PROC g_proc_00438A5C;
PROC g_proc_00438985;
void unset_peon_ai_flags(int* u)
{
    ((void (*)(int*))g_proc_00438A5C)(u);//original
    if (saveload_fixed)
    {
        char rep[] = "\x0\x0";
        WORD p = 0;
        for (int i = 0; i < 8; i++)
        {
            p = *((WORD*)((uintptr_t)SGW_REPAIR_PEONS + 2 * i));
            if (p > 1600)
                PATCH_SET((char*)(SGW_REPAIR_PEONS + 2 * i), rep);
            p = *((WORD*)((uintptr_t)SGW_GOLD_PEONS + 2 * i));
            if (p > 1600)
                PATCH_SET((char*)(SGW_GOLD_PEONS + 2 * i), rep);
            p = *((WORD*)((uintptr_t)SGW_TREE_PEONS + 2 * i));
            if (p > 1600)
                PATCH_SET((char*)(SGW_TREE_PEONS + 2 * i), rep);
        }
    }
}

void tech_built(int p, byte t)
{
    ((void (*)(int, byte))F_TECH_BUILT)(p, t);
}

void tech_reinit()
{
    for (int i = 0; i < 8; i++)
    {
        byte o = *(byte*)(CONTROLER_TYPE + i);
        byte a = 0;
        int s = 0;
        if (o == C_COMP)
        {
            a = *(byte*)(GB_ARROWS + i);
            if (a > 0)tech_built(i, UP_ARROW1);
            if (a > 1)tech_built(i, UP_ARROW2);
            a = *(byte*)(GB_SWORDS + i);
            if (a > 0)tech_built(i, UP_SWORD1);
            if (a > 1)tech_built(i, UP_SWORD2);
            a = *(byte*)(GB_SHIELDS + i);
            if (a > 0)tech_built(i, UP_SHIELD1);
            if (a > 1)tech_built(i, UP_SHIELD2);
            a = *(byte*)(GB_BOAT_ATTACK + i);
            if (a > 0)tech_built(i, UP_BOATATK1);
            if (a > 1)tech_built(i, UP_BOATATK2);
            a = *(byte*)(GB_BOAT_ARMOR + i);
            if (a > 0)tech_built(i, UP_BOATARM1);
            if (a > 1)tech_built(i, UP_BOATARM2);
            a = *(byte*)(GB_CAT_DMG + i);
            if (a > 0)tech_built(i, UP_CATDMG1);
            if (a > 1)tech_built(i, UP_CATDMG2);
            a = *(byte*)(GB_RANGER + i);
            if (a)tech_built(i, UP_RANGER);
            a = *(byte*)(GB_MARKS + i);
            if (a)tech_built(i, UP_SKILL1);
            a = *(byte*)(GB_LONGBOW + i);
            if (a)tech_built(i, UP_SKILL2);
            a = *(byte*)(GB_SCOUTING + i);
            if (a)tech_built(i, UP_SKILL3);

            s = *(int*)(SPELLS_LEARNED + 4 * i);
            if (s & (1 << L_ALTAR_UPGR))tech_built(i, UP_CLERIC);
            if (s & (1 << L_HEAL))tech_built(i, UP_CLERIC1);
            if (s & (1 << L_BLOOD))tech_built(i, UP_CLERIC1);
            if (s & (1 << L_EXORCISM))tech_built(i, UP_CLERIC2);
            if (s & (1 << L_RUNES))tech_built(i, UP_CLERIC2);
            if (s & (1 << L_FLAME_SHIELD))tech_built(i, UP_WIZARD1);
            if (s & (1 << L_RAISE))tech_built(i, UP_WIZARD1);
            if (s & (1 << L_SLOW))tech_built(i, UP_WIZARD2);
            if (s & (1 << L_HASTE))tech_built(i, UP_WIZARD2);
            if (s & (1 << L_INVIS))tech_built(i, UP_WIZARD3);
            if (s & (1 << L_WIND))tech_built(i, UP_WIZARD3);
            if (s & (1 << L_POLYMORF))tech_built(i, UP_WIZARD4);
            if (s & (1 << L_UNHOLY))tech_built(i, UP_WIZARD4);
            if (s & (1 << L_BLIZZARD))tech_built(i, UP_WIZARD5);
            if (s & (1 << L_DD))tech_built(i, UP_WIZARD5);

            find_all_alive_units(U_KEEP);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)tech_built(i, UP_KEEP);
            find_all_alive_units(U_STRONGHOLD);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)tech_built(i, UP_KEEP);
            find_all_alive_units(U_CASTLE);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)
            {
                tech_built(i, UP_KEEP);
                tech_built(i, UP_CASTLE);
            }
            find_all_alive_units(U_FORTRESS);
            sort_stat(S_OWNER, i, CMP_EQ);
            if (units != 0)
            {
                tech_built(i, UP_KEEP);
                tech_built(i, UP_CASTLE);
            }
        }
    }
}

void ai_fix_plugin(bool f)
{
    if (f)
    {
        char b1[] = "\xb2\x02";
        PATCH_SET((char*)AIFIX_PEONS_REP, b1);//2 peon rep
        char b21[] = "\xbb\x8";
        PATCH_SET((char*)AIFIX_GOLD_LUMB1, b21);//gold lumber
        char b22[] = "\xb4\x4";
        PATCH_SET((char*)AIFIX_GOLD_LUMB2, b22);//gold lumber
        char b3[] = "\x1";
        PATCH_SET((char*)AIFIX_BUILD_SIZE, b3);//packed build
        char b4[] = "\xbe\x0\x0\x0\x0\x90\x90";
        PATCH_SET((char*)AIFIX_FIND_HOME, b4);//th corner
        char b6[] = "\x90\x90\x90\x90\x90\x90";
        PATCH_SET((char*)AIFIX_POWERBUILD, b6);//powerbuild
        ai_fixed = true;
    }
    else
    {
        char b1[] = "\x8a\xd0";
        PATCH_SET((char*)AIFIX_PEONS_REP, b1);//2 peon rep
        char b21[] = "\xd0\x7";
        PATCH_SET((char*)AIFIX_GOLD_LUMB1, b21);//gold lumber
        char b22[] = "\xf4\x1";
        PATCH_SET((char*)AIFIX_GOLD_LUMB2, b22);//gold lumber
        char b3[] = "\x6";
        PATCH_SET((char*)AIFIX_BUILD_SIZE, b3);//packed build
        char b4[] = "\xe8\xf8\x2a\x1\x0\x8b\xf0";
        PATCH_SET((char*)AIFIX_FIND_HOME, b4);//th corner
        char b6[] = "\xf\x84\x78\x1\x0\x0";
        PATCH_SET((char*)AIFIX_POWERBUILD, b6);//powerbuild
        ai_fixed = false;
    }
}

PROC g_proc_0040EEDD;
void upgrade_tower(int* u, int id, int b)
{
    if (ai_fixed)
    {
        bool c = false;
        byte o = *((byte*)((uintptr_t)u + S_OWNER));
        if ((get_val(LUMBERMILL, o) == 0) && (get_val(SMITH, o) != 0)) c = true;
        if ((get_val(LUMBERMILL, o) != 0) && (get_val(SMITH, o) != 0) && ((get_val(TOWER, o) % 2) == 0)) c = true;
        if (c)id += 2;
    }
    ((void (*)(int*, int, int))g_proc_0040EEDD)(u, id, b);//original
}

PROC g_proc_00442E25;
void create_skeleton(int x, int y, int id, int o)
{
    if (ai_fixed)
    {
        unit_create((x / 32) + 1, y / 32, id, o % 256, 1);
    }
    else
        ((void (*)(int, int, int, int))g_proc_00442E25)(x, y, id, o);//original
}

PROC g_proc_00425D1C;
int* cast_raise(int* u, int a1, int a2, int a3)
{
    if (ai_fixed)
    {
        byte o = *((byte*)((uintptr_t)u + S_OWNER));
        find_all_alive_units(U_SKELETON);
        sort_stat(S_OWNER, o, CMP_EQ);
        sort_preplaced();
        if (units < 10)
        {
            if (((*(DWORD*)(SPELLS_LEARNED + 4 * o) & (1 << L_RAISE)) == 0))return NULL;
            byte mp = *((byte*)((uintptr_t)u + S_MANA));
            byte cost = *(byte*)(MANACOST + 2 * RAISE_DEAD);
            if (mp < cost)return NULL;
            byte x = *((byte*)((uintptr_t)u + S_X));
            byte y = *((byte*)((uintptr_t)u + S_Y));
            set_region((int)x - 8, (int)y - 8, (int)x + 8, (int)y + 8);//set region around myself
            find_all_units(ANY_BUILDING);//dead body
            sort_in_region();
            sort_hidden();
            sort_near_death();
            if (units != 0)
            {
                byte xx = *((byte*)((uintptr_t)unit[0] + S_X));
                byte yy = *((byte*)((uintptr_t)unit[0] + S_Y));
                give_order(u, xx, yy, ORDER_SPELL_RAISEDEAD);
                return unit[0];
            }
        }
        return NULL;
    }
    else
        return ((int* (*)(int*, int, int, int))g_proc_00425D1C)(u, a1, a2, a3);//original
}

PROC g_proc_0042757E;
int ai_spell(int* u)
{
    if (ai_fixed)
    {
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if ((id == U_MAGE) || (id = U_DK))
        {
            byte x = *((byte*)((uintptr_t)u + S_X));
            byte y = *((byte*)((uintptr_t)u + S_Y));
            set_region((int)x - 30, (int)y - 30, (int)x + 30, (int)y + 30);//set region around myself
            find_all_alive_units(ANY_UNITS);
            sort_in_region();
            byte o = *((byte*)((uintptr_t)u + S_OWNER));
            for (int ui = 0; ui < 16; ui++)
            {
                if (check_ally(o, ui))
                    sort_stat(S_OWNER, ui, CMP_NEQ);
            }
            if (units != 0)
                return ((int (*)(int*))g_proc_0042757E)(u);//original
        }
        else
            return ((int (*)(int*))g_proc_0042757E)(u);//original
        return 0;
    }
    else
        return ((int (*)(int*))g_proc_0042757E)(u);//original
}

PROC g_proc_00427FAE;
void ai_attack(int* u, int b, int a)
{
    if (ai_fixed)
    {
        byte o = *((byte*)((uintptr_t)u + S_OWNER));
        for (int i = 0; i < 16; i++)
        {
            int* p = (int*)(UNITS_LISTS + 4 * i);
            if (p)
            {
                p = (int*)(*p);
                while (p)
                {
                    bool f = ((*((byte*)((uintptr_t)p + S_ID)) == U_MAGE) || (*((byte*)((uintptr_t)p + S_ID)) == U_DK));
                    if (f)
                    {
                        if (!check_unit_dead(p) && !check_unit_hidden(p))
                        {
                            byte ow = *((byte*)((uintptr_t)p + S_OWNER));
                            if (ow==o)
                            {
                                if ((*(byte*)(CONTROLER_TYPE + o) == C_COMP))
                                {
                                    WORD inv = *((WORD*)((uintptr_t)p + S_INVIZ));
                                    if (inv == 0)
                                    {
                                        byte aor = *((byte*)((uintptr_t)p + 94));
                                        if (aor != 2)
                                        {
                                            byte x = *((byte*)((uintptr_t)u + S_X));
                                            byte y = *((byte*)((uintptr_t)u + S_Y));
                                            set_region((int)x - 30, (int)y - 30, (int)x + 30, (int)y + 30);//set region around myself
                                            find_all_alive_units(ANY_UNITS);
                                            sort_in_region();
                                            for (int ui = 0; ui < 16; ui++)
                                            {
                                                if (check_ally(ow, ui))
                                                    sort_stat(S_OWNER, ui, CMP_NEQ);
                                            }
                                            if (units == 0)
                                                ((void (*)(int*, int, int))F_ICE_SET_AI_ORDER)(p, AI_ORDER_ATTACK, a);//ai attack
                                        }
                                    }
                                }
                            }
                        }
                    }
                    p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
                }
            }
        }

        find_all_alive_units(ANY_MEN);
        sort_stat(S_ID, U_DWARWES, CMP_BIGGER_EQ);
        sort_stat(S_ID, U_GOBLINS, CMP_SMALLER_EQ);
        sort_stat(S_OWNER, o, CMP_EQ);
        sort_stat(S_AI_ORDER, AI_ORDER_ATTACK, CMP_NEQ);//not attack already
        for (int i = 0; i < units; i++)
        {
            ((void (*)(int*, int, int))F_ICE_SET_AI_ORDER)(unit[i], AI_ORDER_ATTACK, a);//ai attack
        }
    }
    ((void (*)(int*, int, int))g_proc_00427FAE)(u, b, a);//original
}

void unstuk()
{
    for (int i = 0; i < 16; i++)
    {
        int* p = (int*)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int*)(*p);
            while (p)
            {
                byte id = *((byte*)((uintptr_t)p + S_ID));
                byte ord = *((byte*)((uintptr_t)p + S_ORDER));
                byte aord = *((byte*)((uintptr_t)p + 94));
                bool f = ((((id < U_CRITTER))
                    && !check_unit_preplaced(p) && (ord == ORDER_STOP) && (aord == 2)) ||
                    ((id == U_PEASANT) || (id == U_PEON)));
                if (f)
                {
                    if (!check_unit_dead(p) && !check_unit_hidden(p))
                    {
                        byte o = *((byte*)((uintptr_t)p + S_OWNER));
                        if ((*(byte*)(CONTROLER_TYPE + o) == C_COMP))
                        {
                            byte st = *((byte*)((uintptr_t)p + S_NEXT_FIRE));
                            byte frm = *((byte*)((uintptr_t)p + S_FRAME));
                            byte pfrm = *((byte*)((uintptr_t)p + S_NEXT_FIRE + 1));
                            if (st == 0)
                            {
                                byte map = *(byte*)MAP_SIZE - 1;
                                byte x = *((byte*)((uintptr_t)p + S_X));
                                byte y = *((byte*)((uintptr_t)p + S_Y));
                                int xx = 0, yy = 0, dir = 0;
                                xx += x;
                                yy += y;
                                dir = ((int (*)())F_NET_RANDOM)();
                                dir %= 8;
                                if (dir == 0)
                                {
                                    if (yy > 0)yy -= 1;
                                }
                                if (dir == 1)
                                {
                                    if (yy > 0)yy -= 1;
                                    if (xx < map)xx += 1;
                                }
                                if (dir == 2)
                                {
                                    if (xx < map)xx += 1;
                                }
                                if (dir == 3)
                                {
                                    if (xx < map)xx += 1;
                                    if (yy < map)yy += 1;
                                }
                                if (dir == 4)
                                {
                                    if (yy < map)yy += 1;
                                }
                                if (dir == 5)
                                {
                                    if (yy < map)yy += 1;
                                    if (xx > 0)xx -= 1;
                                }
                                if (dir == 6)
                                {
                                    if (xx > 0)xx -= 1;
                                }
                                if (dir == 7)
                                {
                                    if (xx > 0)xx -= 1;
                                    if (yy > 0)yy -= 1;
                                }
                                if ((id != U_PEON) && (id != U_PEASANT))
                                {
                                    int* trg = NULL;
                                    find_all_alive_units(ANY_UNITS);
                                    sort_hidden();
                                    byte mv = *((byte*)((uintptr_t)p + S_MOVEMENT_TYPE));
                                    if (mv == MOV_LAND)
                                        sort_stat(S_MOVEMENT_TYPE, MOV_LAND, CMP_EQ);
                                    sort_attack_can_hit_range(p);
                                    for (int ui = 0; ui < 16; ui++)
                                    {
                                        if (check_ally(o, ui))//only not allied units
                                            sort_stat(S_OWNER, ui, CMP_NEQ);
                                    }
                                    if (units == 0)
                                    {
                                        find_all_alive_units(ANY_UNITS);
                                        sort_hidden();
                                        byte mv = *((byte*)((uintptr_t)p + S_MOVEMENT_TYPE));
                                        sort_attack_can_hit_range(p);
                                        for (int ui = 0; ui < 16; ui++)
                                        {
                                            if (check_ally(o, ui))//only not allied units
                                                sort_stat(S_OWNER, ui, CMP_NEQ);
                                        }
                                    }
                                    if (units != 0)
                                    {
                                        WORD dist = 0xFFFF;
                                        int ndu = -1;
                                        for (int j = 0; j < units; j++)
                                        {
                                            struct GPOINT
                                            {
                                                WORD x;
                                                WORD y;
                                            };
                                            GPOINT l;
                                            l.x = *((WORD*)((uintptr_t)p + S_X));
                                            l.y = *((WORD*)((uintptr_t)p + S_Y));
                                            WORD dst = 0;
                                            dst = ((WORD(*)(GPOINT*, int*))F_MTX_DIST)(&l, unit[j]);//mtx dist
                                            if (dst < dist)
                                            {
                                                dist = dst;
                                                ndu = j;
                                            }
                                        }
                                        if (ndu != -1)trg = unit[ndu];
                                    }
                                    if (trg)
                                    {
                                        struct GPOINT
                                        {
                                            WORD x;
                                            WORD y;
                                        };
                                        GPOINT l;
                                        l.x = *((WORD*)((uintptr_t)trg + S_X));
                                        l.y = *((WORD*)((uintptr_t)trg + S_Y));
                                        units = 1;
                                        unit[0] = trg;
                                        sort_attack_can_hit_range(p);
                                        int sa = 0;
                                        if (units != 0)
                                            sa = ((int (*)(int*, int, GPOINT*))F_ICE_SET_AI_ORDER)(p, 2, &l);
                                        if (sa)give_order(p, xx % 256, yy % 256, ORDER_ATTACK_AREA);
                                    }
                                }
                                else
                                    give_order(p, xx % 256, yy % 256, ORDER_MOVE);
                                st = 10;
                            }
                            if (st > 0)st -= 1;
                            if (frm != pfrm)st = 255;
                            set_stat(p, st, S_NEXT_FIRE);
                            set_stat(p, frm, S_NEXT_FIRE + 1);
                        }
                    }
                }
                p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void goldmine_ai()
{
    for (int i = 0; i < 16; i++)
    {
        int* p = (int*)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int*)(*p);
            while (p)
            {
                bool f = ((*((byte*)((uintptr_t)p + S_ID)) == U_MINE));
                if (f)
                {
                    if (check_unit_complete(p))
                    {
                        byte x = *((byte*)((uintptr_t)p + S_X));
                        byte y = *((byte*)((uintptr_t)p + S_Y));
                        set_region((int)x - 9, (int)y - 9, (int)x + 8, (int)y + 8);
                        find_all_alive_units(ANY_BUILDING_4x4);
                        sort_in_region();
                        sort_stat(S_ID, U_PORTAL, CMP_NEQ);
                        bool th = units != 0;
                        byte x1, y1, x2, y2;
                        if (x > 3)x1 = x - 3;
                        else x1 = 0;
                        if (y > 3)y1 = y - 3;
                        else y1 = 0;
                        x += 3;
                        y += 3;
                        if (x >= (127 - 3))x2 = 127;
                        else x2 = x + 3;
                        if (y >= (127 - 3))y2 = 127;
                        else y2 = y + 3;
                        char* sq = (char*)*(int*)(MAP_SQ_POINTER);
                        byte mxs = *(byte*)MAP_SIZE;//map size
                        for (int xx = x1; xx < x2; xx++)
                        {
                            for (int yy = y1; yy < y2; yy++)
                            {
                                char buf[] = "\x0";
                                buf[0] = *(char*)(sq + 2 * xx + 2 * yy * mxs + 1);
                                if (th)buf[0] |= SQ_AI_BUILDING >> 8;
                                else buf[0] &= ~(SQ_AI_BUILDING >> 8);
                                PATCH_SET((char*)(sq + 2 * xx + 2 * yy * mxs + 1), buf);
                            }
                        }
                    }
                }
                p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void ret_units()
{
    for (int i = 0; i < units; i++)
    {
        byte o = *(byte*)((uintptr_t)unit[i] + S_LAST_HARVEST_X);
        give_order(unit[i], 0, 0, ORDER_STOP);
        give(unit[i], o);
        byte pf = *((byte*)((uintptr_t)unit[i] + S_PEON_FLAGS));
        if (pf & 0x1)
        {
            pf &= ~0x1;
            set_stat(unit[i], pf, S_PEON_FLAGS);
            byte f2 = *((byte*)((uintptr_t)unit[i] + S_FLAGS2));
            f2 |= 0x1;
            set_stat(unit[i], f2, S_FLAGS2);
        }
    }
}

void give_order_ragekill(int* u, byte x, byte y)
{
    byte id = *((byte*)((uintptr_t)u + S_ID));
    if (id < U_FARM)
    {
        int* tr = NULL;
        for (int i = 0; i < 16; i++)
        {
            int* p = (int*)(UNITS_LISTS + 4 * i);//pointer to units list for each player
            if (p)
            {
                p = (int*)(*p);
                while (p)
                {
                    if (p != u)
                    {
                        if (!check_unit_dead(p) && !check_unit_hidden(p))
                        {
                            byte oa = *(byte*)((uintptr_t)u + S_OWNER);
                            byte ot = *(byte*)((uintptr_t)p + S_OWNER);
                            if (!check_ally(oa, ot))
                            {
                                byte xx = *(byte*)((uintptr_t)p + S_X);
                                byte yy = *(byte*)((uintptr_t)p + S_Y);
                                if ((abs(x - xx) <= 2) && (abs(y - yy) <= 2))
                                {
                                    tr = p;
                                }
                            }
                        }
                    }
                    p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
                }
            }
        }
        int ord = *(int*)(ORDER_FUNCTIONS + 4 * ORDER_ATTACK_TARGET);//orders functions
        if (tr != NULL)
            ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(u, 0, 0, tr, ord);//original war2 order
    }
}

void rage_units()
{
    for (int i = 0; i < units; i++)
    {
        byte x = *(byte*)((uintptr_t)unit[i] + S_X);
        byte y = *(byte*)((uintptr_t)unit[i] + S_Y);
        give_order_ragekill(unit[i], x, y);
    }
}

int cata_real_buttons = 0;

char buttons_cata[10 * BUTTON_SIZE + 1];
void buttons_change_cata()
{
    byte lvl = *(byte*)LEVEL_OBJ;
    if (lvl < LVL_HUMAN12)
    {
        patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_CATAPULT), 0);
        patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_CATAPULT + 4), 0);
    }
    else
    {
        for (int i = 0; i < 200; i++)
        {
            buttons_cata[i] = *(byte*)(cata_real_buttons + i);
        }
        buttons_cata[2 + 80] = 146;
        patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_CATAPULT), (DWORD)10);
        patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_CATAPULT + 4), (DWORD)buttons_cata);
    }
}

void golem_ai()
{
    find_all_alive_units(U_PALADIN);
    if (units != 0)
    {
        int* u = unit[0];
        byte o = *(byte*)((uintptr_t)u + S_OWNER);
        byte x = *(byte*)((uintptr_t)u + S_X);
        byte y = *(byte*)((uintptr_t)u + S_Y);
        set_region(x - 3, y - 3, x + 3, y + 3);
        find_all_alive_units(ANY_MEN);
        sort_self(u);
        sort_in_region();
        for (int i = 0; i < 8; i++)
        {
            if (check_ally(i, o))
                sort_stat(S_OWNER, i, CMP_NEQ);
        }
        byte an = *(byte*)((uintptr_t)u + S_ANIMATION);
        byte fr = *(byte*)((uintptr_t)u + S_FRAME);
        if ((an != ANIM_ATTACK) || (fr == 0))
        {
            if (units == 0)
            {
                char r[] = "\x8";
                PATCH_SET((char*)(UNIT_RANGE_TABLE + U_PALADIN), r);
                r[0] = B_BIG_CANNON;
                PATCH_SET((char*)(UNIT_BULLET_TABLE + U_PALADIN), r);
            }
            else
            {

                char r[] = "\x1";
                PATCH_SET((char*)(UNIT_RANGE_TABLE + U_PALADIN), r);
                r[0] = B_NONE;
                PATCH_SET((char*)(UNIT_BULLET_TABLE + U_PALADIN), r);
                set_region(x - 2, y - 2, x + 2, y + 2);
                sort_in_region();
                if (units >= 3)
                {
                    if (*(byte*)GB_HORSES > 10)
                    {
                        byte ord = *(byte*)((uintptr_t)u + S_ORDER);
                        if (ord != ORDER_SPELL_RUNES)
                        {
                            byte xx = *(byte*)((uintptr_t)unit[0] + S_X);
                            byte yy = *(byte*)((uintptr_t)unit[0] + S_Y);
                            give_order(u, xx, yy, ORDER_SPELL_RUNES);
                        }
                    }
                }
            }
        }
    }
}

PROC g_proc_0045271B;
void update_spells()
{
    ((void (*)())g_proc_0045271B)();//original
    // this function called every game tick
    // you can place your self-writed functions here in the end if need

	if (saveload_fixed)tech_reinit();
    if (ai_fixed)
    {
		unstuk();
		goldmine_ai();
    }
	if (vizs_n > 0)
    {
        for (int i = 0; i < vizs_n; i++)
        {
            viz_area(vizs_areas[i].x, vizs_areas[i].y, vizs_areas[i].p, vizs_areas[i].s);
        }
    }

    buttons_change_cata();
    golem_ai();
    
    find_all_alive_units(U_GRUNT);
    for (int i = 0; i < units; i++)
    {
        int* u = unit[i];
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if (id == U_GRUNT)
        {
            byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
            if (((pf & PEON_HARVEST_GOLD) == 0) && ((pf & PEON_HARVEST_LUMBER) == 0))
            {
                int d = ((int (*)())F_NET_RANDOM)() % 3;
                if (d == 0)
                    pf |= PEON_HARVEST_GOLD;
                else if (d == 1)
                    pf |= PEON_HARVEST_LUMBER;
                else if (d == 2)
                {
                    pf |= PEON_HARVEST_GOLD;
                    pf |= PEON_HARVEST_LUMBER;
                }
                set_stat(u, pf, S_PEON_FLAGS);
            }
        }
    }

    find_all_alive_units(ANY_MEN);
    sort_stat(S_OWNER, PLAYER14, CMP_EQ);
    sort_stat(S_BLOOD, 0, CMP_EQ);
    ret_units();

    find_all_alive_units(ANY_MEN);
    sort_stat(S_OWNER, PLAYER14, CMP_EQ);
    sort_stat(S_BLOOD, 0, CMP_NEQ);
    sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
    rage_units();

    char buf[] = "\x0";
    buf[0] = 0;
    if (get_val(LUMBERMILL, *(byte*)LOCAL_PLAYER) != 0)
        buf[0] = 25;
    if (*(byte*)(GB_LONGBOW + *(byte*)LOCAL_PLAYER) == 1)
        buf[0] += 25;
    PATCH_SET((char*)TEXT_PRODUCTION_LUMBER_TH, buf);//lumber amount
    PATCH_SET((char*)TEXT_PRODUCTION_LUMBER_MILL, buf);//lumber amount mill
    buf[0] = 0;
    if (get_val(TH2, *(byte*)LOCAL_PLAYER) != 0)
        buf[0] = 10;
    if (get_val(TH3, *(byte*)LOCAL_PLAYER) != 0)
        buf[0] = 20;
    if (get_val(SMITH, *(byte*)LOCAL_PLAYER) != 0)
        buf[0] += 10;
    if (get_upgrade(CATA_DMG, *(byte*)LOCAL_PLAYER) != 0)
        buf[0] += 10;
    if (*(byte*)(GB_RANGER + *(byte*)LOCAL_PLAYER) == 1)
        buf[0] += 10;
    PATCH_SET((char*)TEXT_PRODUCTION_GOLD_TH, buf);//gold amount

    find_all_alive_units(U_GREAT_HALL);
    for (int i = 0; i < units; i++)
    {
        byte bord = *((byte*)((uintptr_t)unit[i] + S_BUILD_ORDER));
        byte ord = *((byte*)((uintptr_t)unit[i] + S_ORDER));
        if ((bord == 3) && (ord == ORDER_BLDG_BUILD))
        {
            byte sf = *((byte*)((uintptr_t)unit[i] + S_SEQ_FLAG));
            sf |= 0x20;
            set_stat(unit[i], sf, S_SEQ_FLAG);
        }
    }
    find_all_alive_units(U_STRONGHOLD);
    for (int i = 0; i < units; i++)
    {
        byte bord = *((byte*)((uintptr_t)unit[i] + S_BUILD_ORDER));
        byte ord = *((byte*)((uintptr_t)unit[i] + S_ORDER));
        if ((bord == 3) && (ord == ORDER_BLDG_BUILD))
        {
            byte sf = *((byte*)((uintptr_t)unit[i] + S_SEQ_FLAG));
            sf |= 0x20;
            set_stat(unit[i], sf, S_SEQ_FLAG);
        }
    }

    ((void (*)(int, int, int, int))F_INVALIDATE)(0, 0, m_screen_w, m_screen_h);//screen redraw
}

char buttons_hero[12 * BUTTON_SIZE + 1];

void buttons_change_hero()
{
    if ((*(byte*)(UNIT_RANGE_TABLE + U_DANATH)) == 1)
    {
        buttons_hero[200 + 2] = '\x97';
        buttons_hero[80 + 2] = '\x98';
        buttons_hero[100 + 2] = '\x98';
        buttons_hero[120 + 2] = '\x98';
    }
    else
    {
        buttons_hero[200 + 2] = '\x98';
        buttons_hero[80 + 2] = '\x97';
        buttons_hero[100 + 2] = '\x97';
        buttons_hero[120 + 2] = '\x97';
    }
}

void seq_change(int* u, byte tt)
{
    byte t = tt;
    if (t == 1)
    {
        byte t = *((byte*)((uintptr_t)u + S_ANIMATION_TIMER));
        byte id = *((byte*)((uintptr_t)u + S_ID));
        byte a = *((byte*)((uintptr_t)u + S_ANIMATION));
        byte f = *((byte*)((uintptr_t)u + S_FRAME));
        byte o = *((byte*)((uintptr_t)u + S_OWNER));
        if ((id == U_LOTHAR) && (o == *(byte*)LOCAL_PLAYER))
        {
            if (a == ANIM_MOVE)
            {
                if (get_upgrade(ARROWS, o) == 0)t = 2;
            }
            if (a == ANIM_ATTACK)
            {
                if ((f == 25) || (f == 30) || (f == 35))
                {
                    if (get_upgrade(ARROWS, o) == 0)
                        t += 1;
                    else
                        t = 1;
                }
                else
                {
                    if (get_upgrade(ARROWS, o) == 0)
                        t += 2;
                }
            }
            set_stat(u, t, S_ANIMATION_TIMER);
        }
        if (id == U_DANATH)
        {
            byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
            if (pf != 0)
            {
                if ((a != ANIM_ATTACK) || ((a == ANIM_ATTACK) && (f == 0)))
                {
                    char r[] = "\x1";//range
                    char rr[] = "\x1";//target
                    if ((*(byte*)(UNIT_RANGE_TABLE + U_DANATH)) == 1)
                    {
                        r[0] = '\x5';
                        rr[0] = '\x7';
                    }
                    else
                    {
                        r[0] = '\x1';
                        rr[0] = '\x1';
                    }
                    PATCH_SET((char*)(UNIT_RANGE_TABLE + U_DANATH), r);
                    PATCH_SET((char*)(UNIT_CAN_TARGET + U_DANATH), rr);
                    buttons_change_hero();
                    ((void (*)())F_STATUS_REDRAW)();//status redraw
                    set_stat(u, 0, S_PEON_FLAGS);
                }
            }
            if (a == ANIM_ATTACK)
            {
                if ((*(byte*)(UNIT_RANGE_TABLE + U_DANATH)) != 1)
                    t += 5;
            }
            set_stat(u, t, S_ANIMATION_TIMER);
        }
        if ((id == U_BALLISTA) && ((o == *(byte*)LOCAL_PLAYER) || (o == P_BLUE)))
        {
            if (a == ANIM_MOVE)
            {
                t += 4;
            }
            set_stat(u, t, S_ANIMATION_TIMER);
        }
        if ((id == U_DRAGON) && (o == *(byte*)LOCAL_PLAYER))
        {
            if (a == ANIM_MOVE)
            {
                t = 1;
            }
            if (a == ANIM_ATTACK)
            {
                t = 1;
                set_stat(u, 1, S_ATTACK_COUNTER);
            }
            set_stat(u, t, S_ANIMATION_TIMER);
        }
        if (id == U_PALADIN)
        {
            if (a == ANIM_MOVE)
            {
                t += 4;
            }
            if (a == ANIM_ATTACK)
            {
                if ((*(byte*)(UNIT_RANGE_TABLE + U_PALADIN)) != 1)
                    t += 9;
                else
                {
                    byte ord = *((byte*)((uintptr_t)u + S_ORDER));
                    if (ord == ORDER_SPELL_RUNES)
                        t += 10;
                    else
                    {
                        t += 5;
                    }
                }
                set_stat(u, t, S_ATTACK_COUNTER);
            }
            set_stat(u, t, S_ANIMATION_TIMER);
        }
        if (id == U_CATAPULT)
        {
            if (*(byte*)LEVEL_OBJ >= LVL_HUMAN12)
            {
                if (a == ANIM_MOVE)
                {
                    t += 1;
                }
                if (a == ANIM_ATTACK)
                {
                    if (t <= 30)
                        t = 1;
                    if (t == 100)
                        t = 140;
                }
                set_stat(u, t, S_ANIMATION_TIMER);
            }
        }
    }
}

PROC g_proc_004522B9;
int seq_run(int* u)
{
    byte t = *((byte*)((uintptr_t)u + S_ANIMATION_TIMER));
    bool f = true;
    int original = 0;
    byte id = *((byte*)((uintptr_t)u + S_ID));
    if ((id == U_PEON) || (id == U_PEASANT))
    {
        byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
        byte sn = *((byte*)((uintptr_t)u + S_SEQ_SOUND));
        if (pf & 1)
        {
            if ((sn >= 34) && (sn <= 37))
            {
                int oid = sn - 1;
                def_name_seq = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * oid);
                def_sound_seq = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)k_names[sn - 34]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)k_sounds[sn - 34]);
                original = ((int (*)(int*))g_proc_004522B9)(u);//original
                k_sounds[sn - 34] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)def_sound_seq);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)def_name_seq);//restore default
                f = false;
            }
        }
    }
    else if (id == U_DANATH)
    {
        if ((*(byte*)(UNIT_RANGE_TABLE + U_DANATH)) != 1)
        {
            int r = ((int (*)())F_NET_RANDOM)() % 7;
            int oid = *((byte*)((uintptr_t)u + S_SEQ_SOUND)) - 1;
            def_name_seq = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * oid);
            def_sound_seq = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);//save default
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)rs_names[r]);
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)rs_sounds[r]);
            original = ((int (*)(int*))g_proc_004522B9)(u);//original
            rs_sounds[r] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)def_sound_seq);
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)def_name_seq);//restore default
            f = false;
        }
    }
    else if (id == U_GRUNT)
    {
        byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
        if (((pf & PEON_HARVEST_LUMBER) != 0) && ((pf & PEON_HARVEST_GOLD) == 0))
            set_stat(u, 42, S_SEQ_SOUND);
    }
    else if (id == U_OGRE)
    {
        set_stat(u, 55, S_SEQ_SOUND);
    }
    else if (id == U_KARGATH)
    {
        set_stat(u, 70, S_SEQ_SOUND);
    }
    else if (id == U_UTER)
    {
        set_stat(u, 79, S_SEQ_SOUND);
    }
    else if (id == U_CATAPULT)
    {
        int r = ((int (*)())F_NET_RANDOM)() % 2;
        int oid = *((byte*)((uintptr_t)u + S_SEQ_SOUND)) - 1;
        def_name_seq = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * oid);
        def_sound_seq = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);//save default
        patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)c_names[r]);
        patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)c_sounds[r]);
        original = ((int (*)(int*))g_proc_004522B9)(u);//original
        c_sounds[r] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * oid);
        patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * oid), (DWORD)def_sound_seq);
        patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * oid), (DWORD)def_name_seq);//restore default
        f = false;
    }
    else if (id == U_PALADIN)
    {
        if ((*(byte*)(UNIT_RANGE_TABLE + U_PALADIN)) == 1)
            set_stat(u, 55, S_SEQ_SOUND);
        else
            set_stat(u, 33, S_SEQ_SOUND);

        byte a = *((byte*)((uintptr_t)u + S_ANIMATION));
        byte f = *((byte*)((uintptr_t)u + S_FRAME));
        if ((a == ANIM_ATTACK) && (f == 0))
        {
            byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
            if ((pf && PEON_LOADED) == 0)
                pf |= PEON_LOADED;
            else
                pf &= ~PEON_LOADED;
            set_stat(u, pf, S_PEON_FLAGS);
        }
    }
    if (f)
        original = ((int (*)(int*))g_proc_004522B9)(u);//original
    seq_change(u, t);
    return original;
}

PROC g_proc_00409F3B, //action
g_proc_0040AF70, g_proc_0040AF99, //demolish
g_proc_0041038E, g_proc_00410762, //bullet
g_proc_004428AD;//spell
char fdmg = 0;//final damage
void damage(int* atk, int* trg, char dmg)
{
    fdmg = dmg;
    if ((trg != NULL) && (atk != NULL))
    {
        if (!check_unit_dead(trg))
        {
            byte aid = *((byte*)((uintptr_t)atk + S_ID));//attacker id
            byte tid = *((byte*)((uintptr_t)trg + S_ID));//target id
            byte dmg2 = dmg;//can deal damage by default
            int i = 0;
            while (i < 255)
            {
                if ((tid == ut[i]) && (aid != ua[i]))
                {
                    dmg2 = 0;//canot deal damage
                }
                if ((tid == ut[i]) && (aid == ua[i]))//check if only some certain units can attack that unit
                {
                    dmg2 = dmg;//can deal damage
                    i = 255;
                }
                i++;
                if (ua[i] == 255)//pairs must go in a row
                {
                    i = 255;
                }
            }
            if (*((WORD*)((uintptr_t)trg + S_SHIELD)) != 0)dmg2 = 0;
            fdmg = dmg2;
            WORD hp = *((WORD*)((uintptr_t)trg + S_HP));//unit hp
            if ((tid < U_FARM) && (fdmg >= hp))
            {
                if ((aid != U_HTRANSPORT) || (aid != U_OTRANSPORT))
                {
                    byte k = *((byte*)((uintptr_t)atk + S_KILLS));
                    if (k < 255)k += 1;
                    set_stat(atk, (int)k, S_KILLS);
                }
            }
        }
    }
}

void damage1(int* atk, int* trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int*, int*, char))g_proc_00409F3B)(atk, trg, fdmg);
}

void damage2(int* atk, int* trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int*, int*, char))g_proc_0041038E)(atk, trg, fdmg);
}

void damage3(int* atk, int* trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int*, int*, char))g_proc_0040AF70)(atk, trg, fdmg);
}

void damage4(int* atk, int* trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int*, int*, char))g_proc_0040AF99)(atk, trg, fdmg);
}

void damage5(int* atk, int* trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int*, int*, char))g_proc_00410762)(atk, trg, fdmg);
}

void damage6(int* atk, int* trg, char dmg)
{
    damage(atk, trg, dmg);
    ((void (*)(int*, int*, char))g_proc_004428AD)(atk, trg, fdmg);
}

PROC g_proc_0040DF71;
int* bld_unit_create(int a1,int a2,int a3,byte a4,int* a5)
{
    int *b = (int*)*(int*)UNIT_RUN_UNIT_POINTER;
    int* u = ((int* (*)(int, int, int, byte, int*))g_proc_0040DF71)(a1, a2, a3, a4, a5);
    if (b)
    {
        if (u)
        {
            byte own = *((byte*)((uintptr_t)b + S_OWNER));
            byte uid = *((byte*)((uintptr_t)u + S_ID));
            if (own == *(byte*)LOCAL_PLAYER)
            {
                if (uid == U_GRUNT)
                {
                    if (*(byte*)(GB_MARKS + own) == 1)
                    {
                        byte xx = *((byte*)((uintptr_t)b + S_X)) + 1;
                        byte yy = *((byte*)((uintptr_t)b + S_Y)) + 1;
                        unit_create(xx, yy, U_GRUNT, own, 1);
                    }
                }
            }
            if (ai_fixed)
            {
                byte o = *((byte*)((uintptr_t)u + S_OWNER));
                byte m = *((byte*)((uintptr_t)u + S_MANA));
                if ((*(byte*)(CONTROLER_TYPE + o) == C_COMP))
                {
                    if (m = 0x55)//85 default starting mana
                    {
                        char buf[] = "\xA0";//160
                        PATCH_SET((char*)u + S_MANA, buf);
                    }
                }
            }
        }
    }
    return u;
}

PROC g_proc_00451728;
void unit_kill_deselect(int* u)
{
    int* ud = u;
    ((void (*)(int*))g_proc_00451728)(u);//original
	((void (*)())F_STATUS_REDRAW)();//status redraw
    byte l = *(byte*)LOCAL_PLAYER;
    byte id = *((byte*)((uintptr_t)u + S_ID));
    byte o = *((byte*)((uintptr_t)u + S_OWNER));
    if ((id == U_PIGFARM) && (o == l))
    {
        if (check_unit_complete(u))
        {
            WORD fd = *(WORD*)(FOOD_LIMIT + 2 * l);
            if (fd > 0)fd--;
            *(WORD*)(FOOD_LIMIT + 2 * l) = fd;
        }
    }
}

PROC g_proc_00452939;
void grow_struct_count_tables(int* u)
{
    byte l = *(byte*)LOCAL_PLAYER;
    byte id = *((byte*)((uintptr_t)u + S_ID));
    byte o = *((byte*)((uintptr_t)u + S_OWNER));
    if ((id == U_PIGFARM) && (o == l))
    {
        WORD fd = *(WORD*)(FOOD_LIMIT + 2 * l);
        if (fd < 200)fd++;
        *(WORD*)(FOOD_LIMIT + 2 * l) = fd;
    }
    ((void (*)(int*))g_proc_00452939)(u);//original
}

void allow_table(byte p, int t, byte n, byte a)
{
    if (t == 0)t = ALLOWED_UNITS + (4 * p) + (n / 8);
    else if (t == 1)t = ALLOWED_UPGRADES + (4 * p) + (n / 8);
    else if (t == 2)t = ALLOWED_SPELLS + (4 * p) + (n / 8);
    else t = SPELLS_LEARNED + (4 * p) + (n / 8);
    byte b = *(byte*)t;
    if (a == 1)b |= (1 << (n % 8));
    else b &= (~(1 << (n % 8)));
    char buf[] = "\xff";
    buf[0] = b;
    PATCH_SET((char*)t, buf);
}

void draw_stats_fix(bool b)
{
    if (b)
    {
        char buf[] = "\xa0\x5b";
        PATCH_SET((char*)DEMON_STATS_DRAW, buf);//demon
        PATCH_SET((char*)CRITTER_STATS_DRAW, buf);//critter
    }
    else
    {
        char buf[] = "\xf0\x57";
        PATCH_SET((char*)DEMON_STATS_DRAW, buf);//demon
        PATCH_SET((char*)CRITTER_STATS_DRAW, buf);//critter
    }
}

void call_default_kill()//default kill all victory
{
    byte l = *(byte*)LOCAL_PLAYER;
    if (!slot_alive(l))lose(true);
    else
    {
        if (!check_opponents(l))win(true);
    }
}

//---------------rocks
int* peon_rclick = NULL;

PROC g_proc_0043B888;
int rclick_get_unit_first()
{
    int original = ((int (*)())g_proc_0043B888)();//original
    peon_rclick = (int*)original;
    return original;
}

PROC g_proc_0043B94B;
int rclick_get_unit_next()
{
    int original = ((int (*)())g_proc_0043B94B)();//original
    peon_rclick = (int*)original;
    return original;
}

PROC g_proc_0043B779;//rclick
int rclick_tile_is_chopping_tree(int a)
{
    int original = ((int (*)(int))g_proc_0043B779)(a);//original
    if (peon_rclick != NULL)
    {
        byte pf = *((byte*)((uintptr_t)peon_rclick + S_PEON_FLAGS));
        if (original == 0)
        {
            char buf[] = "\xFF";
            PATCH_SET((char*)ROCKS_TILE_IS_CHOPPING_TREE, buf);
            original = ((int (*)(int))g_proc_0043B779)(a);//original
            buf[0] = '\xFC';
            PATCH_SET((char*)ROCKS_TILE_IS_CHOPPING_TREE, buf);
            if (original == 1)
                pf |= 1;
        }
        else
            pf &= ~1;
        set_stat(peon_rclick, pf, S_PEON_FLAGS);
    }
    return original;
}

PROC g_proc_0043B768;
int rclick_tile_is_tree(int a)
{
    int original = ((int (*)(int))g_proc_0043B768)(a);//original
    if (peon_rclick != NULL)
    {
        byte pf = *((byte*)((uintptr_t)peon_rclick + S_PEON_FLAGS));
        if (original == 0)
        {
            char buf[] = "\xFD";
            PATCH_SET((char*)ROCKS_TILE_IS_TREE, buf);
            original = ((int (*)(int))g_proc_0043B768)(a);//original
            buf[0] = '\xFE';
            PATCH_SET((char*)ROCKS_TILE_IS_TREE, buf);
            if (original == 1)
                pf |= 1;
        }
        else
            pf &= ~1;
        set_stat(peon_rclick, pf, S_PEON_FLAGS);
    }
    return original;
}

PROC g_proc_0042411E;//dispatch
int dispatch_tile_is_chopping_tree(int a)
{
    int original = ((int (*)(int))g_proc_0042411E)(a);//original
    if (original == 0)
    {
        char buf[] = "\xFF";
        PATCH_SET((char*)ROCKS_TILE_IS_CHOPPING_TREE, buf);
        original = ((int (*)(int))g_proc_0042411E)(a);//original
        buf[0] = '\xFC';
        PATCH_SET((char*)ROCKS_TILE_IS_CHOPPING_TREE, buf);
    }
    return original;
}

PROC g_proc_0042412B;//harvest dispatch
PROC g_proc_00451554;//unit kill
PROC g_proc_00453001;//unit check orders
PROC g_proc_0045307D;//unit set curr action
void dispatch_tile_cancel_tree_harvest(int* peon)
{
    byte id = *((byte*)((uintptr_t)peon + S_ID));
    if ((id == U_PEASANT) || (id == U_PEON))
    {
        byte pf = *((byte*)((uintptr_t)peon + S_PEON_FLAGS));
        if (pf & PEON_CHOPPING)
        {
            ((void (*)(int*))g_proc_0042412B)(peon);//original
            pf |= PEON_CHOPPING;// | PEON_SAVED_LOCATION;
            set_stat(peon, pf, S_PEON_FLAGS);
            char buf[] = "\xFF";
            PATCH_SET((char*)ROCKS_TILE_CANCEL, buf);
            buf[0] = '\xFD';
            PATCH_SET((char*)ROCKS_TILE_CANCEL2, buf);
            ((void (*)(int*))g_proc_0042412B)(peon);//original
            buf[0] = '\xFC';
            PATCH_SET((char*)ROCKS_TILE_CANCEL, buf);
            buf[0] = '\xFE';
            PATCH_SET((char*)ROCKS_TILE_CANCEL2, buf);
            //pf &= ~(PEON_CHOPPING | PEON_SAVED_LOCATION);
            //set_stat(peon, pf, S_PEON_FLAGS);
        }
        else
            ((void (*)(int*))g_proc_0042412B)(peon);//original
    }
    else
        ((void (*)(int*))g_proc_0042412B)(peon);//original
}

PROC g_proc_004241AD;//unit set curr action
void dispatch_tile_finish_tree_harvest(int* peon)
{
    ((void (*)(int*))g_proc_004241AD)(peon);//original
    byte x = *((byte*)((uintptr_t)peon + S_ORDER_X));
    byte y = *((byte*)((uintptr_t)peon + S_ORDER_Y));
    byte mxs = *(byte*)MAP_SIZE;//map size
    char* reg = (char*)*(int*)(MAP_REG_POINTER);//map reg
    char preg = *(char*)(reg + 2 * x + 2 * y * mxs);
    if (preg == '\xFF')
    {
        char buf[] = "\xFD";
        PATCH_SET((char*)(reg + 2 * x + 2 * y * mxs), buf);
        tile_remove_rocks(x, y);
        byte pf = *((byte*)((uintptr_t)peon + S_PEON_FLAGS));
        pf &= ~PEON_HARVEST_LUMBER;
        pf |= PEON_HARVEST_GOLD;
        set_stat(peon, pf, S_PEON_FLAGS);
    }
}

PROC g_proc_00424215;
int dispatch_tile_is_tree(int a)
{
    //peon_dispatch = (int*)(a - S_ORDER_X);
    int* peon_dispatch = (int*)*(int*)ROCKS_PEON_DISPATCH;
    int original = 0;
    if (peon_dispatch != NULL)
    {
        byte pf = *((byte*)((uintptr_t)peon_dispatch + S_PEON_FLAGS));
        original = ((int (*)(int))g_proc_00424215)(a);//original
        if (original == 0)
        {
            char buf[] = "\xFD";
            PATCH_SET((char*)ROCKS_TILE_IS_TREE, buf);
            original = ((int (*)(int))g_proc_00424215)(a);//original
            buf[0] = '\xFE';
            PATCH_SET((char*)ROCKS_TILE_IS_TREE, buf);
            if (original == 1)
            {
                buf[0] = '\xFF';
                PATCH_SET((char*)ROCKS_TILE_IS_TREE2, buf);
                pf |= 1;
            }
        }
        else
            pf &= ~1;
        set_stat(peon_dispatch, pf, S_PEON_FLAGS);
    }
    return original;
}

PROC g_proc_0042423D;
void dispatch_tile_start_tree_harvest(int* peon)
{
    ((void (*)(int*))g_proc_0042423D)(peon);//original
    byte pf = *((byte*)((uintptr_t)peon + S_PEON_FLAGS));
    WORD b = *((WORD*)((uintptr_t)peon + S_BLOOD));
    byte chp = 0;
    if (b != 0)chp += 10;
    if (*(char*)ROCKS_TILE_START == '\xFF')
    {
        char buf[] = "\xFC";
        PATCH_SET((char*)ROCKS_TILE_START, buf);
        pf |= 1;//unused flag?
        if (get_upgrade(SHIP_ARMOR, *((byte*)((uintptr_t)peon + S_OWNER))) != 0)
            chp += 20;
    }
    else
    {
        pf &= ~1;
        if (*(byte*)(GB_SCOUTING + *((byte*)((uintptr_t)peon + S_OWNER))) == 1)
            chp += 20;
    }
    set_stat(peon, chp, S_PEON_TREE_CHOPS);
    set_stat(peon, pf, S_PEON_FLAGS);
}

PROC g_proc_0042413C;//dispatch1
PROC g_proc_00424262;//dispatch2
PROC g_proc_004377EC;//dispatch3
int dispatch_tile_find_tree(int* a)
{
    int* peon_dispatch = (int*)*(int*)ROCKS_PEON_DISPATCH;
    int original = 0;
    if (peon_dispatch != NULL)
    {
        byte pf = *((byte*)((uintptr_t)peon_dispatch + S_PEON_FLAGS));
        if ((pf & 1) == 0)
            original = ((int (*)(int*))g_proc_0042413C)(a);//original
        else
        {
            char buf[] = "\xFD";
            PATCH_SET((char*)ROCKS_TILE_FIND_TREE, buf);
            original = ((int (*)(int*))g_proc_0042413C)(a);//original
            buf[0] = '\xFE';
            PATCH_SET((char*)ROCKS_TILE_FIND_TREE, buf);
        }
    }
    return original;
}

void finish_not_function()
{
    *(byte*)LOCAL_PLAYER = 0;
    *(byte*)PACKET_PLAYER = 1;
    *(byte*)PLAYER_HOST = 2;
    *(byte*)ME_HOST = 3;
}
//---------------rocks

//-------------files
const char FILES_PATH[] = ".\\Stonia\\";

void* grp_port;
void* grp_port2;

void* grp_dwarf_dead;
void* grp_dwarf_peon;
void* grp_dwarf_peong;
void* grp_dwarf_peonl;
void* grp_dwarf_cart;
void* grp_dwarf_footmanS;
void* grp_dwarf_footmanM;
void* grp_dwarf_footmanA;
void* grp_dwarf_armored;
void* grp_dwarf_alchemist;
void* grp_dwarf_vert;
void* grp_dwarf_hero;
void* grp_dwarf_herop;
void* grp_canon;

void* grp_water;
void* grp_stone;
void* grp_slime;

void* grp_dwarf_farm;
void* grp_dwarf_barak;
void* grp_dwarf_workshop;
void* grp_dwarf_lumber;
void* grp_dwarf_smelt;
void* grp_dwarf_sklad;
void* grp_dwarf_skladup;
void* grp_dwarf_baseG;
void* grp_dwarf_baseD;
void* grp_dwarf_baseGup;
void* grp_dwarf_baseDup;
void* grp_dwarf_obsG;
void* grp_dwarf_obsD;
void* grp_dwarf_academ;

void* grp_gold_stone;

void* grp_runeword;
void* grp_runecircle;

void* grp_shot;
void* grp_throw;
void* grp_golem_throw;

void* grp_boss_left;
void* grp_boss_right;
void* grp_boss_jump;
void* grp_boss_throw;

void* tbl_names;
void* tbl_obj;
void* tbl_act;
void* tbl_net;
void* tbl_rank;
void* tbl_end;
void* tbl_credits1;
void* tbl_credits2;
void* tbl_credits3;
void* tbl_credits4;
void* tbl_credits5;
void* tbl_credits6;
void* tbl_brif1;
void* tbl_brif2;
void* tbl_brif3;
void* tbl_brif4;
void* tbl_brif5;

void* tbl_dialog;

void* bin_AI;
DWORD bin_AI_size;
void* bin_script;
DWORD bin_script_size;

void* bin_brifing;
DWORD bin_brifing_size;
void* bin_brifing_copy;
void* bin_menu;
DWORD bin_menu_size;
void* bin_menu_copy;
void* bin_sngl;
DWORD bin_sngl_size;
void* bin_sngl_copy;
void* bin_newcmp;
DWORD bin_newcmp_size;
void* bin_newcmp_copy;
void* bin_credits;
DWORD bin_credits_size;
void* bin_credits_copy;

void* tile_cave_cv4;
DWORD tile_cave_cv4_size;
void* tile_cave_vr4;
DWORD tile_cave_vr4_size;
void* tile_cave_vx4;
DWORD tile_cave_vx4_size;
void* tile_cave_palette;

void* tile_wasteland_cv4;
DWORD tile_wasteland_cv4_size;
void* tile_wasteland_vr4;
DWORD tile_wasteland_vr4_size;
void* tile_wasteland_vx4;
DWORD tile_wasteland_vx4_size;
void* tile_wasteland_palette;

void* pcx_splash;
void* pcx_splash_pal;
void* pcx_menu;
void* pcx_menu_pal;
void* pcx_brif1;
void* pcx_brif1_pal;
void* pcx_brif2;
void* pcx_brif2_pal;
void* pcx_brif3;
void* pcx_brif3_pal;
void* pcx_brif4;
void* pcx_brif4_pal;
void* pcx_brif5;
void* pcx_brif5_pal;
void* pcx_act1;
void* pcx_act1_pal;
void* pcx_act2;
void* pcx_act2_pal;
void* pcx_act3;
void* pcx_act3_pal;
void* pcx_act4;
void* pcx_act4_pal;
void* pcx_act5;
void* pcx_act5_pal;
void* pcx_end;
void* pcx_end_pal;
void* pcx_credit1;
void* pcx_credit1_pal;
void* pcx_credit2;
void* pcx_credit2_pal;
void* pcx_credit3;
void* pcx_credit3_pal;
void* pcx_win;
void* pcx_win_pal;
void* pcx_loss;
void* pcx_loss_pal;

void* pud_map1;
DWORD pud_map1_size;
void* pud_map2;
DWORD pud_map2_size;
void* pud_map3;
DWORD pud_map3_size;
void* pud_map4;
DWORD pud_map4_size;
void* pud_map5;
DWORD pud_map5_size;
void* pud_map_atr;
DWORD pud_map_atr_size;

PROC g_proc_004542FB;
int grp_draw_cross(int a, int* u, void* grp, int frame)
{
    void* new_grp = NULL;
    //-------------------------------------------------
    //if level = orc1
    //if race = human
    //etc
    //new_grp = grp_runecircle;

    if (new_grp)
        return ((int (*)(int, int*, void*, int))g_proc_004542FB)(a, u, new_grp, frame);
    else
        return ((int (*)(int, int*, void*, int))g_proc_004542FB)(a, u, grp, frame);//original
}

PROC g_proc_00454DB4;
int grp_draw_bullet(int a, int* u, void* grp, int frame)
{
    void* new_grp = NULL;
    //-------------------------------------------------
    //44 - target
    byte id = *((byte*)((uintptr_t)u + 52));//52 - id
    int* c = (int*)*((int*)((uintptr_t)u + 48));//48 - creator
    if (id == B_LIGHTNING)
    {
        byte cid = *((byte*)((uintptr_t)c + S_ID));
        if (cid == U_MAGE)
            new_grp = grp_shot;
    }
    else if (id == B_BIG_CANNON)
    {
        byte cid = *((byte*)((uintptr_t)c + S_ID));
        if (cid == U_OCANONTOWER)
            new_grp = grp_throw;
        else if (cid == U_PALADIN)
            new_grp = grp_golem_throw;
    }

    if (new_grp)
        return ((int (*)(int, int*, void*, int))g_proc_00454DB4)(a, u, new_grp, frame);
    else
        return ((int (*)(int, int*, void*, int))g_proc_00454DB4)(a, u, grp, frame);//original
}

PROC g_proc_00454BCA;
int grp_draw_unit(int a, int* u, void* grp, int frame)
{
    void* new_grp = NULL;
    //-------------------------------------------------
    byte id = *((byte*)((uintptr_t)u + S_ID));
    if (id == U_PEON)
    {
        new_grp = grp_dwarf_peon;
        byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
        if ((pf & PEON_LOADED) != 0)
        {
            if ((pf & PEON_HARVEST_GOLD) != 0)
                new_grp = grp_dwarf_peong;
            if ((pf & PEON_HARVEST_LUMBER) != 0)
                new_grp = grp_dwarf_peonl;
        }
    }
    else if (id == U_GRUNT)
    {
        byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
        if ((pf & PEON_HARVEST_GOLD) != 0)
            new_grp = grp_dwarf_footmanS;
        else if ((pf & PEON_HARVEST_LUMBER) != 0)
            new_grp = grp_dwarf_footmanM;
        else//if (((pf & PEON_HARVEST_GOLD) != 0) && ((pf & PEON_HARVEST_LUMBER) != 0))
            new_grp = grp_dwarf_footmanA;
    }
    else if (id == U_DANATH)
    {
        if ((*(byte*)(UNIT_RANGE_TABLE + U_DANATH)) == 1)
            new_grp = grp_dwarf_hero;
        else
            new_grp = grp_dwarf_herop;
    }
    else if (id == U_PALADIN)
    {
        if ((*(byte*)(UNIT_RANGE_TABLE + U_PALADIN)) != 1)
            new_grp = grp_boss_throw;
        else
        {
            byte ord = *((byte*)((uintptr_t)u + S_ORDER));
            if (ord==ORDER_SPELL_RUNES)
                new_grp = grp_boss_jump;
            else
            {
                byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
                if ((pf & PEON_LOADED) != 0)
                    new_grp = grp_boss_left;
                else
                    new_grp = grp_boss_right;
            }
        }
    }
    else if (id == U_BALLISTA)
    {
        new_grp = grp_dwarf_cart;
    }
    else if (id == U_CATAPULT)
    {
        byte lvl = *(byte*)LEVEL_OBJ;
        if (lvl < LVL_HUMAN12)
        {
            new_grp = grp_runeword;
            frame = *((byte*)((uintptr_t)u + S_LAST_HARVEST_X + 1)) * 5;
        }
        else
        {
            new_grp = grp_canon;
        }
    }
    else if (id == U_GROM)
    {
        new_grp = grp_dwarf_footmanS;
    }
    else if (id == U_LOTHAR)
    {
        new_grp = grp_dwarf_armored;
    }
    else if (id == U_DRAGON)
    {
        new_grp = grp_dwarf_vert;
    }
    else if (id == U_MAGE)
    {
        new_grp = grp_dwarf_alchemist;
    }
    else if (id == U_HADGAR)
    {
        new_grp = grp_dwarf_alchemist;
    }
    else if (id == U_OGRE)
    {
        new_grp = grp_stone;
    }
    else if (id == U_KARGATH)
    {
        new_grp = grp_water;
    }
    else if (id == U_UTER)
    {
        new_grp = grp_slime;
    }

    if (id == U_DEAD)//dead
    {
        byte m = *((byte*)((uintptr_t)u + S_MOVEMENT_TYPE));
        if (m == 100)
            new_grp = grp_dwarf_dead;
        if (m == 101)
        {
            new_grp = grp_slime;
            frame = 66;
        }
    }

    if (new_grp)
        return ((int (*)(int, int*, void*, int))g_proc_00454BCA)(a, u, new_grp, frame);
    else
        return ((int (*)(int, int*, void*, int))g_proc_00454BCA)(a, u, grp, frame);//original
}

PROC g_proc_00455599;
int grp_draw_building(int a, int* u, void* grp, int frame)
{
    void* new_grp = NULL;
    //-------------------------------------------------
    byte id = *((byte*)((uintptr_t)u + S_ID));
    byte mp = *((byte*)((uintptr_t)u + S_MANA));
    WORD hp = *((WORD*)((uintptr_t)u + S_HP));
    WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * id);//max hp
    if (id == U_CIRCLE)
    {
        byte col = *((byte*)((uintptr_t)u + S_COLOR));
        if (!((col == P_NEUTRAL) || (col == P_RED)))
        {
            new_grp = grp_runecircle;
            if (col == P_BLUE)frame = 1;
            if (col == P_GREEN)frame = 5;
            if (col == P_VIOLET)frame = 6;
            if (col == P_ORANGE)frame = 2;
            if (col == P_BLACK)frame = 4;
            if (col == P_WHITE)frame = 3;
            if (col == P_YELLOW)frame = 0;
        }
    }
    if ((mp > 127) || check_unit_complete(u) || (hp >= (mhp / 2)))
    {
        if (id == U_PIGFARM)new_grp = grp_dwarf_farm;
        else if (id == U_OBARRACK)new_grp = grp_dwarf_barak;
        else if (id == U_OLUMBER)new_grp = grp_dwarf_lumber;
        else if (id == U_ALCHEMIST)new_grp = grp_dwarf_academ;
        else if (id == U_OSMITH)new_grp = grp_dwarf_smelt;
        else if (id == U_OGREMOUND)new_grp = grp_dwarf_workshop;
    }
    if (id == U_OCANONTOWER)
    {
        new_grp = grp_gold_stone;
    }
    if (id == U_GREAT_HALL)
    {
        if (check_unit_complete(u))
        {
            byte bord = *((byte*)((uintptr_t)u + S_BUILD_ORDER));
            byte ord = *((byte*)((uintptr_t)u + S_ORDER));
            if ((bord == 3) && (ord == ORDER_BLDG_BUILD))
            {
                byte m = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
                if (m != 0)
                    new_grp = grp_dwarf_baseG;
                else
                    new_grp = grp_dwarf_baseD;
            }
            else
            {
                new_grp = grp_dwarf_skladup;
            }
        }
        else
        {
            if ((mp > 127) || (hp >= (mhp / 2)))
                new_grp = grp_dwarf_sklad;
        }
    }
    if (id == U_STRONGHOLD)
    {
        byte m = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
        byte bord = *((byte*)((uintptr_t)u + S_BUILD_ORDER));
        byte ord = *((byte*)((uintptr_t)u + S_ORDER));
        if ((bord == 3) && (ord == ORDER_BLDG_BUILD))
        {
            if (m != 0)
                new_grp = grp_dwarf_obsG;
            else
                new_grp = grp_dwarf_obsD;
        }
        else
        {
            if (m != 0)
                new_grp = grp_dwarf_baseGup;
            else
                new_grp = grp_dwarf_baseDup;
        }
    }
    if (id == U_FORTRESS)
    {
        byte m = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
        if (m != 0)
            new_grp = grp_dwarf_obsG;
        else
            new_grp = grp_dwarf_obsD;
    }
    //-------------------------------------------------
    if (new_grp)
        return ((int (*)(int, int*, void*, int))g_proc_00455599)(a, u, new_grp, frame);
    else
        return ((int (*)(int, int*, void*, int))g_proc_00455599)(a, u, grp, frame);//original
}

PROC g_proc_0043AE54;
void grp_draw_building_placebox(void* grp, int frame, int a, int b)
{
    void* new_grp = NULL;
    int* peon = (int*)*(int*)LOCAL_UNITS_SELECTED;
    byte id = (*(int*)b) % 256;
    //-------------------------------------------------
    if (id == U_PIGFARM)new_grp = grp_dwarf_farm;
    else if (id == U_OBARRACK)new_grp = grp_dwarf_barak;
    else if (id == U_OLUMBER)new_grp = grp_dwarf_lumber;
    else if (id == U_ALCHEMIST)new_grp = grp_dwarf_academ;
    else if (id == U_OSMITH)new_grp = grp_dwarf_smelt;
    else if (id == U_OGREMOUND)new_grp = grp_dwarf_workshop;
    else if (id == U_GREAT_HALL)new_grp = grp_dwarf_sklad;
    //-------------------------------------------------
    if (new_grp)
        ((void (*)(void*, int, int, int))g_proc_0043AE54)(new_grp, frame, a, b);
    else
        ((void (*)(void*, int, int, int))g_proc_0043AE54)(grp, frame, a, b);//original
}

int* portrait_unit;

PROC g_proc_0044538D;
void grp_portrait_init(int* a)
{
    ((void (*)(int*))g_proc_0044538D)(a);//original
    portrait_unit = (int*)*((int*)((uintptr_t)a + 0x26));
}

PROC g_proc_004453A7;//draw unit port
void grp_draw_portrait(void* grp, byte frame, int b, int c)
{
    void* new_grp = NULL;
    //-------------------------------------------------
    int* u = portrait_unit;
    if (u != NULL)
    {
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if (id == U_BALLISTA)
            new_grp = grp_port;
        else if (id == U_CATAPULT)
        {
            new_grp = grp_port;
            if (*(byte*)LEVEL_OBJ < LVL_HUMAN12)
            {
                frame = 81;
            }
        }
        else if (id == U_DRAGON)
        {
            new_grp = grp_port;
            frame = 31;
        }
        else if (id == U_PEON)
            new_grp = grp_port;
        else if (id == U_GRUNT)
            new_grp = grp_port;
        else if (id == U_LOTHAR)
            new_grp = grp_port;
        else if (id == U_MAGE)
            new_grp = grp_port;
        else if (id == U_DANATH)
            new_grp = grp_port;
        else if (id == U_HADGAR)
            new_grp = grp_port;
        else if (id == U_TERON)
            new_grp = grp_port;
        else if (id == U_CHOGAL)
            new_grp = grp_port;
        else if (id == U_OGRE)
        {
            new_grp = grp_port;
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                frame = 122;
            }
        }
        else if (id == U_KARGATH)
        {
            new_grp = grp_port;
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                frame = 130;
            }
        }
        else if (id == U_UTER)
            new_grp = grp_port;
        else if (id == U_GREAT_HALL)
            new_grp = grp_port;
        else if (id == U_FORTRESS)
            new_grp = grp_port;
        else if (id == U_FOOTMAN)
            new_grp = grp_port;
        else if (id == U_ARCHER)
            new_grp = grp_port;
        else if (id == U_RANGER)
        {
            new_grp = grp_port;
            frame = 153;
        }
        else if (id == U_OCANONTOWER)
            new_grp = grp_port;
        else if (id == U_PIGFARM)
            new_grp = grp_port;
        else if (id == U_OBARRACK)
            new_grp = grp_port;
        else if (id == U_OLUMBER)
            new_grp = grp_port;
        else if (id == U_OSMITH)
            new_grp = grp_port;
        else if (id == U_OGREMOUND)
            new_grp = grp_port;
        else if (id == U_ALCHEMIST)
            new_grp = grp_port;
        else if (id == U_TROLL)
        {
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                new_grp = grp_port;
                frame = 123;
            }
        }
        else if (id == U_PALADIN)
            new_grp = grp_port;

        if (id == U_STRONGHOLD)
        {
            new_grp = grp_port;
            byte m = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
            if (m == 0)
                frame = 136;
        }
    }
    if ((*(byte*)MAP_ERA == 3) && (new_grp == grp_port))
        new_grp = grp_port2;
    if (new_grp)
        return ((void (*)(void*, byte, int, int))g_proc_004453A7)(new_grp, frame, b, c);
    else
        return ((void (*)(void*, byte, int, int))g_proc_004453A7)(grp, frame, b, c);//original
}

PROC g_proc_004452B0;//draw buttons
void grp_draw_portrait_buttons(void* grp, byte frame, int b, int c)
{
    void* new_grp = NULL;
    //-------------------------------------------------
    int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
    if (u != NULL)
    {
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if (id == U_GREAT_HALL)
        {
            if (check_unit_complete(u))
            {
                byte bord = *((byte*)((uintptr_t)u + S_BUILD_ORDER));
                byte ord = *((byte*)((uintptr_t)u + S_ORDER));
                if ((bord == 3) && (ord == ORDER_BLDG_BUILD))
                {
                    if (frame == 67)
                    {
                        byte m = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
                        if (m == 0)
                            frame = 136;
                    }
                }
            }
        }
        if (id == U_OLUMBER)
        {
            if (check_unit_complete(u))
            {
                byte bord = *((byte*)((uintptr_t)u + S_BUILD_ORDER));
                byte ord = *((byte*)((uintptr_t)u + S_ORDER));
                if ((bord == 2) && (ord == ORDER_BLDG_BUILD))
                {
                    if (frame == 120)frame = 121;
                    else if (frame == 121)frame = 135;
                }
            }
        }
    }
    //-------------------------------------------------
    new_grp = grp_port;

    if ((*(byte*)MAP_ERA == 3) && (new_grp == grp_port))
        new_grp = grp_port2;
    if (new_grp)
        return ((void (*)(void*, byte, int, int))g_proc_004452B0)(new_grp, frame, b, c);
    else
        return ((void (*)(void*, byte, int, int))g_proc_004452B0)(grp, frame, b, c);//original
}

PROC g_proc_0044A65C;
void status_get_tbl(void* tbl, WORD str_id)
{
    int* u = (int*)*(int*)UNIT_STATUS_TBL;
    void* new_tbl = NULL;
    //-------------------------------------------------
    if (u != NULL)
    {
        byte id = *((byte*)((uintptr_t)u + S_ID));
        //byte o = *((byte*)((uintptr_t)u + S_OWNER));
        //if (o == PLAYER14)
        //    o = *((byte*)((uintptr_t)u + S_LAST_HARVEST_X));
        if (id == U_BALLISTA)
        {
            byte lvl = *(byte*)LEVEL_OBJ;
            if (lvl != LVL_HUMAN14)
                new_tbl = tbl_names;
        }
        else if (id == U_CATAPULT)
        {
            new_tbl = tbl_names;
            if (*(byte*)LEVEL_OBJ >= LVL_HUMAN12)
            {
                if (str_id == 7)
                    str_id = 36;
            }
        }
        else if (id == U_DRAGON)
            new_tbl = tbl_names;
        else if (id == U_PEON)
            new_tbl = tbl_names;
        else if (id == U_GRUNT)
            new_tbl = tbl_names;
        else if (id == U_LOTHAR)
            new_tbl = tbl_names;
        else if (id == U_MAGE)
            new_tbl = tbl_names;
        else if (id == U_DANATH)
            new_tbl = tbl_names;
        else if (id == U_DRAGON)
            new_tbl = tbl_names;
        else if (id == U_HADGAR)
            new_tbl = tbl_names;
        else if (id == U_TERON)
            new_tbl = tbl_names;
        else if (id == U_CHOGAL)
            new_tbl = tbl_names;
        else if (id == U_OGRE)
        {
            new_tbl = tbl_names;
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                if (str_id == 9)
                    str_id = 37;
            }
        }
        else if (id == U_KARGATH)
        {
            new_tbl = tbl_names;
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                if (str_id == 49)
                    str_id = 38;
            }
        }
        else if (id == U_UTER)
            new_tbl = tbl_names;
        else if (id == U_ARCHER)
            new_tbl = tbl_names;
        else if (id == U_RANGER)
            new_tbl = tbl_names;
        else if (id == U_TROLL)
        {
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                new_tbl = tbl_names;
                if (str_id == 11)
                    str_id = 39;
            }
        }
        else if (id == U_PALADIN)
            new_tbl = tbl_names;

        else if (id == U_OCANONTOWER)
        {
            new_tbl = tbl_names;
            str_id = 35;
        }
        else if (id == U_PIGFARM)
            new_tbl = tbl_names;
        else if (id == U_OBARRACK)
            new_tbl = tbl_names;
        else if (id == U_OGREMOUND)
            new_tbl = tbl_names;
        else if (id == U_OLUMBER)
            new_tbl = tbl_names;
        else if (id == U_OSMITH)
            new_tbl = tbl_names;
        else if (id == U_ALCHEMIST)
            new_tbl = tbl_names;
        else if (id == U_GREAT_HALL)
            new_tbl = tbl_names;
        else if (id == U_FORTRESS)
            new_tbl = tbl_names;
        else if (id == U_STRONGHOLD)
        {
            new_tbl = tbl_names;
            if (str_id == 91)
            {
                byte m = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
                if (m == 0)
                    str_id = 96;
            }
        }
    }
    //add any conditions
    //-------------------------------------------------
    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0044A65C)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0044A65C)(tbl, str_id);//original

}

int* hover_unit;

PROC g_proc_0044AC83;
void unit_hover_get_id(int a, int* b)
{
    if (b != NULL)
    {
        byte id = *((byte*)((uintptr_t)b + 0x20));
        hover_unit = (int*)*(int*)(LOCAL_UNIT_SELECTED_PANEL + 4 * id);
    }
    else
        hover_unit = NULL;
    ((void (*)(int, int*))g_proc_0044AC83)(a, b);//original
}

PROC g_proc_0044AE27;
void unit_hover_get_tbl(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------
    int* u = hover_unit;
    if (u != NULL)
    {
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if (id == U_BALLISTA)
        {
            byte lvl = *(byte*)LEVEL_OBJ;
            if (lvl != LVL_HUMAN14)
                new_tbl = tbl_names;
        }
        else if (id == U_CATAPULT)
        {
            new_tbl = tbl_names;
            if (*(byte*)LEVEL_OBJ >= LVL_HUMAN12)
                str_id = 36;
        }
        else if (id == U_PEON)
            new_tbl = tbl_names;
        else if (id == U_GRUNT)
            new_tbl = tbl_names;
        else if (id == U_LOTHAR)
            new_tbl = tbl_names;
        else if (id == U_MAGE)
            new_tbl = tbl_names;
        else if (id == U_DANATH)
            new_tbl = tbl_names;
        else if (id == U_DRAGON)
            new_tbl = tbl_names;
        else if (id == U_HADGAR)
            new_tbl = tbl_names;
        else if (id == U_TERON)
            new_tbl = tbl_names;
        else if (id == U_CHOGAL)
            new_tbl = tbl_names;
        else if (id == U_OGRE)
        {
            new_tbl = tbl_names;
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                if (str_id == 9)
                    str_id = 37;
            }  
        }
        else if (id == U_KARGATH)
        {
            new_tbl = tbl_names;
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                if (str_id == 49)
                    str_id = 38;
            }
        }
        else if (id == U_UTER)
            new_tbl = tbl_names;
        else if (id == U_ARCHER)
            new_tbl = tbl_names;
        else if (id == U_RANGER)
            new_tbl = tbl_names;
        else if (id == U_TROLL)
        {
            if (*((byte*)((uintptr_t)u + S_COLOR)) == P_GREEN)
            {
                new_tbl = tbl_names;
                if (str_id == 11)
                    str_id = 39;
            }
        }
        else if (id == U_PALADIN)
            new_tbl = tbl_names;

        else if (id == U_OCANONTOWER)
        {
            new_tbl = tbl_names;
            str_id = 35;
        }
        else if (id == U_PIGFARM)
            new_tbl = tbl_names;
        else if (id == U_OBARRACK)
            new_tbl = tbl_names;
        else if (id == U_OGREMOUND)
            new_tbl = tbl_names;
        else if (id == U_OLUMBER)
            new_tbl = tbl_names;
        else if (id == U_OSMITH)
            new_tbl = tbl_names;
        else if (id == U_ALCHEMIST)
            new_tbl = tbl_names;
        else if (id == U_GREAT_HALL)
            new_tbl = tbl_names;
        else if (id == U_FORTRESS)
            new_tbl = tbl_names;
        else if (id == U_STRONGHOLD)
        {
            new_tbl = tbl_names;
            if (str_id == 91)
            {
                byte m = *((byte*)((uintptr_t)u + S_AI_AIFLAGS));
                if (m == 0)
                    str_id = 96;
            }
        }
    }
    //-------------------------------------------------
    //new_tbl = tbl_names;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0044AE27)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0044AE27)(tbl, str_id);//original

}

PROC g_proc_0044AE56;
void button_description_get_tbl(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------
    /*
    int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
    if (u != NULL)
    {

    }
    */
    //-------------------------------------------------
    new_tbl = tbl_names;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0044AE56)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0044AE56)(tbl, str_id);//original

}

PROC g_proc_0044B23D;
void button_hotkey_get_tbl(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------
    /*
    int* u = (int*)*(int*)LOCAL_UNITS_SELECTED;
    if (u != NULL)
    {

    }
    */
    //-------------------------------------------------
    new_tbl = tbl_names;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0044B23D)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0044B23D)(tbl, str_id);//original

}

PROC g_proc_004354C8;
void objct_get_tbl_custom(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------
    
    //-------------------------------------------------
    new_tbl = tbl_obj;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_004354C8)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_004354C8)(tbl, str_id);//original
}

PROC g_proc_004354FA;
void objct_get_tbl_campanign(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------

    //-------------------------------------------------
    new_tbl = tbl_obj;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_004354FA)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_004354FA)(tbl, str_id);//original
}

PROC g_proc_004300A5;
void objct_get_tbl_briefing_task(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    byte lvl = *(byte*)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN1;
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN2;
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN4;
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN9;
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN12;
    }
    //-------------------------------------------------
    new_tbl = tbl_obj;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_004300A5)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_004300A5)(tbl, str_id);//original
}

PROC g_proc_004300CA;
void objct_get_tbl_briefing_title(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    byte lvl = *(byte*)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN1 + 59;
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN2 + 59;
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN4 + 59;
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN9 + 59;
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_tbl = tbl_obj;
        str_id = 1 + LVL_HUMAN12 + 59;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_004300CA)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_004300CA)(tbl, str_id);//original
}

PROC g_proc_004301CA;
void objct_get_tbl_briefing_text(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    byte lvl = *(byte*)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        new_tbl = tbl_brif1;
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_tbl = tbl_brif2;
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_tbl = tbl_brif3;
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_tbl = tbl_brif4;
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_tbl = tbl_brif5;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_004301CA)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_004301CA)(tbl, str_id);//original
}

char story1[] = "Stonia\\storyteller\\1.wav";
char story2[] = "Stonia\\storyteller\\2.wav";
char story3[] = "Stonia\\storyteller\\3.wav";
char story4[] = "Stonia\\storyteller\\4.wav";
char story5[] = "Stonia\\storyteller\\5.wav";
char story_end[] = "Stonia\\storyteller\\end.wav";

void set_speech(char* speech, char* adr)
{
    patch_setdword((DWORD*)(speech + 4), (DWORD)adr);
    patch_setdword((DWORD*)(speech + 12), 0);
}

DWORD remember_music = 101;
DWORD remember_sound = 101;

PROC g_proc_00430113;
int objct_get_briefing_speech(char* speech)
{
    remember_music = *(DWORD*)VOLUME_MUSIC;
    remember_sound = *(DWORD*)VOLUME_SOUND;
    if (remember_music != 0)
        *(DWORD*)VOLUME_MUSIC = 20;
    *(DWORD*)VOLUME_SOUND = 100;
    ((void (*)(DWORD))F_SET_VOLUME)(SET_VOLUME_PARAM);//set volume

    DWORD remember1 = *(DWORD*)(speech + 4);
    DWORD remember2 = *(DWORD*)(speech + 12);
    byte lvl = *(byte*)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        set_speech(speech, story1);
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        set_speech(speech, story2);
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        set_speech(speech, story3);
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        set_speech(speech, story4);
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        set_speech(speech, story5);
    }
    //-------------------------------------------------

    int original = ((int (*)(char*))g_proc_00430113)(speech);//original
    patch_setdword((DWORD*)(speech + 4), remember1);
    patch_setdword((DWORD*)(speech + 12), remember2);
    return original;
}

bool finale_dlg = false;

PROC g_proc_0041F0F5;
void finale_get_tbl(void* tbl, WORD str_id)
{
    finale_dlg = false;
    void* new_tbl = NULL;
    //-------------------------------------------------

    //-------------------------------------------------
    new_tbl = tbl_end;
    str_id = 1;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0041F0F5)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0041F0F5)(tbl, str_id);//original
}

PROC g_proc_0041F1E8;
void finale_credits_get_tbl(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------

    //-------------------------------------------------
    new_tbl = tbl_credits2;
    str_id = 1;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0041F1E8)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0041F1E8)(tbl, str_id);//original
}

PROC g_proc_0041F027;
int finale_get_speech(char* speech)
{
    remember_music = *(DWORD*)VOLUME_MUSIC;
    remember_sound = *(DWORD*)VOLUME_SOUND;
    if (remember_music != 0)
        *(DWORD*)VOLUME_MUSIC = 67;
    *(DWORD*)VOLUME_SOUND = 100;
    ((void (*)(DWORD))F_SET_VOLUME)(SET_VOLUME_PARAM);//set volume

    DWORD remember1 = *(DWORD*)(speech + 4);
    DWORD remember2 = *(DWORD*)(speech + 12);
    //-------------------------------------------------
    set_speech(speech, story_end);
    //-------------------------------------------------
    int original = ((int (*)(char*))g_proc_0041F027)(speech);//original
    patch_setdword((DWORD*)(speech + 4), remember1);
    patch_setdword((DWORD*)(speech + 12), remember2);
    return original;
}

int cred_num = 0;

PROC g_proc_00417E33;
void credits_small_get_tbl(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------

    //-------------------------------------------------
    if (cred_num == 1)new_tbl = tbl_credits2;
    if (cred_num == 2)new_tbl = tbl_credits4;
    if (cred_num == 3)new_tbl = tbl_credits6;
    str_id = 1;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_00417E33)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_00417E33)(tbl, str_id);//original
}

PROC g_proc_00417E4A;
void credits_big_get_tbl(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------

    //-------------------------------------------------
    if (cred_num == 1)new_tbl = tbl_credits1;
    if (cred_num == 2)new_tbl = tbl_credits3;
    if (cred_num == 3)new_tbl = tbl_credits5;
    str_id = 1;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_00417E4A)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_00417E4A)(tbl, str_id);//original
}

PROC g_proc_0042968A;
void act_get_tbl_small(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    byte lvl = *(byte*)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        new_tbl = tbl_act;
        str_id = 1;
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_tbl = tbl_act;
        str_id = 3;
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_tbl = tbl_act;
        str_id = 5;
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_tbl = tbl_act;
        str_id = 7;
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_tbl = tbl_act;
        str_id = 9;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0042968A)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0042968A)(tbl, str_id);//original
}

PROC g_proc_004296A9;
void act_get_tbl_big(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    byte lvl = *(byte*)LEVEL_OBJ;
    //-------------------------------------------------
    if (lvl == LVL_HUMAN1)
    {
        new_tbl = tbl_act;
        str_id = 2;
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_tbl = tbl_act;
        str_id = 4;
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_tbl = tbl_act;
        str_id = 6;
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_tbl = tbl_act;
        str_id = 8;
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_tbl = tbl_act;
        str_id = 10;
    }
    //-------------------------------------------------

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_004296A9)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_004296A9)(tbl, str_id);//original
}

PROC g_proc_0041C51C;
void netstat_get_tbl_nation(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------

    //-------------------------------------------------
    if ((str_id == 56) || (str_id == 60))
        new_tbl = tbl_net;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_0041C51C)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_0041C51C)(tbl, str_id);//original
}

PROC g_proc_00431229;
void rank_get_tbl(void* tbl, WORD str_id)
{
    void* new_tbl = NULL;
    //-------------------------------------------------

    //-------------------------------------------------
    new_tbl = tbl_rank;

    if (new_tbl)
        return ((void (*)(void*, int))g_proc_00431229)(new_tbl, str_id);
    else
        return ((void (*)(void*, int))g_proc_00431229)(tbl, str_id);//original
}

char instal2[] = "\\stonia\\fonts.mpq";
DWORD loaded_instal2 = 0;

char my_font6[] = "StoniaFonts\\font6.fnt";
char my_font10x[] = "StoniaFonts\\font10x.fnt";
char my_font12x[] = "StoniaFonts\\font12x.fnt";
char my_font32[] = "StoniaFonts\\font32.fnt";
char my_font50[] = "StoniaFonts\\font50.fnt";

void reload_install_exe2()
{
    if (loaded_instal2 == 0)
    {
        *(DWORD*)INSTALL_EXE_POINTER = 0;//remove existing install
        char buf[] = "\x0\x0\x0\x0";
        patch_setdword((DWORD*)buf, (DWORD)instal2);
        PATCH_SET((char*)INSTALL_EXE_NAME1, buf);
        PATCH_SET((char*)INSTALL_EXE_NAME2, buf);//change names
        PATCH_SET((char*)INSTALL_EXE_NAME3, buf);
        ((int (*)(int, int))F_RELOAD_INSTALL_EXE)(1, 0);//load install.exe
        loaded_instal2 = *(DWORD*)INSTALL_EXE_POINTER;
    }
    else
    {
        *(DWORD*)INSTALL_EXE_POINTER = loaded_instal2;
    }
}

PROC g_proc_00424A9C;//32
PROC g_proc_00424AB2;//50
PROC g_proc_004288B2;//12
PROC g_proc_00428896;//10
PROC g_proc_0042887D;//6
void* storm_font_load(char* name, char* a1, int a2)
{
    DWORD orig_instal = *(DWORD*)INSTALL_EXE_POINTER;//remember existing install
    reload_install_exe2();
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\font6.fnt", -1) == CSTR_EQUAL)
    {
        name = my_font6;
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\font10x.fnt", -1) == CSTR_EQUAL)
    {
        name = my_font10x;
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\font12x.fnt", -1) == CSTR_EQUAL)
    {
        name = my_font12x;
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\font32.fnt", -1) == CSTR_EQUAL)
    {
        name = my_font32;
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\font50.fnt", -1) == CSTR_EQUAL)
    {
        name = my_font50;
    }
    void* original = ((void* (*)(char*, char*, int))g_proc_00424A9C)(name, a1, a2);//original
    *(DWORD*)INSTALL_EXE_POINTER = orig_instal;//restore existing install
    return original;
}

void small_fonts_reload()
{
    DWORD orig_instal = *(DWORD*)INSTALL_EXE_POINTER;//remember existing install
    reload_install_exe2();
    *(DWORD*)FONT_POINTER_6 = ((DWORD(*)(char*, char*, int))F_FONT_RELOAD)(my_font6, (char*)FONT_RELOAD_PARAM, 0x3B9);//original storm font load
    *(DWORD*)FONT_POINTER_10 = ((DWORD(*)(char*, char*, int))F_FONT_RELOAD)(my_font10x, (char*)FONT_RELOAD_PARAM, 0x3BA);//original storm font load
    *(DWORD*)FONT_POINTER_12 = ((DWORD(*)(char*, char*, int))F_FONT_RELOAD)(my_font12x, (char*)FONT_RELOAD_PARAM, 0x3BB);//original storm font load
    *(DWORD*)INSTALL_EXE_POINTER = orig_instal;//restore existing install
}

bool pcx_save_raw = false;

void pal_load(byte* palette_adr, void* pal)
{
    if (palette_adr != NULL)
    {
        if (pal != NULL)
        {
            DWORD i = 0;
            while (i < (256 * 4))
            {
                *(byte*)(palette_adr + i) = *(byte*)((DWORD)pal + i);
                i++;
            }
        }
    }
}

PROC g_proc_004372EE;
void pcx_load_menu(char* name, void* pcx_info, byte* palette_adr)
{
    ((void (*)(char*, void*, byte*))g_proc_004372EE)(name, pcx_info, palette_adr);//original
    void* new_pcx_pixels = NULL; 
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\titlemenu_bne.pcx", -1) == CSTR_EQUAL)
    {
        cred_num = 0;
        new_pcx_pixels = pcx_menu;
        pal_load(palette_adr, pcx_menu_pal);
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\title_rel_bne.pcx", -1) == CSTR_EQUAL)
    {
        small_fonts_reload();
        if (pcx_save_raw)
        {
            byte* adr = (byte*)*(DWORD*)((DWORD)pcx_info + 4);
            byte data_pcx[640 * 480];
            byte data_pcx_pal[256 * 4];
            DWORD i = 0;
            while (i < (640 * 480))
            {
                data_pcx[i] = *(char*)(adr + i);
                i++;
            }
            FILE* fp;
            if ((fp = fopen("title_rel.raw", "a+b")) != NULL)
            {
                fwrite(data_pcx, sizeof(byte), 640 * 480, fp);
                fclose(fp);
            }
            i = 0;
            while (i < (256 * 4))
            {
                data_pcx_pal[i] = *(byte*)(palette_adr + i);
                i++;
            }
            if ((fp = fopen("title_rel.pal", "a+b")) != NULL)
            {
                fwrite(data_pcx_pal, sizeof(byte), 256 * 4, fp);
                fclose(fp);
            }
        }
        else
        {
            new_pcx_pixels = pcx_splash;
            pal_load(palette_adr, pcx_splash_pal);
        }
    }
    if (new_pcx_pixels)patch_setdword((DWORD*)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_00430058;
void pcx_load_briefing(char* name, void* pcx_info, byte* palette_adr)
{
    ((void (*)(char*, void*, byte*))g_proc_00430058)(name, pcx_info, palette_adr);//original
    byte lvl = *(byte*)LEVEL_OBJ;
    void* new_pcx_pixels = NULL;
    if (lvl == LVL_HUMAN1)
    {
        new_pcx_pixels = pcx_brif1;
        pal_load(palette_adr, pcx_brif1_pal);
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_pcx_pixels = pcx_brif2;
        pal_load(palette_adr, pcx_brif2_pal);
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_pcx_pixels = pcx_brif3;
        pal_load(palette_adr, pcx_brif3_pal);
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_pcx_pixels = pcx_brif4;
        pal_load(palette_adr, pcx_brif4_pal);
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_pcx_pixels = pcx_brif5;
        pal_load(palette_adr, pcx_brif5_pal);
    }
    if (new_pcx_pixels)patch_setdword((DWORD*)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_00429625;//load palette
PROC g_proc_00429654;//load image
void pcx_load_act(char* name, void* pcx_info, byte* palette_adr)
{
    ((void (*)(char*, void*, byte*))g_proc_00429625)(name, pcx_info, palette_adr);//original
    byte lvl = *(byte*)LEVEL_OBJ;
    void* new_pcx_pixels = NULL;
    if (lvl == LVL_HUMAN1)
    {
        new_pcx_pixels = pcx_act1;
        pal_load(palette_adr, pcx_act1_pal);
    }
    if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
    {
        new_pcx_pixels = pcx_act2;
        pal_load(palette_adr, pcx_act2_pal);
    }
    if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
    {
        new_pcx_pixels = pcx_act3;
        pal_load(palette_adr, pcx_act3_pal);
    }
    if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
    {
        new_pcx_pixels = pcx_act4;
        pal_load(palette_adr, pcx_act4_pal);
    }
    if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
    {
        new_pcx_pixels = pcx_act5;
        pal_load(palette_adr, pcx_act5_pal);
    }
    if (new_pcx_pixels)patch_setdword((DWORD*)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_0041F004;
void pcx_load_final(char* name, void* pcx_info, byte* palette_adr)
{
    finale_dlg = true;
    ((void (*)(char*, void*, byte*))g_proc_0041F004)(name, pcx_info, palette_adr);//original
    void* new_pcx_pixels = NULL;
    new_pcx_pixels = pcx_end;
    pal_load(palette_adr, pcx_end_pal);
    if (new_pcx_pixels)patch_setdword((DWORD*)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_00417DDB;
void pcx_load_credits(char* name, void* pcx_info, byte* palette_adr)
{
    ((void (*)(char*, void*, byte*))g_proc_00417DDB)(name, pcx_info, palette_adr);//original
    void* new_pcx_pixels = NULL;
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\credits.pcx", -1) == CSTR_EQUAL)
    {
        if (cred_num == 0)
        {
            cred_num = 1; 
            new_pcx_pixels = pcx_credit1;
            pal_load(palette_adr, pcx_credit1_pal);
        }
        if (cred_num == 2)
        {
            cred_num = 3;
            new_pcx_pixels = pcx_credit3;
            pal_load(palette_adr, pcx_credit3_pal);
        }
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\2xcredit.pcx", -1) == CSTR_EQUAL)
    {
        cred_num = 2;
        new_pcx_pixels = pcx_credit2;
        pal_load(palette_adr, pcx_credit2_pal);
    }
    if (new_pcx_pixels)patch_setdword((DWORD*)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_0043169E;
void pcx_load_statistic(char* name, void* pcx_info, byte* palette_adr)
{
    ((void (*)(char*, void*, byte*))g_proc_0043169E)(name, pcx_info, palette_adr);//original
    void* new_pcx_pixels = NULL;
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\hwinglu.pcx", -1) == CSTR_EQUAL)
    {
        new_pcx_pixels = pcx_win;
        pal_load(palette_adr, pcx_win_pal);
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\hlosglu.pcx", -1) == CSTR_EQUAL)
    {
        new_pcx_pixels = pcx_loss;
        pal_load(palette_adr, pcx_loss_pal);
    }
    if (new_pcx_pixels)patch_setdword((DWORD*)((DWORD)pcx_info + 4), (DWORD)new_pcx_pixels);
}

PROC g_proc_00462D4D;
void* storm_file_load(char* name, int a1, int a2, int a3, int a4, int a5, int a6)
{
    void* original = ((void* (*)(void*, int, int, int, int, int, int))g_proc_00462D4D)(name, a1, a2, a3, a4, a5, a6);//original
    void* new_file = NULL;
    //-------------------------------------------------
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "rez\\hdisptch.bin", -1) == CSTR_EQUAL)
    {
        memcpy(bin_brifing_copy, bin_brifing, bin_brifing_size);
        new_file = bin_brifing_copy;
        patch_setdword((DWORD*)a2, bin_brifing_size);
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "rez\\mainmenu.bin", -1) == CSTR_EQUAL)
    {
        memcpy(bin_menu_copy, bin_menu, bin_menu_size);
        new_file = bin_menu_copy;
        patch_setdword((DWORD*)a2, bin_menu_size);
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "rez\\snglplay.bin", -1) == CSTR_EQUAL)
    {
        memcpy(bin_sngl_copy, bin_sngl, bin_sngl_size);
        new_file = bin_sngl_copy;
        patch_setdword((DWORD*)a2, bin_sngl_size);
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "rez\\newcmpgn.bin", -1) == CSTR_EQUAL)
    {
        memcpy(bin_newcmp_copy, bin_newcmp, bin_newcmp_size);
        new_file = bin_newcmp_copy;
        patch_setdword((DWORD*)a2, bin_newcmp_size);
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "rez\\creditsBNE.bin", -1) == CSTR_EQUAL)
    {
        memcpy(bin_credits_copy, bin_credits, bin_credits_size);
        new_file = bin_credits_copy;
        patch_setdword((DWORD*)a2, bin_credits_size);
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "rez\\ai.bin", -1) == CSTR_EQUAL)
    {
        new_file = bin_AI;
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "rez\\script.bin", -1) == CSTR_EQUAL)
    {
        new_file = bin_script;
    }
    if (CompareString(LOCALE_SYSTEM_DEFAULT, NORM_IGNORECASE, name, -1, "art\\bgs\\xswamp\\xswamp.cv4", -1) == CSTR_EQUAL)
    {
        byte lvl = *(byte*)LEVEL_OBJ;
        if (lvl >= LVL_HUMAN4)
        {
            new_file = tile_cave_cv4;
        }
        else
        {
            new_file = tile_wasteland_cv4;
        }
    }
    //-------------------------------------------------
    if (new_file)
        return new_file;
    else
        return original;
}

void set_rgb(DWORD* adr, byte r, byte g, byte b)
{
    patch_setdword(adr, r + (g << 8) + (b << 16));
}

void set_rgb4(DWORD* adr, byte r, byte g, byte b)
{
    patch_setdword(adr, (4 * r) + ((4 * g) << 8) + ((4 * b) << 16));
}

void set_rgb8(DWORD* adr, byte r, byte g, byte b)
{
    patch_setdword(adr, (8 * r) + ((8 * g) << 8) + ((8 * b) << 16));
}

void set_palette(void* pal)
{
    for (int i = 0; i < 256; i++)
    {
        byte r = *(byte*)((int)pal + 3 * i);
        byte g = *(byte*)((int)pal + 3 * i + 1);
        byte b = *(byte*)((int)pal + 3 * i + 2);
        set_rgb4((DWORD*)(SCREEN_INITIAL_PALETTE + i * 4), r, g, b);
    }
}

void set_dwarf_palette()
{
    //change human blue buttons to dwarf violet
    int k = 0xB0;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 4, 0, 4); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 6, 0, 6); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 8, 0, 8); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 9, 0, 9); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 11, 0, 11); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 14, 0, 14); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 15, 0, 15); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 16, 0, 16); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 18, 0, 18); k++;
    set_rgb8((DWORD*)(SCREEN_INITIAL_PALETTE + k * 4), 19, 0, 19);
    if (*(byte*)MAP_ERA == 3)
    {
        for (int i = 0x20; i < 0x30; i++)
        {
            *(byte*)(SCREEN_INITIAL_PALETTE + 2 + i * 4) = 8 * 12;
        }
    }
}

PROC g_proc_0041F9FD;
int tilesets_load(int a)
{
    int original = ((int (*)(int))g_proc_0041F9FD)(a);
    byte lvl = *(byte*)LEVEL_OBJ;
    if (lvl >= LVL_HUMAN4)
    {
        patch_setdword((DWORD*)TILESET_POINTER_VX4, (DWORD)tile_cave_vx4);
        patch_setdword((DWORD*)TILESET_POINTER_VR4, (DWORD)tile_cave_vr4);
        //cv4 in storm_file_load
        set_palette(tile_cave_palette);
    }
    else
    {
        patch_setdword((DWORD*)TILESET_POINTER_VX4, (DWORD)tile_wasteland_vx4);
        patch_setdword((DWORD*)TILESET_POINTER_VR4, (DWORD)tile_wasteland_vr4);
        //cv4 in storm_file_load
        set_palette(tile_wasteland_palette);
    }
    set_dwarf_palette();
    return original;
}

PROC g_proc_0041F97D;
int map_file_load(int a, int b, void** map, DWORD* size)
{
    //4AE0A9 demo
    //4958E4 demo timer
    byte lvl = *(byte*)LEVEL_OBJ;
    byte d = *(byte*)GB_DEMO;//demo
    bool f = false;
    if (d == 1)
    {
        *map = pud_map_atr;
        *size = pud_map_atr_size;
        f = true;
    }
    else
    {
        if (lvl == LVL_HUMAN1)
        {
            *map = pud_map1;
            *size = pud_map1_size;
            f = true;
        }
        if ((lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
        {
            *map = pud_map2;
            *size = pud_map2_size;
            f = true;
        }
        if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8))
        {
            *map = pud_map3;
            *size = pud_map3_size;
            f = true;
        }
        if ((lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11))
        {
            *map = pud_map4;
            *size = pud_map4_size;
            f = true;
        }
        if ((lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
        {
            *map = pud_map5;
            *size = pud_map5_size;
            f = true;
        }
    }
    if (f)return 1;
    else return ((int (*)(int, int, void*, DWORD*))g_proc_0041F97D)(a, b, map, size);//original
}

void* file_load(const char name[])
{
    void* file = NULL;
    FILE* fp;
    char path[MAX_PATH] = { 0 };
    _snprintf(path, sizeof(path), "%s%s", FILES_PATH, name);
    if ((fp = fopen(path, "rb")) != NULL)//file opened
    {
        fseek(fp, 0, SEEK_END); // seek to end of file
        DWORD size = ftell(fp); // get current file pointer
        fseek(fp, 0, SEEK_SET); // seek back to beginning of file
        file = malloc(size);
        fread(file, sizeof(unsigned char), size, fp);//read
        fclose(fp);
    }
    return file;
}

void file_load_size(const char name[], void** m, DWORD* s)
{
    void* file = NULL;
    FILE* fp;
    char path[MAX_PATH] = { 0 };
    _snprintf(path, sizeof(path), "%s%s", FILES_PATH, name);
    if ((fp = fopen(path, "rb")) != NULL)//file opened
    {
        fseek(fp, 0, SEEK_END); // seek to end of file
        DWORD size = ftell(fp); // get current file pointer
        *s = size;
        fseek(fp, 0, SEEK_SET); // seek back to beginning of file
        file = malloc(size);
        fread(file, sizeof(unsigned char), size, fp);//read
        fclose(fp);
    }
    *m = file;
}

char video_mod_shab[] = "Stonia\\videos\\%s";
char video1[] = "start.smk";
char video2[] = "intro.smk";
char video3[] = "final.smk";

PROC g_proc_0043B16F;
void smk_play_sprintf_name(int dest, char* shab, char* name)
{
    if ((!lstrcmpi(name, "intro_m.smk")) || (!lstrcmpi(name, "smoke_m.smk")))
    {
        shab = video_mod_shab;
        name = video2;
    }
    if (!lstrcmpi(name, "hvict_m.smk"))
    {
        shab = video_mod_shab;
        name = video3;
    }
    ((void (*)(int, char*, char*))g_proc_0043B16F)(dest, shab, name);//original
}

PROC g_proc_0043B362;
void smk_play_sprintf_blizzard(int dest, char* shab, char* name)
{
    ((void (*)(int, char*, char*))g_proc_0043B362)(dest, video_mod_shab, video1);//original
}

char instal[] = "\\stonia\\music.mpq";
DWORD loaded_instal = 0;
int need_instal = 0;
char music1[] = "epic.wav";
char music2[] = "epilog.wav";
char music3[] = "pirate.wav";
char music4[] = "caves.wav";

char music_orc2[] = "orc2.wav";
char music_orc4[] = "orc4.wav";
char music_orc6[] = "orc6.wav";

void reload_install_exe()
{
    if (loaded_instal == 0)
    {
        *(DWORD*)INSTALL_EXE_POINTER = 0;//remove existing install
        char buf[] = "\x0\x0\x0\x0";
        patch_setdword((DWORD*)buf, (DWORD)instal);
        PATCH_SET((char*)INSTALL_EXE_NAME1, buf);
        PATCH_SET((char*)INSTALL_EXE_NAME2, buf);//change names
        PATCH_SET((char*)INSTALL_EXE_NAME3, buf);
        ((int (*)(int, int))F_RELOAD_INSTALL_EXE)(1, 0);//load install.exe
        loaded_instal = *(DWORD*)INSTALL_EXE_POINTER;
    }
    else
    {
        *(DWORD*)INSTALL_EXE_POINTER = loaded_instal;
    }
}

void music_play_sprintf_name(int dest, char* shab, char* name)
{
    if (!lstrcmpi(name, "human2.wav"))
    {
        name = music_orc2;
    }
    if (!lstrcmpi(name, "human4.wav"))
    {
        name = music_orc4;
    }
    if (!lstrcmpi(name, "human6.wav"))
    {
        name = music_orc6;
    }
    if (!lstrcmpi(name, "owarroom.wav"))
    {
        need_instal = 2;
        name = music1;
    }
    if (!lstrcmpi(name, "hwarroom.wav"))
    {
        byte lvl = *(byte*)LEVEL_OBJ;
        if ((lvl == LVL_HUMAN1) || (lvl == LVL_HUMAN2) || (lvl == LVL_HUMAN3))
        {
            need_instal = 2;
            name = music3;
        }
        else if ((lvl == LVL_HUMAN4) || (lvl == LVL_HUMAN5) || (lvl == LVL_HUMAN6) || (lvl == LVL_HUMAN7) || (lvl == LVL_HUMAN8)
            || (lvl == LVL_HUMAN9) || (lvl == LVL_HUMAN10) || (lvl == LVL_HUMAN11)
            || (lvl == LVL_HUMAN12) || (lvl == LVL_HUMAN13) || (lvl == LVL_HUMAN14))
        {
            need_instal = 2;
            name = music4;
        }
    }
    if (finale_dlg && (*(byte*)LEVEL_OBJ == (LVL_HUMAN14 + 2)))//end of human campanign
    {
        need_instal = 2;
        name = music2;
    }
    DWORD orig = *(DWORD*)F_MUSIC_SPRINTF;//original music sprintf call
    ((void (*)(int, char*, char*))orig)(dest, shab, name);//original
}

PROC g_proc_00440F4A;
PROC g_proc_00440F5F;
int music_play_get_install()
{
    DWORD orig_instal = *(DWORD*)INSTALL_EXE_POINTER;//remember existing install
    if (need_instal == 1)
    {
        reload_install_exe();
        need_instal--;
    }
    if (need_instal == 2)need_instal--;
    int original = ((int (*)())g_proc_00440F4A)();//original
    *(DWORD*)INSTALL_EXE_POINTER = orig_instal;//restore existing install
    return original;
}

void files_init()
{
    grp_port = file_load("icons\\port.grp");
    grp_port2 = file_load("icons\\port2.grp");

    grp_dwarf_dead = file_load("units\\death.grp");
    grp_dwarf_peon = file_load("units\\peon.grp");
    grp_dwarf_peong = file_load("units\\peong.grp");
    grp_dwarf_peonl = file_load("units\\peonl.grp");
    grp_dwarf_cart = file_load("units\\cart.grp");
    grp_dwarf_footmanS = file_load("units\\footmanS.grp");
    grp_dwarf_footmanA = file_load("units\\footmanA.grp");
    grp_dwarf_footmanM = file_load("units\\footmanM.grp");
    grp_dwarf_armored = file_load("units\\armored.grp");
    grp_dwarf_alchemist = file_load("units\\wizard.grp");
    grp_dwarf_vert = file_load("units\\vert.grp");
    grp_dwarf_hero = file_load("units\\hero.grp");
    grp_dwarf_herop = file_load("units\\herop.grp");
    grp_canon = file_load("units\\canon.grp");

    grp_water = file_load("units\\water.grp");
    grp_stone = file_load("units\\stone.grp");
    grp_slime = file_load("units\\slime.grp");
    
    grp_dwarf_farm = file_load("buildings\\farm.grp");
    grp_dwarf_barak = file_load("buildings\\barrak.grp");
    grp_dwarf_workshop = file_load("buildings\\workshop.grp");
    grp_dwarf_lumber = file_load("buildings\\lumber.grp");
    grp_dwarf_smelt = file_load("buildings\\smeltery.grp");
    grp_dwarf_sklad = file_load("buildings\\sklad.grp");
    grp_dwarf_skladup = file_load("buildings\\skladup.grp");
    grp_dwarf_baseG = file_load("buildings\\gnomish.grp");
    grp_dwarf_baseD = file_load("buildings\\dwarwen.grp");
    grp_dwarf_baseGup = file_load("buildings\\gnomishup.grp");
    grp_dwarf_baseDup = file_load("buildings\\dwarwenup.grp");
    grp_dwarf_obsG = file_load("buildings\\obsgnomish.grp");
    grp_dwarf_obsD = file_load("buildings\\obsdwarwen.grp");
    grp_dwarf_academ = file_load("buildings\\academie.grp");

    grp_gold_stone = file_load("buildings\\gold_stone.grp");

    grp_runeword = file_load("runes\\runeword.grp");
    grp_runecircle = file_load("runes\\runecircle.grp");

    grp_shot = file_load("bullets\\shot.grp");
    grp_throw = file_load("bullets\\throw.grp");
    grp_golem_throw = file_load("bullets\\golem_throw.grp");

    grp_boss_left = file_load("boss\\boss_left.grp");
    grp_boss_right = file_load("boss\\boss_right.grp");
    grp_boss_jump = file_load("boss\\boss_jump.grp");
    grp_boss_throw = file_load("boss\\boss_throw.grp");

    tbl_names = file_load("textes\\names.tbl");
    tbl_obj = file_load("textes\\obj.tbl");
    tbl_act = file_load("textes\\act.tbl");
    tbl_net = file_load("textes\\net.tbl");
    tbl_rank = file_load("textes\\rank.tbl");
    tbl_end = file_load("textes\\end.tbl");
    tbl_credits1 = file_load("textes\\credits1.tbl");
    tbl_credits2 = file_load("textes\\credits2.tbl");
    tbl_credits3 = file_load("textes\\credits3.tbl");
    tbl_credits4 = file_load("textes\\credits4.tbl");
    tbl_credits5 = file_load("textes\\credits5.tbl");
    tbl_credits6 = file_load("textes\\credits6.tbl");
    tbl_brif1 = file_load("textes\\brif1.tbl");
    tbl_brif2 = file_load("textes\\brif2.tbl");
    tbl_brif3 = file_load("textes\\brif3.tbl");
    tbl_brif4 = file_load("textes\\brif4.tbl");
    tbl_brif5 = file_load("textes\\brif5.tbl");

    tbl_dialog = file_load("textes\\dialog.tbl");

    file_load_size("bin\\brifing.bin", &bin_brifing, &bin_brifing_size);
    file_load_size("bin\\brifing.bin", &bin_brifing_copy, &bin_brifing_size);
    file_load_size("bin\\menu.bin", &bin_menu, &bin_menu_size);
    file_load_size("bin\\menu.bin", &bin_menu_copy, &bin_menu_size);
    file_load_size("bin\\sngl.bin", &bin_sngl, &bin_sngl_size);
    file_load_size("bin\\sngl.bin", &bin_sngl_copy, &bin_sngl_size);
    file_load_size("bin\\newcmp.bin", &bin_newcmp, &bin_newcmp_size);
    file_load_size("bin\\newcmp.bin", &bin_newcmp_copy, &bin_newcmp_size);
    file_load_size("bin\\credits.bin", &bin_credits, &bin_credits_size);
    file_load_size("bin\\credits.bin", &bin_credits_copy, &bin_credits_size);

    file_load_size("bin\\ai.bin", &bin_AI, &bin_AI_size);
    file_load_size("bin\\script.bin", &bin_script, &bin_script_size);

    file_load_size("tile\\cave.cv4", &tile_cave_cv4, &tile_cave_cv4_size);
    file_load_size("tile\\cave.vr4", &tile_cave_vr4, &tile_cave_vr4_size);
    file_load_size("tile\\cave.vx4", &tile_cave_vx4, &tile_cave_vx4_size);
    tile_cave_palette = file_load("tile\\cave.ppl");
    file_load_size("tile\\wasteland.cv4", &tile_wasteland_cv4, &tile_wasteland_cv4_size);
    file_load_size("tile\\wasteland.vr4", &tile_wasteland_vr4, &tile_wasteland_vr4_size);
    file_load_size("tile\\wasteland.vx4", &tile_wasteland_vx4, &tile_wasteland_vx4_size);
    tile_wasteland_palette = file_load("tile\\wasteland.ppl");

    pcx_splash = file_load("images\\splash.raw");
    pcx_splash_pal = file_load("images\\splash.pal");
    pcx_menu = file_load("images\\menu.raw");
    pcx_menu_pal = file_load("images\\menu.pal");
    pcx_brif1 = file_load("images\\brif1.raw");
    pcx_brif1_pal = file_load("images\\brif1.pal");
    pcx_brif2 = file_load("images\\brif2.raw");
    pcx_brif2_pal = file_load("images\\brif2.pal");
    pcx_brif3 = file_load("images\\brif3.raw");
    pcx_brif3_pal = file_load("images\\brif3.pal");
    pcx_brif4 = file_load("images\\brif4.raw");
    pcx_brif4_pal = file_load("images\\brif4.pal");
    pcx_brif5 = file_load("images\\brif5.raw");
    pcx_brif5_pal = file_load("images\\brif5.pal");
    pcx_act1 = file_load("images\\act1.raw");
    pcx_act1_pal = file_load("images\\act1.pal");
    pcx_act2 = file_load("images\\act2.raw");
    pcx_act2_pal = file_load("images\\act2.pal");
    pcx_act3 = file_load("images\\act3.raw");
    pcx_act3_pal = file_load("images\\act3.pal");
    pcx_act4 = file_load("images\\act4.raw");
    pcx_act4_pal = file_load("images\\act4.pal");
    pcx_act5 = file_load("images\\act5.raw");
    pcx_act5_pal = file_load("images\\act5.pal");
    pcx_end = file_load("images\\end.raw");
    pcx_end_pal = file_load("images\\end.pal");
    pcx_credit1 = file_load("images\\credit1.raw");
    pcx_credit1_pal = file_load("images\\credit1.pal");
    pcx_credit2 = file_load("images\\credit2.raw");
    pcx_credit2_pal = file_load("images\\credit2.pal");
    pcx_credit3 = file_load("images\\credit3.raw");
    pcx_credit3_pal = file_load("images\\credit3.pal");
    pcx_win = file_load("images\\win.raw");
    pcx_win_pal = file_load("images\\win.pal");
    pcx_loss = file_load("images\\loss.raw");
    pcx_loss_pal = file_load("images\\loss.pal");

    file_load_size("maps\\map1.pud", &pud_map1, &pud_map1_size);
    file_load_size("maps\\map2.pud", &pud_map2, &pud_map2_size);
    file_load_size("maps\\map3.pud", &pud_map3, &pud_map3_size);
    file_load_size("maps\\map4.pud", &pud_map4, &pud_map4_size);
    file_load_size("maps\\map5.pud", &pud_map5, &pud_map5_size);
    file_load_size("maps\\atr.pud", &pud_map_atr, &pud_map_atr_size);
}

PROC g_proc_0042A443;
void act_init()
{
    WORD m = *(WORD*)LEVEL_ID;
    if (*(byte*)LEVEL_OBJ == LVL_HUMAN1)*(WORD*)LEVEL_ID = 0x52D0;
    else *(WORD*)LEVEL_ID = 0x52C8;//mission file number
    *(WORD*)PREVIOUS_ACT = 0;//prev act
    ((void (*)())g_proc_0042A443)();//original
    *(WORD*)LEVEL_ID = m;//mission file number restore
}

PROC g_proc_00422D76;
void sound_play_unit_speech(WORD sid, int a, int* u, int b)
{
    bool f = true;
    if (u != NULL)
    {
        WORD sn = 0;
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if (id == U_DANATH)
        {
            if ((sid >= 262) && (sid <= 270))
            {
                sn = sid - 262;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)h_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)h_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                h_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if ((id == U_PIGFARM) && !((sid == 21) || (sid == 22)))
        {
            if (sid == 50)
            {
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)dh_name);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)dh_sound);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                dh_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if (((id == U_LOTHAR) || (id == U_OGREMOUND)) && !((sid == 21) || (sid == 22)))
        {
            if ((sid >= 141) && (sid <= 152))
            {
                sn = sid - 141;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)a_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)a_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                a_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
            else if (sid == 49)
            {
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)da_name);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)da_sound);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                da_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if (((id == U_MAGE) || (id == U_ALCHEMIST)) && !((sid == 21) || (sid == 22)))
        {
            if ((sid >= 213) && (sid <= 222))
            {
                sn = sid - 213;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)al_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)al_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                al_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if ((id == U_PEON) || (id == U_DRAGON) || (id == U_GREAT_HALL) || (id == U_STRONGHOLD) || (id == U_FORTRESS))
        {
            if (sid == 85)sid = 23;
            if ((sid >= 3) && (sid <= 23))
            {
                if ((sid == 21) && (id != U_PEON))sid += 1;
                sn = sid - 3;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)w_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)w_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                w_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
            else if ((sid >= 239) && (sid <= 242))//dragon
            {
                sn = sid - 239;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)f_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)f_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                f_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if ((id == U_GRUNT) || (id == U_OBARRACK))
        {
            if ((sid >= 3) && (sid <= 23))
            {
                sn = sid - 3;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)m_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)m_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                m_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if (id == U_UTER)
        {
            if ((sid >= 153) && (sid <= 164))
            {
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(69, a, u, b);//original
                f = false;
            }
        }
        else if (id == U_TROLL)
        {
            if ((sid >= 203) && (sid <= 212))
            {
                sn = (((int (*)())F_NET_RANDOM)() % 2);
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)secret_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)secret_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                secret_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if (id == U_OGRE)
        {
            if ((sid >= 165) && (sid <= 177))
            {
                sn = 2 + (((int (*)())F_NET_RANDOM)() % 2);
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)secret_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)secret_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                secret_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else if (id == U_KARGATH)
        {
            if ((sid >= 324) && (sid <= 333))
            {
                sn = 4 + (((int (*)())F_NET_RANDOM)() % 3);
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)secret_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)secret_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                secret_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        else
        {
            if ((sid == 21) || (sid == 22))
            {
                sid = 22;
                sn = sid - 3;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)w_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)w_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                w_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
        if ((sid == 24)|| (sid == 25))
        {
            sn = sid - 24;
            def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
            def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)d_names[sn]);
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)d_sounds[sn]);
            ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
            d_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
            patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
            f = false;
        }
        if ((sid == 26) || (sid == 27))
        {
            bool f2 = false;
            if (id == U_PEON)f2 = true;
            else if (id == U_GRUNT)f2 = true;
            else if (id == U_LOTHAR)f2 = true;
            else if (id == U_DANATH)f2 = true;
            else if (id == U_MAGE)f2 = true;
            else if ((id == U_UTER) || (id == U_OGRE) || (id == U_KARGATH) || (id == U_PALADIN))f = false;
            if (f2)
            {
                sid = 26;
                sn = sid - 24;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)d_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)d_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
                d_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
    }
    if (f)((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
}

PROC g_proc_00422D5F;
void sound_play_unit_speech_soft(WORD sid, int a, int* u, int b)
{
    bool f = true;
    if (u != NULL)
    {
        WORD sn = 0;
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if ((sid == 26) || (sid == 27))
        {
            bool f2 = false;
            if (id == U_PEON)f2 = true;
            else if (id == U_GRUNT)f2 = true;
            else if (id == U_LOTHAR)f2 = true;
            else if (id == U_DANATH)f2 = true;
            else if (id == U_MAGE)f2 = true;
            else if ((id == U_UTER) || (id == U_OGRE) || (id == U_KARGATH) || (id == U_PALADIN))f = false;
            if (f2)
            {
                sid = 26;
                sn = sid - 24;
                def_name = (void*)*(int*)(SOUNDS_FILES_LIST + 8 + 24 * sid);
                def_sound = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);//save default
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)d_names[sn]);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)d_sounds[sn]);
                ((void (*)(WORD, int, int*, int))g_proc_00422D5F)(sid, a, u, b);//original
                d_sounds[sn] = (void*)*(int*)(SOUNDS_FILES_LIST + 16 + 24 * sid);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 16 + 24 * sid), (DWORD)def_sound);
                patch_setdword((DWORD*)(SOUNDS_FILES_LIST + 8 + 24 * sid), (DWORD)def_name);//restore default
                f = false;
            }
        }
    }
    if (f)((void (*)(WORD, int, int*, int))g_proc_00422D76)(sid, a, u, b);//original
}
//-------------files

PROC g_proc_0044F37D;
void main_menu_init(int a)
{
    if (remember_music != 101)
        *(DWORD*)VOLUME_MUSIC = remember_music;
    if (remember_sound != 101)
        *(DWORD*)VOLUME_SOUND = remember_sound;
    ((void (*)(DWORD))F_SET_VOLUME)(SET_VOLUME_PARAM);//set volume
    remember_music = 101;
    remember_sound = 101;

    *(byte*)PLAYER_RACE = 0;//human cursor
    ((void (*)(int))g_proc_0044F37D)(a);
}

PROC g_proc_00418937;
void dispatch_die_unitdraw_update_1_man(int* u)
{
    ((void (*)(int*))g_proc_00418937)(u);//original
    bool f = false;
    byte id = *((byte*)((uintptr_t)u + S_ID));
    if (id == U_PEON)f = true;
    if (id == U_GRUNT)f = true;
    if (id == U_GROM)f = true;
    if (id == U_LOTHAR)f = true;
    if (id == U_DANATH)f = true;
    if ((id == U_UTER) || (id == U_OGRE) || (id == U_KARGATH))
    {
        set_stat(u, 101, S_MOVEMENT_TYPE);
    }
    if (f)
        set_stat(u, 100, S_MOVEMENT_TYPE);
    if (id == U_PEASANT)
    {
        set_stat(u, 0, S_PEON_FLAGS);
    }
}

PROC g_proc_00451590;
void unit_kill_peon_change(int* u)
{
    byte id = *((byte*)((uintptr_t)u + S_ID));
    if (id == U_PEASANT)
    {
        byte pf = *((byte*)((uintptr_t)u + S_PEON_FLAGS));
        pf &= ~PEON_LOADED;
        set_stat(u, pf, S_PEON_FLAGS);
    }
    ((void (*)(int*))g_proc_00451590)(u);//original
}

void def_stat(byte u, WORD hp, byte str, byte prc, byte arm, byte rng, byte gold, byte lum, byte oil, byte time)
{
    //change some unit stats (changes for ALL units of this type)

    /*
    to change vision and multiselectable you can use this construction
    char buf[] = "\x0\x0\x0\x0";//fix vision
    patch_setdword((DWORD*)buf, (DWORD)F_VISION6);
    PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_DEMON), buf);
    char buf2[] = "\x1";
    PATCH_SET((char*)(UNIT_MULTISELECTABLE + U_DEMON), buf2);
    */

    char buf2[] = "\x0\x0";
    buf2[0] = hp % 256;
    buf2[1] = hp / 256;
    PATCH_SET((char*)(UNIT_HP_TABLE + 2 * u), buf2);
    char buf[] = "\x0";
    buf[0] = str;
    PATCH_SET((char*)(UNIT_STRENGTH_TABLE + u), buf);
    buf[0] = prc;
    PATCH_SET((char*)(UNIT_PIERCE_TABLE + u), buf);
    buf[0] = arm;
    PATCH_SET((char*)(UNIT_ARMOR_TABLE + u), buf);
    if (rng != 0)
    {
        buf[0] = rng;
        PATCH_SET((char*)(UNIT_RANGE_TABLE + u), buf);
    }
    buf[0] = gold;
    PATCH_SET((char*)(UNIT_GOLD_TABLE + u), buf);
    buf[0] = lum;
    PATCH_SET((char*)(UNIT_LUMBER_TABLE + u), buf);
    buf[0] = oil;
    PATCH_SET((char*)(UNIT_OIL_TABLE + u), buf);
    buf[0] = time;
    PATCH_SET((char*)(UNIT_TIME_TABLE + u), buf);
}

void def_upgr(byte u, WORD gold, WORD lum, WORD oil)
{
    char buf2[] = "\x0\x0";
    buf2[0] = gold % 256;
    buf2[1] = gold / 256;
    PATCH_SET((char*)(UPGR_GOLD_TABLE + 2 * u), buf2);
    buf2[0] = lum % 256;
    buf2[1] = lum / 256;
    PATCH_SET((char*)(UPGR_LUMBER_TABLE + 2 * u), buf2);
    buf2[0] = oil % 256;
    buf2[1] = oil / 256;
    PATCH_SET((char*)(UPGR_OIL_TABLE + 2 * u), buf2);
}

PROC g_proc_0041F915;
int map_load(void* map, DWORD size)
{
    int original = ((int (*)(void*, DWORD))g_proc_0041F915)(map, size);

    byte lvl = *(byte*)LEVEL_OBJ;

    char buf[] = "\x0\x0\x0\x0";
    if (!((lvl >= LVL_HUMAN9) && (lvl <= LVL_HUMAN11)))
    {
        patch_setdword((DWORD*)buf, (DWORD)F_VISION6);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_DANATH), buf);
    }
    else
    {
        patch_setdword((DWORD*)buf, (DWORD)F_VISION4);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_DANATH), buf);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_MAGE), buf);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_TROLL), buf);
        patch_setdword((DWORD*)buf, (DWORD)F_VISION3);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_GRUNT), buf);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_LOTHAR), buf);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_OGRE), buf);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_KARGATH), buf);
        PATCH_SET((char*)(UNIT_VISION_FUNCTIONS_TABLE + 4 * U_UTER), buf);
    }

    def_stat(U_PEON, 40, 3, 3, 1, 1, 50, 0, 0, 50);
    def_stat(U_GRUNT, 60, 5, 4, 3, 1, 90, 0, 0, 65);
    def_stat(U_LOTHAR, 80, 8, 5, 6, 1, 100, 20, 0, 75);
    def_stat(U_MAGE, 50, 0, 7, 1, 3, 120, 0, 0, 75);
    def_stat(U_DANATH, 160, 14, 3, 9, 0, 0, 0, 0, 0);
    def_stat(U_DRAGON, 125, 0, 8, 8, 6, 120, 40, 0, 120);

    def_stat(U_OGRE, 120, 11, 3, 8, 1, 0, 0, 0, 0);
    def_stat(U_KARGATH, 80, 4, 5, 0, 3, 0, 0, 0, 0);
    def_stat(U_UTER, 120, 2, 4, 0, 2, 0, 0, 0, 0);

    if (*(byte*)LEVEL_OBJ != LVL_HUMAN1)
    {
        def_stat(U_PIGFARM, 450, 0, 0, 20, 0, 70, 35, 0, 80);
        def_stat(U_OBARRACK, 600, 0, 0, 20, 0, 100, 50, 0, 140);
    }
    else
    {
        def_stat(U_PIGFARM, 450, 0, 0, 20, 0, 70, 35, 0, 0);
        def_stat(U_OBARRACK, 600, 0, 0, 20, 0, 100, 50, 0, 0);
    }
    def_stat(U_GREAT_HALL, 1250, 0, 0, 20, 0, 125, 100, 0, 255);
    def_stat(U_STRONGHOLD, 1850, 0, 0, 20, 0, 125, 100, 0, 255);
    def_stat(U_FORTRESS, 2400, 0, 0, 20, 0, 250, 125, 0, 255);
    def_stat(U_OSMITH, 800, 0, 0, 20, 0, 120, 40, 0, 160);
    def_stat(U_OLUMBER, 500, 0, 0, 20, 0, 100, 35, 0, 100);
    def_stat(U_OGREMOUND, 900, 0, 0, 20, 0, 125, 50, 0, 200);
    def_stat(U_ALCHEMIST, 500, 0, 0, 20, 0, 125, 50, 0, 120);

    def_stat(U_HTOWER, 100, 0, 0, 20, 0, 100, 40, 0, 50);
    def_stat(U_HCANONTOWER, 160, 50, 0, 20, 0, 50, 10, 0, 80);

    def_stat(U_RUNESTONE, 1000, 0, 0, 20, 0, 0, 0, 0, 0);

    def_upgr(UG_SP_HEAL, 750, 0, 0);
    def_upgr(UG_SP_HASTE, 1000, 0, 0);
    def_upgr(UG_SP_BLOODLUST, 1250, 0, 0);
    def_upgr(UG_SP_RUNES, 2000, 500, 0);
    def_upgr(UG_CATDMG1, 1000, 0, 0);
    def_upgr(UG_UPBOAT_ATTACK1, 750, 0, 0);
    def_upgr(UG_UPBOAT_ARMOR1, 1000, 500, 0);
    def_upgr(UG_UPAXE1, 750, 150, 0);
    def_upgr(UG_UPAXE2, 1500, 300, 0);
    def_upgr(UG_UPSHIELD1, 600, 300, 0);
    def_upgr(UG_UPSHIELD2, 1000, 500, 0);
    def_upgr(UG_UPRANGERS, 1000, 0, 0);
    def_upgr(UG_UPSPEAR1, 1000, 1000, 0);
    def_upgr(UG_UPLONGBOW, 750, 0, 0);
    def_upgr(UG_UPHSCOUTS, 1000, 200, 0);

    return original;
}

PROC g_proc_00424745;//entering
PROC g_proc_004529C0;//grow struct
int goods_into_inventory(int* p)
{
    int tr = (*(int*)((uintptr_t)p + S_ORDER_UNIT_POINTER));
    if (tr != 0)
    {
        bool f = false;
        int* trg = (int*)tr;
        byte o = *((byte*)((uintptr_t)p + S_OWNER));
        byte id = *((byte*)((uintptr_t)p + S_ID));
        byte tid = *((byte*)((uintptr_t)trg + S_ID));
        byte pf = *((byte*)((uintptr_t)p + S_PEON_FLAGS));
        int pflag = *(int*)(UNIT_GLOBAL_FLAGS + id * 4);
        int tflag = *(int*)(UNIT_GLOBAL_FLAGS + tid * 4);
        int res = 100;
        if (pf & PEON_LOADED)
        {
            if (((pflag & IS_SHIP) != 0) && ((tflag & IS_OILRIG) == 0))
            {
                int r = get_val(REFINERY, o);
                if (r != 0)res = 125;
                else res = 100;
                change_res(o, 2, 1, res);
                add_total_res(o, 2, 1, res);
                f = true;
            }
            else
            {
                if (((tflag & IS_TOWNHALL) != 0) || ((tflag & IS_LUMBER) != 0))
                {
                    if (((tflag & IS_TOWNHALL) != 0))
                    {
                        pf |= PEON_IN_CASTLE;
                        set_stat(p, pf, S_PEON_FLAGS);
                    }
                    if (((pf & PEON_HARVEST_GOLD) != 0) && ((tflag & IS_TOWNHALL) != 0))
                    {
                        int r2 = get_val(TH2, o);
                        int r3 = get_val(TH3, o);
                        if (r3 != 0)res = 120;
                        else
                        {
                            if (r2 != 0)res = 110;
                            else res = 100;
                        }
                        if (o == *(byte*)LOCAL_PLAYER)
                        {
                            if (get_upgrade(CATA_DMG, o) != 0)
                                res += 10;
                            if (get_val(SMITH, o) != 0)
                                res += 10;
                            if (*(byte*)(GB_RANGER + o) == 1)
                                res += 10;
                        }
                        pf &= ~PEON_HARVEST_GOLD;
                        change_res(o, 0, 1, res);
                        add_total_res(o, 0, 1, res);
                        f = true;
                    }
                    else
                    {
                        if (((pf & PEON_HARVEST_LUMBER) != 0))
                        {
                            int r = get_val(LUMBERMILL, o);
                            if (r != 0)res = 125;
                            else res = 100;
                            if (o == *(byte*)LOCAL_PLAYER)
                            {
                                if (*(byte*)(GB_LONGBOW + o) == 1)
                                    res += 25;
                            }
                            pf &= ~PEON_HARVEST_LUMBER;
                            change_res(o, 1, 1, res);
                            add_total_res(o, 1, 1, res);
                            f = true;
                        }
                    }
                }
            }
        }
            if (f)
            {
                pf &= ~PEON_LOADED;
                set_stat(p, pf, S_PEON_FLAGS);
                ((void (*)(int*))F_GROUP_SET)(p);
                return 1;
            }
    }
    return 0;
    //return ((int(*)(int*))g_proc_00424745)(p);//original
}

void sounds_ready_table_set(byte id, WORD snd)
{
    char buf[] = "\x0\x0";
    buf[0] = snd % 256;
    buf[1] = snd / 256;
    PATCH_SET((char*)(UNIT_SOUNDS_READY_TABLE + 2 * id), buf);
}

char build_buttons[5 * BUTTON_SIZE + 1];
char abuild_buttons[5 * BUTTON_SIZE + 1];
char Dth_buttons[7 * BUTTON_SIZE + 1];
char lumb_buttons[4 * BUTTON_SIZE + 1];
char farm_buttons[BUTTON_SIZE + 1];
char tower_buttons[BUTTON_SIZE + 1];
char barak_buttons[2 * BUTTON_SIZE + 1];
char smith_buttons[3 * BUTTON_SIZE + 1];
char stable_buttons[2 * BUTTON_SIZE + 1];
char akadem_buttons[5 * BUTTON_SIZE + 1];

char buttons_mage[7 * BUTTON_SIZE + 1];
char buttons_warrior[10 * BUTTON_SIZE + 1];
char buttons_cart[1 * BUTTON_SIZE + 1];

int num_book = 0;

void buttons_init_build()
{
    int k = 0;
    int (*r) (byte) = empty_true;
    void (*r1) (int) = empty_build_building;

    build_buttons[20 * k + 0] = '\x0';//button id?
    build_buttons[20 * k + 1] = '\x0';//button id?
    build_buttons[20 * k + 2] = '\x29';//icon
    build_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(build_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(build_buttons + (20 * k + 8)), (DWORD)r1);
    build_buttons[20 * k + 12] = U_GREAT_HALL;//arg
    build_buttons[20 * k + 13] = U_GREAT_HALL;//unit id
    build_buttons[20 * k + 14] = '\x33';//string from tbl
    build_buttons[20 * k + 15] = '\x1';//string from tbl
    build_buttons[20 * k + 16] = '\x0';//flags?
    build_buttons[20 * k + 17] = '\x0';//flags?
    build_buttons[20 * k + 18] = '\x0';//flags?
    build_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    build_buttons[20 * k + 0] = '\x1';//button id?
    build_buttons[20 * k + 1] = '\x0';//button id?
    build_buttons[20 * k + 2] = '\x2B';//icon
    build_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(build_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(build_buttons + (20 * k + 8)), (DWORD)r1);
    build_buttons[20 * k + 12] = U_OBARRACK;//arg
    build_buttons[20 * k + 13] = U_OBARRACK;//unit id
    build_buttons[20 * k + 14] = '\x2B';//string from tbl
    build_buttons[20 * k + 15] = '\x1';//string from tbl
    build_buttons[20 * k + 16] = '\x0';//flags?
    build_buttons[20 * k + 17] = '\x0';//flags?
    build_buttons[20 * k + 18] = '\x0';//flags?
    build_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    build_buttons[20 * k + 0] = '\x2';//button id?
    build_buttons[20 * k + 1] = '\x0';//button id?
    build_buttons[20 * k + 2] = '\x27';//icon
    build_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(build_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(build_buttons + (20 * k + 8)), (DWORD)r1);
    build_buttons[20 * k + 12] = U_PIGFARM;//arg
    build_buttons[20 * k + 13] = U_PIGFARM;//unit id
    build_buttons[20 * k + 14] = '\x32';//string from tbl
    build_buttons[20 * k + 15] = '\x1';//string from tbl
    build_buttons[20 * k + 16] = '\x0';//flags?
    build_buttons[20 * k + 17] = '\x0';//flags?
    build_buttons[20 * k + 18] = '\x0';//flags?
    build_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    build_buttons[20 * k + 0] = '\x3';//button id?
    build_buttons[20 * k + 1] = '\x0';//button id?
    build_buttons[20 * k + 2] = '\x3C';//icon
    build_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(build_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(build_buttons + (20 * k + 8)), (DWORD)r1);
    build_buttons[20 * k + 12] = U_HTOWER;//arg
    build_buttons[20 * k + 13] = U_HTOWER;//unit id
    build_buttons[20 * k + 14] = '\x2F';//string from tbl
    build_buttons[20 * k + 15] = '\x1';//string from tbl
    build_buttons[20 * k + 16] = '\x0';//flags?
    build_buttons[20 * k + 17] = '\x0';//flags?
    build_buttons[20 * k + 18] = '\x0';//flags?
    build_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    build_buttons[20 * k + 0] = '\x8';//button id?
    build_buttons[20 * k + 1] = '\x0';//button id?
    build_buttons[20 * k + 2] = '\x5b';//icon
    build_buttons[20 * k + 3] = '\x0';//icon
    build_buttons[20 * k + 4] = '\xF0';
    build_buttons[20 * k + 5] = '\x40';
    build_buttons[20 * k + 6] = '\x44';
    build_buttons[20 * k + 7] = '\x0';
    build_buttons[20 * k + 8] = '\x70';
    build_buttons[20 * k + 9] = '\xA6';
    build_buttons[20 * k + 10] = '\x44';
    build_buttons[20 * k + 11] = '\x0';
    build_buttons[20 * k + 12] = '\x0';//arg
    build_buttons[20 * k + 13] = '\x6E';//unit id
    build_buttons[20 * k + 14] = '\x97';//string from tbl
    build_buttons[20 * k + 15] = '\x1';//string from tbl
    build_buttons[20 * k + 16] = '\x0';//flags?
    build_buttons[20 * k + 17] = '\x0';//flags?
    build_buttons[20 * k + 18] = '\x0';//flags?
    build_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)build_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_ORC_BUILD_BUTTONS, b1);
}

void buttons_init_abuild()
{
    int k = 0;
    int (*r) () = _2tir;
    int (*rr) () = _3tir;
    int (*rrr) () = _2tir_L;
    void (*r1) (int) = empty_build_building;

    abuild_buttons[20 * k + 0] = '\x0';//button id?
    abuild_buttons[20 * k + 1] = '\x0';//button id?
    abuild_buttons[20 * k + 2] = '\x2D';//icon
    abuild_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 8)), (DWORD)r1);
    abuild_buttons[20 * k + 12] = U_OLUMBER;//arg
    abuild_buttons[20 * k + 13] = U_OLUMBER;//unit id
    abuild_buttons[20 * k + 14] = '\x2C';//string from tbl
    abuild_buttons[20 * k + 15] = '\x1';//string from tbl
    abuild_buttons[20 * k + 16] = '\x0';//flags?
    abuild_buttons[20 * k + 17] = '\x0';//flags?
    abuild_buttons[20 * k + 18] = '\x0';//flags?
    abuild_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    abuild_buttons[20 * k + 0] = '\x1';//button id?
    abuild_buttons[20 * k + 1] = '\x0';//button id?
    abuild_buttons[20 * k + 2] = '\x2F';//icon
    abuild_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 4)), (DWORD)rrr);
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 8)), (DWORD)r1);
    abuild_buttons[20 * k + 12] = U_OSMITH;//arg
    abuild_buttons[20 * k + 13] = U_OSMITH;//unit id
    abuild_buttons[20 * k + 14] = '\x3A';//string from tbl
    abuild_buttons[20 * k + 15] = '\x1';//string from tbl
    abuild_buttons[20 * k + 16] = '\x0';//flags?
    abuild_buttons[20 * k + 17] = '\x0';//flags?
    abuild_buttons[20 * k + 18] = '\x0';//flags?
    abuild_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    abuild_buttons[20 * k + 0] = '\x3';//button id?
    abuild_buttons[20 * k + 1] = '\x0';//button id?
    abuild_buttons[20 * k + 2] = '\x3B';//icon
    abuild_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 8)), (DWORD)r1);
    abuild_buttons[20 * k + 12] = U_ALCHEMIST;//arg
    abuild_buttons[20 * k + 13] = U_ALCHEMIST;//unit id
    abuild_buttons[20 * k + 14] = '\x42';//string from tbl
    abuild_buttons[20 * k + 15] = '\x1';//string from tbl
    abuild_buttons[20 * k + 16] = '\x0';//flags?
    abuild_buttons[20 * k + 17] = '\x0';//flags?
    abuild_buttons[20 * k + 18] = '\x0';//flags?
    abuild_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    abuild_buttons[20 * k + 0] = '\x4';//button id?
    abuild_buttons[20 * k + 1] = '\x0';//button id?
    abuild_buttons[20 * k + 2] = '\x39';//icon
    abuild_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(abuild_buttons + (20 * k + 8)), (DWORD)r1);
    abuild_buttons[20 * k + 12] = U_OGREMOUND;//arg
    abuild_buttons[20 * k + 13] = U_OGREMOUND;//unit id
    abuild_buttons[20 * k + 14] = '\x2E';//string from tbl
    abuild_buttons[20 * k + 15] = '\x1';//string from tbl
    abuild_buttons[20 * k + 16] = '\x0';//flags?
    abuild_buttons[20 * k + 17] = '\x0';//flags?
    abuild_buttons[20 * k + 18] = '\x0';//flags?
    abuild_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    abuild_buttons[20 * k + 0] = '\x8';//button id?
    abuild_buttons[20 * k + 1] = '\x0';//button id?
    abuild_buttons[20 * k + 2] = '\x5b';//icon
    abuild_buttons[20 * k + 3] = '\x0';//icon
    abuild_buttons[20 * k + 4] = '\xF0';
    abuild_buttons[20 * k + 5] = '\x40';
    abuild_buttons[20 * k + 6] = '\x44';
    abuild_buttons[20 * k + 7] = '\x0';
    abuild_buttons[20 * k + 8] = '\x70';
    abuild_buttons[20 * k + 9] = '\xA6';
    abuild_buttons[20 * k + 10] = '\x44';
    abuild_buttons[20 * k + 11] = '\x0';
    abuild_buttons[20 * k + 12] = '\x0';//arg
    abuild_buttons[20 * k + 13] = '\x6E';//unit id
    abuild_buttons[20 * k + 14] = '\x97';//string from tbl
    abuild_buttons[20 * k + 15] = '\x1';//string from tbl
    abuild_buttons[20 * k + 16] = '\x0';//flags?
    abuild_buttons[20 * k + 17] = '\x0';//flags?
    abuild_buttons[20 * k + 18] = '\x0';//flags?
    abuild_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)abuild_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_ORC_ABUILD_BUTTONS, b1);
}

void buttons_init_barrak()
{
    int k = 0;
    int (*r) (byte) = empty_true;
    int (*rr) (byte) = empty_research_marks;
    void (*r1) (int) = empty_build;
    void (*r2) (int) = empty_build_research;

    barak_buttons[20 * k + 0] = '\x0';//button id?
    barak_buttons[20 * k + 1] = '\x0';//button id?
    barak_buttons[20 * k + 2] = '\x3';//icon
    barak_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(barak_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(barak_buttons + (20 * k + 8)), (DWORD)r1);
    barak_buttons[20 * k + 12] = U_GRUNT;//arg
    barak_buttons[20 * k + 13] = U_GRUNT;//unit id
    barak_buttons[20 * k + 14] = '\x1B';//string from tbl
    barak_buttons[20 * k + 15] = '\x1';//string from tbl
    barak_buttons[20 * k + 16] = '\x0';//flags?
    barak_buttons[20 * k + 17] = '\x0';//flags?
    barak_buttons[20 * k + 18] = '\x0';//flags?
    barak_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    barak_buttons[20 * k + 0] = '\x1';//button id?
    barak_buttons[20 * k + 1] = '\x0';//button id?
    barak_buttons[20 * k + 2] = '\x86';//icon
    barak_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(barak_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(barak_buttons + (20 * k + 8)), (DWORD)r2);
    barak_buttons[20 * k + 12] = '\x0';//arg
    barak_buttons[20 * k + 13] = UG_UPHMARKS;//unit id
    barak_buttons[20 * k + 14] = '\x55';//string from tbl
    barak_buttons[20 * k + 15] = '\x1';//string from tbl
    barak_buttons[20 * k + 16] = '\x0';//flags?
    barak_buttons[20 * k + 17] = '\x0';//flags?
    barak_buttons[20 * k + 18] = '\x0';//flags?
    barak_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)barak_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_OBARRACK, b1);

    char buf[] = "\x90\x90";
    PATCH_SET((char*)0x0044409F, buf);//allow marks without rangers
}

void buttons_init_lumber()
{
    int k = 0;
    int (*rr) (byte) = empty_research_long;
    int (*rr2) (byte) = empty_research_scout;
    int (*rr3) (byte) = empty_research_swords;
    void (*r1) (int) = empty_build_research;

    lumb_buttons[20 * k + 0] = '\x0';//button id?
    lumb_buttons[20 * k + 1] = '\x0';//button id?
    lumb_buttons[20 * k + 2] = '\x84';//icon
    lumb_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 8)), (DWORD)r1);
    lumb_buttons[20 * k + 12] = '\x0';//arg
    lumb_buttons[20 * k + 13] = UG_UPLONGBOW ;//unit id
    lumb_buttons[20 * k + 14] = '\x53';//string from tbl
    lumb_buttons[20 * k + 15] = '\x1';//string from tbl
    lumb_buttons[20 * k + 16] = '\x0';//flags?
    lumb_buttons[20 * k + 17] = '\x0';//flags?
    lumb_buttons[20 * k + 18] = '\x0';//flags?
    lumb_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    lumb_buttons[20 * k + 0] = '\x1';//button id?
    lumb_buttons[20 * k + 1] = '\x0';//button id?
    lumb_buttons[20 * k + 2] = '\x85';//icon
    lumb_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 4)), (DWORD)rr2);
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 8)), (DWORD)r1);
    lumb_buttons[20 * k + 12] = '\x0';//arg
    lumb_buttons[20 * k + 13] = UG_UPHSCOUTS;//unit id
    lumb_buttons[20 * k + 14] = '\x54';//string from tbl
    lumb_buttons[20 * k + 15] = '\x1';//string from tbl
    lumb_buttons[20 * k + 16] = '\x0';//flags?
    lumb_buttons[20 * k + 17] = '\x0';//flags?
    lumb_buttons[20 * k + 18] = '\x0';//flags?
    lumb_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    lumb_buttons[20 * k + 0] = '\x2';//button id?
    lumb_buttons[20 * k + 1] = '\x0';//button id?
    lumb_buttons[20 * k + 2] = '\x79';//icon
    lumb_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 4)), (DWORD)rr3);
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 8)), (DWORD)r1);
    lumb_buttons[20 * k + 12] = '\x0';//arg
    lumb_buttons[20 * k + 13] = UG_UPAXE1;//unit id
    lumb_buttons[20 * k + 14] = '\x4B';//string from tbl
    lumb_buttons[20 * k + 15] = '\x1';//string from tbl
    lumb_buttons[20 * k + 16] = '\x0';//flags?
    lumb_buttons[20 * k + 17] = '\x0';//flags?
    lumb_buttons[20 * k + 18] = '\x0';//flags?
    lumb_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    lumb_buttons[20 * k + 0] = '\x2';//button id?
    lumb_buttons[20 * k + 1] = '\x0';//button id?
    lumb_buttons[20 * k + 2] = '\x87';//icon
    lumb_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 4)), (DWORD)rr3);
    patch_setdword((DWORD*)(lumb_buttons + (20 * k + 8)), (DWORD)r1);
    lumb_buttons[20 * k + 12] = '\x1';//arg
    lumb_buttons[20 * k + 13] = UG_UPAXE2;//unit id
    lumb_buttons[20 * k + 14] = '\x4B';//string from tbl
    lumb_buttons[20 * k + 15] = '\x1';//string from tbl
    lumb_buttons[20 * k + 16] = '\x0';//flags?
    lumb_buttons[20 * k + 17] = '\x0';//flags?
    lumb_buttons[20 * k + 18] = '\x0';//flags?
    lumb_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)lumb_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_OLUMBER, b1);

    char buf[] = "\x90\x90";
    PATCH_SET((char*)0x00443FDF, buf);//allow long without rangers
    PATCH_SET((char*)0x0044403F, buf);//allow scout without rangers
}

void buttons_init_farm()
{
    int k = 0;
    int (*rr) (byte) = empty_research_cat;
    void (*r1) (int) = empty_build_research;

    farm_buttons[20 * k + 0] = '\x0';//button id?
    farm_buttons[20 * k + 1] = '\x0';//button id?
    farm_buttons[20 * k + 2] = '\x8A';//icon
    farm_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(farm_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(farm_buttons + (20 * k + 8)), (DWORD)r1);
    farm_buttons[20 * k + 12] = '\x0';//arg
    farm_buttons[20 * k + 13] = UG_CATDMG1;//unit id
    farm_buttons[20 * k + 14] = '\x5A';//string from tbl
    farm_buttons[20 * k + 15] = '\x1';//string from tbl
    farm_buttons[20 * k + 16] = '\x0';//flags?
    farm_buttons[20 * k + 17] = '\x0';//flags?
    farm_buttons[20 * k + 18] = '\x0';//flags?
    farm_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)farm_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_PIGFARM, b1);
}

void buttons_init_tower()
{
    int k = 0;
    int (*r) (byte) = empty_upgrade_tower;
    void (*r1) (int) = empty_build_upgrade_self;

    tower_buttons[20 * k + 0] = '\x1';//button id?
    tower_buttons[20 * k + 1] = '\x0';//button id?
    tower_buttons[20 * k + 2] = '\x4C';//icon
    tower_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(tower_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(tower_buttons + (20 * k + 8)), (DWORD)r1);
    tower_buttons[20 * k + 12] = '\x1';//arg
    tower_buttons[20 * k + 13] = U_HCANONTOWER;//unit id
    tower_buttons[20 * k + 14] = '\x40';//string from tbl
    tower_buttons[20 * k + 15] = '\x1';//string from tbl
    tower_buttons[20 * k + 16] = '\x0';//flags?
    tower_buttons[20 * k + 17] = '\x0';//flags?
    tower_buttons[20 * k + 18] = '\x0';//flags?
    tower_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)tower_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_HTOWER, b1);
}

void buttons_init_smith()
{
    int k = 0;
    int (*rr) (byte) = empty_research_ranger;
    int (*rr3) (byte) = empty_research_shield;
    void (*r1) (int) = empty_build_research;

    smith_buttons[20 * k + 0] = '\x0';//button id?
    smith_buttons[20 * k + 1] = '\x0';//button id?
    smith_buttons[20 * k + 2] = '\x6';//icon
    smith_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(smith_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(smith_buttons + (20 * k + 8)), (DWORD)r1);
    smith_buttons[20 * k + 12] = '\x0';//arg
    smith_buttons[20 * k + 13] = UG_UPRANGERS;//unit id
    smith_buttons[20 * k + 14] = '\x52';//string from tbl
    smith_buttons[20 * k + 15] = '\x1';//string from tbl
    smith_buttons[20 * k + 16] = '\x0';//flags?
    smith_buttons[20 * k + 17] = '\x0';//flags?
    smith_buttons[20 * k + 18] = '\x0';//flags?
    smith_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    smith_buttons[20 * k + 0] = '\x2';//button id?
    smith_buttons[20 * k + 1] = '\x0';//button id?
    smith_buttons[20 * k + 2] = '\xA5';//icon
    smith_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(smith_buttons + (20 * k + 4)), (DWORD)rr3);
    patch_setdword((DWORD*)(smith_buttons + (20 * k + 8)), (DWORD)r1);
    smith_buttons[20 * k + 12] = '\x0';//arg
    smith_buttons[20 * k + 13] = UG_UPSHIELD1;//unit id
    smith_buttons[20 * k + 14] = '\x4D';//string from tbl
    smith_buttons[20 * k + 15] = '\x1';//string from tbl
    smith_buttons[20 * k + 16] = '\x0';//flags?
    smith_buttons[20 * k + 17] = '\x0';//flags?
    smith_buttons[20 * k + 18] = '\x0';//flags?
    smith_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    smith_buttons[20 * k + 0] = '\x2';//button id?
    smith_buttons[20 * k + 1] = '\x0';//button id?
    smith_buttons[20 * k + 2] = '\xA6';//icon
    smith_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(smith_buttons + (20 * k + 4)), (DWORD)rr3);
    patch_setdword((DWORD*)(smith_buttons + (20 * k + 8)), (DWORD)r1);
    smith_buttons[20 * k + 12] = '\x1';//arg
    smith_buttons[20 * k + 13] = UG_UPSHIELD2;//unit id
    smith_buttons[20 * k + 14] = '\x4D';//string from tbl
    smith_buttons[20 * k + 15] = '\x1';//string from tbl
    smith_buttons[20 * k + 16] = '\x0';//flags?
    smith_buttons[20 * k + 17] = '\x0';//flags?
    smith_buttons[20 * k + 18] = '\x0';//flags?
    smith_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)smith_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_OSMITH, b1);
}

void buttons_init_stable()
{
    int k = 0;
    int (*r) (byte) = empty_true;
    int (*rr) (byte) = empty_research_arrows;
    void (*r1) (int) = empty_build;
    void (*r2) (int) = empty_build_research;

    stable_buttons[20 * k + 0] = '\x0';//button id?
    stable_buttons[20 * k + 1] = '\x0';//button id?
    stable_buttons[20 * k + 2] = '\x8';//icon
    stable_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(stable_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(stable_buttons + (20 * k + 8)), (DWORD)r1);
    stable_buttons[20 * k + 12] = U_LOTHAR;//arg
    stable_buttons[20 * k + 13] = U_LOTHAR;//unit id
    stable_buttons[20 * k + 14] = '\x13';//string from tbl
    stable_buttons[20 * k + 15] = '\x1';//string from tbl
    stable_buttons[20 * k + 16] = '\x0';//flags?
    stable_buttons[20 * k + 17] = '\x0';//flags?
    stable_buttons[20 * k + 18] = '\x0';//flags?
    stable_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    stable_buttons[20 * k + 0] = '\x1';//button id?
    stable_buttons[20 * k + 1] = '\x0';//button id?
    stable_buttons[20 * k + 2] = '\x80';//icon
    stable_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(stable_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(stable_buttons + (20 * k + 8)), (DWORD)r2);
    stable_buttons[20 * k + 12] = '\x0';//arg
    stable_buttons[20 * k + 13] = UG_UPSPEAR1;//unit id
    stable_buttons[20 * k + 14] = '\x4A';//string from tbl
    stable_buttons[20 * k + 15] = '\x1';//string from tbl
    stable_buttons[20 * k + 16] = '\x0';//flags?
    stable_buttons[20 * k + 17] = '\x0';//flags?
    stable_buttons[20 * k + 18] = '\x0';//flags?
    stable_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)stable_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_OGREMOUND, b1);
}

void buttons_init_th()
{
    int k = 0;
    int (*r) (byte) = empty_true;
    int (*r2t) (byte) = empty_flyer;
    int (*rr) (byte) = empty_res_meds;
    int (*rr2) (byte) = empty_res_hammers;
    int (*rr3) (byte) = empty_upgrade_th;
    void (*r1) (int) = empty_build;
    void (*r2) (int) = empty_build_research;
    void (*r3) (int) = empty_upgrade_th1;
    void (*r4) (int) = empty_upgrade_th2;
    void (*r5) (int) = empty_build_upgrade_self;

    Dth_buttons[20 * k + 0] = '\x0';//button id?
    Dth_buttons[20 * k + 1] = '\x0';//button id?
    Dth_buttons[20 * k + 2] = '\x0';//icon
    Dth_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 8)), (DWORD)r1);
    Dth_buttons[20 * k + 12] = U_PEON;//arg
    Dth_buttons[20 * k + 13] = U_PEON;//unit id
    Dth_buttons[20 * k + 14] = '\xE';//string from tbl
    Dth_buttons[20 * k + 15] = '\x1';//string from tbl
    Dth_buttons[20 * k + 16] = '\x0';//flags?
    Dth_buttons[20 * k + 17] = '\x0';//flags?
    Dth_buttons[20 * k + 18] = '\x0';//flags?
    Dth_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    Dth_buttons[20 * k + 0] = '\x1';//button id?
    Dth_buttons[20 * k + 1] = '\x0';//button id?
    Dth_buttons[20 * k + 2] = '\x88';//icon
    Dth_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 4)), (DWORD)rr3);
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 8)), (DWORD)r3);
    Dth_buttons[20 * k + 12] = '\x0';//arg
    Dth_buttons[20 * k + 13] = U_STRONGHOLD;//unit id
    Dth_buttons[20 * k + 14] = '\x3D';//string from tbl
    Dth_buttons[20 * k + 15] = '\x1';//string from tbl
    Dth_buttons[20 * k + 16] = '\x0';//flags?
    Dth_buttons[20 * k + 17] = '\x0';//flags?
    Dth_buttons[20 * k + 18] = '\x0';//flags?
    Dth_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    Dth_buttons[20 * k + 0] = '\x1';//button id?
    Dth_buttons[20 * k + 1] = '\x0';//button id?
    Dth_buttons[20 * k + 2] = '\x43';//icon
    Dth_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 4)), (DWORD)rr3);
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 8)), (DWORD)r4);
    Dth_buttons[20 * k + 12] = '\x0';//arg
    Dth_buttons[20 * k + 13] = U_STRONGHOLD;//unit id
    Dth_buttons[20 * k + 14] = '\x3E';//string from tbl
    Dth_buttons[20 * k + 15] = '\x1';//string from tbl
    Dth_buttons[20 * k + 16] = '\x0';//flags?
    Dth_buttons[20 * k + 17] = '\x0';//flags?
    Dth_buttons[20 * k + 18] = '\x0';//flags?
    Dth_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    Dth_buttons[20 * k + 0] = '\x1';//button id?
    Dth_buttons[20 * k + 1] = '\x0';//button id?
    Dth_buttons[20 * k + 2] = '\x45';//icon
    Dth_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 4)), (DWORD)rr3);
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 8)), (DWORD)r5);
    Dth_buttons[20 * k + 12] = '\x1';//arg
    Dth_buttons[20 * k + 13] = U_FORTRESS;//unit id
    Dth_buttons[20 * k + 14] = '\x3B';//string from tbl
    Dth_buttons[20 * k + 15] = '\x1';//string from tbl
    Dth_buttons[20 * k + 16] = '\x0';//flags?
    Dth_buttons[20 * k + 17] = '\x0';//flags?
    Dth_buttons[20 * k + 18] = '\x0';//flags?
    Dth_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    Dth_buttons[20 * k + 0] = '\x2';//button id?
    Dth_buttons[20 * k + 1] = '\x0';//button id?
    Dth_buttons[20 * k + 2] = '\x1F';//icon
    Dth_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 4)), (DWORD)r2t);
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 8)), (DWORD)r1);
    Dth_buttons[20 * k + 12] = U_DRAGON;//arg
    Dth_buttons[20 * k + 13] = U_DRAGON;//unit id
    Dth_buttons[20 * k + 14] = '\x26';//string from tbl
    Dth_buttons[20 * k + 15] = '\x1';//string from tbl
    Dth_buttons[20 * k + 16] = '\x0';//flags?
    Dth_buttons[20 * k + 17] = '\x0';//flags?
    Dth_buttons[20 * k + 18] = '\x0';//flags?
    Dth_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    Dth_buttons[20 * k + 0] = '\x3';//button id?
    Dth_buttons[20 * k + 1] = '\x0';//button id?
    Dth_buttons[20 * k + 2] = '\x91';//icon
    Dth_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 8)), (DWORD)r2);
    Dth_buttons[20 * k + 12] = '\x0';//arg
    Dth_buttons[20 * k + 13] = UG_UPBOAT_ATTACK1;//unit id
    Dth_buttons[20 * k + 14] = '\x50';//string from tbl
    Dth_buttons[20 * k + 15] = '\x1';//string from tbl
    Dth_buttons[20 * k + 16] = '\x0';//flags?
    Dth_buttons[20 * k + 17] = '\x0';//flags?
    Dth_buttons[20 * k + 18] = '\x0';//flags?
    Dth_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    Dth_buttons[20 * k + 0] = '\x6';//button id?
    Dth_buttons[20 * k + 1] = '\x0';//button id?
    Dth_buttons[20 * k + 2] = '\x9A';//icon
    Dth_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 4)), (DWORD)rr2);
    patch_setdword((DWORD*)(Dth_buttons + (20 * k + 8)), (DWORD)r2);
    Dth_buttons[20 * k + 12] = '\x0';//arg
    Dth_buttons[20 * k + 13] = UG_UPBOAT_ARMOR1;//unit id
    Dth_buttons[20 * k + 14] = '\x4F';//string from tbl
    Dth_buttons[20 * k + 15] = '\x1';//string from tbl
    Dth_buttons[20 * k + 16] = '\x0';//flags?
    Dth_buttons[20 * k + 17] = '\x0';//flags?
    Dth_buttons[20 * k + 18] = '\x0';//flags?
    Dth_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)Dth_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_GREAT_HALL, b1);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_STRONGHOLD, b1);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_FORTRESS, b1);

    char buf[] = "\x90\x90";
    PATCH_SET((char*)0x0044430F, buf);//allow th3 without stables
    PATCH_SET((char*)0x0040DB38, buf);//allow th3 without stables
}

void buttons_init_akadem()
{
    int k = 0;
    int (*r) (byte) = empty_true;
    int (*rr) (byte) = empty_research_spells;
    void (*r1) (int) = empty_build;
    void (*r2) (int) = empty_build_research_spell;

    akadem_buttons[20 * k + 0] = '\x0';//button id?
    akadem_buttons[20 * k + 1] = '\x0';//button id?
    akadem_buttons[20 * k + 2] = '\xE';//icon
    akadem_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 8)), (DWORD)r1);
    akadem_buttons[20 * k + 12] = U_MAGE;//arg
    akadem_buttons[20 * k + 13] = U_MAGE;//unit id
    akadem_buttons[20 * k + 14] = '\x19';//string from tbl
    akadem_buttons[20 * k + 15] = '\x1';//string from tbl
    akadem_buttons[20 * k + 16] = '\x0';//flags?
    akadem_buttons[20 * k + 17] = '\x0';//flags?
    akadem_buttons[20 * k + 18] = '\x0';//flags?
    akadem_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    akadem_buttons[20 * k + 0] = '\x1';//button id?
    akadem_buttons[20 * k + 1] = '\x0';//button id?
    akadem_buttons[20 * k + 2] = '\x6B';//icon
    akadem_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 8)), (DWORD)r2);
    akadem_buttons[20 * k + 12] = A_HEAL;//arg
    akadem_buttons[20 * k + 13] = UG_SP_HEAL;//unit id
    akadem_buttons[20 * k + 14] = '\x81';//string from tbl
    akadem_buttons[20 * k + 15] = '\x1';//string from tbl
    akadem_buttons[20 * k + 16] = '\x0';//flags?
    akadem_buttons[20 * k + 17] = '\x0';//flags?
    akadem_buttons[20 * k + 18] = '\x0';//flags?
    akadem_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    akadem_buttons[20 * k + 0] = '\x2';//button id?
    akadem_buttons[20 * k + 1] = '\x0';//button id?
    akadem_buttons[20 * k + 2] = '\x60';//icon
    akadem_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 8)), (DWORD)r2);
    akadem_buttons[20 * k + 12] = A_HASTE;//arg
    akadem_buttons[20 * k + 13] = UG_SP_HASTE;//unit id
    akadem_buttons[20 * k + 14] = '\x7C';//string from tbl
    akadem_buttons[20 * k + 15] = '\x1';//string from tbl
    akadem_buttons[20 * k + 16] = '\x0';//flags?
    akadem_buttons[20 * k + 17] = '\x0';//flags?
    akadem_buttons[20 * k + 18] = '\x0';//flags?
    akadem_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    akadem_buttons[20 * k + 0] = '\x3';//button id?
    akadem_buttons[20 * k + 1] = '\x0';//button id?
    akadem_buttons[20 * k + 2] = '\x70';//icon
    akadem_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 8)), (DWORD)r2);
    akadem_buttons[20 * k + 12] = A_BLOOD;//arg
    akadem_buttons[20 * k + 13] = UG_SP_BLOODLUST;//unit id
    akadem_buttons[20 * k + 14] = '\x85';//string from tbl
    akadem_buttons[20 * k + 15] = '\x1';//string from tbl
    akadem_buttons[20 * k + 16] = '\x0';//flags?
    akadem_buttons[20 * k + 17] = '\x0';//flags?
    akadem_buttons[20 * k + 18] = '\x0';//flags?
    akadem_buttons[20 * k + 19] = '\x0';//flags?
    k++;
    akadem_buttons[20 * k + 0] = '\x4';//button id?
    akadem_buttons[20 * k + 1] = '\x0';//button id?
    akadem_buttons[20 * k + 2] = '\x22';//icon
    akadem_buttons[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 4)), (DWORD)rr);
    patch_setdword((DWORD*)(akadem_buttons + (20 * k + 8)), (DWORD)r2);
    akadem_buttons[20 * k + 12] = A_RUNES;//arg
    akadem_buttons[20 * k + 13] = UG_SP_RUNES;//unit id
    akadem_buttons[20 * k + 14] = '\x7E';//string from tbl
    akadem_buttons[20 * k + 15] = '\x1';//string from tbl
    akadem_buttons[20 * k + 16] = '\x0';//flags?
    akadem_buttons[20 * k + 17] = '\x0';//flags?
    akadem_buttons[20 * k + 18] = '\x0';//flags?
    akadem_buttons[20 * k + 19] = '\x0';//flags?
    k++;

    char b1[] = "\x0\x0\x0\x0\x0\0\x0\x0";
    b1[0] = k;
    patch_setdword((DWORD*)(b1 + 4), (DWORD)akadem_buttons);
    PATCH_SET((char*)BUTTONS_CARDS + 8 * U_ALCHEMIST, b1);
}

void weapon_change()
{
    int* u = NULL;
    for (int i = 0; (i < 9)&&(u==NULL); i++)
    {
        u = (int*)*(int*)(LOCAL_UNITS_SELECTED + 4 * i);
    }
    if (u != NULL)
    {
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if (id == U_DANATH)
        {
            set_stat(u, PEON_LOADED, S_PEON_FLAGS);
        }
    }
}

void book_of_twilight()
{
    int* u = NULL;
    for (int i = 0; (i < 9) && (u == NULL); i++)
    {
        u = (int*)*(int*)(LOCAL_UNITS_SELECTED + 4 * i);
    }
    if (u != NULL)
    {
        byte id = *((byte*)((uintptr_t)u + S_ID));
        if (id == U_DANATH)
        {
            byte m = *((byte*)((uintptr_t)u + S_MANA));
            if (m == 255)
            {
                set_stat(u, 0, S_MANA);
                set_stat(u, 1500, S_SHIELD);
                sound_play_dialog(30);
                //((void (*)(int*, byte))F_SPELL_SOUND_UNIT)(u, SS_ARMOR);
            }
        }
    }
}

int book_of_twilight_check()
{
    return (*(byte*)(GB_HORSES + 15) != 0) ? 1 : 0;
}

void buttons_init_hero()
{
    int a = BUTTONS_CARDS + 4;
    a = *(int*)a;
    for (int i = 0; i < 200; i++)
    {
        buttons_hero[i] = *(byte*)(a + i);
    }
    char aa[] = "\x6\x0\x97\x0\xf0\x40\x44\x0\xd0\xe6\x40\x0\x1\x39\x6A\x1\x0\x0\x0\x0";
    void (*r1) () = weapon_change;
    patch_setdword((DWORD*)(aa + 8), (DWORD)r1);
    for (int i = 0; i < 20; i++)
    {
        buttons_hero[BUTTON_SIZE * 10 + i] = aa[i];
    }
    char aa2[] = "\x7\x0\x96\x0\xf0\x40\x44\x0\xd0\xe6\x40\x0\x0\x0\x67\x1\x0\x0\x0\x0";
    int (*r) () = book_of_twilight_check;
    void (*r2) () = book_of_twilight;
    patch_setdword((DWORD*)(aa2 + 4), (DWORD)r);
    patch_setdword((DWORD*)(aa2 + 8), (DWORD)r2);
    for (int i = 0; i < 20; i++)
    {
        buttons_hero[220 + i] = aa2[i];
    }
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_DANATH), (DWORD)12);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_DANATH + 4), (DWORD)buttons_hero);
}

void buttons_init_mage()
{
    int a = BUTTONS_CARDS + 8 * U_MAGE + 4;
    a = *(int*)a;
    for (int i = 0; i < 60; i++)
    {
        buttons_mage[i] = *(byte*)(a + i);
    }
    
    int (*r) (byte) = empty_spell_learned;
    void (*r1) (int) = empty_cast_spell;
    int k = 3;
    buttons_mage[20 * k + 0] = '\x3';//button id?
    buttons_mage[20 * k + 1] = '\x0';//button id?
    buttons_mage[20 * k + 2] = '\x6B';//icon
    buttons_mage[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 8)), (DWORD)r1);
    buttons_mage[20 * k + 12] = L_HEAL;//arg
    buttons_mage[20 * k + 13] = ORDER_SPELL_HEAL;//unit id
    buttons_mage[20 * k + 14] = '\x6B';//string from tbl
    buttons_mage[20 * k + 15] = '\x1';//string from tbl
    buttons_mage[20 * k + 16] = '\x0';//flags?
    buttons_mage[20 * k + 17] = '\x0';//flags?
    buttons_mage[20 * k + 18] = '\x0';//flags?
    buttons_mage[20 * k + 19] = '\x0';//flags?
    k++;
    buttons_mage[20 * k + 0] = '\x4';//button id?
    buttons_mage[20 * k + 1] = '\x0';//button id?
    buttons_mage[20 * k + 2] = '\x60';//icon
    buttons_mage[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 8)), (DWORD)r1);
    buttons_mage[20 * k + 12] = L_HASTE;//arg
    buttons_mage[20 * k + 13] = ORDER_SPELL_HASTE;//unit id
    buttons_mage[20 * k + 14] = '\x66';//string from tbl
    buttons_mage[20 * k + 15] = '\x1';//string from tbl
    buttons_mage[20 * k + 16] = '\x0';//flags?
    buttons_mage[20 * k + 17] = '\x0';//flags?
    buttons_mage[20 * k + 18] = '\x0';//flags?
    buttons_mage[20 * k + 19] = '\x0';//flags?
    k++;
    buttons_mage[20 * k + 0] = '\x5';//button id?
    buttons_mage[20 * k + 1] = '\x0';//button id?
    buttons_mage[20 * k + 2] = '\x70';//icon
    buttons_mage[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 8)), (DWORD)r1);
    buttons_mage[20 * k + 12] = L_BLOOD;//arg
    buttons_mage[20 * k + 13] = ORDER_SPELL_BLOODLUST;//unit id
    buttons_mage[20 * k + 14] = '\x6F';//string from tbl
    buttons_mage[20 * k + 15] = '\x1';//string from tbl
    buttons_mage[20 * k + 16] = '\x0';//flags?
    buttons_mage[20 * k + 17] = '\x0';//flags?
    buttons_mage[20 * k + 18] = '\x0';//flags?
    buttons_mage[20 * k + 19] = '\x0';//flags?
    k++;
    buttons_mage[20 * k + 0] = '\x6';//button id?
    buttons_mage[20 * k + 1] = '\x0';//button id?
    buttons_mage[20 * k + 2] = '\x22';//icon
    buttons_mage[20 * k + 3] = '\x0';//icon
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 4)), (DWORD)r);
    patch_setdword((DWORD*)(buttons_mage + (20 * k + 8)), (DWORD)r1);
    buttons_mage[20 * k + 12] = L_RUNES;//arg
    buttons_mage[20 * k + 13] = ORDER_SPELL_RUNES;//unit id
    buttons_mage[20 * k + 14] = '\x68';//string from tbl
    buttons_mage[20 * k + 15] = '\x1';//string from tbl
    buttons_mage[20 * k + 16] = '\x0';//flags?
    buttons_mage[20 * k + 17] = '\x0';//flags?
    buttons_mage[20 * k + 18] = '\x0';//flags?
    buttons_mage[20 * k + 19] = '\x0';//flags?
    k++;

    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_MAGE), (DWORD)7);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_MAGE + 4), (DWORD)buttons_mage);
}

void buttons_init_warrior()
{
    int a = BUTTONS_CARDS + 8 * U_FOOTMAN + 4;
    a = *(int*)a;
    for (int i = 0; i < 200; i++)
    {
        buttons_warrior[i] = *(byte*)(a + i);
    }
    buttons_warrior[BUTTON_SIZE * 4 + 2] = '\x78';
    buttons_warrior[BUTTON_SIZE * 5 + 2] = '\x79';
    buttons_warrior[BUTTON_SIZE * 6 + 2] = '\x87';
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_GRUNT), (DWORD)10);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_GRUNT + 4), (DWORD)buttons_warrior);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_LOTHAR), (DWORD)10);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_LOTHAR + 4), (DWORD)buttons_warrior);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_DRAGON), (DWORD)10);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_DRAGON + 4), (DWORD)buttons_warrior);
}

void buttons_init_cart()
{
    int a = BUTTONS_CARDS + 4;
    a = *(int*)a;
    for (int i = 0; i < 20; i++)
    {
        buttons_cart[i] = *(byte*)(a + i);
    }
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_BALLISTA), (DWORD)1);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_BALLISTA + 4), (DWORD)buttons_cart);
}

void buttons_init_cata()
{
    int a = BUTTONS_CARDS + 8 * U_CATAPULT + 4;
    cata_real_buttons = *(int*)a;
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_CATAPULT), 0);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_CATAPULT + 4), 0);
}

void buttons_init_no_buttons()
{
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_HTANKER), 0);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_HTANKER + 4), 0);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_SUBMARINE), 0);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_SUBMARINE + 4), 0);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_HTRANSPORT), 0);
    patch_setdword((DWORD*)(BUTTONS_CARDS + 8 * U_HTRANSPORT + 4), 0);
}

int* unit_create_place(int x, int y, int id, byte owner)
{
    int* p = (int*)(UNIT_SIZE_TABLE + 4 * id);//unit sizes table
    return ((int* (*)(int, int, int, byte, int*))F_UNIT_CREATE)(x, y, id, owner, p);
}

void magic_blood(int* u)
{
    ((void (*)(int*))0x00442CF0)(u);//orig blood
    byte caster = *((byte*)((uintptr_t)u + S_ID));
    if (caster == U_MAGE)
    {
        int* t = (int*)*((int*)((uintptr_t)u + S_ORDER_UNIT_POINTER));
        WORD b = *((WORD*)((uintptr_t)t + S_BLOOD));
        if (b != 0)
        {
            byte id = *((byte*)((uintptr_t)t + S_ID));
            if ((id != U_PEASANT) && (id != U_PEON))
            {
                if ((id != U_PALADIN))
                {
                    byte o = *((byte*)((uintptr_t)t + S_OWNER));
                    if (o != PLAYER14)
                    {
                        set_stat(t, o, S_LAST_HARVEST_X);
                        set_stat(t, 0, S_LAST_HARVEST_Y);
                        byte pf = *((byte*)((uintptr_t)t + S_PEON_FLAGS));
                        pf &= ~PEON_SAVED_LOCATION;
                        set_stat(t, pf, S_PEON_FLAGS);
                        give_order(t, 0, 0, ORDER_STOP);
                        give(t, PLAYER14);
                        byte f2 = *((byte*)((uintptr_t)t + S_FLAGS2));
                        if (f2 & 0x1)
                        {
                            byte pf = *((byte*)((uintptr_t)t + S_PEON_FLAGS));
                            pf |= 0x1;
                            set_stat(t, pf, S_PEON_FLAGS);
                            f2 &= ~0x1;//uf_rescue
                            set_stat(t, f2, S_FLAGS2);
                        }
                    }
                }
            }
            else
            {
                byte pf = *((byte*)((uintptr_t)t + S_PEON_FLAGS));
                if ((pf & PEON_LOADED) == 0)
                {
                    byte x = *((byte*)((uintptr_t)t + S_X));
                    byte y = *((byte*)((uintptr_t)t + S_Y));
                    give_order(t, x, y, ORDER_HARVEST);
                }
            }
        }
    }
    set_stat(u, ORDER_STOP, S_NEXT_ORDER);
}

void magic_runes(int* u)
{
    byte caster = *((byte*)((uintptr_t)u + S_ID));
    if (caster == U_MAGE)
    {
        byte m = *((byte*)((uintptr_t)u + S_MANA));
        if (m >= 200)
        {
            byte x = *((byte*)((uintptr_t)u + S_ORDER_X));
            byte y = *((byte*)((uintptr_t)u + S_ORDER_Y));
            byte o = *((byte*)((uintptr_t)u + S_OWNER));
            int* cr = unit_create_place(x + 1, y, U_UTER, o);
            if (cr != NULL)
            {
                m -= 200;
                set_stat(u, m, S_MANA);
                ((void (*)(int*, byte))F_SPELL_SOUND_UNIT)(u, SS_EYE);
            }
        } 
    }
    if (caster == U_PALADIN)
    {
        byte x = *((byte*)((uintptr_t)u + S_X));
        byte y = *((byte*)((uintptr_t)u + S_Y));
        bullet_create8_around_unit(u, B_CAT_HIT);
        set_region(x - 2, y - 2, x + 2, y + 2);
        find_all_alive_units(ANY_MEN);
        sort_in_region();
        sort_self(u);
        sort_stat(S_SHIELD, 0, CMP_EQ);
        int d = 10 + (((int (*)())F_NET_RANDOM)() % 10);
        damag_all(d % 256, 0);
        set_stat_all(S_HASTE, -300);
    }
    set_stat(u, ORDER_STOP, S_NEXT_ORDER);
}

void magic_actions_init()
{
    void (*r1) (int*) = magic_blood;
    void (*r2) (int*) = magic_runes;
    char buf[] = "\x0\x0\x0\x0";
    patch_setdword((DWORD*)buf, (DWORD)r1);
    PATCH_SET((char*)(GF_SPELLS_FUNCTIONS + 4 * ORDER_SPELL_BLOODLUST), buf);
    patch_setdword((DWORD*)buf, (DWORD)r2);
    PATCH_SET((char*)(GF_SPELLS_FUNCTIONS + 4 * ORDER_SPELL_RUNES), buf);
}

void buttons_init_all()
{
    buttons_init_build();
    buttons_init_abuild();
    buttons_init_barrak();
    buttons_init_lumber();
    buttons_init_farm();
    buttons_init_tower();
    buttons_init_smith();
    buttons_init_stable();
    buttons_init_th();
    buttons_init_akadem();
    buttons_init_hero();
    buttons_init_mage();
    buttons_init_warrior();
    buttons_init_cart();
    //buttons_init_cata();
    buttons_init_no_buttons();

    magic_actions_init();

    char buf[] = "\x66\xB9\x1\x0\x90\x90\x90\x90";
    char buf2[] = "\x90\x90";
    char buf3[] = "\x0";
    PATCH_SET((char*)TEXT_PRODUCTION_LUMBER_ADD, buf);//lumber add
    PATCH_SET((char*)TEXT_PRODUCTION_LUMBER_AMOUNT, buf3);//lumber amount
    PATCH_SET((char*)TEXT_PRODUCTION_GOLD_ADD, buf2);//gold add
    PATCH_SET((char*)TEXT_PRODUCTION_GOLD_AMOUNT, buf3);//gold amount

    *(byte*)(GLOBAL_MOVEMENT_TERRAIN_FLAGS + 2 * MOV_SHORE) = 0x8C;//water elemental move type
}

void disable_minimap_units(bool d)
{
    if (d)
    {
        char buf[] = "\xEB\x34";
        PATCH_SET((char*)0x0042DD67, buf);//buildings
        char buf2[] = "\xEB";
        PATCH_SET((char*)0x0042DD7E, buf2);//units
    }
    else
    {
        char buf[] = "\x75\x17";
        PATCH_SET((char*)0x0042DD67, buf);//buildings
        char buf2[] = "\x75";
        PATCH_SET((char*)0x0042DD7E, buf2);//units
    }
}

void enemy_all_p14()
{
    for (int i = 0; i < 16; i++)
    {
        if (i != PLAYER14)ally(PLAYER14, i, 0);
        if (i == P_NEUTRAL)ally(PLAYER14, i, 1);
    }
    if (*(byte*)LEVEL_OBJ == LVL_HUMAN1)ally(PLAYER14, P_BLUE, 1);
}

void global_trigs()
{
    if (first_step)
    {
        enemy_all_p14();
        buttons_change_hero();
        ((void (*)())F_STATUS_REDRAW)();//status redraw
    }

    if (get_upgrade(SHIP_DMG, *(byte*)LOCAL_PLAYER) != 0)
    {
        find_all_alive_units(U_PEON);
        sort_stat(S_OWNER, *(byte*)LOCAL_PLAYER, CMP_EQ);
        heal_all(1, 0);
    }

    find_all_alive_units(U_UTER);
    damag_all(1, 0);

    if (book_of_twilight_check() == 1)
    {
        DWORD gf = *(DWORD*)(UNIT_GLOBAL_FLAGS + 4 * U_DANATH);
        gf |= IS_CASTER;
        *(DWORD*)(UNIT_GLOBAL_FLAGS + 4 * U_DANATH) = gf;
    }
    else
    {
        DWORD gf = *(DWORD*)(UNIT_GLOBAL_FLAGS + 4 * U_DANATH);
        gf &= ~IS_CASTER;
        *(DWORD*)(UNIT_GLOBAL_FLAGS + 4 * U_DANATH) = gf;
    }

    byte lvl = *(byte*)LEVEL_OBJ;
    if ((lvl >= LVL_HUMAN9) && (lvl <= LVL_HUMAN11))disable_minimap_units(true);
    else disable_minimap_units(false);
}

int* find_next_codo(byte c)
{
    int* u = NULL;
    if (c == PLAYER10)
    {
        find_all_alive_units(U_BALLISTA);
        sort_stat(S_COLOR, PLAYER9, CMP_EQ);
        u = unit[0];
        if (units == 0)u = NULL;
    }
    if (c == PLAYER11)
    {
        find_all_alive_units(U_BALLISTA);
        sort_stat(S_COLOR, PLAYER10, CMP_EQ);
        u = unit[0];
        if (units == 0)
        {
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER9, CMP_EQ);
            u = unit[0];
            if (units == 0)u = NULL;
        }
    }
    if (c == PLAYER12)
    {
        find_all_alive_units(U_BALLISTA);
        sort_stat(S_COLOR, PLAYER11, CMP_EQ);
        u = unit[0];
        if (units == 0)
        {
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER10, CMP_EQ);
            u = unit[0];
            if (units == 0)
            {
                find_all_alive_units(U_BALLISTA);
                sort_stat(S_COLOR, PLAYER9, CMP_EQ);
                u = unit[0];
                if (units == 0)u = NULL;
            }
        }
    }
    if (c == PLAYER13)
    {
        find_all_alive_units(U_BALLISTA);
        sort_stat(S_COLOR, PLAYER12, CMP_EQ);
        u = unit[0];
        if (units == 0)
        {
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER11, CMP_EQ);
            u = unit[0];
            if (units == 0)
            {
                find_all_alive_units(U_BALLISTA);
                sort_stat(S_COLOR, PLAYER10, CMP_EQ);
                u = unit[0];
                if (units == 0)
                {
                    find_all_alive_units(U_BALLISTA);
                    sort_stat(S_COLOR, PLAYER9, CMP_EQ);
                    u = unit[0];
                    if (units == 0)u = NULL;
                }
            }
        }
    }
    return u;
}

byte dist_codo(int* u1, int* u2)
{
    byte x1 = *((byte*)((uintptr_t)u1 + S_X));
    byte y1 = *((byte*)((uintptr_t)u1 + S_Y));
    byte x2 = *((byte*)((uintptr_t)u2 + S_X));
    byte y2 = *((byte*)((uintptr_t)u2 + S_Y));
    byte dx = abs(x2 - x1);
    byte dy = abs(y2 - y1);
    return (dx > dy ? dx : dy);
}

int* find_first_codo()
{
    int* u = NULL;

    find_all_alive_units(U_BALLISTA);
    sort_stat(S_COLOR, PLAYER9, CMP_EQ);
    if (units != 0)u = unit[0];
    else
    {
        find_all_alive_units(U_BALLISTA);
        sort_stat(S_COLOR, PLAYER10, CMP_EQ);
        if (units != 0)u = unit[0];
        else
        {
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER11, CMP_EQ);
            if (units != 0)u = unit[0];
            //else
        }
    }

    return u;
}

int num_carts = 0;
int num_dwarf = 0;
int num_key = 0;

PROC g_proc_0042479E;
void peon_into_goldmine(int* u)
{
    ((void (*)(int*))g_proc_0042479E)(u);//original
    byte o = *((byte*)((uintptr_t)u + S_OWNER));
    if (*(byte*)(CONTROLER_TYPE + o) == C_COMP)
    {
        int* g = (int*)*((int*)((uintptr_t)u + S_ORDER_UNIT_POINTER));
        WORD r = *((WORD*)((uintptr_t)g + S_RESOURCES));
        if (r < 0xFFFF)r++;
        set_stat(g, r, S_RESOURCES);
    }
}

void show_message_from_tbl(int time, void* tbl, int str_id)
{
    char* msg = ((char* (*)(void*, int))F_GET_LINE_FROM_TBL)(tbl, str_id);
    show_message(time, msg);
}

//write your custom victory functions here
//-------------------------------------------------------------------------------
void v_human1(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_HADGAR;
        ut[0] = U_HADGAR;
        ua[1] = U_CHOGAL;
        ut[1] = U_CHOGAL;
        repair_cat(true);
	}
    else
    {
        //your custom victory conditions
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_HADGAR;
        ut[0] = U_HADGAR;
        ua[1] = U_CHOGAL;
        ut[1] = U_CHOGAL;
        repair_cat(true);

        if (*(byte*)GB_HORSES == 1)
        {
            find_all_alive_units(U_BALLISTA);
            if (units < 3)
            {
                *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(CONTROLER_TYPE + P_BLUE) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_BLUE) = C_NOBODY;
                *(byte*)(CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                lose(true);
            }
            find_all_alive_units(U_DANATH);
            if (units == 0)
            {
                *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(CONTROLER_TYPE + P_BLUE) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_BLUE) = C_NOBODY;
                *(byte*)(CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                lose(true);
            }

            if (*(byte*)(GB_HORSES + 4) == 10)
            {
                show_message_from_tbl(20, tbl_dialog, 19);
                sound_play_dialog(19);
                find_all_alive_units(U_DANATH);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 4) = 11;
            }
            if (*(byte*)(GB_HORSES + 4) == 9)
            {
                if (*(byte*)(GB_HORSES + 11) == 0)*(byte*)(GB_HORSES + 4) = 10;
                else *(byte*)(GB_HORSES + 11) = *(byte*)(GB_HORSES + 11) - 1;
            }
            if (*(byte*)(GB_HORSES + 4) == 8)
            {
                show_message_from_tbl(20, tbl_dialog, 18);
                find_all_alive_units(U_CHOGAL);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 4) = 9;
                *(byte*)(GB_HORSES + 11) = 13;
            }
            if (*(byte*)(GB_HORSES + 4) == 7)
            {
                if (*(byte*)(GB_HORSES + 11) == 0)*(byte*)(GB_HORSES + 4) = 8;
                else *(byte*)(GB_HORSES + 11) = *(byte*)(GB_HORSES + 11) - 1;
            }
            if (*(byte*)(GB_HORSES + 4) == 6)
            {
                show_message_from_tbl(20, tbl_dialog, 16);
                show_message_from_tbl(20, tbl_dialog, 17);
                find_all_alive_units(U_CHOGAL);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 4) = 7;
                *(byte*)(GB_HORSES + 11) = 6;
            }
            if (*(byte*)(GB_HORSES + 4) == 5)
            {
                if (*(byte*)(GB_HORSES + 11) == 0)*(byte*)(GB_HORSES + 4) = 6;
                else *(byte*)(GB_HORSES + 11) = *(byte*)(GB_HORSES + 11) - 1;
            }
            if (*(byte*)(GB_HORSES + 4) == 4)
            {
                show_message_from_tbl(20, tbl_dialog, 14);
                show_message_from_tbl(20, tbl_dialog, 15);
                sound_play_dialog(26);
                find_all_alive_units(U_CHOGAL);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 4) = 5;
                *(byte*)(GB_HORSES + 11) = 9;
            }
            if (*(byte*)(GB_HORSES + 4) == 3)
            {
                if (*(byte*)(GB_HORSES + 11) == 0)*(byte*)(GB_HORSES + 4) = 4;
                else *(byte*)(GB_HORSES + 11) = *(byte*)(GB_HORSES + 11) - 1;
            }
            if (*(byte*)(GB_HORSES + 4) == 2)
            {
                show_message_from_tbl(20, tbl_dialog, 12);
                show_message_from_tbl(20, tbl_dialog, 13);
                sound_play_dialog(25);
                find_all_alive_units(U_CHOGAL);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 4) = 3;
                *(byte*)(GB_HORSES + 11) = 9;
            }
            if (*(byte*)(GB_HORSES + 4) == 1)
            {
                if (*(byte*)(GB_HORSES + 11) == 0)*(byte*)(GB_HORSES + 4) = 2;
                else *(byte*)(GB_HORSES + 11) = *(byte*)(GB_HORSES + 11) - 1;
            }
            if (*(byte*)(GB_HORSES + 4) == 0)
            {
                set_region(35, 60, 40, 70);
                find_all_alive_units(ANY_MEN);
                sort_in_region();
                sort_stat(S_OWNER, P_VIOLET, CMP_EQ);
                if (units != 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 10);
                    show_message_from_tbl(20, tbl_dialog, 11);
                    sound_play_dialog(24);
                    find_all_alive_units(U_CHOGAL);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);
                    *(byte*)(GB_HORSES + 4) = 1;
                    *(byte*)(GB_HORSES + 11) = 9;
                }
            }

            if (*(byte*)(GB_HORSES + 2) == 12)
            {
                find_all_alive_units(U_DANATH);
                give_all(P_VIOLET);
                *(byte*)(GB_HORSES + 2) = 13;
            }
            if (*(byte*)(GB_HORSES + 2) == 11)
            {
                if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 2) = 12;
                else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
            }
            if (*(byte*)(GB_HORSES + 2) == 10)
            {
                show_message_from_tbl(20, tbl_dialog, 9);
                sound_play_dialog(0);
                find_all_alive_units(U_DANATH);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 2) = 11;
                *(byte*)(GB_HORSES + 10) = 5;
            }
            if (*(byte*)(GB_HORSES + 2) == 9)
            {
                if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 2) = 10;
                else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
            }
            if (*(byte*)(GB_HORSES + 2) == 8)
            {
                *(byte*)(GB_HORSES + 3) = 1;
                show_message_from_tbl(20, tbl_dialog, 8);
                *(byte*)(GB_HORSES + 2) = 9;
                *(byte*)(GB_HORSES + 10) = 3;
            }
            if (*(byte*)(GB_HORSES + 2) == 7)
            {
                if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 2) = 8;
                else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
            }
            if (*(byte*)(GB_HORSES + 2) == 6)
            {
                show_message_from_tbl(20, tbl_dialog, 6);
                show_message_from_tbl(20, tbl_dialog, 7);
                sound_play_dialog(23);
                find_all_alive_units(U_HADGAR);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 2) = 7;
                *(byte*)(GB_HORSES + 10) = 7;
            }
            if (*(byte*)(GB_HORSES + 2) == 5)
            {
                if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 2) = 6;
                else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
            }
            if (*(byte*)(GB_HORSES + 2) == 4)
            {
                show_message_from_tbl(20, tbl_dialog, 5);
                sound_play_dialog(22);
                find_all_alive_units(U_HADGAR);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 2) = 5;
                *(byte*)(GB_HORSES + 10) = 7;
            }
            if (*(byte*)(GB_HORSES + 2) == 3)
            {
                if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 2) = 4;
                else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
            }
            if (*(byte*)(GB_HORSES + 2) == 2)
            {
                show_message_from_tbl(20, tbl_dialog, 3);
                show_message_from_tbl(20, tbl_dialog, 4);
                sound_play_dialog(21);
                find_all_alive_units(U_HADGAR);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 2) = 3;
                *(byte*)(GB_HORSES + 10) = 7;
            }
            if (*(byte*)(GB_HORSES + 2) == 1)
            {
                if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 2) = 2;
                else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
            }
            if (*(byte*)(GB_HORSES + 2) == 0)
            {
                set_region(82, 84, 87, 90);
                find_all_alive_units(U_DANATH);
                sort_in_region();
                if (units != 0)
                {
                    order_all(0, 0, ORDER_STOP);
                    give_all(P_BLUE);
                    show_message_from_tbl(20, tbl_dialog, 1);
                    show_message_from_tbl(20, tbl_dialog, 2);
                    sound_play_dialog(20);
                    find_all_alive_units(U_HADGAR);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);
                    *(byte*)(GB_HORSES + 2) = 1;
                    *(byte*)(GB_HORSES + 10) = 9;
                }
            }

            int ord = *(int*)(ORDER_FUNCTIONS + 4 * ORDER_FOLLOW);
            int ords = *(int*)(ORDER_FUNCTIONS + 4 * ORDER_STOP);
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER13, CMP_EQ);
            if (units != 0)
            {
                int* cr = unit[0];
                int* tr = find_next_codo(PLAYER13);
                if (tr != NULL)
                {
                    if (dist_codo(cr, tr) > 2)
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, tr, ord);
                    else
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, NULL, ords);
                }
            }
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER12, CMP_EQ);
            if (units != 0)
            {
                int* cr = unit[0];
                int* tr = find_next_codo(PLAYER12);
                if (tr != NULL)
                {
                    if (dist_codo(cr, tr) > 2)
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, tr, ord);
                    else
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, NULL, ords);
                }
            }
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER11, CMP_EQ);
            if (units != 0)
            {
                int* cr = unit[0];
                int* tr = find_next_codo(PLAYER11);
                if (tr != NULL)
                {
                    if (dist_codo(cr, tr) > 2)
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, tr, ord);
                    else
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, NULL, ords);
                }
            }
            find_all_alive_units(U_BALLISTA);
            sort_stat(S_COLOR, PLAYER10, CMP_EQ);
            if (units != 0)
            {
                int* cr = unit[0];
                int* tr = find_next_codo(PLAYER10);
                if (tr != NULL)
                {
                    if (dist_codo(cr, tr) > 2)
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, tr, ord);
                    else
                        ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(cr, 0, 0, NULL, ords);
                }
            }
            int* fr = find_first_codo();
            if (fr)
            {
                if (*(byte*)(GB_HORSES + 1) >= 27)
                {
                    find_all_alive_units(ANY_MEN);
                    sort_stat(S_OWNER, P_YELLOW, CMP_EQ);
                    order_all(23, 7, ORDER_ATTACK_AREA);
                    set_stat_all(S_AI_DEST_X, 23);
                    set_stat_all(S_AI_DEST_Y, 7);
                }
                if (*(byte*)(GB_HORSES + 1) == 29)
                {
                    find_all_alive_units(U_BALLISTA);
                    num_carts = units;
                    find_all_alive_units(U_LOTHAR);
                    sort_stat(S_OWNER, *(byte*)LOCAL_PLAYER, CMP_EQ);
                    num_dwarf = units;
                    if (*(byte*)(GB_HORSES + 3) == 1)num_key = 1;

                    *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
                    *(byte*)(CONTROLER_TYPE + P_BLUE) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + P_BLUE) = C_NOBODY;
                    *(byte*)(CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                    win(true);
                }
                if (*(byte*)(GB_HORSES + 1) == 28)
                {
                    *(byte*)(GB_HORSES + 1) = 29;
                }
                if (*(byte*)(GB_HORSES + 1) == 27)
                {
                    byte x = 27;
                    byte y = 3;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    sort_stat(S_ANIMATION, ANIM_STOP, CMP_EQ);
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 28;
                }
                if (*(byte*)(GB_HORSES + 1) == 26)
                {
                    byte x = 16;
                    byte y = 17;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 27;
                }
                if (*(byte*)(GB_HORSES + 1) == 25)
                {
                    byte x = 26;
                    byte y = 20;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 26;
                }
                if (*(byte*)(GB_HORSES + 1) == 24)
                {
                    byte x = 52;
                    byte y = 24;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 25;
                }
                if (*(byte*)(GB_HORSES + 1) == 23)
                {
                    byte x = 66;
                    byte y = 19;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 24;
                }
                if (*(byte*)(GB_HORSES + 1) == 22)
                {
                    byte x = 75;
                    byte y = 23;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 23;
                }
                if (*(byte*)(GB_HORSES + 1) == 21)
                {
                    byte x = 111;
                    byte y = 15;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 22;
                }
                if (*(byte*)(GB_HORSES + 1) == 20)
                {
                    byte x = 113;
                    byte y = 32;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 21;
                }
                if (*(byte*)(GB_HORSES + 1) == 19)
                {
                    byte x = 95;
                    byte y = 53;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 20;
                }
                if (*(byte*)(GB_HORSES + 1) == 18)
                {
                    *(byte*)(GB_HORSES + 1) = 19;
                }
                if (*(byte*)(GB_HORSES + 1) == 17)
                {
                    *(byte*)(GB_HORSES + 1) = 18;
                }
                if (*(byte*)(GB_HORSES + 1) == 16)
                {
                    *(byte*)(GB_HORSES + 1) = 17;
                }
                if (*(byte*)(GB_HORSES + 1) == 15)
                {
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 16;
                    int* gr = unit_create_place(1, 93, U_GRIFON, P_YELLOW);
                    give_order(gr, 75, 68, ORDER_ATTACK_AREA);
                    set_stat(gr, 75, S_AI_DEST_X);
                    set_stat(gr, 68, S_AI_DEST_Y);
                }
                if (*(byte*)(GB_HORSES + 1) == 14)
                {
                    byte x = 75;
                    byte y = 68;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    sort_stat(S_ANIMATION, ANIM_STOP, CMP_EQ);
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 15;
                }
                if (*(byte*)(GB_HORSES + 1) == 13)
                {
                    byte x = 61;
                    byte y = 74;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 14;
                }
                if (*(byte*)(GB_HORSES + 1) == 12)
                {
                    byte x = 41;
                    byte y = 76;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 13;
                }
                if (*(byte*)(GB_HORSES + 1) == 11)
                {
                    byte x = 34;
                    byte y = 88;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 12;
                }
                if (*(byte*)(GB_HORSES + 1) == 10)
                {
                    byte x = 19;
                    byte y = 97;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 11;
                }
                if (*(byte*)(GB_HORSES + 1) == 9)
                {
                    *(byte*)(GB_HORSES + 1) = 10;
                }
                if (*(byte*)(GB_HORSES + 1) == 8)
                {
                    *(byte*)(GB_HORSES + 1) = 9;
                }
                if (*(byte*)(GB_HORSES + 1) == 7)
                {
                    *(byte*)(GB_HORSES + 1) = 8;
                }
                if (*(byte*)(GB_HORSES + 1) == 6)
                {
                    *(byte*)(GB_HORSES + 1) = 7;
                }
                if (*(byte*)(GB_HORSES + 1) == 5)
                {
                    byte x = 13;
                    byte y = 111;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    sort_stat(S_ANIMATION, ANIM_STOP, CMP_EQ);
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 6;
                }
                if (*(byte*)(GB_HORSES + 1) == 4)
                {
                    byte x = 26;
                    byte y = 115;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 5;
                }
                if (*(byte*)(GB_HORSES + 1) == 3)
                {
                    byte x = 48;
                    byte y = 108;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 4;
                }
                if (*(byte*)(GB_HORSES + 1) == 2)
                {
                    byte x = 65;
                    byte y = 117;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 3;
                }
                if (*(byte*)(GB_HORSES + 1) == 1)
                {
                    byte x = 77;
                    byte y = 108;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 2;
                }
                if (*(byte*)(GB_HORSES + 1) == 0)
                {
                    byte x = 98;
                    byte y = 108;
                    give_order(fr, x, y, ORDER_MOVE);
                    set_region(x - 1, y - 1, x + 1, y + 1);
                    find_all_alive_units(U_BALLISTA);
                    sort_in_region();
                    if (units != 0)*(byte*)(GB_HORSES + 1) = 1;
                }
            }
        }
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)(CONTROLER_TYPE + P_BLUE) = C_PLAYER;
            ally(P_VIOLET, P_RED, 1);
            ally(P_VIOLET, P_BLUE, 1);
            ally(P_RED, P_BLUE, 1);
            viz(P_VIOLET, P_BLUE, 1);
            ally_one_sided(P_BLUE, P_YELLOW, 0);
            ally_one_sided(P_YELLOW, P_BLUE, 1);
            ally_one_sided(P_BLUE, P_ORANGE, 0);
            ally_one_sided(P_ORANGE, P_BLUE, 1);
            int* u;

            u = unit_create_place(108, 118, U_BALLISTA, P_BLUE);
            set_stat(u, PLAYER9, S_COLOR);
            u = unit_create_place(110, 119, U_BALLISTA, P_BLUE);
            set_stat(u, PLAYER10, S_COLOR);
            u = unit_create_place(112, 120, U_BALLISTA, P_BLUE);
            set_stat(u, PLAYER11, S_COLOR);
            u = unit_create_place(113, 122, U_BALLISTA, P_BLUE);
            set_stat(u, PLAYER12, S_COLOR);
            u = unit_create_place(113, 124, U_BALLISTA, P_BLUE);
            set_stat(u, PLAYER13, S_COLOR);
            
            u = unit_create_place(127, 125, U_PIGFARM, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(124, 124, U_PIGFARM, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(120, 122, U_OBARRACK, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(83, 84, U_PIGFARM, P_NEUTRAL);
            set_stat(u, P_RED, S_COLOR);
            u = unit_create_place(17, 2, U_PIGFARM, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(16, 0, U_PIGFARM, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(19, 4, U_OLUMBER, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(42, 64, U_ALTAR, P_RED);
            set_stat(u, P_BLUE, S_COLOR);
            u = unit_create_place(59, 52, U_DRAGON, P_GREEN);
            set_stat(u, 1, S_FLAGS2);
            u = unit_create_place(27, 7, U_PEON, P_GREEN);
            set_stat(u, 1, S_FLAGS2);
            unit_create(119, 124, U_PEON, P_VIOLET, 1);
            unit_convert(P_VIOLET, U_GROM, U_GRUNT, 1);
            unit_convert(P_GREEN, U_GROM, U_GRUNT, 1);

            *(byte*)GB_HORSES = 1;
        }
        find_all_alive_units(U_RUNESTONE);
        sort_stat(S_OWNER, P_BLUE, CMP_EQ);
        set_stat_all(S_FLAGS2, 0);
        give_all(P_RED);
        find_all_alive_units(U_RUNESTONE);
        sort_stat(S_OWNER, P_VIOLET, CMP_EQ);
        give_all(P_RED);
        set_stat_all(S_FLAGS2, 0);
        find_all_alive_units(U_RUNESTONE);
        sort_stat(S_OWNER, P_RED, CMP_EQ);
        if (units == 1)
            set_upgrade(SWORDS, P_VIOLET, 1);
        if (units == 2)
        {
            set_upgrade(SWORDS, P_VIOLET, 1);
            set_upgrade(ARMOR, P_VIOLET, 1);
        }
        if (units == 3)
        {
            set_upgrade(SWORDS, P_VIOLET, 2);
            set_upgrade(ARMOR, P_VIOLET, 2);
        }
	}
}

void cutscene_key_door()
{
    if (*(byte*)(GB_HORSES + 3) == 0)
    {
        if (*(byte*)(GB_HORSES + 6) == 2)
        {
            sound_play_dialog(2);
            *(byte*)(GB_HORSES + 6) = 3;
        }
        if (*(byte*)(GB_HORSES + 6) == 1)
        {
            if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 6) = 2;
            else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
        }
        if (*(byte*)(GB_HORSES + 6) == 0)
        {
            set_region(5, 27, 6, 28);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 20);
                show_message_from_tbl(20, tbl_dialog, 21);
                sound_play_dialog(1);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 6) = 1;
                *(byte*)(GB_HORSES + 10) = 7;
            }
        }
    }
    else
    {
        if (*(byte*)(GB_HORSES + 6) == 12)
        {
            show_message_from_tbl(20, tbl_dialog, 31);
            *(byte*)(GB_HORSES + 15) = 1;
            num_book = 1;
            ((void (*)())F_STATUS_REDRAW)();
            *(byte*)(GB_HORSES + 6) = 13;
        }
        if (*(byte*)(GB_HORSES + 6) == 11)
        {
            if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 6) = 12;
            else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
        }
        if (*(byte*)(GB_HORSES + 6) == 10)
        {
            show_message_from_tbl(20, tbl_dialog, 30);
            sound_play_dialog(5);
            find_all_alive_units(U_DANATH);
            set_stat_all(S_FLASH_COUNTER, 80);
            set_stat_all(S_FLASH_COLOR, 253);
            *(byte*)(GB_HORSES + 6) = 11;
            *(byte*)(GB_HORSES + 10) = 7;
        }
        if (*(byte*)(GB_HORSES + 6) == 9)
        {
            find_all_alive_units(U_DANATH);
            byte xx = 5, yy = 26;
            if (units != 0)
            {
                xx = *((byte*)((uintptr_t)unit[0] + S_X));
                yy = *((byte*)((uintptr_t)unit[0] + S_Y));
            }
            find_all_alive_units(U_TERON);
            sort_stat(S_MANA, 25, CMP_BIGGER_EQ);
            order_all(xx, yy, ORDER_SPELL_ROT);
            find_all_alive_units(U_TERON);
            sort_stat(S_MANA, 25, CMP_SMALLER);
            if (units != 0)
            {
                int* u = unit[0];
                find_all_alive_units(U_DANATH);
                if (units != 0)
                {
                    int ord = *(int*)(ORDER_FUNCTIONS + 4 * ORDER_ATTACK_TARGET);
                    ((void (*)(int*, int, int, int*, int))F_GIVE_ORDER)(u, 0, 0, unit[0], ord);
                }
            }
            find_all_alive_units(U_TERON);
            if (units == 0)
            {
                find_all_alive_units(U_RUNESTONE);
                kill_all();
                *(byte*)(GB_HORSES + 6) = 10;
            }
        }
        if (*(byte*)(GB_HORSES + 6) == 8)
        {
            show_message_from_tbl(20, tbl_dialog, 28);
            show_message_from_tbl(20, tbl_dialog, 29);
            sound_play_dialog(29);
            find_all_alive_units(U_DANATH);
            byte xx = 5, yy = 26;
            if (units != 0)
            {
                xx = *((byte*)((uintptr_t)unit[0] + S_X));
                yy = *((byte*)((uintptr_t)unit[0] + S_Y));
            }
            find_all_alive_units(U_TERON);
            give_all(P_WHITE);
            set_stat_all(S_FLASH_COUNTER, 80);
            set_stat_all(S_FLASH_COLOR, 253); 
            set_stat_all(S_MANA, 150);
            set_stat_all(S_HASTE, 1000);
            order_all(xx, yy, ORDER_SPELL_DRAINLIFE);
            ua[1] = 255;
            ut[1] = 255;
            *(byte*)(GB_HORSES + 6) = 9;
        }
        if (*(byte*)(GB_HORSES + 6) == 7)
        {
            if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 6) = 8;
            else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
        }
        if (*(byte*)(GB_HORSES + 6) == 6)
        {
            show_message_from_tbl(20, tbl_dialog, 26);
            show_message_from_tbl(20, tbl_dialog, 27);
            sound_play_dialog(28);
            set_region(2, 21, 2, 21);
            find_all_alive_units(ANY_MEN);
            sort_in_region();
            kill_all();
            find_all_alive_units(U_DANATH);
            move_all(2, 21);
            give_all(P_VIOLET);
            set_region(4, 25, 5, 26);
            find_all_alive_units(ANY_MEN);
            sort_in_region();
            kill_all();
            unit_create(5, 25, U_RUNESTONE, P_NEUTRAL, 1);
            find_all_alive_units(U_TERON);
            set_stat_all(S_FLASH_COUNTER, 80);
            set_stat_all(S_FLASH_COLOR, 253);
            ua[1] = U_TERON;
            ut[1] = U_TERON;
            *(byte*)(GB_HORSES + 6) = 7;
            *(byte*)(GB_HORSES + 10) = 10;
        }
        if (*(byte*)(GB_HORSES + 6) == 5)
        {
            if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 6) = 6;
            else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
        }
        if (*(byte*)(GB_HORSES + 6) == 4)
        {
            show_message_from_tbl(20, tbl_dialog, 24);
            show_message_from_tbl(20, tbl_dialog, 25);
            sound_play_dialog(27);
            find_all_alive_units(U_TERON);
            set_stat_all(S_FLASH_COUNTER, 80);
            set_stat_all(S_FLASH_COLOR, 253);
            ua[1] = U_TERON;
            ut[1] = U_TERON;
            *(byte*)(GB_HORSES + 6) = 5;
            *(byte*)(GB_HORSES + 10) = 10;
        }
        if (*(byte*)(GB_HORSES + 6) == 3)
        {
            if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 6) = 4;
            else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
        }
        if (*(byte*)(GB_HORSES + 6) == 2)
        {
            show_message_from_tbl(20, tbl_dialog, 23);
            sound_play_dialog(4);
            find_all_alive_units(U_RUNESTONE);
            kill_all();
            find_all_alive_units(U_DANATH);
            set_stat_all(S_FLASH_COUNTER, 80);
            set_stat_all(S_FLASH_COLOR, 253);
            order_all(2, 22, ORDER_MOVE);
            ua[1] = U_TERON;
            ut[1] = U_TERON;
            int* u = unit_create_place(3, 19, U_TERON, P_RED);
            set_stat(u, P_ORANGE, S_COLOR);
            *(byte*)(GB_HORSES + 6) = 3;
            *(byte*)(GB_HORSES + 10) = 7;
        }
        if (*(byte*)(GB_HORSES + 6) == 1)
        {
            if (*(byte*)(GB_HORSES + 10) == 0)*(byte*)(GB_HORSES + 6) = 2;
            else *(byte*)(GB_HORSES + 10) = *(byte*)(GB_HORSES + 10) - 1;
            if (*(byte*)(GB_HORSES + 10) == 7)sound_play_dialog(3);
        }
        if (*(byte*)(GB_HORSES + 6) == 0)
        {
            set_region(5, 27, 6, 28);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                give_all(P_RED);
                show_message_from_tbl(20, tbl_dialog, 20);
                show_message_from_tbl(20, tbl_dialog, 22);
                sound_play_dialog(1);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
                *(byte*)(GB_HORSES + 6) = 1;
                *(byte*)(GB_HORSES + 10) = 14;
            }
        }
    }
}

void save_book()
{
    if (*(byte*)(GB_HORSES + 15) != 0)
        num_book = *(byte*)(GB_HORSES + 15);
}

void spawn_skeletons()
{
    if (*(byte*)(GB_HORSES + 5) == 0)
    {
        set_region(111, 94, 115, 98);
        find_all_alive_units(U_MINE);
        sort_in_region();
        sort_stat(S_RESOURCES, 100, CMP_SMALLER);
        if (units != 0)
        {
            *(byte*)(GB_HORSES + 5) = 1;
        }
    }
    if (*(byte*)(GB_HORSES + 5) == 1)
    {
        unit_create(112, 93, U_SKELETON, P_WHITE, 1);
        unit_create(114, 93, U_SKELETON, P_WHITE, 1);
        unit_create(116, 93, U_SKELETON, P_WHITE, 1);
        unit_create(110, 94, U_SKELETON, P_WHITE, 1);
        unit_create(117, 95, U_SKELETON, P_WHITE, 1);
        unit_create(109, 96, U_SKELETON, P_WHITE, 1);
        unit_create(111, 96, U_SKELETON, P_WHITE, 1);
        unit_create(118, 97, U_SKELETON, P_WHITE, 1);
        unit_create(111, 98, U_SKELETON, P_WHITE, 1);
        unit_create(117, 98, U_SKELETON, P_WHITE, 1);
        unit_create(115, 99, U_SKELETON, P_WHITE, 1);
        unit_create(113, 100, U_SKELETON, P_WHITE, 1);
        unit_create(117, 100, U_SKELETON, P_WHITE, 1);

        byte mxs = *(byte*)MAP_SIZE;//map size
        char* cel = (char*)*(int*)(MAP_CELLS_POINTER);//map cells
        char buft[] = "\x7\x1";
        byte xx, yy;
        xx = 111;
        yy = 93;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 113;
        yy = 93;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 115;
        yy = 93;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 109;
        yy = 94;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 116;
        yy = 95;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 108;
        yy = 96;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 110;
        yy = 96;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 117;
        yy = 97;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 110;
        yy = 98;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 116;
        yy = 98;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 114;
        yy = 99;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 112;
        yy = 100;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);
        xx = 116;
        yy = 100;
        PATCH_SET((char*)(cel + 2 * xx + 2 * yy * mxs), buft);

        byte cx = *(byte*)CAMERA_X;
        byte cy = *(byte*)CAMERA_Y;
        center_view(0, 0);
        center_view(cx, cy);

        *(byte*)(GB_HORSES + 5) = 2;
    }
}

void v_human2(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        ai_fix_plugin(true);
        saveload_fixed = true;
        trigger_time(40);
        manacost(HASTE, 30);
        repair_cat(true);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
	}
	else
	{
		//your custom victory conditions
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        repair_cat(true);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
        ally(P_VIOLET, P_RED, 1);
        viz(P_VIOLET, P_RED, 1);
        *(byte*)(CONTROLER_TYPE + P_RED) = C_PLAYER;

        if (*(byte*)(GB_HORSES + 1) != 0)
            num_carts = *(byte*)(GB_HORSES + 1);
        if (*(byte*)(GB_HORSES + 2) != 0)
            num_dwarf = *(byte*)(GB_HORSES + 2);
        if (*(byte*)(GB_HORSES + 3) != 0)
            num_key = *(byte*)(GB_HORSES + 3);
        save_book();

        cutscene_key_door();

        if (*(byte*)GB_HORSES == 1)
        {
            if (get_val(STABLES, P_VIOLET) != 0)
            {
                if (get_val(FARM, P_VIOLET) >= 16)
                {
                    *(byte*)GB_HORSES = 2;
                    *(byte*)LEVEL_OBJ = LVL_HUMAN3;
                    show_message_from_tbl(20, tbl_dialog, 73);
                    show_message_from_tbl(20, tbl_dialog, 74);
                    viz_area_add(126, 41, 1 << P_VIOLET, 4);
                }
            }
        }
        if (*(byte*)GB_HORSES == 0)
        {
            unit_create(75, 104, U_DRAGON, P_VIOLET, 1);
            unit_create(92, 115, U_PIGFARM, P_VIOLET, 1);
            unit_create(93, 117, U_PIGFARM, P_VIOLET, 1);
            unit_create(93, 105, U_GREAT_HALL, P_VIOLET, 1);
            unit_create(94, 110, U_PEON, P_VIOLET, 1);
            unit_create(93, 112, U_LOTHAR, P_VIOLET, num_dwarf);
            int num_carts1 = num_carts;
            if (num_carts1 != 0)
            {
                unit_create(96, 119, U_BALLISTA, P_VIOLET, 1);
                num_carts1--;
            }
            if (num_carts1 != 0)
            {
                unit_create(96, 121, U_BALLISTA, P_VIOLET, 1);
                num_carts1--;
            }
            if (num_carts1 != 0)
            {
                unit_create(96, 123, U_BALLISTA, P_VIOLET, 1);
                num_carts1--;
            }
            if (num_carts1 != 0)
            {
                unit_create(97, 125, U_BALLISTA, P_VIOLET, 1);
                num_carts1--;
            }
            if (num_carts1 != 0)
            {
                unit_create(98, 127, U_BALLISTA, P_VIOLET, 1);
                num_carts1--;
            }
            find_all_alive_units(U_BALLISTA);
            order_all(96, 113, ORDER_MOVE);
            unit_convert(P_VIOLET, U_GROM, U_GRUNT, 1);
            *(byte*)(GB_HORSES + 1) = num_carts;
            *(byte*)(GB_HORSES + 2) = num_dwarf;
            *(byte*)(GB_HORSES + 3) = num_key;
            *(byte*)GB_HORSES = 1;
        }
        find_all_alive_units(U_BALLISTA);
        set_region(96, 113, 97, 114);
        sort_in_region();
        if (units != 0)
        {
            change_res(P_VIOLET, 0, 1, 1000);
            change_res(P_VIOLET, 1, 1, 500);
            int* u;
            u = unit_create_place(96, 113, U_PEON, P_VIOLET);
            set_stat(u, 1, S_PEON_FLAGS);
            give_order(u, 101, 108, ORDER_HARVEST);
            u = unit_create_place(96, 113, U_PEON, P_VIOLET);
            set_stat(u, 1, S_PEON_FLAGS);
            give_order(u, 102, 107, ORDER_HARVEST);
            u = unit_create_place(96, 113, U_PEON, P_VIOLET);
            give_order(u, 88, 119, ORDER_HARVEST);
            units = 1;
            remove_all();
        }
        spawn_skeletons();
        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
            lose(true);
        }
	}
}

void v_human3(bool rep_init)
{
    if (rep_init)
    {
        //your initialize
        ai_fix_plugin(true);
        saveload_fixed = true;
        trigger_time(40);
        manacost(HASTE, 30);
        repair_cat(true);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
    }
    else
    {
        //your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN2;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        repair_cat(true);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
        ally(P_VIOLET, P_RED, 1);
        viz(P_VIOLET, P_RED, 1);
        *(byte*)(CONTROLER_TYPE + P_RED) = C_PLAYER;

        if (*(byte*)(GB_HORSES + 1) != 0)
            num_carts = *(byte*)(GB_HORSES + 1);
        if (*(byte*)(GB_HORSES + 2) != 0)
            num_dwarf = *(byte*)(GB_HORSES + 2);
        if (*(byte*)(GB_HORSES + 3) != 0)
            num_key = *(byte*)(GB_HORSES + 3);
        save_book();

        cutscene_key_door();

        if (*(byte*)GB_HORSES == 2)
        {
            find_all_alive_units(U_DANATH);
            set_region(126, 41, 127, 42);
            sort_in_region();
            if (units != 0)
            {
                *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
                *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
                win(true);
            }
        }
        find_all_alive_units(U_BALLISTA);
        set_region(96, 113, 97, 114);
        sort_in_region();
        if (units != 0)
        {
            change_res(P_VIOLET, 0, 1, 1000);
            change_res(P_VIOLET, 1, 1, 500);
            int* u;
            u = unit_create_place(96, 113, U_PEON, P_VIOLET);
            set_stat(u, 1, S_PEON_FLAGS);
            give_order(u, 101, 108, ORDER_HARVEST);
            u = unit_create_place(96, 113, U_PEON, P_VIOLET);
            set_stat(u, 1, S_PEON_FLAGS);
            give_order(u, 102, 107, ORDER_HARVEST);
            u = unit_create_place(96, 113, U_PEON, P_VIOLET);
            give_order(u, 88, 119, ORDER_HARVEST);
            units = 1;
            remove_all();
        }
        spawn_skeletons();
        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_ORANGE) = C_NOBODY;
            lose(true);
        }
    }
}

void spawn_golems()
{
    for (int i = 0; i < 16; i++)
    {
        int* p = (int*)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int*)(*p);
            while (p)
            {
                bool f = ((*((byte*)((uintptr_t)p + S_ID)) == U_OCANONTOWER));
                if (f)
                {
                    
                    byte x = *((byte*)((uintptr_t)p + S_X));
                    byte y = *((byte*)((uintptr_t)p + S_Y));
                    set_region((int)x - 7, (int)y - 7, (int)x + 9, (int)y + 9);
                    find_all_alive_units(ANY_MEN);
                    sort_in_region();
                    sort_stat(S_OWNER, *(byte*)LOCAL_PLAYER, CMP_EQ);
                    if (units != 0)
                    {
                        find_all_alive_units(ANY_MEN);
                        sort_in_region();
                        sort_stat(S_ID, U_OGRE, CMP_EQ);
                        if (units < 5)
                        {
                            WORD t = *((WORD*)((uintptr_t)p + S_HASTE));
                            if (t == 0)
                            {
                                unit_create(x, y, U_OGRE, P_WHITE, 1);
                                set_stat(p, 1000, S_HASTE);
                            }
                        }
                    }
                }
                p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void v_human4(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
	}
	else
	{
		//your custom victory conditions
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;

        save_book();

        if (*(byte*)GB_HORSES == 1)
        {
            DWORD guld = *(DWORD*)(GOLD + 4 * P_VIOLET);
            if (guld >= 50000)
            {
                *(byte*)GB_HORSES = 2;
                *(byte*)LEVEL_OBJ = LVL_HUMAN5;
                show_message_from_tbl(20, tbl_dialog, 73);
                show_message_from_tbl(20, tbl_dialog, 75);
                viz_area_add(89, 15, 1 << P_VIOLET, 4);
                change_res(P_VIOLET, 3, 1, 50000);
            }
        }
        if (*(byte*)GB_HORSES == 0)
        {
            allow_table(P_VIOLET, 3, A_HASTE, 1);
            unit_convert(P_VIOLET, U_GROM, U_GRUNT, 1);
            find_all_alive_units(U_KARGATH);
            set_stat_all(S_MOVEMENT_TYPE, MOV_SHORE);
            find_all_alive_units(U_RUNESTONE);
            set_stat_all(S_HP, 100);
            ally(P_RED, P_WHITE, 1);
            *(byte*)(GB_HORSES + 15) = num_book;
            *(byte*)CONTROLER_TYPE = C_COMP;
            *(byte*)GB_HORSES = 1;
        }

        find_all_alive_units(ANY_BUILDING);
        sort_stat(S_OWNER, P_VIOLET, CMP_EQ);
        sort_complete();
        if (units != 0)
        {
            find_all_alive_units(U_KARGATH);
            sort_stat(S_OWNER, P_RED, CMP_EQ);
            if (units == 0)
            {
                set_region(66, 106, 94, 127);
                find_all_alive_units(U_KARGATH);
                sort_in_region();
                sort_stat(S_OWNER, P_WHITE, CMP_EQ);
                if (units != 0)give(unit[0], P_RED);
            }
            else
            {
                sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
                set_region(66, 106, 94, 127);
                sort_in_region();
                set_stat_all(S_AI_DEST_X, 65);
                set_stat_all(S_AI_DEST_Y, 126);
                order_all(65, 126, ORDER_ATTACK_AREA);
                find_all_alive_units(U_KARGATH);
                sort_stat(S_OWNER, P_RED, CMP_EQ);
                set_region(64, 125, 65, 127);
                sort_in_region();
                set_stat_all(S_AI_DEST_X, 28);
                set_stat_all(S_AI_DEST_Y, 99);
                order_all(28, 99, ORDER_ATTACK_AREA);
            }
        }

        int* pn = NULL;
        if (*(byte*)(GB_HORSES + 7) == 2)
        {
            pn = unit_create_place(1, 105, U_PEON, P_VIOLET);
            *(byte*)(GB_HORSES + 7) = 3;
        }
        if (*(byte*)(GB_HORSES + 7) == 1)
        {
            pn = unit_create_place(1, 105, U_PEON, P_VIOLET);
            give_order(pn, 1, 105, ORDER_MOVE);
            pn = unit_create_place(1, 106, U_PEON, P_VIOLET);
            give_order(pn, 1, 106, ORDER_MOVE);
            *(byte*)(GB_HORSES + 7) = 2;
        }
        if (*(byte*)(GB_HORSES + 7) == 0)
        {
            pn = unit_create_place(1, 105, U_PEON, P_VIOLET);
            give_order(pn, 2, 105, ORDER_MOVE);
            pn = unit_create_place(1, 106, U_PEON, P_VIOLET);
            give_order(pn, 2, 106, ORDER_MOVE);
            *(byte*)(GB_HORSES + 7) = 1;
        }

        set_region(76, 21, 99, 38);
        find_all_alive_units(U_PEON);
        sort_in_region();
        sort_stat(S_PEON_TREE_CHOPS, 0, CMP_NEQ);
        set_stat_all(S_PEON_TREE_CHOPS, 0);

        spawn_golems();

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            lose(true);
        }
	}
}

void random_move(int* p)
{
    byte map = *(byte*)MAP_SIZE - 1;
    byte x = *((byte*)((uintptr_t)p + S_X));
    byte y = *((byte*)((uintptr_t)p + S_Y));
    int xx = 0, yy = 0, dir = 0;
    xx += x;
    yy += y;
    dir = ((int (*)())F_NET_RANDOM)();
    dir %= 8;
    if (dir == 0)
    {
        if (yy > 0)yy -= 1;
    }
    if (dir == 1)
    {
        if (yy > 0)yy -= 1;
        if (xx < map)xx += 1;
    }
    if (dir == 2)
    {
        if (xx < map)xx += 1;
    }
    if (dir == 3)
    {
        if (xx < map)xx += 1;
        if (yy < map)yy += 1;
    }
    if (dir == 4)
    {
        if (yy < map)yy += 1;
    }
    if (dir == 5)
    {
        if (yy < map)yy += 1;
        if (xx > 0)xx -= 1;
    }
    if (dir == 6)
    {
        if (xx > 0)xx -= 1;
    }
    if (dir == 7)
    {
        if (xx > 0)xx -= 1;
        if (yy > 0)yy -= 1;
    }
    give_order(p, xx % 256, yy % 256, ORDER_MOVE);
}

void v_human5(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
	}
	else
	{
		//your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN4;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;

        save_book();

        if (*(byte*)GB_HORSES == 2)
        {
            set_region(82, 33, 83, 34);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                *(byte*)GB_HORSES = 3;
                *(byte*)LEVEL_OBJ = LVL_HUMAN6;
                show_message_from_tbl(20, tbl_dialog, 73);
                show_message_from_tbl(20, tbl_dialog, 76);
            }
        }

        find_all_alive_units(U_OGRE);
        sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
        for (int i = 0; i < units; i++)random_move(unit[i]);
        find_all_alive_units(U_KARGATH);
        sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
        for (int i = 0; i < units; i++)random_move(unit[i]);

        set_region(76, 21, 99, 38);
        find_all_alive_units(U_PEON);
        sort_in_region();
        sort_stat(S_PEON_TREE_CHOPS, 0, CMP_NEQ);
        set_stat_all(S_PEON_TREE_CHOPS, 0);

        spawn_golems();

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            lose(true);
        }
	}
}

void v_human6(bool rep_init)
{
    if (rep_init)
    {
        //your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
    }
    else
    {
        //your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN4;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;

        save_book();

        if (*(byte*)GB_HORSES == 3)
        {
            find_all_alive_units(U_RUNESTONE);
            sort_stat(S_HP, 1000, CMP_EQ);
            if (units == 12)
            {
                *(byte*)GB_HORSES = 4;
                *(byte*)LEVEL_OBJ = LVL_HUMAN7;
                show_message_from_tbl(20, tbl_dialog, 73);
                show_message_from_tbl(20, tbl_dialog, 77);
            }
        }

        find_all_alive_units(U_OGRE);
        sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
        for (int i = 0; i < units; i++)random_move(unit[i]);
        find_all_alive_units(U_KARGATH);
        sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
        for (int i = 0; i < units; i++)random_move(unit[i]);

        set_region(76, 21, 99, 38);
        find_all_alive_units(U_PEON);
        sort_in_region();
        sort_stat(S_PEON_TREE_CHOPS, 0, CMP_NEQ);
        set_stat_all(S_PEON_TREE_CHOPS, 0);

        spawn_golems();

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            lose(true);
        }
    }
}

void gamesound_play(WORD s, byte x, byte y)
{
    DWORD xy = x + 256 * 256 * y;
    ((void (*)(WORD, DWORD, byte, byte))F_GAMESOUND_XY)(s, xy, 1, 1);
}

void v_human7(bool rep_init)
{
    if (rep_init)
    {
        //your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
    }
    else
    {
        //your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN4;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;

        save_book();

        if (*(byte*)GB_HORSES == 4)
        {
            int f1 = 0, f2 = 0, f3 = 0;
            set_region(23, 49, 24, 50);
            find_all_alive_units(U_MAGE);
            sort_in_region();
            f1 = units;
            set_region(59, 74, 60, 75);
            find_all_alive_units(U_MAGE);
            sort_in_region();
            f2 = units;
            set_region(84, 59, 85, 60);
            find_all_alive_units(U_MAGE);
            sort_in_region();
            f3 = units;
            if ((f1 == 4) && (f2 == 4) && (f3 == 4))
            {
                *(byte*)GB_HORSES = 5;
                *(byte*)LEVEL_OBJ = LVL_HUMAN8;
                show_message_from_tbl(20, tbl_dialog, 73);
                show_message_from_tbl(20, tbl_dialog, 78);
                for (int i = 76; i <= 99; i++)
                {
                    for (int j = 21; j <= 38; j++)
                    {
                        tile_remove_rocks(i, j);
                    }
                }
                center_view(76, 22);
                viz_area_add(82, 23, 1 << P_VIOLET, 9);
                viz_area_add(84, 31, 1 << P_VIOLET, 9);
                viz_area_add(97, 34, 1 << P_VIOLET, 9);
                bullet_create(77 * 32 + 16, 23 * 32 + 16, B_CAT_HIT);
                bullet_create(79 * 32 + 16, 26 * 32 + 16, B_CAT_HIT);
                bullet_create(80 * 32 + 16, 28 * 32 + 16, B_CAT_HIT);
                bullet_create(82 * 32 + 16, 30 * 32 + 16, B_CAT_HIT);
                bullet_create(84 * 32 + 16, 31 * 32 + 16, B_CAT_HIT);
                bullet_create(86 * 32 + 16, 32 * 32 + 16, B_CAT_HIT);
                bullet_create(88 * 32 + 16, 33 * 32 + 16, B_CAT_HIT);
                bullet_create(90 * 32 + 16, 33 * 32 + 16, B_CAT_HIT);
                bullet_create(92 * 32 + 16, 34 * 32 + 16, B_CAT_HIT);
                bullet_create(94 * 32 + 16, 34 * 32 + 16, B_CAT_HIT);
                bullet_create(97 * 32 + 16, 35 * 32 + 16, B_CAT_HIT);
                unit_create(10, 11, U_OGRE, P_WHITE, 10);
                unit_create(104, 70, U_OGRE, P_WHITE, 10);
                find_all_alive_units(ANY_MEN);
                sort_stat(S_OWNER, P_WHITE, CMP_EQ);
                set_stat_all(S_AI_DEST_X, 85);
                set_stat_all(S_AI_DEST_Y, 27);
                order_all(85, 27, ORDER_ATTACK_AREA);
            }
        }

        find_all_alive_units(U_OGRE);
        sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
        for (int i = 0; i < units; i++)random_move(unit[i]);
        find_all_alive_units(U_KARGATH);
        sort_stat(S_ORDER, ORDER_STOP, CMP_EQ);
        for (int i = 0; i < units; i++)random_move(unit[i]);

        set_region(76, 21, 99, 38);
        find_all_alive_units(U_PEON);
        sort_in_region();
        sort_stat(S_PEON_TREE_CHOPS, 0, CMP_NEQ);
        set_stat_all(S_PEON_TREE_CHOPS, 0);

        spawn_golems();

        find_all_alive_units(U_DANATH);
        if (units == 0) 
        {
            *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            lose(true);
        }
    }
}

void v_human8(bool rep_init)
{
    if (rep_init)
    {
        //your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;
    }
    else
    {
        //your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN4;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_RUNESTONE;
        ut[0] = U_RUNESTONE;

        save_book();

        if (*(byte*)GB_HORSES == 5)
        {
            find_all_alive_units(U_DANATH);
            set_region(89, 15, 90, 16);
            sort_in_region();
            if (units != 0)
            {
                *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
                *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
                *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
                win(true);
            }
        }

        spawn_golems();

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            *(byte*)(CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_RED) = C_NOBODY;
            *(byte*)(CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            *(byte*)(STARTING_CONTROLER_TYPE + P_WHITE) = C_NOBODY;
            lose(true);
        }
    }
}

void lit_towers()
{
    for (int i = 0; i < 16; i++)
    {
        int* p = (int*)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int*)(*p);
            while (p)
            {
                bool f = ((*((byte*)((uintptr_t)p + S_ID)) == U_HTOWER));
                if (f)
                {
                    WORD hp = *((WORD*)((uintptr_t)p + S_HP));
                    byte x = *((byte*)((uintptr_t)p + S_X));
                    byte y = *((byte*)((uintptr_t)p + S_Y));
                    if (hp >= 100)
                    {
                        set_region((int)x - 2, (int)y - 2, (int)x + 3, (int)y + 3);
                        find_all_alive_units(ANY_MEN);
                        sort_in_region();
                        sort_stat(S_OWNER, *(byte*)LOCAL_PLAYER, CMP_EQ);
                        if (units != 0)set_stat(p, 40, S_HP);
                    }
                    else
                    {
                        viz_area_add(x, y, 1 << P_VIOLET, 9);
                    }
                }
                p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void sort_in_region_exact()
{
    int k = 0;
    byte x = 0, y = 0;
    for (int i = 0; i < units; i++)
    {
        x = *((byte*)((uintptr_t)unit[i] + S_X));
        y = *((byte*)((uintptr_t)unit[i] + S_Y));
        if (in_region(x, y, reg[0], reg[1], reg[2], reg[3]))
        {
            unitt[k] = unit[i];
            k++;
        }
    }
    units = k;
    for (int i = 0; i < units; i++)
    {
        unit[i] = unitt[i];
    }
}

void circle_rune()
{
    for (int i = 0; i < 16; i++)
    {
        int* p = (int*)(UNITS_LISTS + 4 * i);
        if (p)
        {
            p = (int*)(*p);
            while (p)
            {
                bool f = ((*((byte*)((uintptr_t)p + S_ID)) == U_CIRCLE));
                if (f)
                {
                    byte x = *((byte*)((uintptr_t)p + S_X));
                    byte y = *((byte*)((uintptr_t)p + S_Y));
                    byte c = *((byte*)((uintptr_t)p + S_COLOR));
                    set_region((int)x, (int)y, (int)x + 1, (int)y + 1);
                    find_all_alive_units(U_CATAPULT);
                    sort_in_region_exact();
                    if (*(byte*)GB_HORSES < 21)
                    {
                        sort_stat(S_COLOR, c, CMP_EQ);
                        if (units != 0)
                        {
                            kill_all();
                            unit_kill(p);
                        }
                    }
                    else
                    {
                        if (units != 0)
                        {
                            set_stat_all(S_COLOR, c);
                        }
                    }
                }
                p = (int*)(*((int*)((uintptr_t)p + S_NEXT_UNIT_POINTER)));
            }
        }
    }
}

void v_human9(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_CATAPULT;
        ut[0] = U_CATAPULT;
	}
	else
	{
		//your custom victory conditions
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_CATAPULT;
        ut[0] = U_CATAPULT;
        lit_towers();

        save_book();

        if (*(byte*)GB_HORSES == 23)
        {
            set_region(53, 0, 54, 1);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                *(byte*)GB_HORSES = 24;
                *(byte*)LEVEL_OBJ = LVL_HUMAN11;
            }
        }

        if (*(byte*)GB_HORSES == 17)
        {
            set_region(52, 16, 54, 16);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 66);
                show_message_from_tbl(20, tbl_dialog, 67);
                show_message_from_tbl(20, tbl_dialog, 68);
                sound_play_dialog(13);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);

                byte mxs = *(byte*)MAP_SIZE;//map size
                char* sq = (char*)*(int*)(MAP_SQ_POINTER);//map cells
                char buf[] = "\x0\x0";//passable land
                for (int i = 17; i <= 26; i++)
                {
                    PATCH_SET((char*)(sq + 2 * 58 + 2 * i * mxs), buf);
                }

                *(byte*)GB_HORSES = 18;
                *(byte*)LEVEL_OBJ = LVL_HUMAN10;
            }
        }
        if (*(byte*)GB_HORSES == 5)
        {
            *(byte*)GB_HORSES = 6;
            *(byte*)LEVEL_OBJ = LVL_HUMAN10;
            show_message_from_tbl(20, tbl_dialog, 73);
            show_message_from_tbl(20, tbl_dialog, 79);
        }
        if (*(byte*)GB_HORSES == 4)
        {
            *(byte*)GB_HORSES = 5;
        }
        if (*(byte*)GB_HORSES == 3)
        {
            *(byte*)GB_HORSES = 4;
            find_all_alive_units(U_DANATH);
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 34);
                show_message_from_tbl(20, tbl_dialog, 35);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
            }
        }
        if (*(byte*)GB_HORSES == 2)
        {
            *(byte*)GB_HORSES = 3;
        }
        if (*(byte*)GB_HORSES == 1)
        {
            set_region(45, 97, 61, 127);
            find_all_alive_units(U_SKELETON);
            sort_in_region();
            if (units == 0)
            {
                *(byte*)GB_HORSES = 2;
                find_all_alive_units(U_DANATH);
                if (units != 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 32);
                    show_message_from_tbl(20, tbl_dialog, 33);
                    sound_play_dialog(6);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);
                }

                byte mxs = *(byte*)MAP_SIZE;//map size
                char* sq = (char*)*(int*)(MAP_SQ_POINTER);//map cells
                char buf[] = "\x0\x0";//passable land
                for (int i = 101; i <= 106; i++)
                {
                    PATCH_SET((char*)(sq + 2 * 45 + 2 * i * mxs), buf);
                    PATCH_SET((char*)(sq + 2 * 61 + 2 * i * mxs), buf);
                }
            }
        }

        if (*(byte*)GB_HORSES == 0)
        {
            find_all_alive_units(ANY_UNITS);
            sort_stat(S_OWNER, P_YELLOW, CMP_EQ);
            set_stat_all(S_COLOR, P_WHITE);
            give_all(P_WHITE);
            find_all_alive_units(ANY_UNITS);
            sort_stat(S_OWNER, P_GREEN, CMP_EQ);
            sort_stat(S_ID, U_OGRE, CMP_NEQ);
            sort_stat(S_ID, U_KARGATH, CMP_NEQ);
            sort_stat(S_ID, U_TROLL, CMP_NEQ);
            give_all(P_NEUTRAL);
            set_stat_all(S_FLAGS2, 0);
            find_all_alive_units(ANY_UNITS);
            sort_stat(S_OWNER, P_ORANGE, CMP_EQ);
            sort_stat(S_ID, U_HTANKER, CMP_NEQ);
            sort_stat(S_ID, U_SUBMARINE, CMP_NEQ);
            give_all(P_NEUTRAL);
            set_stat_all(S_FLAGS2, 0);
            find_all_alive_units(U_STRONGHOLD);
            set_stat(unit[0], 0, S_AI_AIFLAGS);
            set_stat(unit[1], 1, S_AI_AIFLAGS);
            unit_convert(P_VIOLET, U_GROM, U_GRUNT, 1);
            find_all_alive_units(U_KARGATH);
            set_stat_all(S_MOVEMENT_TYPE, MOV_SHORE);

            int* u = NULL;

            u = unit_create_place(26, 92, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(44, 92, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLUE, S_COLOR);
            u = unit_create_place(26, 82, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_ORANGE, S_COLOR);
            u = unit_create_place(44, 82, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(35, 87, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_YELLOW, S_COLOR);

            u = unit_create_place(70, 82, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(63, 91, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLUE, S_COLOR);
            u = unit_create_place(78, 92, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLACK, S_COLOR);
            u = unit_create_place(76, 73, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(63, 74, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_WHITE, S_COLOR);

            u = unit_create_place(86, 90, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(97, 88, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_YELLOW, S_COLOR);
            u = unit_create_place(91, 83, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_ORANGE, S_COLOR);
            u = unit_create_place(87, 77, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_WHITE, S_COLOR);
            u = unit_create_place(95, 76, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLUE, S_COLOR);

            u = unit_create_place(31, 59, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_WHITE, S_COLOR);
            u = unit_create_place(33, 54, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLUE, S_COLOR);
            u = unit_create_place(39, 57, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_ORANGE, S_COLOR);
            u = unit_create_place(28, 49, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(35, 47, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_YELLOW, S_COLOR);
            u = unit_create_place(41, 49, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLACK, S_COLOR);

            u = unit_create_place(16, 11, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_WHITE, S_COLOR);
            u = unit_create_place(21, 11, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_YELLOW, S_COLOR);
            u = unit_create_place(26, 11, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_ORANGE, S_COLOR);
            u = unit_create_place(33, 11, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLUE, S_COLOR);
            u = unit_create_place(38, 11, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_GREEN, S_COLOR);
            u = unit_create_place(43, 11, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_BLACK, S_COLOR);

            u = unit_create_place(15, 8, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(19, 8, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(24, 9, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(36, 9, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(44, 8, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(41, 7, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(36, 6, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(31, 6, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(24, 5, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(30, 4, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);
            u = unit_create_place(19, 4, U_CIRCLE, P_NEUTRAL);
            set_stat(u, P_VIOLET, S_COLOR);

            set_region(62, 31, 68, 68);
            find_all_alive_units(U_SKELETON);
            sort_in_region();
            if (units != 0)
            {
                int k = ((int (*)())F_NET_RANDOM)() % units;
                set_stat(unit[k], P_VIOLET, S_COLOR);
            }

            *(byte*)(GB_HORSES + 15) = num_book;

            *(byte*)GB_HORSES = 1;
        }

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            lose(true);
        }
	}
}

void v_human10(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_CATAPULT;
        ut[0] = U_CATAPULT;
	}
	else
	{
		//your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN9;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_CATAPULT;
        ut[0] = U_CATAPULT;
        lit_towers();

        save_book();

        if (*(byte*)GB_HORSES == 18)
        {
            find_all_alive_units(U_TEMPLE);
            if (units == 0)
            {
                *(byte*)GB_HORSES = 19;
                *(byte*)LEVEL_OBJ = LVL_HUMAN11;
                find_all_alive_units(U_DANATH);
                if (units != 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 63);
                    show_message_from_tbl(20, tbl_dialog, 64);
                    show_message_from_tbl(20, tbl_dialog, 65);
                    sound_play_dialog(14);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);
                }
            }
        }

        if (*(byte*)GB_HORSES == 10)
        {
            *(byte*)GB_HORSES = 11;
            *(byte*)LEVEL_OBJ = LVL_HUMAN11;
            show_message_from_tbl(20, tbl_dialog, 73);
            show_message_from_tbl(20, tbl_dialog, 80);
        }

        if (*(byte*)GB_HORSES == 9)
        {
            *(byte*)GB_HORSES = 10;
        }

        if (*(byte*)GB_HORSES == 8)
        {
            *(byte*)GB_HORSES = 9;
            find_all_alive_units(U_DANATH);
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 43);
                show_message_from_tbl(20, tbl_dialog, 44);
                sound_play_dialog(7);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
            }
        }

        if (*(byte*)GB_HORSES == 7)
        {
            *(byte*)GB_HORSES = 8;
        }

        if (*(byte*)GB_HORSES == 6)
        {
            if ((*(byte*)(GB_HORSES + 1) == 1) &&
                (*(byte*)(GB_HORSES + 2) == 1) &&
                (*(byte*)(GB_HORSES + 3) == 1) &&
                (*(byte*)(GB_HORSES + 4) == 1) &&
                (*(byte*)(GB_HORSES + 5) == 1))
            {
                *(byte*)GB_HORSES = 7;
            }
            if (*(byte*)(GB_HORSES + 1) == 0)
            {
                find_all_alive_units(U_RUNESTONE);
                if (units == 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 36);
                    show_message_from_tbl(20, tbl_dialog, 37);
                    *(byte*)(GB_HORSES + 1) = 1;
                }
            }
            if (*(byte*)(GB_HORSES + 2) == 0)
            {
                find_all_alive_units(U_ALTAR);
                if (units == 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 38);
                    *(byte*)(GB_HORSES + 2) = 1;
                }
            }
            if (*(byte*)(GB_HORSES + 3) == 0)
            {
                find_all_alive_units(U_TURTLE);
                if (units == 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 39);
                    *(byte*)(GB_HORSES + 3) = 1;
                }
            }
            if (*(byte*)(GB_HORSES + 4) == 0)
            {
                find_all_alive_units(U_OFOUNDRY);
                if (units == 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 40);
                    *(byte*)(GB_HORSES + 4) = 1;
                }
            }
            if (*(byte*)(GB_HORSES + 5) == 0)
            {
                find_all_alive_units(U_SKELETON);
                sort_stat(S_COLOR, P_VIOLET, CMP_EQ);
                if (units == 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 41);
                    show_message_from_tbl(20, tbl_dialog, 42);
                    *(byte*)(GB_HORSES + 5) = 1;
                }
            }
        }

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            lose(true);
        }
	}
}

void v_human11(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_CATAPULT;
        ut[0] = U_CATAPULT;
	}
	else
	{
		//your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN9;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        ua[0] = U_CATAPULT;
        ut[0] = U_CATAPULT;
        lit_towers();
        circle_rune();

        save_book();

        int* u = NULL;

        if (*(byte*)GB_HORSES == 24)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            win(true);
        }

        if (*(byte*)GB_HORSES == 22)
        {
            *(byte*)GB_HORSES = 23;
            *(byte*)LEVEL_OBJ = LVL_HUMAN9;
            unit_create(54, 0, U_DK, P_WHITE, 2);
            unit_create(54, 1, U_SKELETON, P_WHITE, 6);
            find_all_alive_units(U_DANATH);
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 60);
                show_message_from_tbl(20, tbl_dialog, 61);
                show_message_from_tbl(20, tbl_dialog, 62);
                sound_play_dialog(16);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
            }
        }

        if (*(byte*)GB_HORSES == 21)
        {
            bool r1 = false, r2 = false, r3 = false, r4 = false, r5 = false, r6 = false;

            set_region(16, 5, 16, 5);
            find_all_alive_units(U_CATAPULT);
            sort_in_region();
            sort_stat(S_COLOR, P_YELLOW, CMP_EQ);
            sort_stat(S_LAST_HARVEST_X + 1, 3, CMP_EQ);
            if (units != 0)r1 = true;
            set_region(21, 6, 21, 6);
            find_all_alive_units(U_CATAPULT);
            sort_in_region();
            sort_stat(S_COLOR, P_ORANGE, CMP_EQ);
            sort_stat(S_LAST_HARVEST_X + 1, 4, CMP_EQ);
            if (units != 0)r2 = true;
            set_region(27, 5, 27, 5);
            find_all_alive_units(U_CATAPULT);
            sort_in_region();
            sort_stat(S_COLOR, P_BLUE, CMP_EQ);
            sort_stat(S_LAST_HARVEST_X + 1, 0, CMP_EQ);
            if (units != 0)r3 = true;
            set_region(33, 5, 33, 5);
            find_all_alive_units(U_CATAPULT);
            sort_in_region();
            sort_stat(S_COLOR, P_GREEN, CMP_EQ);
            sort_stat(S_LAST_HARVEST_X + 1, 2, CMP_EQ);
            if (units != 0)r4 = true;
            set_region(38, 6, 38, 6);
            find_all_alive_units(U_CATAPULT);
            sort_in_region();
            sort_stat(S_COLOR, P_BLACK, CMP_EQ);
            sort_stat(S_LAST_HARVEST_X + 1, 5, CMP_EQ);
            if (units != 0)r5 = true;
            set_region(42, 5, 42, 5);
            find_all_alive_units(U_CATAPULT);
            sort_in_region();
            sort_stat(S_COLOR, P_WHITE, CMP_EQ);
            sort_stat(S_LAST_HARVEST_X + 1, 1, CMP_EQ);
            if (units != 0)r6 = true;

            if (r1 && r2 && r3 && r4 && r5 && r6)
            {
                find_all_alive_units(U_CATAPULT);
                kill_all();
                find_all_alive_units(U_CIRCLE);
                sort_stat(S_COLOR, P_NEUTRAL, CMP_NEQ);
                kill_all();

                *(byte*)GB_HORSES = 22;
                tile_remove_rocks(53, 12);
                center_view(48, 8);
            }

            if (*(byte*)(GB_HORSES + 10) == 0)
            {
                set_region(20, 21, 32, 24);
                find_all_alive_units(U_DANATH);
                sort_in_region();
                if (units != 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 57);
                    show_message_from_tbl(20, tbl_dialog, 58);
                    show_message_from_tbl(20, tbl_dialog, 59);
                    sound_play_dialog(12);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);

                    set_region(21, 26, 31, 26);
                    find_all_alive_units(U_UTER);
                    sort_in_region();
                    kill_all();

                    u = unit_create_place(22, 26, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_YELLOW, S_COLOR);
                    set_stat(u, 3, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(24, 26, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_ORANGE, S_COLOR);
                    set_stat(u, 4, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(26, 26, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_BLUE, S_COLOR);
                    set_stat(u, 0, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(28, 26, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_GREEN, S_COLOR);
                    set_stat(u, 2, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(30, 26, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_BLACK, S_COLOR);
                    set_stat(u, 5, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(32, 26, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_WHITE, S_COLOR);
                    set_stat(u, 1, S_LAST_HARVEST_X + 1);

                    *(byte*)(GB_HORSES + 10) = 1;
                }
            }
        }

        if (*(byte*)GB_HORSES == 20)
        {
            set_region(25, 47, 44, 60);
            find_all_alive_units(U_CIRCLE);
            sort_in_region();
            if (units == 0)
            {
                *(byte*)GB_HORSES = 21;
                find_all_alive_units(U_DANATH);
                if (units != 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 53);
                    show_message_from_tbl(20, tbl_dialog, 54);
                    show_message_from_tbl(20, tbl_dialog, 55);
                    show_message_from_tbl(20, tbl_dialog, 56);
                    sound_play_dialog(15);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);

                    u = unit_create_place(17, 5, U_CATAPULT, P_VIOLET);
                    set_stat(u, 0, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(22, 6, U_CATAPULT, P_VIOLET);
                    set_stat(u, 1, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(28, 5, U_CATAPULT, P_VIOLET);
                    set_stat(u, 2, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(34, 5, U_CATAPULT, P_VIOLET);
                    set_stat(u, 3, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(39, 6, U_CATAPULT, P_VIOLET);
                    set_stat(u, 4, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(43, 5, U_CATAPULT, P_VIOLET);
                    set_stat(u, 5, S_LAST_HARVEST_X + 1);

                    center_view(19, 4);
                    tile_remove_rocks(53, 15);

                    byte mxs = *(byte*)MAP_SIZE;//map size
                    char* sq = (char*)*(int*)(MAP_SQ_POINTER);//map cells
                    char buf[] = "\x0\x0";//passable land
                    for (int i = 17; i <= 26; i++)
                    {
                        PATCH_SET((char*)(sq + 2 * 48 + 2 * i * mxs), buf);
                    }
                }
            }
        }

        if (*(byte*)GB_HORSES == 19)
        {
            set_region(52, 16, 54, 16);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 51);
                show_message_from_tbl(20, tbl_dialog, 52);
                sound_play_dialog(10);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);

                u = unit_create_place(31, 51, U_CATAPULT, P_VIOLET);
                set_stat(u, P_WHITE, S_COLOR);
                set_stat(u, 0, S_LAST_HARVEST_X + 1);
                u = unit_create_place(42, 52, U_CATAPULT, P_VIOLET);
                set_stat(u, P_GREEN, S_COLOR);
                set_stat(u, 1, S_LAST_HARVEST_X + 1);
                u = unit_create_place(45, 59, U_CATAPULT, P_VIOLET);
                set_stat(u, P_YELLOW, S_COLOR);
                set_stat(u, 2, S_LAST_HARVEST_X + 1);
                u = unit_create_place(44, 60, U_CATAPULT, P_VIOLET);
                set_stat(u, P_BLUE, S_COLOR);
                set_stat(u, 3, S_LAST_HARVEST_X + 1);
                u = unit_create_place(31, 55, U_CATAPULT, P_VIOLET);
                set_stat(u, P_ORANGE, S_COLOR);
                set_stat(u, 4, S_LAST_HARVEST_X + 1);
                u = unit_create_place(29, 59, U_CATAPULT, P_VIOLET);
                set_stat(u, P_BLACK, S_COLOR);
                set_stat(u, 5, S_LAST_HARVEST_X + 1);

                center_view(25, 47);
                *(byte*)GB_HORSES = 20;
            }
        }

        if (*(byte*)GB_HORSES == 16)
        {
            *(byte*)GB_HORSES = 17;
            *(byte*)LEVEL_OBJ = LVL_HUMAN9;
            center_view(41, 66);
        }

        if (*(byte*)GB_HORSES == 15)
        {
            *(byte*)GB_HORSES = 16;
            set_region(45, 65, 48, 77);
            find_all_alive_units(U_HTOWER);
            sort_in_region();
            set_stat_all(S_HP, 10);
            set_region(25, 65, 44, 77);
            find_all_alive_units(U_SKELETON);
            sort_in_region();
            order_all(54, 71, ORDER_PATROL);
            set_stat_all(S_AI_DEST_X, 50);
            set_stat_all(S_AI_DEST_Y, 71);
        }

        if (*(byte*)GB_HORSES == 14)
        {
            set_region(83, 73, 98, 93);
            find_all_alive_units(U_CIRCLE);
            sort_in_region();
            if (units == 0)
            {
                *(byte*)GB_HORSES = 15;
                center_view(46, 89);
                tile_remove_rocks(53, 93);
            }
        }

        if (*(byte*)GB_HORSES == 13)
        {
            set_region(62, 73, 78, 93);
            find_all_alive_units(U_CIRCLE);
            sort_in_region();
            if (units == 0)
            {
                *(byte*)GB_HORSES = 14;
                find_all_alive_units(U_DANATH);
                if (units != 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 49);
                    show_message_from_tbl(20, tbl_dialog, 50);
                    sound_play_dialog(11);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);

                    find_all_alive_units(U_HTRANSPORT);
                    kill_all();

                    u = unit_create_place(87, 83, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_WHITE, S_COLOR);
                    set_stat(u, 0, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(89, 90, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_GREEN, S_COLOR);
                    set_stat(u, 1, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(97, 84, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_YELLOW, S_COLOR);
                    set_stat(u, 2, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(87, 80, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_BLUE, S_COLOR);
                    set_stat(u, 3, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(85, 76, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_ORANGE, S_COLOR);
                    set_stat(u, 4, S_LAST_HARVEST_X + 1);
                    
                    center_view(83, 75);
                    tile_remove_rocks(53, 96);
                }
            }
        }

        if (*(byte*)GB_HORSES == 12)
        {
            set_region(25, 82, 44, 93);
            find_all_alive_units(U_CIRCLE);
            sort_in_region();
            if (units == 0)
            {
                *(byte*)GB_HORSES = 13;
                find_all_alive_units(U_DANATH);
                if (units != 0)
                {
                    show_message_from_tbl(20, tbl_dialog, 48);
                    sound_play_dialog(9);
                    set_stat_all(S_FLASH_COUNTER, 80);
                    set_stat_all(S_FLASH_COLOR, 253);

                    unit_create_place(77, 83, U_HTRANSPORT, P_VIOLET);

                    u = unit_create_place(68, 84, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_WHITE, S_COLOR);
                    set_stat(u, 0, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(63, 87, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_GREEN, S_COLOR);
                    set_stat(u, 1, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(74, 75, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_BLACK, S_COLOR);
                    set_stat(u, 2, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(64, 80, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_BLUE, S_COLOR);
                    set_stat(u, 3, S_LAST_HARVEST_X + 1);
                    u = unit_create_place(75, 93, U_CATAPULT, P_VIOLET);
                    set_stat(u, P_VIOLET, S_COLOR);
                    set_stat(u, 4, S_LAST_HARVEST_X + 1);

                    center_view(65, 78);
                    tile_remove_rocks(53, 99);
                }
            }
        }

        if (*(byte*)GB_HORSES == 11)
        {
            set_region(52, 100, 54, 100);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 45);
                show_message_from_tbl(20, tbl_dialog, 46);
                show_message_from_tbl(20, tbl_dialog, 47);
                sound_play_dialog(8);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);

                *(byte*)GB_HORSES = 12;

                u = unit_create_place(42, 92, U_CATAPULT, P_VIOLET);
                set_stat(u, P_ORANGE, S_COLOR);
                set_stat(u, 0, S_LAST_HARVEST_X + 1);
                u = unit_create_place(28, 87, U_CATAPULT, P_VIOLET);
                set_stat(u, P_GREEN, S_COLOR);
                set_stat(u, 1, S_LAST_HARVEST_X + 1);
                u = unit_create_place(45, 87, U_CATAPULT, P_VIOLET);
                set_stat(u, P_YELLOW, S_COLOR);
                set_stat(u, 2, S_LAST_HARVEST_X + 1);
                u = unit_create_place(29, 91, U_CATAPULT, P_VIOLET);
                set_stat(u, P_BLUE, S_COLOR);
                set_stat(u, 3, S_LAST_HARVEST_X + 1);
                u = unit_create_place(41, 84, U_CATAPULT, P_VIOLET);
                set_stat(u, P_VIOLET, S_COLOR);
                set_stat(u, 4, S_LAST_HARVEST_X + 1);

                center_view(25, 82);
            }
        }

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            lose(true);
        }
	}
}

void song_change(int id)
{
    if ((id >= 0) && (id <= 25))
    {
        *(byte*)SONG_ID = id % 256;
        if (((int (*)())F_SONG_LOAD)())
        {
            ((void (*)())F_SONG_STOP)();
            ((void (*)(int))F_SONG_PLAY)(*(byte*)SONG_ID);
        }
    }
}

void v_human12(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        upgr(SWORDS, 0);
        repair_cat(true);
        comps_vision(true);
        ua[0] = U_CATAPULT;
        ut[0] = U_PALADIN;
        ua[1] = U_HWALL;
        ut[1] = U_HWALL;
        ua[2] = U_RUNESTONE;
        ut[2] = U_RUNESTONE;
        ua[3] = U_MINE;
        ut[3] = U_MINE;
        ua[4] = U_PIGFARM;
        ut[4] = U_PIGFARM;
        ua[5] = U_OSMITH;
        ut[5] = U_OSMITH;
        ua[6] = U_ALCHEMIST;
        ut[6] = U_ALCHEMIST;
        ua[7] = U_OGREMOUND;
        ut[7] = U_OGREMOUND;
        ua[8] = U_HCANONTOWER;
        ut[8] = U_HCANONTOWER;
	}
	else
	{
		//your custom victory conditions
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        upgr(SWORDS, 0);
        repair_cat(true);
        comps_vision(true);

        save_book();

        set_upgrade(SWORDS, P_RED, 98);
        viz_area_add(21, 41, 1 << P_VIOLET, 9);
        viz_area_add(41, 41, 1 << P_VIOLET, 9);
        viz_area_add(21, 21, 1 << P_VIOLET, 9);
        viz_area_add(41, 21, 1 << P_VIOLET, 9);
        viz_area_add(21, 31, 1 << P_VIOLET, 9);
        viz_area_add(31, 21, 1 << P_VIOLET, 9);
        viz_area_add(41, 31, 1 << P_VIOLET, 9);
        viz_area_add(31, 41, 1 << P_VIOLET, 9);
        viz_area_add(31, 31, 1 << P_VIOLET, 9);

        find_all_alive_units(U_PALADIN);
        if (units != 0)
        {
            WORD hp = *((WORD*)((uintptr_t)unit[0] + S_HP));
            WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * U_PALADIN);//max hp
            if (hp == mhp)
            {
                ua[0] = U_CATAPULT;
                ut[0] = U_PALADIN;
            }
            else
            {
                ua[0] = 255;
                ut[0] = 255;
            }
        }

        if (*(byte*)GB_HORSES == 20)
        {
            find_all_alive_units(U_PALADIN);
            if (units == 0)
            {
                find_all_alive_units(U_HWALL);
                kill_all();
                *(byte*)LEVEL_OBJ = LVL_HUMAN14;
                *(byte*)GB_HORSES = 21;
            }
        }
        if (*(byte*)GB_HORSES == 10)
        {
            find_all_alive_units(U_DANATH);
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 69);
                show_message_from_tbl(20, tbl_dialog, 70);
                sound_play_dialog(17);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
            }
            set_region(19, 33, 21, 39);
            find_all_alive_units(U_HWALL);
            sort_in_region();
            kill_all();

            set_region(3, 31, 16, 43);
            find_all_alive_units(ANY_MEN);
            sort_in_region();
            order_all(31, 31, ORDER_ATTACK_AREA);
            set_stat_all(S_AI_DEST_X, 31);
            set_stat_all(S_AI_DEST_Y, 31);

            *(byte*)LEVEL_OBJ = LVL_HUMAN13;
            *(byte*)GB_HORSES = 11;
        }
        if (*(byte*)GB_HORSES == 9)
        {
            *(byte*)GB_HORSES = 10;
        }
        if (*(byte*)GB_HORSES == 8)
        {
            *(byte*)GB_HORSES = 9;
        }
        if (*(byte*)GB_HORSES == 7)
        {
            *(byte*)GB_HORSES = 8;
        }
        if (*(byte*)GB_HORSES == 6)
        {
            *(byte*)GB_HORSES = 7;
        }
        if (*(byte*)GB_HORSES == 5)
        {
            *(byte*)GB_HORSES = 6;
        }
        if (*(byte*)GB_HORSES == 4)
        {
            *(byte*)GB_HORSES = 5;
        }
        if (*(byte*)GB_HORSES == 3)
        {
            *(byte*)GB_HORSES = 4;
        }
        if (*(byte*)GB_HORSES == 2)
        {
            *(byte*)GB_HORSES = 3;
        }
        if (*(byte*)GB_HORSES == 1)
        {
            ally(P_VIOLET, P_BLACK, 1);

            find_all_alive_units(U_DANATH);
            order_all(31, 33, ORDER_MOVE);
            find_all_alive_units(U_GRUNT);
            sort_stat(S_OWNER, P_BLACK, CMP_EQ);
            give_order(unit[0], 29, 28, ORDER_MOVE);
            give_order(unit[1], 28, 29, ORDER_MOVE);
            give_order(unit[2], 33, 28, ORDER_MOVE);
            give_order(unit[3], 34, 29, ORDER_MOVE);
            give_order(unit[4], 28, 33, ORDER_MOVE);
            give_order(unit[5], 29, 34, ORDER_MOVE);
            give_order(unit[6], 33, 34, ORDER_MOVE);
            give_order(unit[7], 34, 33, ORDER_MOVE);
            find_all_alive_units(U_LOTHAR);
            sort_stat(S_OWNER, P_BLACK, CMP_EQ);
            give_order(unit[0], 29, 29, ORDER_MOVE);
            give_order(unit[1], 33, 29, ORDER_MOVE);
            give_order(unit[2], 29, 33, ORDER_MOVE);
            give_order(unit[3], 33, 33, ORDER_MOVE);
            find_all_alive_units(U_MAGE);
            give_order(unit[0], 26, 31, ORDER_MOVE);
            give_order(unit[1], 36, 31, ORDER_MOVE);

            set_region(31, 33, 31, 33);
            find_all_alive_units(U_DANATH);
            sort_in_region();
            if (units != 0)
            {
                find_all_alive_units(ANY_MEN);
                sort_stat(S_OWNER, P_BLACK, CMP_EQ);
                sort_stat(S_ORDER, ORDER_STOP, CMP_NEQ);
                if (units == 0)
                {
                    find_all_alive_units(ANY_MEN);
                    sort_stat(S_OWNER, P_BLACK, CMP_EQ);
                    give_all(P_VIOLET);

                    unit_create(30, 42, U_RUNESTONE, P_NEUTRAL, 1);
                    unit_create(33, 42, U_RUNESTONE, P_NEUTRAL, 1);
                    unit_create(31, 44, U_MINE, P_NEUTRAL, 1);
                    unit_create(32, 42, U_HWALL, P_NEUTRAL, 1);
                    unit_create(32, 43, U_HWALL, P_NEUTRAL, 1);
                    unit_create(30, 44, U_HWALL, P_NEUTRAL, 1);
                    unit_create(30, 45, U_HWALL, P_NEUTRAL, 1);
                    unit_create(30, 46, U_HWALL, P_NEUTRAL, 1);
                    unit_create(34, 44, U_HWALL, P_NEUTRAL, 1);
                    unit_create(34, 45, U_HWALL, P_NEUTRAL, 1);
                    unit_create(34, 46, U_HWALL, P_NEUTRAL, 1);

                    song_change(21);

                    find_all_alive_units(ANY_MEN);
                    sort_stat(S_OWNER, P_VIOLET, CMP_EQ);
                    sort_stat(S_ID, U_MAGE, CMP_NEQ);
                    order_all(31, 31, ORDER_ATTACK_AREA);

                    *(byte*)GB_HORSES = 2;
                }
            } 
        }
        if (*(byte*)GB_HORSES == 0)
        {
            viz(P_VIOLET, P_BLACK, 1);
            ally(P_RED, P_ORANGE, 0);
            ally(P_RED, P_WHITE, 0);
            ally(P_RED, P_YELLOW, 0);
            
            unit_convert(P_BLACK, U_GROM, U_GRUNT, 1);
            find_all_alive_units(U_KARGATH);
            set_stat_all(S_MOVEMENT_TYPE, MOV_SHORE);
            
            unit_create(44, 33, U_HCANONTOWER, P_ORANGE, 1);
            unit_create(44, 40, U_HCANONTOWER, P_ORANGE, 1);
            find_all_alive_units(ANY_BUILDING);
            sort_stat(S_OWNER, P_ORANGE, CMP_EQ);
            give_all(P_NEUTRAL);

            find_all_alive_units(ANY_MEN);
            sort_stat(S_OWNER, P_BLACK, CMP_EQ);
            sort_stat(S_ID, U_DANATH, CMP_NEQ);
            set_stat_all(S_COLOR, P_VIOLET);

            int* u = unit_create_place(62, 37, U_CATAPULT, P_NEUTRAL);
            set_stat(u, 10, S_HP);
            set_stat(u, P_GREEN, S_COLOR);

            *(byte*)(GB_HORSES + 15) = num_book;

            *(byte*)(CONTROLER_TYPE + P_BLACK) = C_PLAYER;

            *(byte*)GB_HORSES = 1;
        }

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            lose(true);
        }
        find_all_alive_units(U_CATAPULT);
        if (units == 0)
        {
            find_all_alive_units(U_PALADIN);
            if (units != 0)
            {
                WORD hp = *((WORD*)((uintptr_t)unit[0] + S_HP));
                WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * U_PALADIN);//max hp
                if (hp == mhp)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        if (i != P_VIOLET)
                        {
                            *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                            *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                        }
                    }
                    lose(true);
                }
            }
        }
	}
}

void v_human13(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        upgr(SWORDS, 0);
        repair_cat(true);
        comps_vision(true);
        ua[0] = U_CATAPULT;
        ut[0] = U_PALADIN;
        ua[1] = U_HWALL;
        ut[1] = U_HWALL;
        ua[2] = U_RUNESTONE;
        ut[2] = U_RUNESTONE;
        ua[3] = U_MINE;
        ut[3] = U_MINE;
        ua[4] = U_PIGFARM;
        ut[4] = U_PIGFARM;
        ua[5] = U_OSMITH;
        ut[5] = U_OSMITH;
        ua[6] = U_ALCHEMIST;
        ut[6] = U_ALCHEMIST;
        ua[7] = U_OGREMOUND;
        ut[7] = U_OGREMOUND;
        ua[8] = U_HCANONTOWER;
        ut[8] = U_HCANONTOWER;
	}
	else
	{
		//your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN12;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        upgr(SWORDS, 0);
        repair_cat(true);
        comps_vision(true);

        save_book();

        set_upgrade(SWORDS, P_RED, 98);
        viz_area_add(21, 41, 1 << P_VIOLET, 9);
        viz_area_add(41, 41, 1 << P_VIOLET, 9);
        viz_area_add(21, 21, 1 << P_VIOLET, 9);
        viz_area_add(41, 21, 1 << P_VIOLET, 9);
        viz_area_add(21, 31, 1 << P_VIOLET, 9);
        viz_area_add(31, 21, 1 << P_VIOLET, 9);
        viz_area_add(41, 31, 1 << P_VIOLET, 9);
        viz_area_add(31, 41, 1 << P_VIOLET, 9);
        viz_area_add(31, 31, 1 << P_VIOLET, 9);

        if (*(byte*)GB_HORSES == 13)
        {
            set_region(41, 30, 63, 44);
            find_all_alive_units(ANY_MEN);
            sort_in_region();
            sort_stat(S_OWNER, P_ORANGE, CMP_BIGGER_EQ);
            sort_stat(S_OWNER, P_NEUTRAL, CMP_NEQ);
            if (units == 0)
            {
                find_all_alive_units(ANY_MEN);
                sort_stat(S_OWNER, P_BLUE, CMP_EQ);
                give_all(P_VIOLET);

                *(byte*)LEVEL_OBJ = LVL_HUMAN14;
                *(byte*)GB_HORSES = 14;
            }
        }
        if (*(byte*)GB_HORSES == 12)
        {
            set_region(42, 3, 62, 27);
            find_all_alive_units(ANY_MEN);
            sort_in_region();
            sort_stat(S_OWNER, P_ORANGE, CMP_BIGGER_EQ);
            if (units == 0)
            {
                set_region(41, 35, 43, 39);
                find_all_alive_units(U_HWALL);
                sort_in_region();
                kill_all();

                set_region(41, 30, 60, 44);
                find_all_alive_units(ANY_MEN);
                sort_in_region();
                order_all(31, 31, ORDER_ATTACK_AREA);
                set_stat_all(S_AI_DEST_X, 31);
                set_stat_all(S_AI_DEST_Y, 31);

                *(byte*)GB_HORSES = 13;
            }
        }
        if (*(byte*)GB_HORSES == 11)
        {
            set_region(0, 13, 20, 60);
            find_all_alive_units(ANY_MEN);
            sort_in_region();
            sort_stat(S_OWNER, P_ORANGE, CMP_BIGGER_EQ);
            if (units == 0)
            {
                set_region(41, 20, 43, 26);
                find_all_alive_units(U_HWALL);
                sort_in_region();
                kill_all();

                set_region(41, 16, 62, 27);
                find_all_alive_units(ANY_MEN);
                sort_in_region();
                order_all(31, 31, ORDER_ATTACK_AREA);
                set_stat_all(S_AI_DEST_X, 31);
                set_stat_all(S_AI_DEST_Y, 31);

                *(byte*)GB_HORSES = 12;
            }
        }

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            lose(true);
        }
        find_all_alive_units(U_CATAPULT);
        if (units == 0)
        {
            find_all_alive_units(U_PALADIN);
            if (units != 0)
            {
                WORD hp = *((WORD*)((uintptr_t)unit[0] + S_HP));
                WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * U_PALADIN);//max hp
                if (hp == mhp)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        if (i != P_VIOLET)
                        {
                            *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                            *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                        }
                    }
                    lose(true);
                }
            }
        }
	}
}

void v_human14(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
        trigger_time(40);
        manacost(HASTE, 30);
        upgr(SWORDS, 0);
        repair_cat(true);
        comps_vision(true);
        ua[0] = U_CATAPULT;
        ut[0] = U_PALADIN;
        ua[1] = U_HWALL;
        ut[1] = U_HWALL;
        ua[2] = U_RUNESTONE;
        ut[2] = U_RUNESTONE;
        ua[3] = U_MINE;
        ut[3] = U_MINE;
        ua[4] = U_PIGFARM;
        ut[4] = U_PIGFARM;
        ua[5] = U_OSMITH;
        ut[5] = U_OSMITH;
        ua[6] = U_ALCHEMIST;
        ut[6] = U_ALCHEMIST;
        ua[7] = U_OGREMOUND;
        ut[7] = U_OGREMOUND;
        ua[8] = U_HCANONTOWER;
        ut[8] = U_HCANONTOWER;
	}
	else
	{
		//your custom victory conditions
        if (*(byte*)GB_HORSES == 0)
        {
            *(byte*)LEVEL_OBJ = LVL_HUMAN12;
            return;
        }
        vizs_n = 0;
        trigger_time(40);
        manacost(HASTE, 30);
        upgr(SWORDS, 0);
        repair_cat(true);
        comps_vision(true);

        save_book();

        set_upgrade(SWORDS, P_RED, 98);
        viz_area_add(21, 41, 1 << P_VIOLET, 9);
        viz_area_add(41, 41, 1 << P_VIOLET, 9);
        viz_area_add(21, 21, 1 << P_VIOLET, 9);
        viz_area_add(41, 21, 1 << P_VIOLET, 9);
        viz_area_add(21, 31, 1 << P_VIOLET, 9);
        viz_area_add(31, 21, 1 << P_VIOLET, 9);
        viz_area_add(41, 31, 1 << P_VIOLET, 9);
        viz_area_add(31, 41, 1 << P_VIOLET, 9);
        viz_area_add(31, 31, 1 << P_VIOLET, 9);

        if (*(byte*)GB_HORSES == 21)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            *(WORD*)LEVEL_ID = 0x52e2;//human14 file num
            win(true);
        }
        if (*(byte*)GB_HORSES == 19)
        {
            viz_area_add(57, 52, 1 << P_VIOLET, 9);
            find_all_alive_units(U_SKELETON);
            sort_stat(S_OWNER, P_ORANGE, CMP_EQ);
            if (units==0)unit_create(64, 47, U_SKELETON, P_ORANGE, 1);
            find_all_alive_units(U_SKELETON);
            sort_stat(S_OWNER, P_WHITE, CMP_EQ);
            if (units == 0)unit_create(64, 49, U_SKELETON, P_WHITE, 1);
            find_all_alive_units(U_SKELETON);
            sort_stat(S_OWNER, P_YELLOW, CMP_EQ);
            if (units == 0)unit_create(64, 51, U_SKELETON, P_YELLOW, 1);
            find_all_alive_units(U_SKELETON);
            order_all(47, 50, ORDER_ATTACK_AREA);
            set_stat_all(S_AI_DEST_X, 47);
            set_stat_all(S_AI_DEST_Y, 50);

            find_all_alive_units(U_PEON);
            if (units == 0)
            {
                for (int i = 0; i < 8; i++)
                {
                    if (i != P_VIOLET)
                    {
                        *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                        *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                    }
                }
                lose(true);
            }

            find_all_alive_units(U_CATAPULT);
            WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * U_CATAPULT);//max hp
            sort_stat(S_HP, mhp, CMP_EQ);
            if (units != 0)
            {
                give_all(P_VIOLET);
                *(byte*)LEVEL_OBJ = LVL_HUMAN12;
                *(byte*)GB_HORSES = 20;
            }
        }
        if (*(byte*)GB_HORSES == 18)
        {
            find_all_alive_units(U_DANATH);
            if (units != 0)
            {
                show_message_from_tbl(20, tbl_dialog, 71);
                show_message_from_tbl(20, tbl_dialog, 72);
                sound_play_dialog(18);
                set_stat_all(S_FLASH_COUNTER, 80);
                set_stat_all(S_FLASH_COLOR, 253);
            }
            viz_area_add(57, 52, 1 << P_VIOLET, 9);
            *(byte*)GB_HORSES = 19;
        }
        if (*(byte*)GB_HORSES == 17)
        {
            viz_area_add(57, 52, 1 << P_VIOLET, 9);
            *(byte*)GB_HORSES = 18;
        }
        if (*(byte*)GB_HORSES == 16)
        {
            viz_area_add(57, 52, 1 << P_VIOLET, 9);
            unit_create(64, 47, U_SKELETON, P_ORANGE, 1);
            unit_create(64, 49, U_SKELETON, P_WHITE, 1);
            unit_create(64, 51, U_SKELETON, P_YELLOW, 1);
            find_all_alive_units(U_SKELETON);
            order_all(47, 50, ORDER_ATTACK_AREA);
            set_stat_all(S_AI_DEST_X, 47);
            set_stat_all(S_AI_DEST_Y, 50);
            *(byte*)GB_HORSES = 17;
        }
        if (*(byte*)GB_HORSES == 15)
        {
            viz_area_add(57, 52, 1 << P_VIOLET, 9);
            *(byte*)GB_HORSES = 16;
        }
        if (*(byte*)GB_HORSES == 14)
        {
            find_all_alive_units(U_SKELETON);
            kill_all();
            set_region(46, 50, 51, 50);
            find_all_alive_units(U_PEON);
            if (units == 0)
            {
                for (int i = 0; i < 8; i++)
                {
                    if (i != P_VIOLET)
                    {
                        *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                        *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                    }
                }
                lose(true);
            }
            sort_in_region();
            if (units != 0)
            {
                *(byte*)GB_HORSES = 15;
            }
        }

        find_all_alive_units(U_DANATH);
        if (units == 0)
        {
            for (int i = 0; i < 8; i++)
            {
                if (i != P_VIOLET)
                {
                    *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                    *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                }
            }
            lose(true);
        }
        find_all_alive_units(U_CATAPULT);
        if (units == 0)
        {
            find_all_alive_units(U_PALADIN);
            if (units != 0)
            {
                WORD hp = *((WORD*)((uintptr_t)unit[0] + S_HP));
                WORD mhp = *(WORD*)(UNIT_HP_TABLE + 2 * U_PALADIN);//max hp
                if (hp == mhp)
                {
                    for (int i = 0; i < 8; i++)
                    {
                        if (i != P_VIOLET)
                        {
                            *(byte*)(CONTROLER_TYPE + i) = C_NOBODY;
                            *(byte*)(STARTING_CONTROLER_TYPE + i) = C_NOBODY;
                        }
                    }
                    lose(true);
                }
            }
        }
	}
}

void v_xhuman1(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman2(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman3(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman4(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman5(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman6(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman7(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman8(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman9(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman10(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman11(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xhuman12(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
        win(true);
	}
}

void v_orc1(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc2(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc3(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc4(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc5(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc6(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc7(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc8(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc9(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc10(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc11(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc12(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc13(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_orc14(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc1(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc2(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc3(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc4(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc5(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc6(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc7(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc8(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc9(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc10(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc11(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_xorc12(bool rep_init)
{
	if (rep_init)
	{
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}

void v_custom(bool rep_init)
{
	if (rep_init)
	{
		//pathfind_fix(true);
		//ai_fix_plugin(true);
		//your initialize
	}
	else
	{
		//your custom victory conditions
	}
}
//-------------------------------------------------------------------------------

void (*triggers[])(bool) = { v_human1,v_orc1,v_human2,v_orc2,v_human3,v_orc3,v_human4,v_orc4,v_human5,v_orc5,v_human6,v_orc6,v_human7,v_orc7,v_human8,v_orc8,v_human9,v_orc9,v_human10,v_orc10,v_human11,v_orc11,v_human12,v_orc12,v_human13,v_orc13,v_human14,v_orc14,v_xhuman1,v_xorc1,v_xhuman2,v_xorc2,v_xhuman3,v_xorc3,v_xhuman4,v_xorc4,v_xhuman5,v_xorc5,v_xhuman6,v_xorc6,v_xhuman7,v_xorc7,v_xhuman8,v_xorc8,v_xhuman9,v_xorc9,v_xhuman10,v_xorc10,v_xhuman11,v_xorc11,v_xhuman12,v_xorc12 };

void trig()
{
    byte lvl = *(byte*)LEVEL_OBJ;
    if (a_custom)
    {
        v_custom(false);
    }
    else
    {
        if ((lvl >= 0) && (lvl < 52))
            ((void (*)(bool))triggers[lvl])(false);
        else
            v_custom(false);
    }
    global_trigs();
	first_step = false;
}

void trig_init()
{
	first_step = true;
    byte lvl = *(byte*)LEVEL_OBJ;
    if (a_custom)
    {
        v_custom(true);
    }
    else
    {
        if ((lvl >= 0) && (lvl < 52))
            ((void (*)(bool))triggers[lvl])(true);
        else
            v_custom(true);
    }
    if ((lvl >= LVL_HUMAN9) && (lvl <= LVL_HUMAN11))disable_minimap_units(true);
    else disable_minimap_units(false);
}

void replace_def()
{
    //set all vars to default
    memset(ua, 255, sizeof(ua));
    memset(ut, 255, sizeof(ut));
	memset(vizs_areas, 0, sizeof(vizs_areas));
	vizs_n = 0;
    ai_fixed = false;
	saveload_fixed = false;
}

void replace_common()
{
    //peon can build any buildings
    char ballbuildings[] = "\x0\x0";//d1 05
    PATCH_SET((char*)BUILD_ALL_BUILDINGS1, ballbuildings);
    char ballbuildings2[] = "\x0";//0a
    PATCH_SET((char*)BUILD_ALL_BUILDINGS2, ballbuildings2);
    char ballbuildings3[] = "\x0";//68
    PATCH_SET((char*)BUILD_ALL_BUILDINGS3, ballbuildings3);

    //any building can train any unit
    char ballunits[] = "\xeb";//0x74
    PATCH_SET((char*)BUILD_ALL_UNITS1, ballunits);
    char ballunits2[] = "\xA1\xBC\x47\x49\x0\x90\x90";//8b 04 85 bc 47 49 00
    PATCH_SET((char*)BUILD_ALL_UNITS2, ballunits2);

    //any building can make any research
    char allres[] = "\xB1\x1\x90";
    PATCH_SET((char*)BUILD_ALL_RESEARCH1, allres);
    PATCH_SET((char*)BUILD_ALL_RESEARCH2, allres);

    //allow all units cast all spells
    char allsp[] = "\x90\x90";
    PATCH_SET((char*)CAST_ALL_SPELLS, allsp);

    //show kills
    byte d = S_KILLS;
    char sdmg[] = "\x8a\x90\x82\x0\x0\x0\x8b\xfa";//units
    sdmg[2] = d;
    PATCH_SET((char*)SPEED_STAT_UNITS, sdmg);
    char sdmg2[] = "\x8a\x82\x82\x0\x0\x0\x90\x90\x90";//catas
    sdmg2[2] = d;
    PATCH_SET((char*)SPEED_STAT_CATS, sdmg2);
    char sdmg3[] = "\x8a\x88\x82\x0\x0\x0\x90\x90\x90";//archers
    sdmg3[2] = d;
    PATCH_SET((char*)SPEED_STAT_ARCHERS, sdmg3);
    char sdmg4[] = "\x8a\x82\x82\x0\x0\x0\x90\x90\x90";//berserkers
    sdmg4[2] = d;
    PATCH_SET((char*)SPEED_STAT_BERSERKERS, sdmg4);
    char sdmg5[] = "\x8a\x88\x82\x0\x0\x0\x90\x90\x90";//ships
    sdmg5[2] = d;
    PATCH_SET((char*)SPEED_STAT_SHIPS, sdmg5);

    char dmg_fix[] = "\xeb";
    PATCH_SET((char*)DMG_FIX, dmg_fix);

    draw_stats_fix(true);
}

void replace_back()
{
    //replace all to default
    comps_vision(false);
    repair_cat(false);
    trigger_time('\xc8');
    upgr(SWORDS, 2);
    upgr(ARMOR, 2);
    upgr(ARROWS, 1);
    upgr(SHIP_DMG, 5);
    upgr(SHIP_ARMOR, 5);
    upgr(CATA_DMG, 15);
    manacost(VISION, 70);
    manacost(HEAL, 6);
    manacost(GREATER_HEAL, 5);
    manacost(EXORCISM, 4);
    manacost(FIREBALL, 100);
    manacost(FLAME_SHIELD, 80);
    manacost(SLOW, 50);
    manacost(INVIS, 200);
    manacost(POLYMORPH, 200);
    manacost(BLIZZARD, 25);
    manacost(EYE_OF_KILROG, 70);
    manacost(BLOOD, 50);
    manacost(RAISE_DEAD, 50);
    manacost(COIL, 100);
    manacost(WHIRLWIND, 100);
    manacost(HASTE, 50);
    manacost(UNHOLY_ARMOR, 100);
    manacost(RUNES, 200);
    manacost(DEATH_AND_DECAY, 25);
    draw_stats_fix(false);
	ai_fix_plugin(false);
}

void replace_trigger()
{
    replace_back();
    replace_def();
    replace_common();

    buttons_init_all();

    //replace original victory trigger
    char trig_jmp[] = "\x74\x1A";//74 0F
    PATCH_SET((char*)VICTORY_JMP, trig_jmp);
    char rep[] = "\xc7\x05\x38\x0d\x4c\x0\x30\x8C\x45\x0";
    void (*repf) () = trig;
    patch_setdword((DWORD*)(rep + 6), (DWORD)repf);
    PATCH_SET((char*)VICTORY_TRIGGER, rep);
	trig_init();
}

PROC g_proc_0042A4A1;
void new_game(int a, int b, long c)
{
    if (remember_music != 101)
        *(DWORD*)VOLUME_MUSIC = remember_music;
    if (remember_sound != 101)
        *(DWORD*)VOLUME_SOUND = remember_sound;
    ((void (*)(DWORD))F_SET_VOLUME)(SET_VOLUME_PARAM);//set volume
    remember_music = 101;
    remember_sound = 101;

    a_custom = b % 256;//custom game or campaign
	if (a_custom)*(byte*)LEVEL_OBJ=53;//remember custom obj
	else
	{
	if (*(byte*)LEVEL_OBJ==53)a_custom=1;//fix for when saved game loads custom get broken
	}
    replace_trigger();
    memset((void*)GB_HORSES, 0, 16 * sizeof(byte));
    ((void (*)(int, int, long))g_proc_0042A4A1)(a, b, c);//original
}

PROC g_proc_0041F7E4;
int load_game(int a)
{
    int original = ((int (*)(int))g_proc_0041F7E4)(a);//original
    replace_trigger();

    return original;
}

PROC g_proc_0045614E;
void receive_cheat_single(int c, int a1)
{
    char buf[] = "azaz";
    show_message(20, buf);
}

void drawing()
{
    ((void (*)(int, int, int, int))F_INVALIDATE)(0, 0, m_screen_w, m_screen_h);
}

PROC g_proc_00421F57;
void draw_hook3()
{
    ((void (*)())g_proc_00421F57)();//original
    drawing();
}

void hook(int adr, PROC* p, char* func)
{
    *p = patch_call((char*)adr, func);
}

void common_hooks()
{
    hook(0x0045271B, &g_proc_0045271B, (char*)update_spells);
    hook(0x004522B9, &g_proc_004522B9, (char*)seq_run);

    hook(0x0041038E, &g_proc_0041038E, (char*)damage1);
    hook(0x00409F3B, &g_proc_00409F3B, (char*)damage2);
    hook(0x0040AF70, &g_proc_0040AF70, (char*)damage3);
    hook(0x0040AF99, &g_proc_0040AF99, (char*)damage4);
    hook(0x00410762, &g_proc_00410762, (char*)damage5);
    hook(0x004428AD, &g_proc_004428AD, (char*)damage6);

    hook(0x0040DF71, &g_proc_0040DF71, (char*)bld_unit_create);
    hook(0x00451728, &g_proc_00451728, (char*)unit_kill_deselect);
    hook(0x00452939, &g_proc_00452939, (char*)grow_struct_count_tables);
    hook(0x0042479E, &g_proc_0042479E, (char*)peon_into_goldmine);
    hook(0x00424745, &g_proc_00424745, (char*)goods_into_inventory);
    hook(0x004529C0, &g_proc_004529C0, (char*)goods_into_inventory);
	hook(0x00451054, &g_proc_00451054, (char*)count_add_to_tables_load_game);
    hook(0x00438A5C, &g_proc_00438A5C, (char*)unset_peon_ai_flags);
    hook(0x00438985, &g_proc_00438985, (char*)unset_peon_ai_flags);
	
	hook(0x0040EEDD, &g_proc_0040EEDD, (char*)upgrade_tower);
    hook(0x00442E25, &g_proc_00442E25, (char*)create_skeleton);
    hook(0x00425D1C, &g_proc_00425D1C, (char*)cast_raise);
    hook(0x0042757E, &g_proc_0042757E, (char*)ai_spell);
    hook(0x00427FAE, &g_proc_00427FAE, (char*)ai_attack);
	
	hook(0x0042A4A1, &g_proc_0042A4A1, (char*)new_game);
	hook(0x0041F7E4, &g_proc_0041F7E4, (char*)load_game);
}

void rocks_hooks()
{
    patch_ljmp((char*)0x0044E74B, (char*)finish_not_function);
    char buf[] = "\x57\x66\x8B\x4\x48\x66\x3D\xFD\xFF\x74\x0A\x66\x3D\xFF\xFF\xF\x85\x0\x0\x0\x0\xE9\x0\x0\x0\x0";
    PATCH_SET((char*)finish_not_function, buf);
    patch_ljmp((char*)finish_not_function + 16, (char*)0x0044E835);//return
    patch_ljmp((char*)finish_not_function + 21, (char*)0x0044E758);//continue
    char buf2[] = "\x85";
    PATCH_SET((char*)finish_not_function + 16, buf2);//repair jnz

    hook(0x0043B888, &g_proc_0043B888, (char*)rclick_get_unit_first);
    hook(0x0043B94B, &g_proc_0043B94B, (char*)rclick_get_unit_next);
    hook(0x0043B779, &g_proc_0043B779, (char*)rclick_tile_is_chopping_tree);
    hook(0x0043B768, &g_proc_0043B768, (char*)rclick_tile_is_tree);
    hook(0x0042411E, &g_proc_0042411E, (char*)dispatch_tile_is_chopping_tree);
    hook(0x0042412B, &g_proc_0042412B, (char*)dispatch_tile_cancel_tree_harvest);
    hook(0x00451554, &g_proc_00451554, (char*)dispatch_tile_cancel_tree_harvest);
    hook(0x00453001, &g_proc_00453001, (char*)dispatch_tile_cancel_tree_harvest);
    hook(0x0045307D, &g_proc_0045307D, (char*)dispatch_tile_cancel_tree_harvest);
    hook(0x004241AD, &g_proc_004241AD, (char*)dispatch_tile_finish_tree_harvest);
    hook(0x00424215, &g_proc_00424215, (char*)dispatch_tile_is_tree);
    hook(0x0042423D, &g_proc_0042423D, (char*)dispatch_tile_start_tree_harvest);
    hook(0x0042413C, &g_proc_0042413C, (char*)dispatch_tile_find_tree);
    hook(0x00424262, &g_proc_00424262, (char*)dispatch_tile_find_tree);
    hook(0x004377EC, &g_proc_004377EC, (char*)dispatch_tile_find_tree);
}

void files_hooks()
{
    files_init();

    hook(0x004542FB, &g_proc_004542FB, (char*)grp_draw_cross);
    hook(0x00454DB4, &g_proc_00454DB4, (char*)grp_draw_bullet);
    hook(0x00454BCA, &g_proc_00454BCA, (char*)grp_draw_unit);
    hook(0x00455599, &g_proc_00455599, (char*)grp_draw_building);
    hook(0x0043AE54, &g_proc_0043AE54, (char*)grp_draw_building_placebox);
    hook(0x0044538D, &g_proc_0044538D, (char*)grp_portrait_init);
    hook(0x004453A7, &g_proc_004453A7, (char*)grp_draw_portrait);
    hook(0x004452B0, &g_proc_004452B0, (char*)grp_draw_portrait_buttons);
    hook(0x0044A65C, &g_proc_0044A65C, (char*)status_get_tbl);
    hook(0x0044AC83, &g_proc_0044AC83, (char*)unit_hover_get_id);
    hook(0x0044AE27, &g_proc_0044AE27, (char*)unit_hover_get_tbl);
    hook(0x0044AE56, &g_proc_0044AE56, (char*)button_description_get_tbl);
    hook(0x0044B23D, &g_proc_0044B23D, (char*)button_hotkey_get_tbl);
    hook(0x004354C8, &g_proc_004354C8, (char*)objct_get_tbl_custom);
    hook(0x004354FA, &g_proc_004354FA, (char*)objct_get_tbl_campanign);
    hook(0x004300A5, &g_proc_004300A5, (char*)objct_get_tbl_briefing_task);
    hook(0x004300CA, &g_proc_004300CA, (char*)objct_get_tbl_briefing_title);
    hook(0x004301CA, &g_proc_004301CA, (char*)objct_get_tbl_briefing_text);
    hook(0x00430113, &g_proc_00430113, (char*)objct_get_briefing_speech);
    hook(0x0041F0F5, &g_proc_0041F0F5, (char*)finale_get_tbl);
    hook(0x0041F1E8, &g_proc_0041F1E8, (char*)finale_credits_get_tbl);
    hook(0x0041F027, &g_proc_0041F027, (char*)finale_get_speech);
    hook(0x00417E33, &g_proc_00417E33, (char*)credits_small_get_tbl);
    hook(0x00417E4A, &g_proc_00417E4A, (char*)credits_big_get_tbl);
    hook(0x0042968A, &g_proc_0042968A, (char*)act_get_tbl_small);
    hook(0x004296A9, &g_proc_004296A9, (char*)act_get_tbl_big);
    hook(0x0041C51C, &g_proc_0041C51C, (char*)netstat_get_tbl_nation);
    hook(0x00431229, &g_proc_00431229, (char*)rank_get_tbl);
    hook(0x004372EE, &g_proc_004372EE, (char*)pcx_load_menu);
    hook(0x00430058, &g_proc_00430058, (char*)pcx_load_briefing);
    hook(0x00429625, &g_proc_00429625, (char*)pcx_load_act);
    hook(0x00429654, &g_proc_00429654, (char*)pcx_load_act);
    hook(0x0041F004, &g_proc_0041F004, (char*)pcx_load_final);
    hook(0x00417DDB, &g_proc_00417DDB, (char*)pcx_load_credits);
    hook(0x0043169E, &g_proc_0043169E, (char*)pcx_load_statistic);
    hook(0x00462D4D, &g_proc_00462D4D, (char*)storm_file_load);
    hook(0x0041F9FD, &g_proc_0041F9FD, (char*)tilesets_load);
    
    hook(0x0041F97D, &g_proc_0041F97D, (char*)map_file_load);

    hook(0x0042A443, &g_proc_0042A443, (char*)act_init);

    hook(0x0044F37D, &g_proc_0044F37D, (char*)main_menu_init);

    hook(0x0043B16F, &g_proc_0043B16F, (char*)smk_play_sprintf_name);
    hook(0x0043B362, &g_proc_0043B362, (char*)smk_play_sprintf_blizzard);

    hook(0x00440F4A, &g_proc_00440F4A, (char*)music_play_get_install);
    hook(0x00440F5F, &g_proc_00440F5F, (char*)music_play_get_install);
    patch_call((char*)0x00440F41, (char*)music_play_sprintf_name);
    char buf[] = "\x90";
    PATCH_SET((char*)(0x00440F41 + 5), buf);//7 bytes call

    hook(0x00424A9C, &g_proc_00424A9C, (char*)storm_font_load);
    hook(0x00424AB2, &g_proc_00424AB2, (char*)storm_font_load);
    hook(0x004288B2, &g_proc_004288B2, (char*)storm_font_load);
    hook(0x00428896, &g_proc_00428896, (char*)storm_font_load);
    hook(0x0042887D, &g_proc_0042887D, (char*)storm_font_load);

    hook(0x00422D76, &g_proc_00422D76, (char*)sound_play_unit_speech);
    hook(0x00422D5F, &g_proc_00422D5F, (char*)sound_play_unit_speech_soft);
}

void fix_lesnik_credits()
{
    bool f2020 = true;
    char buf2020[] = "\x9b\xf6\xc1\x20\x74\x6\xd9\xeb\xdd\x5d\xf8\x9b\xc9\xc3";
    for (int i = 0; i < 14; i++)
    {
        if ((*(char*)(0x0048F2E0 + i)) != buf2020[i])f2020 = false;
    }
    if (f2020)
    {
        char buf[] = "\x68\x18\x01\x00\x00\x68\xBC\x56\x49\x00\x68\x30\x2D\x46\x00\x68\xA8\x56\x49\x00\xE8\x27\x1D\xFF\xFF\x68\x1C\x01\x00\x00\x68\xBC\x56\x49\x00\x68\xA0\x2F\x46\x00\x68\x30\x2D\x46\x00\x68\x94\x56\x49\x00\xA3\x80\xE0\x4A\x00\xC7\x05\x88\xE0\x4A\x00\x84\x56\x49\x00\xC7\x05\x84\xE0\x4A\x00\x01\x00\x00\x00\xE8\x90\xD0\xFE\xFF\x68\x00\x7D\x41\x00\x6A\x04\x50\xE8\xD3\xDA\xFE\xFF\x83\xC4\x30\x83\xF8\xFE\x0F\x84\x89\x00\x00\x00\x68\x22\x01\x00\x00\x68\xBC\x56\x49\x00\x68\xA0\x2F\x46\x00\x68\x30\x2D\x46\x00\x68\x94\x56\x49\x00\xC7\x05\x88\xE0\x4A\x00\x70\x56\x49\x00\xC7\x05\x84\xE0\x4A\x00\x03\x00\x00\x00\xE8\x45\xD0\xFE\xFF\x68\x00\x7D\x41\x00\x6A\x04\x50\xE8\x88\xDA\xFE\xFF\x83\xC4\x20\x83\xF8\xFE\x74\x42\x68\x29\x01\x00\x00\x68\xBC\x56\x49\x00\x68\xA0\x2F\x46\x00\x68\x30\x2D\x46\x00\x68\x94\x56\x49\x00\xC7\x05\x88\xE0\x4A\x00\x84\x56\x49\x00\xC7\x05\x84\xE0\x4A\x00\x05\x00\x00\x00\xE8\xFE\xCF\xFE\xFF\x68\x00\x7D\x41\x00\x6A\x04\x50\xE8\x41\xDA\xFE\xFF\x83\xC4\x20\xA1\x80\xE0\x4A\x00\x68\x2D\x01\x00\x00\x68\xBC\x56\x49\x00\x50\xE8\x89\xB2\x04\x00\x83\xC4\x0C\xC3";
        PATCH_SET((char*)0x00417BF0, buf);
    }
}

bool dll_called = false;

extern "C" __declspec(dllexport) void w2p_init()
{
    DWORD check_exe = *(DWORD*)0x48F2F0;
    if (check_exe == 0x4E4F5453)
    {
        if (!dll_called)
        {
            fix_lesnik_credits();

            m_screen_w = *(WORD*)SCREEN_SIZE_W;
            m_screen_h = *(WORD*)SCREEN_SIZE_H;
            hook(0x00421F57, &g_proc_00421F57, (char*)draw_hook3);

            common_hooks();
            rocks_hooks();
            files_hooks();

            buttons_init_cata();

            sounds_ready_table_set(U_LOTHAR, 145);

            hook(0x0041F915, &g_proc_0041F915, (char*)map_load);
            hook(0x00418937, &g_proc_00418937, (char*)dispatch_die_unitdraw_update_1_man);

            char buf[] = "\x90\x90\x90";
            PATCH_SET((char*)0x0045158A, buf);//peon die with res sprite bug fix
            hook(0x00451590, &g_proc_00451590, (char*)unit_kill_peon_change);

            char buf_showpath[] = "\xB0\x1\x90\x90\x90";
            PATCH_SET((char*)0x00416691, buf_showpath);
            hook(0x0045614E, &g_proc_0045614E, (char*)receive_cheat_single);

            *(byte*)(0x0049D93C) = 0;//remove text
            *(byte*)(0x0049DC24) = 0;//from main menu
            patch_call((char*)0x004886B3, (char*)0x0048CCA2);//remove fois HD text from main menu

            char buf_i[] = "\xE8\x1B\x31\x5\x0";
            PATCH_SET((char*)0x00428E08, buf_i);//fix fois install

            char buf_b[] = "\x90\x90";
            PATCH_SET((char*)0x0040FCF3, buf_b);//fix big bullet shoot create fire

            char buf_unload_check[] = "\x0\x0\x0\x0";
            PATCH_SET((char*)0x0048F2F0, buf_unload_check);//dll unload STON bytes

            dll_called = true;
        }
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID)
{
    DWORD check_exe = *(DWORD*)0x48F2F0;
    if (check_exe == 0x4E4F5453)//STON
    {
        if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        {
            bool ex = false;
            FILE* file;
            if ((file = fopen("INSTALL.MPQ", "r")))
            {
                fclose(file);
                ex = true;
            }
            if (ex)
            {
                char mpq[] = "mpq";
                PATCH_SET((char*)(0x49DBFC + 9), mpq);//install exe/mpq name
            }
            w2p_init();
        }
    }
    return TRUE;
}