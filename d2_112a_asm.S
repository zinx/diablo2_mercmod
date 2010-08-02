.globl _merc_inventory_modify_112a
_merc_inventory_modify_112a:
	mov %eax, 4(%esp)
	jmp _merc_inventory_modify@4

.globl _merc_store_location_112a
_merc_store_location_112a:
	mov 0x10(%esp), %eax 
	movzbl 1(%eax), %ecx /* stuffed in client. */
	mov %cl, (_last_chosen_loc)
	ret

.globl _merc_loc_check_item_game_112a
_merc_loc_check_item_game_112a:
	/* ebp = unit */
	/* 0x30+4(%esp) = item */
	/* ebx = old item */
	/* ecx = destination location */
	/* eax = other item */
	/* edx = other location */

	mov %ecx, 0x2C+4(%esp)
	movzbl (_last_chosen_loc), %ecx
	test %ecx, %ecx
	jnz 1f

	pushal
	lea 4*5/*edx*/(%esp), %edx /* othloc pointer */
	lea 4*7/*eax*/(%esp), %eax /* othitem pointer */
	lea 4*6/*ecx*/(%esp), %ecx /* loc pointer */
	lea 4*4/*ebx*/(%esp), %ebx /* olditem pointer */
	push %edx
	push %eax
	push %ecx
	push %ebx
	pushl 4*4+4*8+0x30+4(%esp) /* item */
	push %ebp                  /* unit */
	call _merc_choose_item_loc@24
	popal
	jmp 2f

1:
	mov %ebp, 0x14+4(%esp)
	cmp %ecx, %edx
	jne 2f
	mov 0x2C+4(%esp), %edx
	xchg %ebx, %eax

2:
	mov %ecx, 0x14+4(%esp)

	pushal
	lea 4*8+0x2C+4(%esp), %edi
	push %edi                 /* remove_item ptr */
	push %edx                 /* othloc */
	push %eax                 /* othitem */
	push %ecx                 /* loc */
	push %ebx                 /* olditem */
	push 4*5+4*8+0x30+4(%esp) /* item */
	push %ebp                 /* unit */
	call _merc_check_item_remove@28
	neg %eax
	popal

	mov 0x2C+4(%esp), %ebx

	jl 1f /* allow */
	jnz 2f /* Force deny */

	/* Passthrough */
	mov %eax, 0x14+4(%esp)
	cmpl $0x167, 4(%ebp)
	ret

1:	/* allow */
	addl $0x9a5a2-0x9a572, (%esp)
	ret

2:	/* Force deny */
	addl $0x9a62a-0x9a572, (%esp)
	ret

.globl _merc_check_item_compat_112a
_merc_check_item_compat_112a:
	/* smashable: ebx, esi, edi, ebp */
	/* stack: esp+4 = unit, esp+8 = other_location */
	/* edi = unit */
	/* edx = item to equip */
	/* eax = equipped item */
	/* ecx = other item */
	/* esi = destination location */
	/* ebp = other location */

	mov 4(%esp), %edi

	push %eax               /* save equipped item */
	push %ecx               /* anti-clobber */
	push %edx

	push %ebp               /* other location */
	push %ecx               /* other item */
	push %esi               /* destination location */
	push %eax               /* equipped item */
	push %edx               /* item */
	push %edi               /* unit */
	call _merc_check_item_compat@24

	pop %edx                /* anti-clobber */
	pop %ecx

	neg %eax
	jl 1f /* force allow */
	jnz 1f /* force deny */

	/* passthrough */
	pop %eax		/* restore equipped item */
	jmp *(_original_merc_check_item_compat_112a)

1:
	and $1, %eax
	add $4, %esp		/* don't need stored equipped item */
	ret $0x8

.globl _merc_check_item_class_112a
_merc_check_item_class_112a:
	/* smashable: ebx, esi */
	/* stack: item (arg to class req getter) */
	/* edi = item */
	pushal
	push %edi
	call *(_item_get_required_class)
	mov %eax, 4*7(%esp)     /* for passthrough */

	/* edi = item */
	/* ebp = unit */
	/* eax = required class (after call above) */
	push %eax               /* required class */
	push %edi               /* item */
	push %ebp		/* unit */
	call _merc_check_item_class@12
	neg %eax
	popal

	jl 1f /* force allow */
	jnz 2f /* force deny */

	/* passthrough */
	ret $4

1:	/* force allow */
	mov $7, %eax
	ret $4

2:	/* force deny */
	xor %ebp, %ebp
	ret $4

.globl _merc_check_item_game_drop_112a
_merc_check_item_game_drop_112a:
	/* stack: item $3 */
	/* esi = item */
	/* edi = merc */
	/* smashable: (nothing) */
	pushal
	push %esi               /* item */
	call *(_item_get_required_class)

	push %eax               /* required class */
	push %esi               /* item */
	push %edi		/* unit */
	call _merc_check_item_class@12
	neg %eax
	popal

	jl 1f /* force allow */
	jnz 2f /* force deny */

	/* passthrough */
	jmp *(_item_check_type)

1:	/* force allow */
	addl $0x9b290-0x9b1d7, (%esp)
	mov $1, %ebx
	ret $8

2:	/* force deny */
	addl $0x9b290-0x9b1d7, (%esp)
	xor %ebx, %ebx
	ret $8

.globl _merc_check_item_client_112a
_merc_check_item_client_112a:
	/* stack: item $3 */
	/* ebx = item */
	/* ebp = merc */
	/* smashable: esi, ecx */
	pushal
	push %ebx
	call *(_item_get_required_class)

	push %eax               /* required class */
	push %ebx               /* item */
	push %ebp		/* unit */
	call _merc_check_item_class@12
	neg %eax
	popal

	jl 1f /* force allow */
	jnz 2f /* force deny */

	/* passthrough */
	jmp *(_item_check_type)

1:	/* force allow */
	addl $0xa931f-0xa929b, (%esp)
	mov $1, %esi
	ret $8

2:	/* force deny */
	addl $0xa931f-0xa929b, (%esp)
	xor %esi, %esi
	ret $8

.globl _merc_check_item_client_drop_112a
_merc_check_item_client_drop_112a:
	/* stack: item $3 */
	/* edi = item */
	/* ebx = merc id */
	/* smashable: ecx, esi, ebp */
	pushal
	push %edi
	call *(_item_get_required_class)

	push %eax                /* required class */
	push %edi                /* item */
	push %ebx                /* merc id */
	call _merc_check_item_class_from_id@12
	neg %eax
	popal

	jl 1f /* force allow */
	jnz 2f /* force deny */

	/* passthrough */
	jmp *(_item_check_type)

1:	/* force allow */
	addl $0xbe1b0-0xbe06f, (%esp)
	mov $1, %ebp
	ret $8

2:	/* force deny */
	addl $0xbe1cf-0xbe06f, (%esp)
	xor %ebp, %ebp
	ret $8

.global _merc_inventory_mpq_load_112a
_merc_inventory_mpq_load_112a:
	call _merc_inventory_mpq_load@0
	ret $8
