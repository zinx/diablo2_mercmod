#include "common.h"

#include "d2.h"
#include "mercmod.h"
#include <patch.h>

#include "util.h"

#define MOD_NAME "mercmod"
#define AUTHOR "Zinx Verituse <zinx@zenthought.org>"
#define FULL_NAME MOD_NAME " by "AUTHOR" [built @ "__DATE__" "__TIME__"]"

#define INI_NAME MOD_NAME ".ini"
#define MPQ_NAME MOD_NAME ".mpq"

#define MERCOPT_MAX (5+7)
#define MERCOPT_TYPE_MAX 32

typedef struct mercopt_s {
	int merc_class;
	int allow_class;
	int allow_2h;
	int dual_types;
	int dual_type[MERCOPT_TYPE_MAX];
	int except_types;
	int except_type[MERCOPT_TYPE_MAX];
	int allow_types;
	int allow_type[MERCOPT_TYPE_MAX];
	int deny_types;
	int deny_type[MERCOPT_TYPE_MAX];
} mercopt_t;

/* defaults */
static mercopt_t mercopt[MERCOPT_MAX] = {
/*Act1*/	{
			271, 1<<PC_AMAZON, 0,
			0, {},
			1, {IT_SHIELD},
			5, {IT_MISSILE_WEAPON, IT_MISSILE, IT_RING, IT_AMULET, IT_ANY_ARMOR},
			1, {IT_WEAPON},
		},
/*Act2*/	{
			338, 1<<PC_PALADIN, 1,
			0, {},
			1, {IT_SHIELD},
			5, {IT_POLEARM, IT_SPEAR, IT_RING, IT_AMULET, IT_ANY_ARMOR},
			1, {IT_WEAPON},
		},
/*Act3*/		{
			359, 1<<PC_SORCERESS, 0,
			0, {},
			0, {},
			5, {IT_SWORD, IT_RING, IT_AMULET, IT_ANY_ARMOR},
			1, {IT_WEAPON},
		},
/*Act4*/	{
			561, 1<<PC_BARBARIAN, 1,
			1, {IT_AXE},
			1, {IT_SHIELD},
			4, {IT_MELEE_WEAPON, IT_RING, IT_AMULET, IT_ANY_ARMOR},
			1, {IT_WEAPON},
		},
/*Act0*/	{
			560, 1<<PC_BARBARIAN, 1,
			1, {IT_SWORD},
			1, {IT_SHIELD},
			4, {IT_MELEE_WEAPON, IT_RING, IT_AMULET, IT_ANY_ARMOR},
			1, {IT_WEAPON},
		},
};

static mercopt_t *mercopt_for_class(int merc_class)
{
	int i;
	if (!merc_class) return NULL;
	for (i = 0; i < MERCOPT_MAX; ++i) {
		if (merc_class == mercopt[i].merc_class)
			return &mercopt[i];
	}
	return NULL;
}

STDCALL int merc_check_item_compat(d2_unit_t *merc, d2_item_t *item, d2_item_t *olditem, d2_loc_t loc, d2_item_t *othitem, d2_loc_t othloc)
{
	int i;
	mercopt_t *opt = mercopt_for_class(unit_get_class(merc));
	int equippable;

	if (!opt) {
		/* Pass through to normal checker */
		return 0;
	}

	equippable = merc_check_item_class(merc, item, item_get_required_class(item));
	if (equippable == -1)
		return -1;

	if (item && othitem) {
		for (i = 0; i < opt->dual_types; ++i) {
			if (item_check_type(item, opt->dual_type[i])) {
				if (item_check_type(othitem, opt->dual_type[i])) {
					/* Forcibly allow dual wielding */
					return 1;
				}
			}
		}

		if (item_check_type(item, IT_WEAPON) && item_check_type(othitem, IT_WEAPON)) {
			/* Forcibly deny dual wielding */
			return -1;
		}
	}

	return equippable;
}

STDCALL int merc_check_item_class_from_class(int merc_class, d2_item_t *item, int item_req_class)
{
	int i;
	mercopt_t *opt = mercopt_for_class(merc_class);

	if (!opt) {
		/* Pass through to normal checker */
		return 0;
	}

	for (i = 0; i < opt->except_types; ++i) {
		if (item_check_type(item, opt->except_type[i])) {
			/* Force deny */
			return -1;
		}
	}

	if (item_is_2H(item) && !item_get_shoots_type(item) && !opt->allow_2h) {
		return -1;
	}

	if (item_req_class < 7) {
		if (!(opt->allow_class & (1<<item_req_class))) {
			/* Force deny */
			return -1;
		}
	}

	for (i = 0; i < opt->allow_types; ++i) {
		if (item_check_type(item, opt->allow_type[i])) {
			/* Force allow */
			return 1;
		}
	}

	for (i = 0; i < opt->deny_types; ++i) {
		if (item_check_type(item, opt->deny_type[i])) {
			/* Force deny */
			return -1;
		}
	}

	/* Pass through to normal checker */
	return 0;
}

