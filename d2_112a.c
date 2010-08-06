#include "common.h"
#include <patch.h>

#include "d2.h"
#include "mercmod.h"

extern void merc_inventory_modify_112a();

extern void merc_store_location_112a();
extern void merc_loc_check_item_game_112a();

extern void merc_check_item_compat_112a();
extern void merc_check_item_class_112a();
extern void merc_check_item_game_112a();
extern void merc_check_item_game_drop_112a();
extern void merc_check_item_client_112a();
extern void merc_check_item_client_drop_112a();

void *original_merc_check_item_compat_112a;
extern void merc_inventory_mpq_load_112a();

#define HEADER <d2_112a_client.h>
#define NAME d2client_112a
#include "declPatch.h"

#define HEADER <d2_112a_game.h>
#define NAME d2game_112a
#include "declPatch.h"

#define HEADER <d2_112a_common.h>
#define NAME d2common_112a
#include "declPatch.h"

#define HEADER <d2_112a_win.h>
#define NAME d2win_112a
#include "declPatch.h"

int patch_112a(void)
{
	if (patch_d2client_112a()) goto err_client;
	if (patch_d2game_112a()) goto err_game;
	if (patch_d2common_112a()) goto err_common;
	if (patch_d2win_112a()) goto err_win;
	return 0;

err_win:
	unpatch_d2common_112a();
err_common:
	unpatch_d2game_112a();
err_game:
	unpatch_d2client_112a();
err_client:
	return -1;
}

void unpatch_112a(void)
{
	unpatch_d2win_112a();
	unpatch_d2common_112a();
	unpatch_d2game_112a();
	unpatch_d2client_112a();
}
