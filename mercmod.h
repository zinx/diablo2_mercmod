#ifndef mercmod_MERCMOD_h
#define mercmod_MERCMOD_h 1

#include "d2.h"

STDCALL int merc_check_item_compat(d2_unit_t *merc, d2_item_t *item, d2_item_t *olditem, d2_loc_t loc, d2_item_t *othitem, d2_loc_t othloc);
STDCALL int merc_check_item_remove(d2_unit_t *merc, d2_item_t *item, d2_item_t *olditem, d2_loc_t loc, d2_item_t *othitem, d2_loc_t othloc, d2_item_t **remove_item);
STDCALL void merc_choose_item_loc(d2_unit_t *merc, d2_item_t *item, d2_item_t **olditem, d2_loc_t *loc, d2_item_t **othitem, d2_loc_t *othloc);
STDCALL int merc_check_item_class(d2_unit_t *merc, d2_item_t *item, int item_req_class);
STDCALL int merc_check_item_class_from_id(int merc_id, d2_item_t *item, int item_req_class);
STDCALL int merc_check_item_class_from_class(int merc_class, d2_item_t *item, int item_req_class);
STDCALL int merc_check_item(d2_unit_t *merc, d2_item_t *item, int item_req_class);
STDCALL void merc_inventory_modify(d2_invrec_t (*inv)[12]);

STDCALL int merc_inventory_mpq_load(void);
STDCALL void merc_inventory_mpq_unload(void);

#endif