STDCALL int merc_check_item_class_from_id(int merc_id, d2_item_t *item, int item_req_class)
{
	return merc_check_item_class_from_class(unit_get_class_from_id(merc_id), item, item_req_class);
}

STDCALL int merc_check_item_class(d2_unit_t *merc, d2_item_t *item, int item_req_class)
{
	return merc_check_item_class_from_class(unit_get_class(merc), item, item_req_class);
}

STDCALL int merc_check_item_remove(d2_unit_t *merc, d2_item_t *item, d2_item_t *olditem, d2_loc_t loc, d2_item_t *othitem, d2_loc_t othloc, d2_item_t **remove_item)
{
	mercopt_t *opt;

	if (unit_is_inventory(merc))
		merc = merc->inv_owner_unit;

	opt = mercopt_for_class(unit_get_class(merc));

	if (!opt) {
		/* Passthrough */
		return 0;
	}

	*remove_item = NULL;

	if (item && othitem) {
		int it, ot;

		if ((ot = item_get_shoots_type(item))) {
			if (!(it = item_get_quiver_type(othitem))
			    || !item_check_type(item, it)
			    || !item_check_type(othitem, ot)
			) {
				*remove_item = othitem;
			}
		} else if (item_is_2H(item)) {
			*remove_item = othitem;
		} else if ((it = item_get_shoots_type(othitem))) {
			if (!(ot = item_get_quiver_type(item))
			    || !item_check_type(item, it)
			    || !item_check_type(othitem, ot)
			) {
				*remove_item = othitem;
			}
		} else if (item_is_2H(othitem)) {
			*remove_item = othitem;
		} else if (item_get_quiver_type(item) || item_get_quiver_type(othitem)) {
			*remove_item = othitem;
		} else if (merc_check_item_compat(merc, item, olditem, loc, othitem, othloc) == -1) {
			*remove_item = othitem;
		}
	}

	if (olditem) {
		if (*remove_item) {
			/* Force deny */
			return -1;
		}

		*remove_item = olditem;
	}

	return 1;
}

#define swap(a,b) ({__typeof(a) _tmp = (a); (a) = (b); (b) = _tmp;})

STDCALL void merc_choose_item_loc(d2_unit_t *merc, d2_item_t *item, d2_item_t **olditem, d2_loc_t *loc, d2_item_t **othitem, d2_loc_t *othloc)
{
	if (unit_is_inventory(merc))
		merc = merc->inv_owner_unit;

	item_get_equip_locs(item, loc, othloc);
	*olditem = unit_get_equipped_item(merc->inv, *loc);
	*othitem = unit_get_equipped_item(merc->inv, *othloc);

	if (item_check_type(item, IT_SECOND_HAND)) {
		swap(*loc, *othloc);
		swap(*olditem, *othitem);
	}

	if (
	    ((*olditem && !*othitem) || merc_check_item_compat(merc, item, *olditem, *loc, *othitem, *othloc) == -1)
	    && merc_check_item_compat(merc, item, *othitem, *othloc, *olditem, *loc) != -1
	) {
		swap(*loc, *othloc);
		swap(*olditem, *othitem);
	}
}

#define INVREC_MAX 2
static int invrec_idx[INVREC_MAX] = {13, 29};
static d2_invrec_t invrec[INVENTORY_REC_MAX][INVENTORY_REC_ITEM_MAX] = {
	{
		{-1,-1,-1,-1,0,0},
		{-1,-1,-1,-1,29,29},
		{20,75,4,116,55,112},
		{136,191,64,147,55,83},
		{251,307,4,116,56,112},
		{136,191,3,59,55,56},
		{208,233,32,58,25,26},
		{93,118,121,147,25,26},
		{208,233,122,148,25,26},
		{136,191,154,178,55,24},
		{251,306,122,178,55,56},
		{20,75,121,178,55,57},
	},
	{
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,29,29},
		{100,155,64,176,55,112},
		{216,271,124,207,55,83},
		{331,387,64,176,56,112},
		{216,271,63,119,55,56},
		{288,313,92,118,25,26},
		{173,198,181,207,25,26},
		{288,313,182,208,25,26},
		{216,271,214,238,55,24},
		{331,386,182,238,55,56},
		{100,155,181,238,55,57},
	}
};

STDCALL void merc_inventory_modify(d2_invrec_t inv[INVENTORY_REC_MAX][INVENTORY_REC_ITEM_MAX])
{
	int i;

	for (i = 0; i < INVREC_MAX; ++i) {
		if (invrec_idx[i] >= 0) {
			memcpy(inv[invrec_idx[i]], invrec[i], sizeof(invrec[i]));
			LOG("Patched inventory record %d", invrec_idx[i]);
		}
	}
}

static int merc_load_image_mpq = 1;
static HANDLE merc_mpq;

