PATCH_BEGIN("D2Common.dll", 0xe3fd0984)

PATCH_VAR(0x67E70, unit_get_equipped_item) /* D2Common_10099 */
PATCH_VAR(0x6A660, unit_can_equip_item) /* D2Common_10731 */
PATCH_VAR(0x298D0, item_check_type) /* D2Common_10231 */
PATCH_VAR(0x28770, item_get_type_id) /* D2Common_10264 */
PATCH_VAR(0x287C0, item_get_type) /* D2Common_10681 */
PATCH_VAR(0x28800, item_get_equip_locs) /* D2Common_10896 */
PATCH_VAR(0x2A280, item_get_shoots_type) /* D2Common_10413 */
PATCH_VAR(0x2A230, item_get_quiver_type) /* D2Common_10255 */ /* D2Common_10292? */
PATCH_VAR(0x29720, item_get_required_class) /* D2Common_10432 */
PATCH_VAR(0x29FC0, item_is_2H) /* D2Common_10480 */

PATCH_VAR(0x6A470, original_merc_check_item_compat_112a)

/* add the new inventory to the .bin after loading */
PATCH(0x1BDE9, 0xE9 /* jmp ... */)
PATCH_REL(0x1BDEA, merc_inventory_modify_112a)
PATCH_ORIG(0x1BDE9, 0xC2, 0x04, 0x00, 0xCC, 0xCC)

/* override function that checks if both items can be equipped */
PATCH(0x6A6E8, 0x89, 0xC7 /* mov %eax, %edi */)
PATCH(0x6A6F7, 0x89, 0xF8 /* mov %edi, %eax */)
PATCH_REL(0x6A6FC, merc_check_item_compat_112a)
PATCH_ORIG(0x6A6E8, 0x8B, 0xF0)
PATCH_ORIG(0x6A6F7, 0x8B, 0xC6)
PATCH_ORIG(0x6A6FC, 0x70, 0xFD, 0xFF, 0xFF)
/* same as above, but secondary slots */
PATCH(0x6A71D, 0x89, 0xC7 /* mov %eax, %edi */)
PATCH(0x6A72C, 0x89, 0xF8 /* mov %edi, %eax */)
PATCH_REL(0x6A731, merc_check_item_compat_112a)
PATCH_ORIG(0x6A71D, 0x8B, 0xF0)
PATCH_ORIG(0x6A72C, 0x8B, 0xC6)
PATCH_ORIG(0x6A731, 0x3B, 0xFD, 0xFF, 0xFF)

/* override portion of level/stat/class check function that checks class */
PATCH_REL(0x2C6F9, merc_check_item_class_112a)
PATCH_ORIG(0x2C6F9, 0x23, 0xD0, 0xFF, 0xFF)

PATCH_END()
