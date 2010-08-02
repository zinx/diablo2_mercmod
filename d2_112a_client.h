PATCH_BEGIN("D2Client.dll", 0x8a620d68)

PATCH_VAR(0xC3190, unit_get_class_from_id)

/* stuff equipment location (retreived in merc_store_location_112a)*/
PATCH(0xB4933,
	0x87, 0xF3 /* xchg esi, ebx ; ebx=location */
)
PATCH(0xB493A,
	0x93, /* xchg eax, ebx ; eax=location, ebx=ret */
	0x96  /* xchg eax, esi ; esi=location */
)
PATCH(0xB4941,
	0x89, 0xF1 /* mov ecx, esi ; ecx=location */
)
PATCH_ORIG(0xB4933, 0x8B, 0xF3)
PATCH_ORIG(0xB493A, 0x8B, 0xD8)
PATCH_ORIG(0xB4941, 0x33, 0xC9)

/* allow override of hardcoded merc class info */
PATCH_REL(0xA9297, merc_check_item_client_112a)
PATCH_ORIG(0xA9297, 0x3D, 0x2F, 0xF6, 0xFF)

/* drop on merc icon support */
PATCH_REL(0xBE06B, merc_check_item_client_drop_112a)
PATCH_ORIG(0xBE06B, 0x69, 0xE1, 0xF4, 0xFF)

PATCH_END()