STDCALL int merc_inventory_mpq_load(void)
{
	char path[MAX_PATH], *game_path;

	if (!merc_load_image_mpq)
		return 1;

	if (SFileOpenArchive(MPQ_NAME, 6100, 2, &merc_mpq)) {
		LOG("Loaded \""MPQ_NAME"\" from current directory");
		return 1;
	}

	game_path = util_game_path();
	snprintf(path, sizeof(path), "%s/%s", game_path, MPQ_NAME);

	if (SFileOpenArchive(path, 6100, 2, &merc_mpq)) {
		LOG("Loaded \""MPQ_NAME"\" from \"%s\"", game_path);
		return 1;
	}

	LOG("Unable to load \""MPQ_NAME"\" or \"%s\".", path);
	return 1;
}

STDCALL void merc_inventory_mpq_unload(void)
{
	if (!merc_mpq)
		return;

	SFileCloseArchive(merc_mpq);
}

static int merc_parse_array(char *value, int *array, int max)
{
	int *v = array;

	value += strspn(value, " \t");

	while (max) {
		char *end;
		*v++ = strtol(value, &end, 0);
		--max;

		end += strspn(end, " \t");
		if (*end == ',') ++end;
		end += strspn(end, " \t");
		if (end == value) return 0;
		if (!*end) break;
		value = end;
	}

	return v - array;
}

static void merc_ini_load(void)
{
	char *value;
	int i;
	ini_t *ini = ini_load(INI_NAME);
	if (!ini) {
		LOG("No "INI_NAME" found.");
		return;
	}

	while (ini_seek_section(ini, NULL)) {
		int i;
		char *end;
		char *section = ini_section(ini);

		if (strncasecmp(section, "merc", 4) != 0)
			continue;

		i = strtoul(&section[4], &end, 10) - 1;
		if (!section[4] || *end) continue;
		if (i < 0 || i >= MERCOPT_MAX) continue;

		while (ini_seek_setting(ini, NULL)) {
			char *key = ini_key(ini);
			char *value = ini_value(ini);

			if (strcasecmp(key, "class") == 0) {
				mercopt[i].merc_class = strtoul(value, NULL, 0);
			} else if (strcasecmp(key, "allow_class") == 0) {
				int j;
				int klass[7];
				int klasses = merc_parse_array(value, klass, 7);
				mercopt[i].allow_class = 0;
				for (j = 0; j < klasses; ++j)
					mercopt[i].allow_class |= 1 << klass[j];
			} else if (strcasecmp(key, "allow_2h") == 0) {
				mercopt[i].allow_2h = !!strtoul(value, NULL, 0);
			} else if (strcasecmp(key, "dual") == 0) {
				mercopt[i].dual_types = merc_parse_array(value, mercopt[i].dual_type, MERCOPT_TYPE_MAX);
			} else if (strcasecmp(key, "except") == 0) {
				mercopt[i].except_types = merc_parse_array(value, mercopt[i].except_type, MERCOPT_TYPE_MAX);
			} else if (strcasecmp(key, "allow") == 0) {
				mercopt[i].allow_types = merc_parse_array(value, mercopt[i].allow_type, MERCOPT_TYPE_MAX);
			} else if (strcasecmp(key, "deny") == 0) {
				mercopt[i].deny_types = merc_parse_array(value, mercopt[i].deny_type, MERCOPT_TYPE_MAX);
			} /* else, ignore ( option for new version? :-D ) */
		}
	}

	value = ini_get_setting(ini, "mercmod", "loadmpq");
	if (value) { merc_load_image_mpq = !!strtoul(value, NULL, 0); }

	for (i = 0; i < INVREC_MAX; ++i) {
		char setting[32];
		snprintf(setting, sizeof(setting), "inv%d", i + 1);
		value = ini_get_setting(ini, "mercmod", setting);
		if (value) {
			int j, r;
			int arr[1 + 6*INVENTORY_REC_ITEM_MAX];

			int count = merc_parse_array(value, arr, 1 + 6*INVENTORY_REC_ITEM_MAX);
			if (count <= 0) continue;

			invrec_idx[i] = arr[0];
			for (j = 1, r = 0; j <= (count-6); j += 6) {
				invrec[i][r].left = arr[j];
				invrec[i][r].right = arr[j+1];
				invrec[i][r].top = arr[j+2];
				invrec[i][r].bottom = arr[j+3];
				invrec[i][r].w = arr[j+4];
				invrec[i][r].h = arr[j+5];
			}
		}
	}

	ini_free(ini);
}

static int mercmod_init()
{
	int ret;

	merc_ini_load();
	ret = patch_d2();
	if (ret) return ret;

	merc_inventory_mpq_load();
	if (*inventory_rec_ptr)
		merc_inventory_modify(*inventory_rec_ptr);

	return 0;
}

static void mercmod_exit()
{
	unpatch_d2();
}

WINAPI BOOL DllMain (HANDLE hDll, DWORD dwReason, LPVOID lpReserved)
{
	int ret;
	switch (dwReason) {
	case DLL_PROCESS_ATTACH:
		ret = mercmod_init();
		if (!ret)
			LOG(FULL_NAME " loaded");
		else
			LOG(FULL_NAME " failed to load (%d)", ret);
		return !ret;
	case DLL_PROCESS_DETACH:
		mercmod_exit();
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
