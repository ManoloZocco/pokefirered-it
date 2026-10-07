EventScript_CutTree::
	goto_if_questlog EventScript_ReleaseEnd
	lockall
	goto_if_unset FLAG_BADGE02_GET, EventScript_CantCutTree
	checkpartymove MOVE_CUT
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CantCutTree
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	buffermovename STR_VAR_2, MOVE_CUT
	msgbox Text_CutTreeDown, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DontCutTree
	msgbox Text_MonUsedMove
	closemessage
	dofieldeffect FLDEFF_USE_CUT_ON_TREE
	waitstate
	goto EventScript_CutTreeDown
	end

EventScript_FldEffCut::
	lockall
	dofieldeffect FLDEFF_USE_CUT_ON_TREE
	waitstate
	goto EventScript_CutTreeDown
	end

EventScript_CutTreeDown::
	applymovement VAR_LAST_TALKED, Movement_CutTreeDown
	waitmovement 0
	removeobject VAR_LAST_TALKED
	releaseall
	end

Movement_CutTreeDown::
	cut_tree
	step_end

EventScript_CantCutTree::
	msgbox Text_TreeCanBeCutDown, MSGBOX_SIGN
	releaseall
	end

EventScript_DontCutTree::
	closemessage
	releaseall
	end

.ifdef ITALIAN
Text_CutTreeDown::
	.string "L’albero si può abbattere con\n"
	.string "TAGLIO.\p"
	.string "Vuoi usare TAGLIO?$"
.else
Text_CutTreeDown::
	.string "This tree looks like it can be CUT\n"
	.string "down!\p"
	.string "Would you like to CUT it?$"

.endif
.ifdef ITALIAN
Text_MonUsedMove::
	.string "{STR_VAR_1} usa {STR_VAR_2}.$"
.else
Text_MonUsedMove::
	.string "{STR_VAR_1} used {STR_VAR_2}!$"

.endif
.ifdef ITALIAN
Text_TreeCanBeCutDown::
	.string "L'albero si può abbattere con\n"
	.string "TAGLIO.$"
.else
Text_TreeCanBeCutDown::
	.string "This tree looks like it can be CUT\n"
	.string "down!$"

.endif
EventScript_RockSmash::
	goto_if_questlog EventScript_ReleaseEnd
	lockall
	goto_if_unset FLAG_BADGE06_GET, EventScript_CantSmashRock
	checkpartymove MOVE_ROCK_SMASH
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CantSmashRock
	setfieldeffectargument 0, VAR_RESULT
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	buffermovename STR_VAR_2, MOVE_ROCK_SMASH
	msgbox Text_UseRockSmash, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DontSmashRock
	msgbox Text_MonUsedMove
	closemessage
	dofieldeffect FLDEFF_USE_ROCK_SMASH
	waitstate
	goto EventScript_UseRockSmash
	end

EventScript_FldEffRockSmash::
	lockall
	dofieldeffect FLDEFF_USE_ROCK_SMASH
	waitstate
	goto EventScript_UseRockSmash
	end

EventScript_UseRockSmash::
	applymovement VAR_LAST_TALKED, Movement_BreakRock
	waitmovement 0
	removeobject VAR_LAST_TALKED
	special RockSmashWildEncounter
	goto_if_eq VAR_RESULT, FALSE, EventScript_RockSmashNoEncounter
	waitstate
	releaseall
	end

EventScript_RockSmashNoEncounter::
	releaseall
	end

Movement_BreakRock::
	rock_smash_break
	step_end

EventScript_CantSmashRock::
	msgbox Text_MonMaySmashRock, MSGBOX_SIGN
	end

EventScript_DontSmashRock::
	closemessage
	releaseall
	end

.ifdef ITALIAN
Text_UseRockSmash::
	.string "Questa roccia si può frantumare.\n"
	.string "Vuoi usare SPACCAROCCIA?$"
.else
Text_UseRockSmash::
	.string "This rock appears to be breakable.\n"
	.string "Would you like to use ROCK SMASH?$"

.endif
.ifdef ITALIAN
Text_MonMaySmashRock::
	.string "È una roccia dura, ma un POKéMON\n"
	.string "dovrebbe poterla frantumare.$"
