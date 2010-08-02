#ifndef mercmod_D2_h
#define mercmod_D2_h 1

enum {
	PC_AMAZON,
	PC_SORCERESS,
	PC_NECROMANCER,
	PC_PALADIN,
	PC_BARBARIAN,
	PC_DRUID,
	PC_ASSASSIN,
	PC_MAX,
};

typedef struct d2_unit_s d2_unit_t;
struct d2_unit_s {
	int type;
	int unit_class;
	d2_unit_t *inv_owner_unit;
	int id;
	int unk10_5C[20];
	d2_unit_t *inv;
	int unk64_6C[3];
	d2_unit_t *update_unit;
	/* more fields follow */
};

enum {
	IT_SHIELD = 2,
	IT_ARMOR = 3,
	IT_BOW_QUIVER = 5,
	IT_XBOW_QUIVER = 6,
	IT_RING = 0xa,
	IT_AMULET = 0xc,
	IT_BOW = 0x1b,
	IT_AXE = 0x1c,
	IT_SWORD = 0x1e,
	IT_SPEAR = 0x21,
	IT_POLEARM = 0x22,
	IT_XBOW = 0x23,
	IT_WEAPON = 0x2d,
	IT_MELEE_WEAPON = 0x2e,
	IT_MISSILE_WEAPON = 0x2f,
	IT_ANY_ARMOR = 0x32,
	IT_SECOND_HAND = 0x36,
	IT_MISSILE = 0x38,
};

typedef struct d2_item_s d2_item_t;
struct d2_item_t {
	int unk0;
	int id;
	/* more fields follow */
};

typedef unsigned char d2_loc_t;

typedef struct d2_invrec_s d2_invrec_t;
struct d2_invrec_s {
	DWORD left, right, top, bottom;
	BYTE w, h; /* or x, y */
	WORD zero;
};

extern STDCALL d2_item_t *(*unit_get_equipped_item)(d2_unit_t *unit, d2_loc_t loc);
extern FASTCALL int (*unit_get_class_from_id)(int id);
extern int unit_get_class(d2_unit_t *unit);
extern int unit_is_inventory(d2_unit_t *unit);
extern STDCALL int (*unit_can_equip_item)(d2_unit_t *unit, d2_loc_t loc, d2_item_t *item, int reequip);

extern STDCALL int (*item_check_type)(d2_item_t *item, int type);
extern STDCALL int (*item_get_type_id)(int id);
extern STDCALL int (*item_get_type)(d2_item_t *item);
extern STDCALL void (*item_get_equip_locs)(d2_item_t *item, d2_loc_t *left, d2_loc_t *right);
extern STDCALL int (*item_get_shoots_type)(d2_item_t *item);
extern STDCALL int (*item_get_quiver_type)(d2_item_t *item);
extern STDCALL int (*item_get_required_class)(d2_item_t *item);
extern STDCALL int (*item_is_2H)(d2_item_t *item);

extern d2_loc_t last_chosen_loc;

#define INVENTORY_REC_MAX 0x20
#define INVENTORY_REC_ITEM_MAX 12
extern d2_invrec_t (*inventory_rec_ptr)[INVENTORY_REC_MAX][INVENTORY_REC_ITEM_MAX];

int patch_d2(void);
void unpatch_d2(void);

#endif
