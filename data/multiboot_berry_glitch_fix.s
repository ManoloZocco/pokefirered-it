	.section .rodata

gMultiBootProgram_BerryGlitchFix_Start::
.ifdef ITALIAN
	.incbin "data/mb_berry_fix_it.gba"
.else
	.incbin "data/mb_berry_fix.gba"
.endif
gMultiBootProgram_BerryGlitchFix_End::