.else
Text_MonMaySmashRock::
	.string "It's a rugged rock, but a POKéMON\n"
	.string "may be able to smash it.$"

.endif
EventScript_StrengthBoulder::
	goto_if_questlog EventScript_ReleaseEnd
	lockall
	goto_if_unset FLAG_BADGE04_GET, EventScript_CantMoveBoulder
	goto_if_set FLAG_SYS_USE_STRENGTH, EventScript_AlreadyUsedStrength
	checkpartymove MOVE_STRENGTH
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CantMoveBoulder
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_UseStrength, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_DontUseStrength
	closemessage
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	goto EventScript_UseStrength
	end

EventScript_FldEffStrength::
	lockall
	dofieldeffect FLDEFF_USE_STRENGTH
	waitstate
	goto EventScript_UseStrength
	end

EventScript_UseStrength::
	setflag FLAG_SYS_USE_STRENGTH
	msgbox Text_MonUsedStrengthCanMoveBoulders, MSGBOX_SIGN
	end

EventScript_CantMoveBoulder::
	msgbox Text_MonMayPushBoulder, MSGBOX_SIGN
	end

EventScript_AlreadyUsedStrength::
	msgbox Text_StrengthMadeMovingBouldersPossible, MSGBOX_SIGN
	end

EventScript_DontUseStrength::
	closemessage
	releaseall
	end

.ifdef ITALIAN
Text_UseStrength::
	.string "È un masso enorme, ma un POKéMON\n"
	.string "dovrebbe poterlo spostare.\p"
	.string "Vuoi usare FORZA?$"
.else
Text_UseStrength::
	.string "It's a big boulder, but a POKéMON\n"
	.string "may be able to push it aside.\p"
	.string "Would you like to use STRENGTH?$"

.endif
.ifdef ITALIAN
Text_MonUsedStrengthCanMoveBoulders::
	.string "{STR_VAR_1} usa FORZA.\p"
	.string "La FORZA di {STR_VAR_1} riesce a\n"
	.string "spostare i massi.$"
.else
Text_MonUsedStrengthCanMoveBoulders::
	.string "{STR_VAR_1} used STRENGTH!\p"
	.string "{STR_VAR_1}'s STRENGTH made it\n"
	.string "possible to move boulders around!$"

.endif
.ifdef ITALIAN
Text_MonMayPushBoulder::
	.string "È un masso enorme, ma un POKéMON\n"
	.string "dovrebbe poterlo spostare.$"
.else
Text_MonMayPushBoulder::
	.string "It's a big boulder, but a POKéMON\n"
	.string "may be able to push it aside.$"

.endif
.ifdef ITALIAN
Text_StrengthMadeMovingBouldersPossible::
	.string "FORZA ha reso possibile spostare\n"
	.string "i massi.$"
.else
Text_StrengthMadeMovingBouldersPossible::
	.string "STRENGTH made it possible to move\n"
	.string "boulders around.$"

.endif
EventScript_Waterfall::
	goto_if_questlog EventScript_ReleaseEnd
	lockall
	checkpartymove MOVE_WATERFALL
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_WaterCrashingDown
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	msgbox Text_UseWaterfall, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_EndWaterfall
	msgbox Text_MonUsedWaterfall
	dofieldeffect FLDEFF_USE_WATERFALL
	goto EventScript_EndWaterfall

EventScript_CantUseWaterfall::
	lockall
EventScript_WaterCrashingDown:
	msgbox Text_WallOfWaterCrashingDown
EventScript_EndWaterfall:
	releaseall
	end

.ifdef ITALIAN
Text_WallOfWaterCrashingDown::
	.string "Una parete d'acqua che si schianta\n"
	.string "con un potente fragore…$"
.else
Text_WallOfWaterCrashingDown::
	.string "A wall of water is crashing down\n"
	.string "with a mighty roar.$"

.endif
.ifdef ITALIAN
Text_UseWaterfall::
	.string "C'è un'enorme cascata.\n"
	.string "Vuoi usare CASCATA?$"
