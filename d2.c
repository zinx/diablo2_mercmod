#include "common.h"
#include "d2.h"
#include "d2_112a.h"

STDCALL d2_item_t *(*unit_get_equipped_item)(d2_unit_t *unit, d2_loc_t loc);
FASTCALL int (*unit_get_class_from_id)(int id);
STDCALL int (*unit_can_equip_item)(d2_unit_t *unit, d2_loc_t loc, d2_item_t *item, int reequip);

STDCALL int (*item_check_type)(d2_item_t *item, int type);
STDCALL int (*item_get_type_id)(int id);
STDCALL int (*item_get_type)(d2_item_t *item);
STDCALL void (*item_get_equip_locs)(d2_item_t *item, d2_loc_t *left, d2_loc_t *right);
STDCALL int (*item_get_shoots_type)(d2_item_t *item);
STDCALL int (*item_get_quiver_type)(d2_item_t *item);
STDCALL int (*item_get_required_class)(d2_item_t *item);
STDCALL int (*item_is_2H)(d2_item_t *item);

d2_invrec_t (*inventory_rec_ptr)[INVENTORY_REC_MAX][INVENTORY_REC_ITEM_MAX];

d2_loc_t last_chosen_loc;

int unit_is_inventory(d2_unit_t *unit)
{
	return unit->type == 0x01020304;
}

int unit_get_class(d2_unit_t *unit)
{
	if (!unit) return -1;
	if (unit_is_inventory(unit))
		unit = unit->inv_owner_unit;
	return unit->unit_class;
}

int patch_d2(void)
{
	return patch_112a();
}

void unpatch_d2(void)
{
	unpatch_112a();
}
