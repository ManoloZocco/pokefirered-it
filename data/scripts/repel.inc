EventScript_RepelWoreOff::
	msgbox Text_RepelWoreOff, MSGBOX_SIGN
	end

.ifdef ITALIAN
Text_RepelWoreOff::
	.string "Finito l'effetto del REPELLENTE…$"
.else
Text_RepelWoreOff::
	.string "REPEL's effect wore off…$"
.endif
