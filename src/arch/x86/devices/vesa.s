# Init VESA Mode
.global init_vesa
init_vesa:
    mov	0x105+0x4000, %bx
    mov	$0x4f02, %ax
    int	$0x10
