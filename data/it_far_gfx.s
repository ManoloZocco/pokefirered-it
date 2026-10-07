@ Italian localized graphics block, extracted from the retail ROM (0x08EAF540-0x08EB22EC)
.ifdef ITALIAN
	.section .rodata
	.align 2
	.global sHoennTrainerCardFront_Tilemap
sHoennTrainerCardFront_Tilemap:
	.incbin "graphics/it/far_gfx.bin", 0x0, 0x214
	.global sHoennTrainerCardFrontLink_Tilemap
sHoennTrainerCardFrontLink_Tilemap:
	.incbin "graphics/it/far_gfx.bin", 0x214, 0x1f4
	.global sKantoTrainerCardBack_Tilemap
sKantoTrainerCardBack_Tilemap:
	.incbin "graphics/it/far_gfx.bin", 0x408, 0x12c
	.global gCreditsCopyright_Pal
gCreditsCopyright_Pal:
	.incbin "graphics/it/far_gfx.bin", 0x534, 0x20
	.global gCreditsCopyright_Tiles
gCreditsCopyright_Tiles:
	.incbin "graphics/it/far_gfx.bin", 0x554, 0x124
	.global gCreditsCopyright_Tilemap
gCreditsCopyright_Tilemap:
	.incbin "graphics/it/far_gfx.bin", 0x678, 0x144
	.global sPresents_Gfx
sPresents_Gfx:
	.incbin "graphics/it/far_gfx.bin", 0x7bc, 0x64
	.global sBg_Pal
sBg_Pal:
	.incbin "graphics/it/far_gfx.bin", 0x820, 0xa0
	.global sBg_Tiles
sBg_Tiles:
	.incbin "graphics/it/far_gfx.bin", 0x8c0, 0x7e0
	.global sBg_Tilemap
sBg_Tilemap:
	.incbin "graphics/it/far_gfx.bin", 0x10a0, 0x2e8
	.global sSpritePal_321Start
sSpritePal_321Start:
	.incbin "graphics/it/far_gfx.bin", 0x1388, 0x20
	.global sSpriteSheet_321Start
sSpriteSheet_321Start:
	.incbin "graphics/it/far_gfx.bin", 0x13a8, 0x434
	.global sBonuses_Pal
sBonuses_Pal:
	.incbin "graphics/it/far_gfx.bin", 0x17dc, 0x20
	.global sBonuses_Gfx
sBonuses_Gfx:
	.incbin "graphics/it/far_gfx.bin", 0x17fc, 0xd04
	.global sBonuses_Tilemap
sBonuses_Tilemap:
	.incbin "graphics/it/far_gfx.bin", 0x2500, 0x570
	.global sKantoDexTiles
sKantoDexTiles:
	.incbin "graphics/it/far_gfx.bin", 0x2a70, 0x108
	.global sNatDexTiles
sNatDexTiles:
	.incbin "graphics/it/far_gfx.bin", 0x2b78, 0xec
	.global sCategoryMonInfoBgTiles
sCategoryMonInfoBgTiles:
	.incbin "graphics/it/far_gfx.bin", 0x2c64, 0x148
.endif
