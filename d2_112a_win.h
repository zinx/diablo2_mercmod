PATCH_BEGIN("D2Win.dll", 0xeb6f09cd)

PATCH(0x8D79, 0xE9 /* jmp ... */)
PATCH_REL(0x8D7A, merc_inventory_mpq_load_112a)

PATCH(0x87C7, 0xE9 /* jmp ... */)
PATCH_REL(0x87C8, merc_inventory_mpq_unload)

PATCH_END()