.else
Text_UseWaterfall::
	.string "It's a large waterfall.\n"
	.string "Would you like to use WATERFALL?$"

.endif
.ifdef ITALIAN
Text_MonUsedWaterfall::
	.string "{STR_VAR_1} usa CASCATA.$"
.else
Text_MonUsedWaterfall::
	.string "{STR_VAR_1} used WATERFALL.$"

@ Unused leftover from R/S
.endif
EventScript_DeepWater::
	lockall
	checkpartymove MOVE_DIVE
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_MayGoUnderwater
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	setfieldeffectargument 1, 1
	msgbox Text_SeaIsDeepUseDive, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_EndDive
	msgbox Text_MonUsedDive
	dofieldeffect FLDEFF_USE_DIVE
	goto EventScript_EndDive

EventScript_CantDive::
	lockall
EventScript_MayGoUnderwater:
	msgbox Text_MonMayGoUnderwater
EventScript_EndDive:
	releaseall
	end

EventScript_TrySurface::
	lockall
	checkpartymove MOVE_DIVE
	goto_if_eq VAR_RESULT, PARTY_SIZE, EventScript_CantSurface
	bufferpartymonnick STR_VAR_1, VAR_RESULT
	setfieldeffectargument 0, VAR_RESULT
	setfieldeffectargument 1, 1
	msgbox Text_LightFilteringUseDive, MSGBOX_YESNO
	goto_if_eq VAR_RESULT, NO, EventScript_EndSurface
	msgbox Text_MonUsedDive
	dofieldeffect FLDEFF_USE_DIVE
	goto EventScript_EndSurface

EventScript_CantSurface::
	lockall
	msgbox Text_MonMaySurface
	goto EventScript_EndSurface

EventScript_ObstacleCantSurface::
	lockall
	msgbox Text_DiveCantBeUsedHere
EventScript_EndSurface:
	releaseall
	end

.ifdef ITALIAN
Text_MonMayGoUnderwater::
	.string "Il mare è profondo qui. Ci si\n"
	.string "potrebbe immergere anche un\l"
	.string "POKéMON.$"
.else
Text_MonMayGoUnderwater::
	.string "The sea is deep here. A POKéMON\n"
	.string "may be able to go underwater.$"

.endif
.ifdef ITALIAN
Text_SeaIsDeepUseDive::
	.string "Il mare è profondo qui.\n"
	.string "Vuoi usare SUB?$"
.else
Text_SeaIsDeepUseDive::
	.string "The sea is deep here.\n"
	.string "Would you like to use DIVE?$"

.endif
.ifdef ITALIAN
Text_MonUsedDive::
	.string "{STR_VAR_1} usa SUB.$"
.else
Text_MonUsedDive::
	.string "{STR_VAR_1} used DIVE.$"

.endif
.ifdef ITALIAN
Text_MonMaySurface::
	.string "Una luce illumina l'acqua dall'alto.\n"
	.string "Da qui un POKéMON potrebbe\l"
	.string "riemergere.$"
.else
Text_MonMaySurface::
	.string "Light is filtering down from above.\n"
	.string "A POKéMON may be able to surface.$"

.endif
.ifdef ITALIAN
Text_LightFilteringUseDive::
	.string "Una luce illumina l'acqua dall'alto.\n"
	.string "Vuoi usare SUB?$"
.else
Text_LightFilteringUseDive::
	.string "Light is filtering down from above.\n"
	.string "Would you like to use DIVE?$"

.endif
.ifdef ITALIAN
Text_DiveCantBeUsedHere::
	.string "C'è qualcosa che blocca in\n"
	.string "superficie. Non puoi usare SUB qui.$"
.else
Text_DiveCantBeUsedHere::
	.string "There is an obstacle above.\n"
	.string "DIVE can't be used here.$"

.endif
EventScript_FailSweetScent::
	msgbox Text_LooksLikeNothingHere, MSGBOX_SIGN
	end

.ifdef ITALIAN
Text_LooksLikeNothingHere::
	.string "Pare non ci sia nulla qui…$"
.else
Text_LooksLikeNothingHere::
	.string "Looks like there's nothing here…$"
.endif
