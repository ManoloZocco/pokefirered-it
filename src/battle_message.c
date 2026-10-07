#include "global.h"
#include "gflib.h"
#include "battle.h"
#include "battle_anim.h"
#include "strings.h"
#include "battle_message.h"
#include "link.h"
#include "event_scripts.h"
#include "event_data.h"
#include "item.h"
#include "battle_tower.h"
#include "trainer_tower.h"
#include "battle_setup.h"
#include "field_specials.h"
#include "new_menu_helpers.h"
#include "battle_controllers.h"
#include "graphics.h"
#include "battle_ai_switch_items.h"
#include "constants/moves.h"
#include "constants/items.h"
#include "constants/trainers.h"
#include "constants/weather.h"

struct BattleWindowText
{
    u8 fillValue;
    u8 fontId;
    u8 x;
    u8 y;
    u8 letterSpacing;
    u8 lineSpacing;
    u8 speed;
    u8 fgColor;
    u8 bgColor;
    u8 shadowColor;
};

static EWRAM_DATA u8 sBattlerAbilities[MAX_BATTLERS_COUNT] = {};
static EWRAM_DATA struct BattleMsgData *sBattleMsgDataPtr = NULL;

static void ChooseMoveUsedParticle(u8 *textPtr);
static void ChooseTypeOfMoveUsedString(u8 *textPtr);
static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst);

static const u8 sText_Empty1[] = _("");
static const u8 sText_Trainer1LoseText[] = _("{B_TRAINER1_LOSE_TEXT}");
static const u8 sText_Trainer2LoseText[] = _("{B_TRAINER2_LOSE_TEXT}");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1RecallPkmn1[] = _("{B_TRAINER1_NAME}: {RIVAL}, rientra!");
#else
static const u8 sText_Trainer1RecallPkmn1[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME}, come back!");
#endif
static const u8 sText_Trainer1WinText[] = _("{B_TRAINER1_WIN_TEXT}");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1RecallPkmn2[] = _("{B_TRAINER1_NAME}: {EVIL_TEAM}, rientra!");
#else
static const u8 sText_Trainer1RecallPkmn2[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON2_NAME}, come back!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1RecallBoth[] = _("{B_TRAINER1_NAME}: {RIVAL} e\n"
    "{EVIL_TEAM}, rientrate!");
#else
static const u8 sText_Trainer1RecallBoth[] = _("{B_TRAINER1_NAME}: {B_OPPONENT_MON1_NAME} and\n{B_OPPONENT_MON2_NAME}, come back!");
#endif
static const u8 sText_Trainer2WinText[] = _("{B_TRAINER2_WIN_TEXT}");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnGainedEXP[] = _("{UNKNOWN_STR} riceve{PLAYER} \n"
    "{B_BUFF3} punti ESP.!\p");
#else
static const u8 sText_PkmnGainedEXP[] = _("{B_BUFF1} gained{B_BUFF2}\n{B_BUFF3} EXP. Points!\p");
#endif
static const u8 sText_EmptyString4[] = _("");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ABoosted[] = _(" la bellezza di");
#else
static const u8 sText_ABoosted[] = _(" a boosted");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnGrewToLv[] = _("{UNKNOWN_STR} sale al L. {PLAYER}!{WAIT_SE}\p");
#else
static const u8 sText_PkmnGrewToLv[] = _("{B_BUFF1} grew to\nLV. {B_BUFF2}!{WAIT_SE}\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLearnedMove[] = _("{UNKNOWN_STR} impara {PLAYER}!{WAIT_SE}\p");
#else
static const u8 sText_PkmnLearnedMove[] = _("{B_BUFF1} learned\n{B_BUFF2}!{WAIT_SE}\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TryToLearnMove1[] = _("{UNKNOWN_STR} sta cercando di imparare\n"
    "{PLAYER}.\p");
#else
static const u8 sText_TryToLearnMove1[] = _("{B_BUFF1} is trying to\nlearn {B_BUFF2}.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TryToLearnMove2[] = _("Ma {UNKNOWN_STR} non può conoscere\n"
    "più di quattro mosse.\p");
#else
static const u8 sText_TryToLearnMove2[] = _("But, {B_BUFF1} can't learn\nmore than four moves.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TryToLearnMove3[] = _("Vuoi cancellare una mossa per\n"
    "far posto a {PLAYER}?");
#else
static const u8 sText_TryToLearnMove3[] = _("Delete a move to make\nroom for {B_BUFF2}?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnForgotMove[] = _("{UNKNOWN_STR} scorda {PLAYER}…\p");
#else
static const u8 sText_PkmnForgotMove[] = _("{B_BUFF1} forgot\n{B_BUFF2}.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StopLearningMove[] = _("{PAUSE 32}Bloccare l’apprendimento\n"
    "di {PLAYER}?");
#else
static const u8 sText_StopLearningMove[] = _("{PAUSE 32}Stop learning\n{B_BUFF2}?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DidNotLearnMove[] = _("{UNKNOWN_STR} non ha imparato\n"
    "{PLAYER}.\p");
#else
static const u8 sText_DidNotLearnMove[] = _("{B_BUFF1} did not learn\n{B_BUFF2}.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_UseNextPkmn[] = _("Usare un altro POKéMON?");
#else
static const u8 sText_UseNextPkmn[] = _("Use next POKéMON?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AttackMissed[] = _("{B_ATK_NAME_WITH_PREFIX} fallisce!");
#else
static const u8 sText_AttackMissed[] = _("{B_ATK_NAME_WITH_PREFIX}'s\nattack missed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnProtectedItself[] = _("{B_DEF_NAME_WITH_PREFIX} si protegge!");
#else
static const u8 sText_PkmnProtectedItself[] = _("{B_DEF_NAME_WITH_PREFIX}\nprotected itself!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AvoidedDamage[] = _("{B_DEF_NAME_WITH_PREFIX} evita\n"
    "il colpo con {B_DEF_ABILITY}!");
#else
static const u8 sText_AvoidedDamage[] = _("{B_DEF_NAME_WITH_PREFIX} avoided\ndamage with {B_DEF_ABILITY}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMakesGroundMiss[] = _("{B_DEF_NAME_WITH_PREFIX} neutralizza le\n"
    "mosse di TERRA con {B_DEF_ABILITY}!");
#else
static const u8 sText_PkmnMakesGroundMiss[] = _("{B_DEF_NAME_WITH_PREFIX} makes GROUND\nmoves miss with {B_DEF_ABILITY}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAvoidedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} evita l’attacco!");
#else
static const u8 sText_PkmnAvoidedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} avoided\nthe attack!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItDoesntAffect[] = _("Non ha effetto su\n"
    "{B_DEF_NAME_WITH_PREFIX}…");
#else
static const u8 sText_ItDoesntAffect[] = _("It doesn't affect\n{B_DEF_NAME_WITH_PREFIX}…");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AttackerFainted[] = _("{B_ATK_NAME_WITH_PREFIX} è esausto!\p");
#else
static const u8 sText_AttackerFainted[] = _("{B_ATK_NAME_WITH_PREFIX}\nfainted!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TargetFainted[] = _("{B_DEF_NAME_WITH_PREFIX} è esausto!\p");
#else
static const u8 sText_TargetFainted[] = _("{B_DEF_NAME_WITH_PREFIX}\nfainted!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerGotMoney[] = _("{B_PLAYER_NAME} vince\n"
    "¥{UNKNOWN_STR}!\p");
#else
static const u8 sText_PlayerGotMoney[] = _("{B_PLAYER_NAME} got ¥{B_BUFF1}\nfor winning!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerWhiteout[] = _("{B_PLAYER_NAME} non ha più\n"
    "POKéMON utili!\p");
#else
static const u8 sText_PlayerWhiteout[] = _("{B_PLAYER_NAME} is out of\nusable POKéMON!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerPanicked[] = _("Nel panico, {B_PLAYER_NAME} perde ¥{UNKNOWN_STR}…\p"
    "… … … …\p"
    "{B_PLAYER_NAME} è fuori combattimento!{PAUSE_UNTIL_PRESS}");
#else
static const u8 sText_PlayerPanicked[] = _("{B_PLAYER_NAME} panicked and lost ¥{B_BUFF1}…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerWhiteoutAgainstTrainer[] = _("{B_PLAYER_NAME} non ha più\n"
    "POKéMON utili!\p"
    "La sfida è vinta da\n"
    "{B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!{PAUSE_UNTIL_PRESS}");
#else
static const u8 sText_PlayerWhiteoutAgainstTrainer[] = _("{B_PLAYER_NAME} is out of\nusable POKéMON!\pPlayer lost against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!{PAUSE_UNTIL_PRESS}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerPaidAsPrizeMoney[] = _("{B_PLAYER_NAME} paga ¥{UNKNOWN_STR} per la sconfitta.\p"
    "… … … …\p"
    "{B_PLAYER_NAME} è fuori combattimento!{PAUSE_UNTIL_PRESS}");
#else
static const u8 sText_PlayerPaidAsPrizeMoney[] = _("{B_PLAYER_NAME} paid ¥{B_BUFF1} as the prize\nmoney…\p… … … …\p{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerWhiteout2[] = _("{B_PLAYER_NAME} è fuori combattimento!{PAUSE_UNTIL_PRESS}");
#else
static const u8 sText_PlayerWhiteout2[] = _("{B_PLAYER_NAME} whited out!{PAUSE_UNTIL_PRESS}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PreventsEscape[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} impedisce\n"
    "la fuga con {B_SCR_ACTIVE_ABILITY}!\p");
#else
static const u8 sText_PreventsEscape[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} prevents\nescape with {B_SCR_ACTIVE_ABILITY}!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_CantEscape2[] = _("Non si scappa!\p");
#else
static const u8 sText_CantEscape2[] = _("Can't escape!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AttackerCantEscape[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "non può scappare!");
#else
static const u8 sText_AttackerCantEscape[] = _("{B_ATK_NAME_WITH_PREFIX} can't escape!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HitXTimes[] = _("Colpi subiti: {UNKNOWN_STR}!");
#else
static const u8 sText_HitXTimes[] = _("Hit {B_BUFF1} time(s)!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFellAsleep[] = _("{B_EFF_NAME_WITH_PREFIX}\n"
    "s’è addormentato!");
#else
static const u8 sText_PkmnFellAsleep[] = _("{B_EFF_NAME_WITH_PREFIX}\nfell asleep!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMadeSleep[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "addormenta {B_EFF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnMadeSleep[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade {B_EFF_NAME_WITH_PREFIX} sleep!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAlreadyAsleep[] = _("{B_DEF_NAME_WITH_PREFIX} sta già dormendo!");
#else
static const u8 sText_PkmnAlreadyAsleep[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready asleep!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAlreadyAsleep2[] = _("{B_ATK_NAME_WITH_PREFIX} sta già dormendo!");
#else
static const u8 sText_PkmnAlreadyAsleep2[] = _("{B_ATK_NAME_WITH_PREFIX} is\nalready asleep!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasntAffected[] = _("{B_DEF_NAME_WITH_PREFIX} è incolume!");
#else
static const u8 sText_PkmnWasntAffected[] = _("{B_DEF_NAME_WITH_PREFIX}\nwasn't affected!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX} è stato \n"
    "avvelenato!");
#else
static const u8 sText_PkmnWasPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX}\nwas poisoned!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPoisonedBy[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "avvelena {B_EFF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnPoisonedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\npoisoned {B_EFF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHurtByPoison[] = _("Il veleno ha effetto\n"
    "su {B_ATK_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnHurtByPoison[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby poison!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAlreadyPoisoned[] = _("{B_DEF_NAME_WITH_PREFIX} è già\n"
    "avvelenato.");
#else
static const u8 sText_PkmnAlreadyPoisoned[] = _("{B_DEF_NAME_WITH_PREFIX} is already\npoisoned.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBadlyPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX}\n"
    "è iperavvelenato!");
#else
static const u8 sText_PkmnBadlyPoisoned[] = _("{B_EFF_NAME_WITH_PREFIX} is badly\npoisoned!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnEnergyDrained[] = _("Viene prelevata energia\n"
    "da {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnEnergyDrained[] = _("{B_DEF_NAME_WITH_PREFIX} had its\nenergy drained!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasBurned[] = _("{B_EFF_NAME_WITH_PREFIX} è stato \n"
    "scottato!");
#else
static const u8 sText_PkmnWasBurned[] = _("{B_EFF_NAME_WITH_PREFIX} was burned!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBurnedBy[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "scotta {B_EFF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnBurnedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nburned {B_EFF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHurtByBurn[] = _("{B_ATK_NAME_WITH_PREFIX} soffre\n"
    "per la scottatura!");
#else
static const u8 sText_PkmnHurtByBurn[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby its burn!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAlreadyHasBurn[] = _("{B_DEF_NAME_WITH_PREFIX} è già scottato.");
#else
static const u8 sText_PkmnAlreadyHasBurn[] = _("{B_DEF_NAME_WITH_PREFIX} already\nhas a burn.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasFrozen[] = _("{B_EFF_NAME_WITH_PREFIX} è stato\n"
    "congelato!");
#else
static const u8 sText_PkmnWasFrozen[] = _("{B_EFF_NAME_WITH_PREFIX} was\nfrozen solid!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFrozenBy[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "congela {B_EFF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnFrozenBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nfroze {B_EFF_NAME_WITH_PREFIX} solid!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIsFrozen[] = _("{B_ATK_NAME_WITH_PREFIX} è congelato!");
#else
static const u8 sText_PkmnIsFrozen[] = _("{B_ATK_NAME_WITH_PREFIX} is\nfrozen solid!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasDefrosted[] = _("{B_DEF_NAME_WITH_PREFIX} è stato\n"
    "scongelato!");
#else
static const u8 sText_PkmnWasDefrosted[] = _("{B_DEF_NAME_WITH_PREFIX} was\ndefrosted!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasDefrosted2[] = _("{B_ATK_NAME_WITH_PREFIX} è stato\n"
    "scongelato!");
#else
static const u8 sText_PkmnWasDefrosted2[] = _("{B_ATK_NAME_WITH_PREFIX} was\ndefrosted!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasDefrostedBy[] = _("{B_ATK_NAME_WITH_PREFIX} è stato\n"
    "scongelato da {B_CURRENT_MOVE}!");
#else
static const u8 sText_PkmnWasDefrostedBy[] = _("{B_ATK_NAME_WITH_PREFIX} was\ndefrosted by {B_CURRENT_MOVE}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasParalyzed[] = _("{B_EFF_NAME_WITH_PREFIX} è stato\n"
    "paralizzato!\l"
    "Forse non riuscirà ad attaccare!");
#else
static const u8 sText_PkmnWasParalyzed[] = _("{B_EFF_NAME_WITH_PREFIX} is paralyzed!\nIt may be unable to move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasParalyzedBy[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "paralizza {B_EFF_NAME_WITH_PREFIX}!\l"
    "Forse non riuscirà ad attaccare!");
#else
static const u8 sText_PkmnWasParalyzedBy[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nparalyzed {B_EFF_NAME_WITH_PREFIX}!\lIt may be unable to move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIsParalyzed[] = _("{B_ATK_NAME_WITH_PREFIX} è paralizzato!\n"
    "Non può attaccare!");
#else
static const u8 sText_PkmnIsParalyzed[] = _("{B_ATK_NAME_WITH_PREFIX} is paralyzed!\nIt can't move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIsAlreadyParalyzed[] = _("{B_DEF_NAME_WITH_PREFIX}\n"
    "è già paralizzato!");
#else
static const u8 sText_PkmnIsAlreadyParalyzed[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready paralyzed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHealedParalysis[] = _("{B_DEF_NAME_WITH_PREFIX}\n"
    "è guarito dalla paralisi!");
#else
static const u8 sText_PkmnHealedParalysis[] = _("{B_DEF_NAME_WITH_PREFIX} was\nhealed of paralysis!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnDreamEaten[] = _("Mangia il sogno di\n"
    "{B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnDreamEaten[] = _("{B_DEF_NAME_WITH_PREFIX}'s\ndream was eaten!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatsWontIncrease[] = _("Non aumenta\n"
    "{UNKNOWN_STR} di {B_ATK_NAME_WITH_PREFIX}.");
#else
static const u8 sText_StatsWontIncrease[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\nwon't go higher!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatsWontDecrease[] = _("Non diminuisce \n"
    "{UNKNOWN_STR} di {B_DEF_NAME_WITH_PREFIX}.");
#else
static const u8 sText_StatsWontDecrease[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\nwon't go lower!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TeamStoppedWorking[] = _("{UNKNOWN_STR} della tua squadra\n"
    "non funziona!");
#else
static const u8 sText_TeamStoppedWorking[] = _("Your team's {B_BUFF1}\nstopped working!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FoeStoppedWorking[] = _("{UNKNOWN_STR} dell’avversario\n"
    "non funziona!");
#else
static const u8 sText_FoeStoppedWorking[] = _("The foe's {B_BUFF1}\nstopped working!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIsConfused[] = _("{B_ATK_NAME_WITH_PREFIX} è confuso!");
#else
static const u8 sText_PkmnIsConfused[] = _("{B_ATK_NAME_WITH_PREFIX} is\nconfused!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHealedConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} \n"
    "non è più confuso!");
#else
static const u8 sText_PkmnHealedConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} snapped\nout of confusion!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasConfused[] = _("{B_EFF_NAME_WITH_PREFIX} è stato confuso!");
#else
static const u8 sText_PkmnWasConfused[] = _("{B_EFF_NAME_WITH_PREFIX} became\nconfused!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAlreadyConfused[] = _("{B_DEF_NAME_WITH_PREFIX} è già confuso!");
#else
static const u8 sText_PkmnAlreadyConfused[] = _("{B_DEF_NAME_WITH_PREFIX} is\nalready confused!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFellInLove[] = _("{B_DEF_NAME_WITH_PREFIX} è innamorato!");
#else
static const u8 sText_PkmnFellInLove[] = _("{B_DEF_NAME_WITH_PREFIX}\nfell in love!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnInLove[] = _("{B_ATK_NAME_WITH_PREFIX} è innamorato\n"
    "di {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnInLove[] = _("{B_ATK_NAME_WITH_PREFIX} is in love\nwith {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnImmobilizedByLove[] = _("L’innamoramento impedisce\n"
    "a {B_ATK_NAME_WITH_PREFIX} di attaccare!");
#else
static const u8 sText_PkmnImmobilizedByLove[] = _("{B_ATK_NAME_WITH_PREFIX} is\nimmobilized by love!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBlownAway[] = _("{B_DEF_NAME_WITH_PREFIX} è spazzato via!");
#else
static const u8 sText_PkmnBlownAway[] = _("{B_DEF_NAME_WITH_PREFIX} was\nblown away!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnChangedType[] = _("{B_ATK_NAME_WITH_PREFIX} si trasforma\n"
    "nel tipo {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnChangedType[] = _("{B_ATK_NAME_WITH_PREFIX} transformed\ninto the {B_BUFF1} type!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFlinched[] = _("{B_ATK_NAME_WITH_PREFIX} tentenna!");
#else
static const u8 sText_PkmnFlinched[] = _("{B_ATK_NAME_WITH_PREFIX} flinched!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRegainedHealth[] = _("{B_DEF_NAME_WITH_PREFIX} s’è ripreso!");
#else
static const u8 sText_PkmnRegainedHealth[] = _("{B_DEF_NAME_WITH_PREFIX} regained\nhealth!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHPFull[] = _("{B_DEF_NAME_WITH_PREFIX} ha tutti i PS!");
#else
static const u8 sText_PkmnHPFull[] = _("{B_DEF_NAME_WITH_PREFIX}'s\nHP is full!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRaisedSpDef[] = _("{B_CURRENT_MOVE} del POKéMON {B_ATK_PREFIX2}\n"
    "aumenta la DIF. SPEC.!");
#else
static const u8 sText_PkmnRaisedSpDef[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised SP. DEF!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRaisedSpDefALittle[] = _("{B_CURRENT_MOVE} del POKéMON {B_ATK_PREFIX2}\n"
    "aumenta un po’ la DIF. SPEC.!");
#else
static const u8 sText_PkmnRaisedSpDefALittle[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised SP. DEF a little!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRaisedDef[] = _("{B_CURRENT_MOVE} del POKéMON {B_ATK_PREFIX2}\n"
    "aumenta la DIFESA!");
#else
static const u8 sText_PkmnRaisedDef[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised DEFENSE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRaisedDefALittle[] = _("{B_CURRENT_MOVE} del POKéMON {B_ATK_PREFIX2}\n"
    "aumenta un po’ la DIFESA!");
#else
static const u8 sText_PkmnRaisedDefALittle[] = _("{B_ATK_PREFIX2}'s {B_CURRENT_MOVE}\nraised DEFENSE a little!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCoveredByVeil[] = _("Un velo ricopre la squadra\n"
    "del POKéMON {B_ATK_PREFIX2}!");
#else
static const u8 sText_PkmnCoveredByVeil[] = _("{B_ATK_PREFIX2}'s party is covered\nby a veil!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnUsedSafeguard[] = _("SALVAGUARDIA protegge\n"
    "la squadra di {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnUsedSafeguard[] = _("{B_DEF_NAME_WITH_PREFIX}'s party is protected\nby SAFEGUARD!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSafeguardExpired[] = _("Cade SALVAGUARDIA della\n"
    "squadra del POKéMON {B_ATK_PREFIX3}!");
#else
static const u8 sText_PkmnSafeguardExpired[] = _("{B_ATK_PREFIX3}'s party is no longer\nprotected by SAFEGUARD!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWentToSleep[] = _("{B_ATK_NAME_WITH_PREFIX} va a dormire!");
#else
static const u8 sText_PkmnWentToSleep[] = _("{B_ATK_NAME_WITH_PREFIX} went\nto sleep!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSleptHealthy[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "dorme e si riprende!");
#else
static const u8 sText_PkmnSleptHealthy[] = _("{B_ATK_NAME_WITH_PREFIX} slept and\nbecame healthy!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWhippedWhirlwind[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "genera un turbine!");
#else
static const u8 sText_PkmnWhippedWhirlwind[] = _("{B_ATK_NAME_WITH_PREFIX} whipped\nup a whirlwind!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTookSunlight[] = _("{B_ATK_NAME_WITH_PREFIX} assorbe la luce!");
#else
static const u8 sText_PkmnTookSunlight[] = _("{B_ATK_NAME_WITH_PREFIX} took\nin sunlight!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLoweredHead[] = _("{B_ATK_NAME_WITH_PREFIX} \n"
    "abbassa la testa!");
#else
static const u8 sText_PkmnLoweredHead[] = _("{B_ATK_NAME_WITH_PREFIX} lowered\nits head!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIsGlowing[] = _("{B_ATK_NAME_WITH_PREFIX} sta brillando!");
#else
static const u8 sText_PkmnIsGlowing[] = _("{B_ATK_NAME_WITH_PREFIX} is glowing!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFlewHigh[] = _("{B_ATK_NAME_WITH_PREFIX} vola in alto!");
#else
static const u8 sText_PkmnFlewHigh[] = _("{B_ATK_NAME_WITH_PREFIX} flew\nup high!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnDugHole[] = _("{B_ATK_NAME_WITH_PREFIX} scava una fossa!");
#else
static const u8 sText_PkmnDugHole[] = _("{B_ATK_NAME_WITH_PREFIX} dug a hole!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHidUnderwater[] = _("{B_ATK_NAME_WITH_PREFIX} \n"
    "sparisce sott’acqua!");
#else
static const u8 sText_PkmnHidUnderwater[] = _("{B_ATK_NAME_WITH_PREFIX} hid\nunderwater!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSprangUp[] = _("{B_ATK_NAME_WITH_PREFIX} salta fuori!");
#else
static const u8 sText_PkmnSprangUp[] = _("{B_ATK_NAME_WITH_PREFIX} sprang up!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSqueezedByBind[] = _("{B_ATK_NAME_WITH_PREFIX} stritola\n"
    "{B_DEF_NAME_WITH_PREFIX} con LEGATUTTO!");
#else
static const u8 sText_PkmnSqueezedByBind[] = _("{B_DEF_NAME_WITH_PREFIX} was squeezed by\n{B_ATK_NAME_WITH_PREFIX}'s BIND!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTrappedInVortex[] = _("{B_DEF_NAME_WITH_PREFIX} è intrappolato\n"
    "nel vortice!");
#else
static const u8 sText_PkmnTrappedInVortex[] = _("{B_DEF_NAME_WITH_PREFIX} was trapped\nin the vortex!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTrappedBySandTomb[] = _("{B_DEF_NAME_WITH_PREFIX} è intrappolato\n"
    "da SABBIOTOMBA!");
#else
static const u8 sText_PkmnTrappedBySandTomb[] = _("{B_DEF_NAME_WITH_PREFIX} was trapped\nby SAND TOMB!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWrappedBy[] = _("{B_ATK_NAME_WITH_PREFIX} usa AVVOLGI-\n"
    "BOTTA su {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnWrappedBy[] = _("{B_DEF_NAME_WITH_PREFIX} was WRAPPED by\n{B_ATK_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnClamped[] = _("{B_ATK_NAME_WITH_PREFIX} usa\n"
    "TENAGLIA su {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnClamped[] = _("{B_ATK_NAME_WITH_PREFIX} CLAMPED\n{B_DEF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHurtBy[] = _("{B_ATK_NAME_WITH_PREFIX} è ferito\n"
    "da {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnHurtBy[] = _("{B_ATK_NAME_WITH_PREFIX} is hurt\nby {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFreedFrom[] = _("{B_ATK_NAME_WITH_PREFIX} è liberato\n"
    "da {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnFreedFrom[] = _("{B_ATK_NAME_WITH_PREFIX} was freed\nfrom {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCrashed[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "si sbilancia e si schianta!");
#else
static const u8 sText_PkmnCrashed[] = _("{B_ATK_NAME_WITH_PREFIX} kept going\nand crashed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gBattleText_MistShroud[] = _("Il POKéMON {B_ATK_PREFIX2}\n"
    "è avvolto dalla NEBBIA!");
#else
const u8 gBattleText_MistShroud[] = _("{B_ATK_PREFIX2} became\nshrouded in MIST!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnProtectedByMist[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "è protetto dalla NEBBIA!");
#else
static const u8 sText_PkmnProtectedByMist[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is protected\nby MIST!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gBattleText_GetPumped[] = _("{B_ATK_NAME_WITH_PREFIX} si gonfia!");
#else
const u8 gBattleText_GetPumped[] = _("{B_ATK_NAME_WITH_PREFIX} is getting\npumped!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHitWithRecoil[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "subisce il contraccolpo!");
#else
static const u8 sText_PkmnHitWithRecoil[] = _("{B_ATK_NAME_WITH_PREFIX} is hit\nwith recoil!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnProtectedItself2[] = _("{B_ATK_NAME_WITH_PREFIX} è pronto a \n"
    "proteggersi!");
#else
static const u8 sText_PkmnProtectedItself2[] = _("{B_ATK_NAME_WITH_PREFIX} protected\nitself!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBuffetedBySandstorm[] = _("{B_ATK_NAME_WITH_PREFIX} è colpito\n"
    "da una tempesta di sabbia!");
#else
static const u8 sText_PkmnBuffetedBySandstorm[] = _("{B_ATK_NAME_WITH_PREFIX} is buffeted\nby the sandstorm!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPeltedByHail[] = _("{B_ATK_NAME_WITH_PREFIX} è colpito\n"
    "da GRANDINE!");
#else
static const u8 sText_PkmnPeltedByHail[] = _("{B_ATK_NAME_WITH_PREFIX} is pelted\nby HAIL!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXWoreOff[] = _("Finito l’effetto di\n"
    "{UNKNOWN_STR} del POKéMON {B_ATK_PREFIX1}!");
#else
static const u8 sText_PkmnsXWoreOff[] = _("{B_ATK_PREFIX1}'s {B_BUFF1}\nwore off!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSeeded[] = _("{B_DEF_NAME_WITH_PREFIX} è pieno di semi!");
#else
static const u8 sText_PkmnSeeded[] = _("{B_DEF_NAME_WITH_PREFIX} was seeded!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnEvadedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} schiva l’attacco!");
#else
static const u8 sText_PkmnEvadedAttack[] = _("{B_DEF_NAME_WITH_PREFIX} evaded\nthe attack!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSappedByLeechSeed[] = _("PARASSISEME sottrae energia\n"
    "a {B_ATK_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnSappedByLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX}'s health is\nsapped by LEECH SEED!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFastAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} dorme.");
#else
static const u8 sText_PkmnFastAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} is fast\nasleep.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWokeUp[] = _("{B_ATK_NAME_WITH_PREFIX} si è svegliato!");
#else
static const u8 sText_PkmnWokeUp[] = _("{B_ATK_NAME_WITH_PREFIX} woke up!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnUproarKeptAwake[] = _("Ma BARAONDA di\n"
    "{B_SCR_ACTIVE_NAME_WITH_PREFIX} lo tiene sveglio!");
#else
static const u8 sText_PkmnUproarKeptAwake[] = _("But {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s UPROAR\nkept it awake!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWokeUpInUproar[] = _("{B_ATK_NAME_WITH_PREFIX} si sveglia\n"
    "a causa di BARAONDA!");
#else
static const u8 sText_PkmnWokeUpInUproar[] = _("{B_ATK_NAME_WITH_PREFIX} woke up\nin the UPROAR!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCausedUproar[] = _("{B_ATK_NAME_WITH_PREFIX} scatena\n"
    "una BARAONDA!");
#else
static const u8 sText_PkmnCausedUproar[] = _("{B_ATK_NAME_WITH_PREFIX} caused\nan UPROAR!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMakingUproar[] = _("{B_ATK_NAME_WITH_PREFIX} sta facendo\n"
    "una BARAONDA!");
#else
static const u8 sText_PkmnMakingUproar[] = _("{B_ATK_NAME_WITH_PREFIX} is making\nan UPROAR!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCalmedDown[] = _("{B_ATK_NAME_WITH_PREFIX} si calma.");
#else
static const u8 sText_PkmnCalmedDown[] = _("{B_ATK_NAME_WITH_PREFIX} calmed down.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCantSleepInUproar[] = _("Ma {B_DEF_NAME_WITH_PREFIX} non riesce\n"
    "a dormire con BARAONDA!");
#else
static const u8 sText_PkmnCantSleepInUproar[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an UPROAR!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnStockpiled[] = _("{B_ATK_NAME_WITH_PREFIX} usa ACCUMULO:\n"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_PkmnStockpiled[] = _("{B_ATK_NAME_WITH_PREFIX} STOCKPILED\n{B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCantStockpile[] = _("{B_ATK_NAME_WITH_PREFIX} non può più\n"
    "usare ACCUMULO!");
#else
static const u8 sText_PkmnCantStockpile[] = _("{B_ATK_NAME_WITH_PREFIX} can't\nSTOCKPILE any more!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCantSleepInUproar2[] = _("Ma {B_DEF_NAME_WITH_PREFIX} non riesce\n"
    "a dormire con BARAONDA!");
#else
static const u8 sText_PkmnCantSleepInUproar2[] = _("But {B_DEF_NAME_WITH_PREFIX} can't\nsleep in an UPROAR!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_UproarKeptPkmnAwake[] = _("Ma BARAONDA di\n"
    "{B_DEF_NAME_WITH_PREFIX} lo tiene sveglio!");
#else
static const u8 sText_UproarKeptPkmnAwake[] = _("But the UPROAR kept\n{B_DEF_NAME_WITH_PREFIX} awake!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnStayedAwakeUsing[] = _("{B_DEF_NAME_WITH_PREFIX} rimane sveglio\n"
    "grazie a {B_DEF_ABILITY}!");
#else
static const u8 sText_PkmnStayedAwakeUsing[] = _("{B_DEF_NAME_WITH_PREFIX} stayed awake\nusing its {B_DEF_ABILITY}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnStoringEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} \n"
    "accumula energia!");
#else
static const u8 sText_PkmnStoringEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} is storing\nenergy!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnUnleashedEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} libera energia!");
#else
static const u8 sText_PkmnUnleashedEnergy[] = _("{B_ATK_NAME_WITH_PREFIX} unleashed\nenergy!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFatigueConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} è confuso\n"
    "per la fatica!");
#else
static const u8 sText_PkmnFatigueConfusion[] = _("{B_ATK_NAME_WITH_PREFIX} became\nconfused due to fatigue!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPickedUpItem[] = _("{B_PLAYER_NAME} raccoglie ¥{UNKNOWN_STR}!\p");
#else
static const u8 sText_PkmnPickedUpItem[] = _("{B_PLAYER_NAME} picked up\n¥{B_BUFF1}!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnUnaffected[] = _("{B_DEF_NAME_WITH_PREFIX} è incolume!");
#else
static const u8 sText_PkmnUnaffected[] = _("{B_DEF_NAME_WITH_PREFIX} is\nunaffected!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTransformedInto[] = _("{B_ATK_NAME_WITH_PREFIX} si trasforma\n"
    "in {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnTransformedInto[] = _("{B_ATK_NAME_WITH_PREFIX} transformed\ninto {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMadeSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} crea \n"
    "un SOSTITUTO!");
#else
static const u8 sText_PkmnMadeSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} made\na SUBSTITUTE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHasSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} ha già\n"
    "un SOSTITUTO!");
#else
static const u8 sText_PkmnHasSubstitute[] = _("{B_ATK_NAME_WITH_PREFIX} already\nhas a SUBSTITUTE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SubstituteDamaged[] = _("Il SOSTITUTO è colpito\n"
    "al posto di {B_DEF_NAME_WITH_PREFIX}!\p");
#else
static const u8 sText_SubstituteDamaged[] = _("The SUBSTITUTE took damage\nfor {B_DEF_NAME_WITH_PREFIX}!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSubstituteFaded[] = _("Il SOSTITUTO di\n"
    "{B_DEF_NAME_WITH_PREFIX} svanisce!\p");
#else
static const u8 sText_PkmnSubstituteFaded[] = _("{B_DEF_NAME_WITH_PREFIX}'s\nSUBSTITUTE faded!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMustRecharge[] = _("{B_ATK_NAME_WITH_PREFIX} deve ricaricarsi!");
#else
static const u8 sText_PkmnMustRecharge[] = _("{B_ATK_NAME_WITH_PREFIX} must\nrecharge!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRageBuilding[] = _("Cresce l’IRA di\n"
    "{B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnRageBuilding[] = _("{B_DEF_NAME_WITH_PREFIX}'s RAGE\nis building!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMoveWasDisabled[] = _("INIBITORE ha messo {UNKNOWN_STR}\n"
    "di {B_DEF_NAME_WITH_PREFIX} fuori uso!");
#else
static const u8 sText_PkmnMoveWasDisabled[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\nwas disabled!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMoveDisabledNoMore[] = _("Termina l’effetto di INIBITORE\n"
    "su {B_ATK_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnMoveDisabledNoMore[] = _("{B_ATK_NAME_WITH_PREFIX} is disabled\nno more!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnGotEncore[] = _("{B_DEF_NAME_WITH_PREFIX} è colpito\n"
    "da RIPETI!");
#else
static const u8 sText_PkmnGotEncore[] = _("{B_DEF_NAME_WITH_PREFIX} got\nan ENCORE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnEncoreEnded[] = _("Termina l’effetto di RIPETI\n"
    "su {B_ATK_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnEncoreEnded[] = _("{B_ATK_NAME_WITH_PREFIX}'s ENCORE\nended!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTookAim[] = _("{B_ATK_NAME_WITH_PREFIX} prende\n"
    "la mira su {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnTookAim[] = _("{B_ATK_NAME_WITH_PREFIX} took aim\nat {B_DEF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSketchedMove[] = _("{B_ATK_NAME_WITH_PREFIX} disegna\n"
    "uno SCHIZZO di {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnSketchedMove[] = _("{B_ATK_NAME_WITH_PREFIX} SKETCHED\n{B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTryingToTakeFoe[] = _("{B_ATK_NAME_WITH_PREFIX} tenta di\n"
    "trascinare con sé l’avversario!");
#else
static const u8 sText_PkmnTryingToTakeFoe[] = _("{B_ATK_NAME_WITH_PREFIX} is trying\nto take its foe with it!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTookFoe[] = _("{B_DEF_NAME_WITH_PREFIX} trascina\n"
    "con sé {B_ATK_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnTookFoe[] = _("{B_DEF_NAME_WITH_PREFIX} took\n{B_ATK_NAME_WITH_PREFIX} with it!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnReducedPP[] = _("{UNKNOWN_STR} di {B_DEF_NAME_WITH_PREFIX}\n"
    "cala di {PLAYER}!");
#else
static const u8 sText_PkmnReducedPP[] = _("Reduced {B_DEF_NAME_WITH_PREFIX}'s\n{B_BUFF1} by {B_BUFF2}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnStoleItem[] = _("{B_ATK_NAME_WITH_PREFIX} ruba\n"
    "{B_LAST_ITEM} di\l"
    "{B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnStoleItem[] = _("{B_ATK_NAME_WITH_PREFIX} stole\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TargetCantEscapeNow[] = _("{B_DEF_NAME_WITH_PREFIX} \n"
    "non può scappare!");
#else
static const u8 sText_TargetCantEscapeNow[] = _("{B_DEF_NAME_WITH_PREFIX} can't\nescape now!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFellIntoNightmare[] = _("{B_DEF_NAME_WITH_PREFIX} ha un INCUBO!");
#else
static const u8 sText_PkmnFellIntoNightmare[] = _("{B_DEF_NAME_WITH_PREFIX} fell into\na NIGHTMARE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLockedInNightmare[] = _("{B_ATK_NAME_WITH_PREFIX} è\n"
    "intrappolato in un INCUBO!");
#else
static const u8 sText_PkmnLockedInNightmare[] = _("{B_ATK_NAME_WITH_PREFIX} is locked\nin a NIGHTMARE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLaidCurse[] = _("{B_ATK_NAME_WITH_PREFIX} riduce i suoi PS\n"
    "per lanciare una MALEDIZIONE\l"
    "su {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnLaidCurse[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP and\nlaid a CURSE on {B_DEF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAfflictedByCurse[] = _("{B_ATK_NAME_WITH_PREFIX} è colpito\n"
    "dalla MALEDIZIONE!");
#else
static const u8 sText_PkmnAfflictedByCurse[] = _("{B_ATK_NAME_WITH_PREFIX} is afflicted\nby the CURSE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SpikesScattered[] = _("Ci sono PUNTE ovunque!");
#else
static const u8 sText_SpikesScattered[] = _("SPIKES were scattered all around\nthe opponent's side!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHurtBySpikes[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "soffre per le PUNTE!");
#else
static const u8 sText_PkmnHurtBySpikes[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} is hurt\nby SPIKES!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIdentified[] = _("{B_ATK_NAME_WITH_PREFIX} identifica\n"
    "{B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnIdentified[] = _("{B_ATK_NAME_WITH_PREFIX} identified\n{B_DEF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPerishCountFell[] = _("ULTIMOCANTO di\n"
    "{B_ATK_NAME_WITH_PREFIX}: meno {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnPerishCountFell[] = _("{B_ATK_NAME_WITH_PREFIX}'s PERISH count\nfell to {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBracedItself[] = _("{B_ATK_NAME_WITH_PREFIX} si rinvigorisce!");
#else
static const u8 sText_PkmnBracedItself[] = _("{B_ATK_NAME_WITH_PREFIX} braced\nitself!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnEnduredHit[] = _("{B_DEF_NAME_WITH_PREFIX} RESISTE!");
#else
static const u8 sText_PkmnEnduredHit[] = _("{B_DEF_NAME_WITH_PREFIX} ENDURED\nthe hit!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_MagnitudeStrength[] = _("MAGNITUDO {UNKNOWN_STR}!");
#else
static const u8 sText_MagnitudeStrength[] = _("MAGNITUDE {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCutHPMaxedAttack[] = _("{B_ATK_NAME_WITH_PREFIX} riduce i suoi PS\n"
    "per massimizzare l’ATTACCO!");
#else
static const u8 sText_PkmnCutHPMaxedAttack[] = _("{B_ATK_NAME_WITH_PREFIX} cut its own HP\nand maximized ATTACK!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCopiedStatChanges[] = _("{B_ATK_NAME_WITH_PREFIX} copia modifiche\n"
    "statistiche di {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnCopiedStatChanges[] = _("{B_ATK_NAME_WITH_PREFIX} copied\n{B_DEF_NAME_WITH_PREFIX}'s stat changes!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnGotFree[] = _("{B_ATK_NAME_WITH_PREFIX} si libera da\n"
    "{UNKNOWN_STR} di\l"
    "{B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnGotFree[] = _("{B_ATK_NAME_WITH_PREFIX} got free of\n{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnShedLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "sparge PARASSISEME!");
#else
static const u8 sText_PkmnShedLeechSeed[] = _("{B_ATK_NAME_WITH_PREFIX} shed\nLEECH SEED!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBlewAwaySpikes[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "spazza via le PUNTE!");
#else
static const u8 sText_PkmnBlewAwaySpikes[] = _("{B_ATK_NAME_WITH_PREFIX} blew away\nSPIKES!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFledFromBattle[] = _("{B_ATK_NAME_WITH_PREFIX} se la dà a\n"
    "gambe!");
#else
static const u8 sText_PkmnFledFromBattle[] = _("{B_ATK_NAME_WITH_PREFIX} fled from\nbattle!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnForesawAttack[] = _("{B_ATK_NAME_WITH_PREFIX} \n"
    "prevede l’attacco!");
#else
static const u8 sText_PkmnForesawAttack[] = _("{B_ATK_NAME_WITH_PREFIX} foresaw\nan attack!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX} subisce\n"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_PkmnTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX} took the\n{B_BUFF1} attack!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnChoseXAsDestiny[] = _("{B_ATK_NAME_WITH_PREFIX} sceglie\n"
    "{B_CURRENT_MOVE} come suo destino!");
#else
static const u8 sText_PkmnChoseXAsDestiny[] = _("{B_ATK_NAME_WITH_PREFIX} chose\n{B_CURRENT_MOVE} as its destiny!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAttack[] = _("Attacco di {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnAttack[] = _("{B_BUFF1}'s attack!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCenterAttention[] = _("{B_ATK_NAME_WITH_PREFIX} è al\n"
    "centro dell’attenzione!");
#else
static const u8 sText_PkmnCenterAttention[] = _("{B_ATK_NAME_WITH_PREFIX} became the\ncenter of attention!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnChargingPower[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "inizia a caricarsi!");
#else
static const u8 sText_PkmnChargingPower[] = _("{B_ATK_NAME_WITH_PREFIX} began\ncharging power!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_NaturePowerTurnedInto[] = _("NATURFORZA si trasforma in\n"
    "{B_CURRENT_MOVE}!");
#else
static const u8 sText_NaturePowerTurnedInto[] = _("NATURE POWER turned into\n{B_CURRENT_MOVE}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnStatusNormal[] = _("Lo stato di {B_ATK_NAME_WITH_PREFIX}\n"
    "torna normale!");
#else
static const u8 sText_PkmnStatusNormal[] = _("{B_ATK_NAME_WITH_PREFIX}'s status\nreturned to normal!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSubjectedToTorment[] = _("{B_DEF_NAME_WITH_PREFIX} subisce\n"
    "l’ATTACCALITE!");
#else
static const u8 sText_PkmnSubjectedToTorment[] = _("{B_DEF_NAME_WITH_PREFIX} was subjected\nto TORMENT!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTighteningFocus[] = _("{B_ATK_NAME_WITH_PREFIX} \n"
    "restringe la mira!");
#else
static const u8 sText_PkmnTighteningFocus[] = _("{B_ATK_NAME_WITH_PREFIX} is tightening\nits focus!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFellForTaunt[] = _("{B_DEF_NAME_WITH_PREFIX}\n"
    "è in balia di PROVOCAZIONE!");
#else
static const u8 sText_PkmnFellForTaunt[] = _("{B_DEF_NAME_WITH_PREFIX} fell for\nthe TAUNT!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnReadyToHelp[] = _("{B_ATK_NAME_WITH_PREFIX} è pronto ad\n"
    "aiutare {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnReadyToHelp[] = _("{B_ATK_NAME_WITH_PREFIX} is ready to\nhelp {B_DEF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSwitchedItems[] = _("{B_ATK_NAME_WITH_PREFIX} scambia\n"
    "lo strumento!");
#else
static const u8 sText_PkmnSwitchedItems[] = _("{B_ATK_NAME_WITH_PREFIX} switched\nitems with its opponent!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnObtainedX[] = _("{B_ATK_NAME_WITH_PREFIX} ottiene\n"
    "{UNKNOWN_STR}.");
#else
static const u8 sText_PkmnObtainedX[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnObtainedX2[] = _("{B_DEF_NAME_WITH_PREFIX} ottiene\n"
    "{PLAYER}.");
#else
static const u8 sText_PkmnObtainedX2[] = _("{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnObtainedXYObtainedZ[] = _("{B_ATK_NAME_WITH_PREFIX} ottiene\n"
    "{UNKNOWN_STR}.\p"
    "{B_DEF_NAME_WITH_PREFIX} ottiene\n"
    "{PLAYER}.");
#else
static const u8 sText_PkmnObtainedXYObtainedZ[] = _("{B_ATK_NAME_WITH_PREFIX} obtained\n{B_BUFF1}.\p{B_DEF_NAME_WITH_PREFIX} obtained\n{B_BUFF2}.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCopiedFoe[] = _("{B_ATK_NAME_WITH_PREFIX} copia\n"
    "{B_DEF_ABILITY} di\l"
    "{B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnCopiedFoe[] = _("{B_ATK_NAME_WITH_PREFIX} copied\n{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMadeWish[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "esprime un DESIDERIO!");
#else
static const u8 sText_PkmnMadeWish[] = _("{B_ATK_NAME_WITH_PREFIX} made a WISH!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWishCameTrue[] = _("Il DESIDERIO di {UNKNOWN_STR}\n"
    "si avvera!");
#else
static const u8 sText_PkmnWishCameTrue[] = _("{B_BUFF1}'s WISH\ncame true!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPlantedRoots[] = _("{B_ATK_NAME_WITH_PREFIX} pianta le radici!");
#else
static const u8 sText_PkmnPlantedRoots[] = _("{B_ATK_NAME_WITH_PREFIX} planted its roots!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAbsorbedNutrients[] = _("{B_ATK_NAME_WITH_PREFIX} assorbe\n"
    "sostanze nutritive con le radici!");
#else
static const u8 sText_PkmnAbsorbedNutrients[] = _("{B_ATK_NAME_WITH_PREFIX} absorbed\nnutrients with its roots!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAnchoredItself[] = _("{B_DEF_NAME_WITH_PREFIX} è ancorato\n"
    "al suolo grazie alle radici!");
#else
static const u8 sText_PkmnAnchoredItself[] = _("{B_DEF_NAME_WITH_PREFIX} anchored\nitself with its roots!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasMadeDrowsy[] = _("{B_ATK_NAME_WITH_PREFIX} fa\n"
    "assopire {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnWasMadeDrowsy[] = _("{B_ATK_NAME_WITH_PREFIX} made\n{B_DEF_NAME_WITH_PREFIX} drowsy!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnKnockedOff[] = _("{B_ATK_NAME_WITH_PREFIX} blocca\n"
    "{B_LAST_ITEM} di\l"
    "{B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnKnockedOff[] = _("{B_ATK_NAME_WITH_PREFIX} knocked off\n{B_DEF_NAME_WITH_PREFIX}'s {B_LAST_ITEM}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSwappedAbilities[] = _("{B_ATK_NAME_WITH_PREFIX} scambia abilità!");
#else
static const u8 sText_PkmnSwappedAbilities[] = _("{B_ATK_NAME_WITH_PREFIX} swapped abilities\nwith its opponent!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSealedOpponentMove[] = _("{B_ATK_NAME_WITH_PREFIX} blocca una\n"
    "o più mosse dell’avversario!");
#else
static const u8 sText_PkmnSealedOpponentMove[] = _("{B_ATK_NAME_WITH_PREFIX} sealed the\nopponent's move(s)!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWantsGrudge[] = _("{B_ATK_NAME_WITH_PREFIX} serba\n"
    "RANCORE all’avversario!");
#else
static const u8 sText_PkmnWantsGrudge[] = _("{B_ATK_NAME_WITH_PREFIX} wants the\nopponent to bear a GRUDGE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLostPPGrudge[] = _("{UNKNOWN_STR} di {B_ATK_NAME_WITH_PREFIX}\n"
    "perde tutti i PP\l"
    "a causa di RANCORE!");
#else
static const u8 sText_PkmnLostPPGrudge[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1} lost\nall its PP due to the GRUDGE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnShroudedItself[] = _("{B_ATK_NAME_WITH_PREFIX} si avvolge\n"
    "in {B_CURRENT_MOVE}!");
#else
static const u8 sText_PkmnShroudedItself[] = _("{B_ATK_NAME_WITH_PREFIX} shrouded\nitself in {B_CURRENT_MOVE}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMoveBounced[] = _("{B_CURRENT_MOVE} di {B_ATK_NAME_WITH_PREFIX}\n"
    "rimbalza a causa di MAGIVELO!");
#else
static const u8 sText_PkmnMoveBounced[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE}\nwas bounced back by MAGIC COAT!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWaitsForTarget[] = _("{B_ATK_NAME_WITH_PREFIX} aspetta\n"
    "la mossa dell’avversario!");
#else
static const u8 sText_PkmnWaitsForTarget[] = _("{B_ATK_NAME_WITH_PREFIX} waits for its foe\nto make a move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSnatchedMove[] = _("{B_DEF_NAME_WITH_PREFIX} ruba la mossa di\n"
    "{B_SCR_ACTIVE_NAME_WITH_PREFIX} con SCIPPO!");
#else
static const u8 sText_PkmnSnatchedMove[] = _("{B_DEF_NAME_WITH_PREFIX} SNATCHED\n{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ElectricityWeakened[] = _("La potenza dell’elettricità\n"
    "è stata indebolita!");
#else
static const u8 sText_ElectricityWeakened[] = _("Electricity's power was\nweakened!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FireWeakened[] = _("La potenza del fuoco\n"
    "è stata indebolita!");
#else
static const u8 sText_FireWeakened[] = _("Fire's power was\nweakened!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_XFoundOneY[] = _("{B_ATK_NAME_WITH_PREFIX}\n"
    "trova {B_LAST_ITEM}!");
#else
static const u8 sText_XFoundOneY[] = _("{B_ATK_NAME_WITH_PREFIX} found\none {B_LAST_ITEM}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SoothingAroma[] = _("La zona è pervasa da\n"
    "un piacevole profumo!");
#else
static const u8 sText_SoothingAroma[] = _("A soothing aroma wafted\nthrough the area!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItemsCantBeUsedNow[] = _("Impossibile usare strumenti qui.{PAUSE 64}");
#else
static const u8 sText_ItemsCantBeUsedNow[] = _("Items can't be used now.{PAUSE 64}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ForXCommaYZ[] = _("Per {B_SCR_ACTIVE_NAME_WITH_PREFIX},\n"
    "la {B_LAST_ITEM} {UNKNOWN_STR}");
#else
static const u8 sText_ForXCommaYZ[] = _("For {B_SCR_ACTIVE_NAME_WITH_PREFIX},\n{B_LAST_ITEM} {B_BUFF1}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnUsedXToGetPumped[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} usa\n"
    "{B_LAST_ITEM}: aumentano\l"
    "i brutti colpi!");
#else
static const u8 sText_PkmnUsedXToGetPumped[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} used\n{B_LAST_ITEM} to hustle!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLostFocus[] = _("{B_ATK_NAME_WITH_PREFIX} perde la mira\n"
    "e rimane immobile!");
#else
static const u8 sText_PkmnLostFocus[] = _("{B_ATK_NAME_WITH_PREFIX} lost its\nfocus and couldn't move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWasDraggedOut[] = _("{B_DEF_NAME_WITH_PREFIX} è tirato dentro!\p");
#else
static const u8 sText_PkmnWasDraggedOut[] = _("{B_DEF_NAME_WITH_PREFIX} was\ndragged out!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TheWallShattered[] = _("La barriera si frantuma!");
#else
static const u8 sText_TheWallShattered[] = _("The wall shattered!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ButNoEffect[] = _("Ma è inefficace!");
#else
static const u8 sText_ButNoEffect[] = _("But it had no effect!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHasNoMovesLeft[] = _("{B_ACTIVE_NAME_WITH_PREFIX}\n"
    "non ha più mosse!\p");
#else
static const u8 sText_PkmnHasNoMovesLeft[] = _("{B_ACTIVE_NAME_WITH_PREFIX} has no\nmoves left!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMoveIsDisabled[] = _("A causa di INIBITORE, {B_CURRENT_MOVE}\n"
    "di {B_ACTIVE_NAME_WITH_PREFIX} è fuori uso!\p");
#else
static const u8 sText_PkmnMoveIsDisabled[] = _("{B_ACTIVE_NAME_WITH_PREFIX}'s {B_CURRENT_MOVE}\nis disabled!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCantUseMoveTorment[] = _("{B_ACTIVE_NAME_WITH_PREFIX} non può usare la\n"
    "stessa mossa 2 volte per\l"
    "l’ATTACCALITE!\p");
#else
static const u8 sText_PkmnCantUseMoveTorment[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use the same\nmove in a row due to the TORMENT!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCantUseMoveTaunt[] = _("{B_ACTIVE_NAME_WITH_PREFIX} non può usare\n"
    "{B_CURRENT_MOVE} dopo PROVOCAZIONE!\p");
#else
static const u8 sText_PkmnCantUseMoveTaunt[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use\n{B_CURRENT_MOVE} after the TAUNT!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCantUseMoveSealed[] = _("{B_ACTIVE_NAME_WITH_PREFIX} non può usare\n"
    "la mossa bloccata {B_CURRENT_MOVE}!\p");
#else
static const u8 sText_PkmnCantUseMoveSealed[] = _("{B_ACTIVE_NAME_WITH_PREFIX} can't use the\nsealed {B_CURRENT_MOVE}!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnMadeItRain[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "provoca la pioggia!");
#else
static const u8 sText_PkmnMadeItRain[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it rain!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRaisedSpeed[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "aumenta la VELOCITÀ!");
#else
static const u8 sText_PkmnRaisedSpeed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nraised its SPEED!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnProtectedBy[] = _("{B_DEF_NAME_WITH_PREFIX} è protetto\n"
    "da {B_DEF_ABILITY}!");
#else
static const u8 sText_PkmnProtectedBy[] = _("{B_DEF_NAME_WITH_PREFIX} was protected\nby {B_DEF_ABILITY}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPreventsUsage[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "impedisce a {B_ATK_NAME_WITH_PREFIX}\l"
    "di usare {B_CURRENT_MOVE}!");
#else
static const u8 sText_PkmnPreventsUsage[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents {B_ATK_NAME_WITH_PREFIX}\lfrom using {B_CURRENT_MOVE}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRestoredHPUsing[] = _("{B_DEF_NAME_WITH_PREFIX} ricarica PS\n"
    "usando {B_DEF_ABILITY}!");
#else
static const u8 sText_PkmnRestoredHPUsing[] = _("{B_DEF_NAME_WITH_PREFIX} restored HP\nusing its {B_DEF_ABILITY}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXMadeYUseless[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "neutralizza {B_CURRENT_MOVE}!");
#else
static const u8 sText_PkmnsXMadeYUseless[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} useless!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnChangedTypeWith[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "lo ha reso di tipo {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnChangedTypeWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade it the {B_BUFF1} type!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPreventsParalysisWith[] = _("{B_DEF_ABILITY} di {B_EFF_NAME_WITH_PREFIX}\n"
    "previene la paralisi!");
#else
static const u8 sText_PkmnPreventsParalysisWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents paralysis!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPreventsRomanceWith[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "previene l’innamoramento!");
#else
static const u8 sText_PkmnPreventsRomanceWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents romance!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPreventsPoisoningWith[] = _("{B_DEF_ABILITY} di {B_EFF_NAME_WITH_PREFIX}\n"
    "previene l’avvelenamento!");
#else
static const u8 sText_PkmnPreventsPoisoningWith[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents poisoning!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPreventsConfusionWith[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "previene la confusione!");
#else
static const u8 sText_PkmnPreventsConfusionWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevents confusion!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnRaisedFirePowerWith[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "aumenta la potenza del tipo FUOCO!");
#else
static const u8 sText_PkmnRaisedFirePowerWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nraised its FIRE power!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnAnchorsItselfWith[] = _("{B_DEF_NAME_WITH_PREFIX} è ancorato\n"
    "al suolo grazie a {B_DEF_ABILITY}!");
#else
static const u8 sText_PkmnAnchorsItselfWith[] = _("{B_DEF_NAME_WITH_PREFIX} anchors\nitself with {B_DEF_ABILITY}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnCutsAttackWith[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "riduce ATT. di {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnCutsAttackWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncuts {B_DEF_NAME_WITH_PREFIX}'s ATTACK!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPreventsStatLossWith[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "evita calo delle statistiche!");
#else
static const u8 sText_PkmnPreventsStatLossWith[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nprevents stat loss!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHurtsWith[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "colpisce {B_ATK_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnHurtsWith[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nhurt {B_ATK_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTraced[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} TRACCIA\n"
    "{PLAYER} di\l"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_PkmnTraced[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} TRACED\n{B_BUFF1}'s {B_BUFF2}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXPreventsBurns[] = _("{B_EFF_ABILITY} di {B_EFF_NAME_WITH_PREFIX}\n"
    "previene le scottature!");
#else
static const u8 sText_PkmnsXPreventsBurns[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY}\nprevents burns!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXBlocksY[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "blocca {B_CURRENT_MOVE}!");
#else
static const u8 sText_PkmnsXBlocksY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nblocks {B_CURRENT_MOVE}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXBlocksY2[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "blocca {B_CURRENT_MOVE}!");
#else
static const u8 sText_PkmnsXBlocksY2[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nblocks {B_CURRENT_MOVE}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXRestoredHPALittle2[] = _("{B_ATK_ABILITY} di {B_ATK_NAME_WITH_PREFIX}\n"
    "ristabilisce parte dei PS!");
#else
static const u8 sText_PkmnsXRestoredHPALittle2[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nrestored its HP a little!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXWhippedUpSandstorm[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "genera una tempesta di sabbia!");
#else
static const u8 sText_PkmnsXWhippedUpSandstorm[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nwhipped up a sandstorm!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXIntensifiedSun[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "intensifica i raggi solari!");
#else
static const u8 sText_PkmnsXIntensifiedSun[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nintensified the sun's rays!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXPreventsYLoss[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "evita calo di {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnsXPreventsYLoss[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nprevents {B_BUFF1} loss!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXInfatuatedY[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "fa infatuare {B_ATK_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnsXInfatuatedY[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ninfatuated {B_ATK_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXMadeYIneffective[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "rende inefficace {B_CURRENT_MOVE}!");
#else
static const u8 sText_PkmnsXMadeYIneffective[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nmade {B_CURRENT_MOVE} ineffective!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXCuredYProblem[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "cura il problema di {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnsXCuredYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItSuckedLiquidOoze[] = _("Succhia la MELMA!");
#else
static const u8 sText_ItSuckedLiquidOoze[] = _("It sucked up the\nLIQUID OOZE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTransformed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} si trasforma!");
#else
static const u8 sText_PkmnTransformed[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX} transformed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXTookAttack[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "riceve l’attacco!");
#else
static const u8 sText_PkmnsXTookAttack[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\ntook the attack!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_LAST_ABILITY} di {UNKNOWN_STR}\n"
    "evita lo scambio!\p");
#else
const u8 gText_PkmnsXPreventsSwitching[] = _("{B_BUFF1}'s {B_LAST_ABILITY}\nprevents switching!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PreventedFromWorking[] = _("{B_DEF_ABILITY} di {B_DEF_NAME_WITH_PREFIX}\n"
    "blocca {UNKNOWN_STR}\l"
    "di {B_SCR_ACTIVE_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PreventedFromWorking[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_DEF_ABILITY}\nprevented {B_SCR_ACTIVE_NAME_WITH_PREFIX}'s\l{B_BUFF1} from working!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXMadeItIneffective[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "l’ha neutralizzato!");
#else
static const u8 sText_PkmnsXMadeItIneffective[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nmade it ineffective!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXPreventsFlinching[] = _("{B_EFF_ABILITY} di {B_EFF_NAME_WITH_PREFIX}\n"
    "evita il tentennamento!");
#else
static const u8 sText_PkmnsXPreventsFlinching[] = _("{B_EFF_NAME_WITH_PREFIX}'s {B_EFF_ABILITY}\nprevents flinching!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXPreventsYsZ[] = _("{B_ATK_ABILITY} di {B_ATK_NAME_WITH_PREFIX}\n"
    "blocca {B_DEF_ABILITY}\l"
    "di {B_DEF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnsXPreventsYsZ[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_ATK_ABILITY}\nprevents {B_DEF_NAME_WITH_PREFIX}'s\l{B_DEF_ABILITY} from working!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXCuredItsYProblem[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "cura il problema di {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnsXCuredItsYProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\ncured its {B_BUFF1} problem!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsXHadNoEffectOnY[] = _("{B_SCR_ACTIVE_ABILITY} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "non agisce su {B_EFF_NAME_WITH_PREFIX}!");
#else
static const u8 sText_PkmnsXHadNoEffectOnY[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_SCR_ACTIVE_ABILITY}\nhad no effect on {B_EFF_NAME_WITH_PREFIX}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TooScaredToMove[] = _("{B_ATK_NAME_WITH_PREFIX} non si muove!\n"
    "Ha una fifa…");
#else
static const u8 sText_TooScaredToMove[] = _("{B_ATK_NAME_WITH_PREFIX} is too scared to move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GetOutGetOut[] = _("SPETTRO: Fuori… fuori…");
#else
static const u8 sText_GetOutGetOut[] = _("GHOST: Get out…… Get out……");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatSharply[] = _("sale di molto!");
#else
static const u8 sText_StatSharply[] = _("sharply ");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gBattleText_Rose[] = _("sale!");
#else
const u8 gBattleText_Rose[] = _("rose!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatHarshly[] = _("cala a picco!");
#else
static const u8 sText_StatHarshly[] = _("harshly ");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatFell[] = _("cala!");
#else
static const u8 sText_StatFell[] = _("fell!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AttackersStatRose[] = _("Ehi, {UNKNOWN_STR} di \n"
    "{B_ATK_NAME_WITH_PREFIX} {PLAYER}");
#else
static const u8 sText_AttackersStatRose[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_DefendersStatRose[] = _("Ehi, {UNKNOWN_STR} di\n"
    "{B_DEF_NAME_WITH_PREFIX} {PLAYER}");
#else
const u8 gText_DefendersStatRose[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_UsingItemTheStatOfPkmnRose[] = _("Con {B_LAST_ITEM}, {UNKNOWN_STR} di\n"
    "{B_SCR_ACTIVE_NAME_WITH_PREFIX} {PLAYER}");
#else
static const u8 sText_UsingItemTheStatOfPkmnRose[] = _("Using {B_LAST_ITEM}, the {B_BUFF1}\nof {B_SCR_ACTIVE_NAME_WITH_PREFIX} {B_BUFF2}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AttackersStatFell[] = _("{UNKNOWN_STR} di {B_ATK_NAME_WITH_PREFIX}\n"
    "{PLAYER}");
#else
static const u8 sText_AttackersStatFell[] = _("{B_ATK_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DefendersStatFell[] = _("Ehi, {UNKNOWN_STR} di\n"
    "{B_DEF_NAME_WITH_PREFIX} {PLAYER}");
#else
static const u8 sText_DefendersStatFell[] = _("{B_DEF_NAME_WITH_PREFIX}'s {B_BUFF1}\n{B_BUFF2}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatsWontIncrease2[] = _("Statistiche di {B_ATK_NAME_WITH_PREFIX}\n"
    "non aumenteranno!");
#else
static const u8 sText_StatsWontIncrease2[] = _("{B_ATK_NAME_WITH_PREFIX}'s stats won't\ngo any higher!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatsWontDecrease2[] = _("Statistiche di {B_DEF_NAME_WITH_PREFIX}\n"
    "non caleranno!");
#else
static const u8 sText_StatsWontDecrease2[] = _("{B_DEF_NAME_WITH_PREFIX}'s stats won't\ngo any lower!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_CriticalHit[] = _("Brutto colpo!");
#else
static const u8 sText_CriticalHit[] = _("A critical hit!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_OneHitKO[] = _("KO in un attacco!");
#else
static const u8 sText_OneHitKO[] = _("It's a one-hit KO!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_123Poof[] = _("{PAUSE 32}1, {PAUSE 15}2 e {PAUSE 15}… {PAUSE 15}… {PAUSE 15}…{PAUSE 15}{PLAY_SE}ぅ  puf!\p");
#else
static const u8 sText_123Poof[] = _("{PAUSE 32}1, {PAUSE 15}2, and{PAUSE 15}… {PAUSE 15}… {PAUSE 15}… {PAUSE 15}{PLAY_SE SE_BALL_BOUNCE_1}Poof!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AndEllipsis[] = _("e al suo posto…\p");
#else
static const u8 sText_AndEllipsis[] = _("And…\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HMMovesCantBeForgotten[] = _("Ora è impossibile\n"
    "scordare mosse MN.\p");
#else
static const u8 sText_HMMovesCantBeForgotten[] = _("HM moves can't be\nforgotten now.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_NotVeryEffective[] = _("Non è molto efficace…");
#else
static const u8 sText_NotVeryEffective[] = _("It's not very effective…");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SuperEffective[] = _("È superefficace!");
#else
static const u8 sText_SuperEffective[] = _("It's super effective!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE}Ù Scampato pericolo!\p");
#else
static const u8 sText_GotAwaySafely[] = _("{PLAY_SE SE_FLEE}Got away safely!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFledUsingIts[] = _("{PLAY_SE}Ù {B_ATK_NAME_WITH_PREFIX} fugge\n"
    "usando {B_LAST_ITEM}!\p");
#else
static const u8 sText_PkmnFledUsingIts[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing its {B_LAST_ITEM}!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnFledUsing[] = _("{PLAY_SE}Ù {B_ATK_NAME_WITH_PREFIX} fugge\n"
    "usando {B_ATK_ABILITY}!\p");
#else
static const u8 sText_PkmnFledUsing[] = _("{PLAY_SE SE_FLEE}{B_ATK_NAME_WITH_PREFIX} fled\nusing {B_ATK_ABILITY}!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_WildPkmnFled[] = _("{PLAY_SE}Ù {UNKNOWN_STR} selvatico fugge!");
#else
static const u8 sText_WildPkmnFled[] = _("{PLAY_SE SE_FLEE}Wild {B_BUFF1} fled!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("Hai avuto la meglio su\n"
    "{B_LINK_OPPONENT1_NAME}!");
#else
static const u8 sText_PlayerDefeatedLinkTrainer[] = _("Player defeated\n{B_LINK_OPPONENT1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TwoLinkTrainersDefeated[] = _("{B_LINK_OPPONENT2_NAME} e {B_LINK_OPPONENT1_NAME} hanno\n"
    "perso la sfida!");
#else
static const u8 sText_TwoLinkTrainersDefeated[] = _("Player beat {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("{B_LINK_OPPONENT1_NAME}\n"
    "ha vinto la sfida!");
#else
static const u8 sText_PlayerLostAgainstLinkTrainer[] = _("Player lost against\n{B_LINK_OPPONENT1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerLostToTwo[] = _("{B_LINK_OPPONENT2_NAME} e {B_LINK_OPPONENT1_NAME}\n"
    "hanno vinto la sfida!");
#else
static const u8 sText_PlayerLostToTwo[] = _("Player lost to {B_LINK_OPPONENT1_NAME}\nand {B_LINK_OPPONENT2_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("La sfida contro {B_LINK_OPPONENT1_NAME}\n"
    "si è conclusa in parità!");
#else
static const u8 sText_PlayerBattledToDrawLinkTrainer[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("La sfida con {B_LINK_OPPONENT2_NAME} e\n"
    "{B_LINK_OPPONENT1_NAME} si è conclusa in\l"
    "parità!");
#else
static const u8 sText_PlayerBattledToDrawVsTwo[] = _("Player battled to a draw against\n{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_WildFled[] = _("{PLAY_SE}Ù {B_LINK_OPPONENT1_NAME} se l’è data a gambe!");
#else
static const u8 sText_WildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} fled!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TwoWildFled[] = _("{PLAY_SE}Ù {B_LINK_OPPONENT1_NAME} e\n"
    "{B_LINK_OPPONENT2_NAME} se la sono data a gambe!");
#else
static const u8 sText_TwoWildFled[] = _("{PLAY_SE SE_FLEE}{B_LINK_OPPONENT1_NAME} and\n{B_LINK_OPPONENT2_NAME} fled!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_NoRunningFromTrainers[] = _("Non puoi sottrarti alla\n"
    "lotta con un ALLENATORE!\p");
#else
static const u8 sText_NoRunningFromTrainers[] = _("No! There's no running\nfrom a TRAINER battle!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_CantEscape[] = _("Non si scappa!\p");
#else
static const u8 sText_CantEscape[] = _("Can't escape!\p");
#endif
static const u8 sText_DontLeaveBirch[] = _(""); // Dummied
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ButNothingHappened[] = _("Ma non succede nulla!");
#else
static const u8 sText_ButNothingHappened[] = _("But nothing happened!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ButItFailed[] = _("Ma fallisce!");
#else
static const u8 sText_ButItFailed[] = _("But it failed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItHurtConfusion[] = _("È così confuso da\n"
    "colpirsi da solo!");
#else
static const u8 sText_ItHurtConfusion[] = _("It hurt itself in its\nconfusion!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_MirrorMoveFailed[] = _("La SPECULMOSSA ha fallito!");
#else
static const u8 sText_MirrorMoveFailed[] = _("The MIRROR MOVE failed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StartedToRain[] = _("Inizia a piovere!");
#else
static const u8 sText_StartedToRain[] = _("It started to rain!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DownpourStarted[] = _("Inizia un acquazzone!");
#else
static const u8 sText_DownpourStarted[] = _("A downpour started!"); // corresponds to DownpourText in pokegold and pokecrystal and is used by Rain Dance in GSC
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_RainContinues[] = _("Continua a piovere.");
#else
static const u8 sText_RainContinues[] = _("Rain continues to fall.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DownpourContinues[] = _("L’acquazzone continua.");
#else
static const u8 sText_DownpourContinues[] = _("The downpour continues."); // unused
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_RainStopped[] = _("Ha smesso di piovere.");
#else
static const u8 sText_RainStopped[] = _("The rain stopped.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SandstormBrewed[] = _("Sta arrivando una tempesta di sabbia!");
#else
static const u8 sText_SandstormBrewed[] = _("A sandstorm brewed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SandstormRages[] = _("La tempesta di sabbia imperversa!");
#else
static const u8 sText_SandstormRages[] = _("The sandstorm rages.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SandstormSubsided[] = _("La tempesta di sabbia cessa.");
#else
static const u8 sText_SandstormSubsided[] = _("The sandstorm subsided.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SunlightGotBright[] = _("La luce solare diventa intensa!");
#else
static const u8 sText_SunlightGotBright[] = _("The sunlight got bright!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SunlightStrong[] = _("La luce solare è fortissima!");
#else
static const u8 sText_SunlightStrong[] = _("The sunlight is strong.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SunlightFaded[] = _("La luce solare torna normale!");
#else
static const u8 sText_SunlightFaded[] = _("The sunlight faded.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StartedHail[] = _("Inizia a grandinare!");
#else
static const u8 sText_StartedHail[] = _("It started to hail!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HailContinues[] = _("Continua a grandinare.");
#else
static const u8 sText_HailContinues[] = _("Hail continues to fall.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HailStopped[] = _("Ha smesso di grandinare.");
#else
static const u8 sText_HailStopped[] = _("The hail stopped.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FailedToSpitUp[] = _("Ma SFOGHENERGIA\n"
    "fallisce!");
#else
static const u8 sText_FailedToSpitUp[] = _("But it failed to SPIT UP\na thing!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FailedToSwallow[] = _("Ma INTROENERGIA\n"
    "fallisce!");
#else
static const u8 sText_FailedToSwallow[] = _("But it failed to SWALLOW\na thing!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_WindBecameHeatWave[] = _("Il vento si è trasformato in\n"
    "ONDACALDA!");
#else
static const u8 sText_WindBecameHeatWave[] = _("The wind turned into a\nHEAT WAVE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_StatChangesGone[] = _("Eliminate tutte le modifiche\n"
    "delle statistiche!");
#else
static const u8 sText_StatChangesGone[] = _("All stat changes were\neliminated!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_CoinsScattered[] = _("Ci sono monete sparse ovunque!");
#else
static const u8 sText_CoinsScattered[] = _("Coins scattered everywhere!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TooWeakForSubstitute[] = _("Troppo debole! Non può creare\n"
    "un SOSTITUTO!");
#else
static const u8 sText_TooWeakForSubstitute[] = _("It was too weak to make\na SUBSTITUTE!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SharedPain[] = _("I POKéMON condividono\n"
    "i PS!");
#else
static const u8 sText_SharedPain[] = _("The battlers shared\ntheir pain!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_BellChimed[] = _("Suona la campana!");
#else
static const u8 sText_BellChimed[] = _("A bell chimed!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FaintInThree[] = _("Tutti i POKéMON che la ascoltano\n"
    "saranno esausti in tre turni!");
#else
static const u8 sText_FaintInThree[] = _("All affected POKéMON will\nfaint in three turns!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_NoPPLeft[] = _("Non ha più PP per\n"
    "questa mossa!\p");
#else
static const u8 sText_NoPPLeft[] = _("There's no PP left for\nthis move!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ButNoPPLeft[] = _("Ma non ha più PP per\n"
    "questa mossa!");
#else
static const u8 sText_ButNoPPLeft[] = _("But there was no PP left\nfor the move!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIgnoresAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} ignora gli\n"
    "ordini, sta dormendo!");
#else
static const u8 sText_PkmnIgnoresAsleep[] = _("{B_ATK_NAME_WITH_PREFIX} ignored\norders while asleep!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIgnoredOrders[] = _("{B_ATK_NAME_WITH_PREFIX} ignora gli\n"
    "ordini!");
#else
static const u8 sText_PkmnIgnoredOrders[] = _("{B_ATK_NAME_WITH_PREFIX} ignored\norders!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBeganToNap[] = _("{B_ATK_NAME_WITH_PREFIX} fa un riposino!");
#else
static const u8 sText_PkmnBeganToNap[] = _("{B_ATK_NAME_WITH_PREFIX} began to nap!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLoafing[] = _("{B_ATK_NAME_WITH_PREFIX} sta ciondolando!");
#else
static const u8 sText_PkmnLoafing[] = _("{B_ATK_NAME_WITH_PREFIX} is\nloafing around!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWontObey[] = _("{B_ATK_NAME_WITH_PREFIX} non obbedisce!");
#else
static const u8 sText_PkmnWontObey[] = _("{B_ATK_NAME_WITH_PREFIX} won't\nobey!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnTurnedAway[] = _("{B_ATK_NAME_WITH_PREFIX} disobbedisce!");
#else
static const u8 sText_PkmnTurnedAway[] = _("{B_ATK_NAME_WITH_PREFIX} turned away!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnPretendNotNotice[] = _("{B_ATK_NAME_WITH_PREFIX} fa finta \n"
    "di niente!");
#else
static const u8 sText_PkmnPretendNotNotice[] = _("{B_ATK_NAME_WITH_PREFIX} pretended\nnot to notice!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_EnemyAboutToSwitchPkmn[] = _("La prossima scelta di {B_TRAINER1_NAME},\n"
    "{B_TRAINER1_CLASS}, sarà {PLAYER}.\p"
    "{B_PLAYER_NAME}, vuoi cambiare\n"
    "POKéMON?");
#else
static const u8 sText_EnemyAboutToSwitchPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} is\nabout to use {B_BUFF2}.\pWill {B_PLAYER_NAME} change\nPOKéMON?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnLearnedMove2[] = _("{B_ATK_NAME_WITH_PREFIX} ha imparato\n"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_PkmnLearnedMove2[] = _("{B_ATK_NAME_WITH_PREFIX} learned\n{B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("Hai avuto la meglio su\n"
    "{B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!\p");
#else
static const u8 sText_PlayerDefeatedLinkTrainerTrainer1[] = _("Player defeated\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ThrewARock[] = _("{B_PLAYER_NAME} lancia\n"
    "un SASSO a {RIVAL}!");
#else
static const u8 sText_ThrewARock[] = _("{B_PLAYER_NAME} threw a ROCK\nat the {B_OPPONENT_MON1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ThrewSomeBait[] = _("{B_PLAYER_NAME} lancia\n"
    "l’ESCA a {RIVAL}!");
#else
static const u8 sText_ThrewSomeBait[] = _("{B_PLAYER_NAME} threw some BAIT\nat the {B_OPPONENT_MON1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnWatchingCarefully[] = _("{RIVAL} guarda\n"
    "attentamente!");
#else
static const u8 sText_PkmnWatchingCarefully[] = _("{B_OPPONENT_MON1_NAME} is watching\ncarefully!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIsAngry[] = _("{RIVAL} è infuriato!");
#else
static const u8 sText_PkmnIsAngry[] = _("{B_OPPONENT_MON1_NAME} is angry!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnIsEating[] = _("{RIVAL} mangia!");
#else
static const u8 sText_PkmnIsEating[] = _("{B_OPPONENT_MON1_NAME} is eating!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_OutOfSafariBalls[] = _("{PLAY_SE}ぢ ANNUNCIO: Hai finito tutte le\n"
    "SAFARI BALL! Fine del gioco!\p");
#else
static const u8 sText_OutOfSafariBalls[] = _("{PLAY_SE SE_DING_DONG}ANNOUNCER: You're out of\nSAFARI BALLS! Game over!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_WildPkmnAppeared[] = _("Appare {RIVAL} selvatico!\p");
#else
static const u8 sText_WildPkmnAppeared[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_WildPkmnAppeared2[] = _("Appare {RIVAL} selvatico!\p");
#else
static const u8 sText_WildPkmnAppeared2[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_WildPkmnAppearedPause[] = _("Appare {RIVAL} selvatico!{PAUSE 127}");
#else
static const u8 sText_WildPkmnAppearedPause[] = _("Wild {B_OPPONENT_MON1_NAME} appeared!{PAUSE 127}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TwoWildPkmnAppeared[] = _("Appaiono {EVIL_TEAM} e\n"
    "{RIVAL} selvatici!\p");
#else
static const u8 sText_TwoWildPkmnAppeared[] = _("Wild {B_OPPONENT_MON1_NAME} and\n{B_OPPONENT_MON2_NAME} appeared!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GhostAppearedCantId[] = _("Appare lo SPETTRO!\p"
    "Uffa!\n"
    "Lo SPETTRO non può essere\l"
    "identificato!\p");
#else
static const u8 sText_GhostAppearedCantId[] = _("The GHOST appeared!\pDarn!\nThe GHOST can't be ID'd!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TheGhostAppeared[] = _("Appare lo SPETTRO!\p");
#else
static const u8 sText_TheGhostAppeared[] = _("The GHOST appeared!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SilphScopeUnveil[] = _("La SPETTROSONDA rivela l’identità\n"
    "dello SPETTRO!");
#else
static const u8 sText_SilphScopeUnveil[] = _("SILPH SCOPE unveiled the GHOST's\nidentity!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TheGhostWas[] = _("Lo SPETTRO era MAROWAK!\p");
#else
static const u8 sText_TheGhostWas[] = _("The GHOST was MAROWAK!\p\n");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1WantsToBattle[] = _("Parte la sfida di\n"
    "{B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!\p");
#else
static const u8 sText_Trainer1WantsToBattle[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nwould like to battle!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkTrainerWantsToBattle[] = _("Parte la sfida di\n"
    "{B_LINK_OPPONENT1_NAME}!");
#else
static const u8 sText_LinkTrainerWantsToBattle[] = _("{B_LINK_OPPONENT1_NAME}\nwants to battle!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("Parte la sfida di\n"
    "{B_LINK_OPPONENT1_NAME} e {B_LINK_OPPONENT2_NAME}!");
#else
static const u8 sText_TwoLinkTrainersWantToBattle[] = _("{B_LINK_OPPONENT1_NAME} and {B_LINK_OPPONENT2_NAME}\nwant to battle!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1SentOutPkmn[] = _("È il turno di {RIVAL}, mandato in\n"
    "campo da {B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!{PAUSE 60}");
#else
static const u8 sText_Trainer1SentOutPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent\nout {B_OPPONENT_MON1_NAME}!{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("Ecco {RIVAL} e {EVIL_TEAM},\n"
    "mandati in campo da\l"
    "{B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!{PAUSE 60}");
#else
static const u8 sText_Trainer1SentOutTwoPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent\nout {B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1SentOutPkmn2[] = _("È il turno di {UNKNOWN_STR}, mandato in\n"
    "campo da {B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!");
#else
static const u8 sText_Trainer1SentOutPkmn2[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME} sent\nout {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} manda\n"
    "in campo {RIVAL}!");
#else
static const u8 sText_LinkTrainerSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out\n{B_OPPONENT_MON1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME} manda in campo\n"
    "{RIVAL} e {EVIL_TEAM}!");
#else
static const u8 sText_LinkTrainerSentOutTwoPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out\n{B_OPPONENT_MON1_NAME} and {B_OPPONENT_MON2_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TwoLinkTrainersSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} manda in campo\n"
    "{EVIL_LEADER}!\p"
    "{B_LINK_OPPONENT2_NAME} manda in campo\n"
    "{EVIL_LEGENDARY}!");
#else
static const u8 sText_TwoLinkTrainersSentOutPkmn[] = _("{B_LINK_OPPONENT1_NAME} sent out {B_LINK_OPPONENT_MON1_NAME}!\n{B_LINK_OPPONENT2_NAME} sent out {B_LINK_OPPONENT_MON2_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{B_LINK_OPPONENT1_NAME} manda in campo\n"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_LinkTrainerSentOutPkmn2[] = _("{B_LINK_OPPONENT1_NAME} sent out\n{B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} manda in campo\n"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_LinkTrainerMultiSentOutPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} sent out\n{B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GoPkmn[] = _("Vai, {KUN}!");
#else
static const u8 sText_GoPkmn[] = _("Go! {B_PLAYER_MON1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GoTwoPkmn[] = _("Avanti, {KUN} e\n"
    "{VERSION}!");
#else
static const u8 sText_GoTwoPkmn[] = _("Go! {B_PLAYER_MON1_NAME} and\n{B_PLAYER_MON2_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GoPkmn2[] = _("Vai, {UNKNOWN_STR}!");
#else
static const u8 sText_GoPkmn2[] = _("Go! {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DoItPkmn[] = _("Dai, {UNKNOWN_STR}!");
#else
static const u8 sText_DoItPkmn[] = _("Do it! {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GoForItPkmn[] = _("Coraggio, {UNKNOWN_STR}!");
#else
static const u8 sText_GoForItPkmn[] = _("Go for it, {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("Nemico debole!\n"
    "Forza, {UNKNOWN_STR}!");
#else
static const u8 sText_YourFoesWeakGetEmPkmn[] = _("Your foe's weak!\nGet 'em, {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkPartnerSentOutPkmnGoPkmn[] = _("{B_LINK_PARTNER_NAME} manda in campo\n"
    "{GOOD_LEADER}!\p"
    "Vai, {GOOD_TEAM}!");
#else
static const u8 sText_LinkPartnerSentOutPkmnGoPkmn[] = _("{B_LINK_PARTNER_NAME} sent out {B_LINK_PLAYER_MON2_NAME}!\nGo! {B_LINK_PLAYER_MON1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnThatsEnough[] = _("{UNKNOWN_STR}, basta così!\n"
    "Rientra!");
#else
static const u8 sText_PkmnThatsEnough[] = _("{B_BUFF1}, that's enough!\nCome back!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnComeBack[] = _("{UNKNOWN_STR}, rientra!");
#else
static const u8 sText_PkmnComeBack[] = _("{B_BUFF1}, come back!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnOkComeBack[] = _("{UNKNOWN_STR}, OK!\n"
    "Rientra!");
#else
static const u8 sText_PkmnOkComeBack[] = _("{B_BUFF1}, OK!\nCome back!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 sText_PkmnGoodComeBack[] = _("{UNKNOWN_STR}, bene!\n"
    "Rientra!");
#else
const u8 sText_PkmnGoodComeBack[] = _("{B_BUFF1}, good!\nCome back!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1WithdrewPkmn[] = _("{UNKNOWN_STR} è ritirato dalla lotta\n"
    "da {B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!");
#else
static const u8 sText_Trainer1WithdrewPkmn[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nwithdrew {B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME} ritira\n"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_LinkTrainer1WithdrewPkmn[] = _("{B_LINK_OPPONENT1_NAME} withdrew\n{B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} ritira\n"
    "{UNKNOWN_STR}!");
#else
static const u8 sText_LinkTrainer2WithdrewPkmn[] = _("{B_LINK_SCR_TRAINER_NAME} withdrew\n{B_BUFF1}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_WildPkmnPrefix[] = _(" selvatico");
#else
static const u8 sText_WildPkmnPrefix[] = _("Wild ");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FoePkmnPrefix[] = _(" nemico");
#else
static const u8 sText_FoePkmnPrefix[] = _("Foe ");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FoePkmnPrefix2[] = _("nemico");
#else
static const u8 sText_FoePkmnPrefix2[] = _("Foe");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AllyPkmnPrefix[] = _("amico");
#else
static const u8 sText_AllyPkmnPrefix[] = _("Ally");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FoePkmnPrefix3[] = _("nemico");
#else
static const u8 sText_FoePkmnPrefix3[] = _("Foe");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AllyPkmnPrefix2[] = _("amico");
#else
static const u8 sText_AllyPkmnPrefix2[] = _("Ally");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_FoePkmnPrefix4[] = _("nemico");
#else
static const u8 sText_FoePkmnPrefix4[] = _("Foe");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AllyPkmnPrefix3[] = _("amico");
#else
static const u8 sText_AllyPkmnPrefix3[] = _("Ally");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX} usa\n"
    "{PLAYER}");
#else
static const u8 sText_AttackerUsedX[] = _("{B_ATK_NAME_WITH_PREFIX} used\n{B_BUFF2}");
#endif
static const u8 sText_ExclamationMark[] = _("!");
static const u8 sText_ExclamationMark2[] = _("!");
static const u8 sText_ExclamationMark3[] = _("!");
static const u8 sText_ExclamationMark4[] = _("!");
static const u8 sText_ExclamationMark5[] = _("!");

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HP2[] = _("PS");
#else
static const u8 sText_HP2[] = _("HP");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Attack2[] = _("ATTACCO");
#else
static const u8 sText_Attack2[] = _("ATTACK");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Defense2[] = _("DIFESA");
#else
static const u8 sText_Defense2[] = _("DEFENSE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Speed[] = _("VELOC.");
#else
static const u8 sText_Speed[] = _("SPEED");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SpAtk2[] = _("ATT. SPEC.");
#else
static const u8 sText_SpAtk2[] = _("SP. ATK");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SpDef2[] = _("DIF. SPEC.");
#else
static const u8 sText_SpDef2[] = _("SP. DEF");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Accuracy[] = _("precisione");
#else
static const u8 sText_Accuracy[] = _("accuracy");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Evasiveness[] = _("elusione");
#else
static const u8 sText_Evasiveness[] = _("evasiveness");
#endif

const u8 *const gStatNamesTable[] = {
    sText_HP2,
    sText_Attack2,
    sText_Defense2,
    sText_Speed,
    sText_SpAtk2,
    sText_SpDef2,
    sText_Accuracy,
    sText_Evasiveness
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PokeblockWasTooSpicy[] = _("è troppo pepata!");
#else
static const u8 sText_PokeblockWasTooSpicy[] = _("was too spicy!"); //
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PokeblockWasTooDry[] = _("è troppo secca!");
#else
static const u8 sText_PokeblockWasTooDry[] = _("was too dry!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PokeblockWasTooSweet[] = _("è troppo dolce!");
#else
static const u8 sText_PokeblockWasTooSweet[] = _("was too sweet!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PokeblockWasTooBitter[] = _("è troppo amara!");
#else
static const u8 sText_PokeblockWasTooBitter[] = _("was too bitter!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PokeblockWasTooSour[] = _("è troppo aspra!");
#else
static const u8 sText_PokeblockWasTooSour[] = _("was too sour!");
#endif

const u8 *const gPokeblockWasTooXStringTable[] = {
    sText_PokeblockWasTooSpicy,
    sText_PokeblockWasTooDry,
    sText_PokeblockWasTooSweet,
    sText_PokeblockWasTooBitter,
    sText_PokeblockWasTooSour
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerUsedItem[] = _("{B_PLAYER_NAME} usa {B_LAST_ITEM}!");
#else
static const u8 sText_PlayerUsedItem[] = _("{B_PLAYER_NAME} used\n{B_LAST_ITEM}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_OldManUsedItem[] = _("Il vecchietto usa\n"
    "{B_LAST_ITEM}!");
#else
static const u8 sText_OldManUsedItem[] = _("The old man used\n{B_LAST_ITEM}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PokedudeUsedItem[] = _("GUIDO usa\n"
    "{B_LAST_ITEM}!");
#else
static const u8 sText_PokedudeUsedItem[] = _("The POKé DUDE used\n{B_LAST_ITEM}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1UsedItem[] = _("{B_LAST_ITEM} è lo strumento usato\n"
    "da {B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!");
#else
static const u8 sText_Trainer1UsedItem[] = _("{B_TRAINER1_CLASS} {B_TRAINER1_NAME}\nused {B_LAST_ITEM}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TrainerBlockedBall[] = _("La BALL è stata bloccata!");
#else
static const u8 sText_TrainerBlockedBall[] = _("The TRAINER blocked the BALL!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DontBeAThief[] = _("Non si ruba!");
#else
static const u8 sText_DontBeAThief[] = _("Don't be a thief!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItDodgedBall[] = _("Ha schivato la BALL! Questo\n"
    "POKéMON non può essere catturato!");
#else
static const u8 sText_ItDodgedBall[] = _("It dodged the thrown BALL!\nThis POKéMON can't be caught!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_YouMissedPkmn[] = _("Ti è sfuggito il POKéMON!");
#else
static const u8 sText_YouMissedPkmn[] = _("You missed the POKéMON!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnBrokeFree[] = _("Oh, no!\n"
    "Il POKéMON si è liberato!");
#else
static const u8 sText_PkmnBrokeFree[] = _("Oh, no!\nThe POKéMON broke free!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItAppearedCaught[] = _("Ah! Sembrava preso,\n"
    "eh? E invece no!");
#else
static const u8 sText_ItAppearedCaught[] = _("Aww!\nIt appeared to be caught!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AarghAlmostHadIt[] = _("Grrr!\n"
    "Per un pelo!");
#else
static const u8 sText_AarghAlmostHadIt[] = _("Aargh!\nAlmost had it!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ShootSoClose[] = _("Nooo!\n"
    "Era così vicino!");
#else
static const u8 sText_ShootSoClose[] = _("Shoot!\nIt was so close, too!");
#endif
static const u8 sText_ItDodgedBall2[] = _("よけられた!\nこいつは つかまりそうにないぞ!"); // Unused version of the Marowak ghost dodging text
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GotchaPkmnCaught[] = _("Preso!\n"
    "{RIVAL} è catturato!{WAIT_SE}{PLAY_BGM}ぢÀ\p");
#else
static const u8 sText_GotchaPkmnCaught[] = _("Gotcha!\n{B_OPPONENT_MON1_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GotchaPkmnCaught2[] = _("Preso!\n"
    "{RIVAL} è catturato!{WAIT_SE}{PLAY_BGM}ぢÀ{PAUSE 127}");
#else
static const u8 sText_GotchaPkmnCaught2[] = _("Gotcha!\n{B_OPPONENT_MON1_NAME} was caught!{WAIT_SE}{PLAY_BGM MUS_CAUGHT}{PAUSE 127}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_GiveNicknameCaptured[] = _("Vuoi dare un soprannome\n"
    "a {RIVAL}?");
#else
static const u8 sText_GiveNicknameCaptured[] = _("Give a nickname to the\ncaptured {B_OPPONENT_MON1_NAME}?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnSentToPC[] = _("{RIVAL} è stato inviato\n"
    "al PC di {B_PC_CREATOR_NAME}.");
#else
static const u8 sText_PkmnSentToPC[] = _("{B_OPPONENT_MON1_NAME} was sent to\n{B_PC_CREATOR_NAME} PC.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Someones[] = _("???");
#else
static const u8 sText_Someones[] = _("someone's");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Bills[] = _("BILL");
#else
static const u8 sText_Bills[] = _("BILL's");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnDataAddedToDex[] = _("I dati di {RIVAL} sono stati\n"
    "inseriti nel POKéDEX.\p");
#else
static const u8 sText_PkmnDataAddedToDex[] = _("{B_OPPONENT_MON1_NAME}'s data was\nadded to the POKéDEX.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItIsRaining[] = _("Piove.");
#else
static const u8 sText_ItIsRaining[] = _("It is raining."); // used only in RSE when a battle starts in a rainy area
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SandstormIsRaging[] = _("Imperversa una tempesta di sabbia.");
#else
static const u8 sText_SandstormIsRaging[] = _("A sandstorm is raging.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_BoxIsFull[] = _("I BOX sono pieni!\n"
    "Non ne puoi catturare altri!\p");
#else
static const u8 sText_BoxIsFull[] = _("The BOX is full!\nYou can't catch any more!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_EnigmaBerry[] = _("BACCAENIGMA");
#else
static const u8 sText_EnigmaBerry[] = _("ENIGMA BERRY");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_BerrySuffix[] = _("BACCA{STR_VAR_3}");
#else
static const u8 sText_BerrySuffix[] = _(" BERRY");
#endif
static const u8 sText_Enigma[] = _("ナゾ");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemCuredParalysis[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "guarisce la paralisi!");
#else
static const u8 sText_PkmnsItemCuredParalysis[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured paralysis!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemCuredPoison[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "guarisce l’avvelenamento!");
#else
static const u8 sText_PkmnsItemCuredPoison[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured poison!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemHealedBurn[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "guarisce la scottatura!");
#else
static const u8 sText_PkmnsItemHealedBurn[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nhealed its burn!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemDefrostedIt[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "l’ha scongelato!");
#else
static const u8 sText_PkmnsItemDefrostedIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ndefrosted it!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemWokeIt[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "l’ha svegliato!");
#else
static const u8 sText_PkmnsItemWokeIt[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nwoke it from its sleep!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemSnappedOut[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "elimina la sua confusione!");
#else
static const u8 sText_PkmnsItemSnappedOut[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nsnapped it out of confusion!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemCuredProblem[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "risolve il problema di {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnsItemCuredProblem[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\ncured its {B_BUFF1} problem!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemNormalizedStatus[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "normalizza il suo stato!");
#else
static const u8 sText_PkmnsItemNormalizedStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nnormalized its status!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemRestoredHealth[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "ristabilisce la salute!");
#else
static const u8 sText_PkmnsItemRestoredHealth[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored health!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemRestoredPP[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "ristabilisce i PP di {UNKNOWN_STR}!");
#else
static const u8 sText_PkmnsItemRestoredPP[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored {B_BUFF1}'s PP!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemRestoredStatus[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "ristabilisce le sue statistiche!");
#else
static const u8 sText_PkmnsItemRestoredStatus[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored its status!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnsItemRestoredHPALittle[] = _("{B_LAST_ITEM} di {B_SCR_ACTIVE_NAME_WITH_PREFIX}\n"
    "ristabilisce parte dei PS!");
#else
static const u8 sText_PkmnsItemRestoredHPALittle[] = _("{B_SCR_ACTIVE_NAME_WITH_PREFIX}'s {B_LAST_ITEM}\nrestored its HP a little!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ItemAllowsOnlyYMove[] = _("{B_LAST_ITEM} consente di usare\n"
    "soltanto {B_CURRENT_MOVE}!\p");
#else
static const u8 sText_ItemAllowsOnlyYMove[] = _("{B_LAST_ITEM}'s effect allows only\n{B_CURRENT_MOVE} to be used!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHungOnWithX[] = _("{B_DEF_NAME_WITH_PREFIX} resiste\n"
    "usando {B_LAST_ITEM}!");
#else
static const u8 sText_PkmnHungOnWithX[] = _("{B_DEF_NAME_WITH_PREFIX} hung on\nusing its {B_LAST_ITEM}!");
#endif
const u8 gText_EmptyString3[] = _("");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayedFluteCatchyTune[] = _("{B_PLAYER_NAME} ha suonato il {B_LAST_ITEM}.\p"
    "È una melodia orecchiabile!");
#else
static const u8 sText_PlayedFluteCatchyTune[] = _("{B_PLAYER_NAME} played the {B_LAST_ITEM}.\pNow, that's a catchy tune!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayedThe[] = _("{B_PLAYER_NAME} suona\n"
    "il {B_LAST_ITEM}.");
#else
static const u8 sText_PlayedThe[] = _("{B_PLAYER_NAME} played the\n{B_LAST_ITEM}.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PkmnHearingFluteAwoke[] = _("Il FLAUTO risveglia il POKéMON\n"
    "addormentato.");
#else
static const u8 sText_PkmnHearingFluteAwoke[] = _("The POKéMON hearing the FLUTE\nawoke!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_YouThrowABallNowRight[] = _("Ora si lancia una BALL, vero?\n"
    "Cercherò di fare del mio meglio!");
#else
static const u8 sText_YouThrowABallNowRight[] = _("You throw a BALL now, right?\nI… I'll do my best!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ForPetesSake[] = _("OAK: Ehi, quanta fretta!\p"
    "{B_PLAYER_NAME}, è la tua prima lotta,\n"
    "vero?\p"
    "Beh, allora devo farti\n"
    "un’introduzione.\p"
    "In una Lotta di POKéMON, gli\n"
    "ALLENATORI si sfidano facendo\l"
    "combattere i loro POKéMON.\p");
#else
const u8 gText_ForPetesSake[] = _("OAK: Oh, for Pete's sake…\nSo pushy, as always.\p{B_PLAYER_NAME}.\pYou've never had a POKéMON battle\nbefore, have you?\pA POKéMON battle is when TRAINERS\npit their POKéMON against each\lother.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_TheTrainerThat[] = _("Vince chi manda KO tutti i POKéMON\n"
    "dell’ALLENATORE avversario,\l"
    "riducendo a zero i loro PS.\p");
#else
const u8 gText_TheTrainerThat[] = _("The TRAINER that makes the other\nTRAINER's POKéMON faint by lowering\ltheir HP to “0,” wins.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_TryBattling[] = _("Forse, però, invece di tante\n"
    "parole, è meglio una\l"
    "dimostrazione pratica. \p"
    "Dai, prova a lottare!");
#else
const u8 gText_TryBattling[] = _("But rather than talking about it,\nyou'll learn more from experience.\pTry battling and see for yourself.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_InflictingDamageIsKey[] = _("OAK: L’obiettivo principale di ogni\n"
    "lotta è infliggere danni\l"
    "all’avversario.\p");
#else
const u8 gText_InflictingDamageIsKey[] = _("OAK: Inflicting damage on the foe\nis the key to any battle.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_LoweringStats[] = _("OAK: Diminuendo le statistiche\n"
    "dell’avversario potrai acquisire un\l"
    "vantaggio.\p");
#else
const u8 gText_LoweringStats[] = _("OAK: Lowering the foe's stats\nwill put you at an advantage.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_KeepAnEyeOnHP[] = _("OAK: Non perdere mai di vista i PS\n"
    "del tuo POKéMON in campo.\p"
    "Se si riducono a zero, il POKéMON\n"
    "va KO e non può più lottare.\p");
#else
const u8 gText_KeepAnEyeOnHP[] = _("OAK: Keep your eyes on your\nPOKéMON's HP.\pIt will faint if the HP drops to\n“0.”\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_OakNoRunningFromATrainer[] = _("OAK: No! Non puoi sottrarti ad una\n"
    "lotta con un ALLENATORE!\p");
#else
const u8 gText_OakNoRunningFromATrainer[] = _("OAK: No! There's no running away\nfrom a TRAINER POKéMON battle!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_WinEarnsPrizeMoney[] = _("OAK: Eccellente!\p"
    "Se vinci ottieni un premio in denaro\n"
    "e i tuoi POKéMON guadagnano\l"
    "punti ESP.\p"
    "Rafforza i tuoi POKéMON sfidando\n"
    "gli ALLENATORI!\p");
#else
const u8 gText_WinEarnsPrizeMoney[] = _("OAK: Hm! Excellent!\pIf you win, you earn prize money,\nand your POKéMON will grow!\pBattle other TRAINERS and make\nyour POKéMON strong!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_HowDissapointing[] = _("OAK: Che delusione…\p"
    "Se vinci ottieni un premio in denaro\n"
    "e i tuoi POKéMON guadagnano\l"
    "punti ESP.\p"
    "Ma se perdi, {B_PLAYER_NAME}, sei tu a\n"
    "dover pagare un premio in denaro.\p"
    "OK, per questa volta pago io per\n"
    "te, visto che non lo sapevi.\p"
    "Fuori di qui, però, nessuno ti farà\n"
    "sconti, ricordatelo!\p"
    "Per questo ti conviene rafforzare\n"
    "la tua squadra affrontando i\l"
    "POKéMON selvatici che incontri.\p");
#else
const u8 gText_HowDissapointing[] = _("OAK: Hm…\nHow disappointing…\pIf you win, you earn prize money,\nand your POKéMON grow.\pBut if you lose, {B_PLAYER_NAME}, you end\nup paying prize money…\pHowever, since you had no warning\nthis time, I'll pay for you.\pBut things won't be this way once\nyou step outside these doors.\pThat's why you must strengthen your\nPOKéMON by battling wild POKéMON.\p");
#endif

const u8 *const gBattleStringsTable[BATTLESTRINGS_COUNT - BATTLESTRINGS_TABLE_START] = {
    [STRINGID_TRAINER1LOSETEXT - BATTLESTRINGS_TABLE_START]              = sText_Trainer1LoseText,
    [STRINGID_PKMNGAINEDEXP - BATTLESTRINGS_TABLE_START]                 = sText_PkmnGainedEXP,
    [STRINGID_PKMNGREWTOLV - BATTLESTRINGS_TABLE_START]                  = sText_PkmnGrewToLv,
    [STRINGID_PKMNLEARNEDMOVE - BATTLESTRINGS_TABLE_START]               = sText_PkmnLearnedMove,
    [STRINGID_TRYTOLEARNMOVE1 - BATTLESTRINGS_TABLE_START]               = sText_TryToLearnMove1,
    [STRINGID_TRYTOLEARNMOVE2 - BATTLESTRINGS_TABLE_START]               = sText_TryToLearnMove2,
    [STRINGID_TRYTOLEARNMOVE3 - BATTLESTRINGS_TABLE_START]               = sText_TryToLearnMove3,
    [STRINGID_PKMNFORGOTMOVE - BATTLESTRINGS_TABLE_START]                = sText_PkmnForgotMove,
    [STRINGID_STOPLEARNINGMOVE - BATTLESTRINGS_TABLE_START]              = sText_StopLearningMove,
    [STRINGID_DIDNOTLEARNMOVE - BATTLESTRINGS_TABLE_START]               = sText_DidNotLearnMove,
    [STRINGID_PKMNLEARNEDMOVE2 - BATTLESTRINGS_TABLE_START]              = sText_PkmnLearnedMove2,
    [STRINGID_ATTACKMISSED - BATTLESTRINGS_TABLE_START]                  = sText_AttackMissed,
    [STRINGID_PKMNPROTECTEDITSELF - BATTLESTRINGS_TABLE_START]           = sText_PkmnProtectedItself,
    [STRINGID_STATSWONTINCREASE2 - BATTLESTRINGS_TABLE_START]            = sText_StatsWontIncrease2,
    [STRINGID_AVOIDEDDAMAGE - BATTLESTRINGS_TABLE_START]                 = sText_AvoidedDamage,
    [STRINGID_ITDOESNTAFFECT - BATTLESTRINGS_TABLE_START]                = sText_ItDoesntAffect,
    [STRINGID_ATTACKERFAINTED - BATTLESTRINGS_TABLE_START]               = sText_AttackerFainted,
    [STRINGID_TARGETFAINTED - BATTLESTRINGS_TABLE_START]                 = sText_TargetFainted,
    [STRINGID_PLAYERGOTMONEY - BATTLESTRINGS_TABLE_START]                = sText_PlayerGotMoney,
    [STRINGID_PLAYERWHITEOUT - BATTLESTRINGS_TABLE_START]                = sText_PlayerWhiteout,
    [STRINGID_PLAYERWHITEOUT2 - BATTLESTRINGS_TABLE_START]               = sText_PlayerPanicked,
    [STRINGID_PREVENTSESCAPE - BATTLESTRINGS_TABLE_START]                = sText_PreventsEscape,
    [STRINGID_HITXTIMES - BATTLESTRINGS_TABLE_START]                     = sText_HitXTimes,
    [STRINGID_PKMNFELLASLEEP - BATTLESTRINGS_TABLE_START]                = sText_PkmnFellAsleep,
    [STRINGID_PKMNMADESLEEP - BATTLESTRINGS_TABLE_START]                 = sText_PkmnMadeSleep,
    [STRINGID_PKMNALREADYASLEEP - BATTLESTRINGS_TABLE_START]             = sText_PkmnAlreadyAsleep,
    [STRINGID_PKMNALREADYASLEEP2 - BATTLESTRINGS_TABLE_START]            = sText_PkmnAlreadyAsleep2,
    [STRINGID_PKMNWASNTAFFECTED - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasntAffected,
    [STRINGID_PKMNWASPOISONED - BATTLESTRINGS_TABLE_START]               = sText_PkmnWasPoisoned,
    [STRINGID_PKMNPOISONEDBY - BATTLESTRINGS_TABLE_START]                = sText_PkmnPoisonedBy,
    [STRINGID_PKMNHURTBYPOISON - BATTLESTRINGS_TABLE_START]              = sText_PkmnHurtByPoison,
    [STRINGID_PKMNALREADYPOISONED - BATTLESTRINGS_TABLE_START]           = sText_PkmnAlreadyPoisoned,
    [STRINGID_PKMNBADLYPOISONED - BATTLESTRINGS_TABLE_START]             = sText_PkmnBadlyPoisoned,
    [STRINGID_PKMNENERGYDRAINED - BATTLESTRINGS_TABLE_START]             = sText_PkmnEnergyDrained,
    [STRINGID_PKMNWASBURNED - BATTLESTRINGS_TABLE_START]                 = sText_PkmnWasBurned,
    [STRINGID_PKMNBURNEDBY - BATTLESTRINGS_TABLE_START]                  = sText_PkmnBurnedBy,
    [STRINGID_PKMNHURTBYBURN - BATTLESTRINGS_TABLE_START]                = sText_PkmnHurtByBurn,
    [STRINGID_PKMNWASFROZEN - BATTLESTRINGS_TABLE_START]                 = sText_PkmnWasFrozen,
    [STRINGID_PKMNFROZENBY - BATTLESTRINGS_TABLE_START]                  = sText_PkmnFrozenBy,
    [STRINGID_PKMNISFROZEN - BATTLESTRINGS_TABLE_START]                  = sText_PkmnIsFrozen,
    [STRINGID_PKMNWASDEFROSTED - BATTLESTRINGS_TABLE_START]              = sText_PkmnWasDefrosted,
    [STRINGID_PKMNWASDEFROSTED2 - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasDefrosted2,
    [STRINGID_PKMNWASDEFROSTEDBY - BATTLESTRINGS_TABLE_START]            = sText_PkmnWasDefrostedBy,
    [STRINGID_PKMNWASPARALYZED - BATTLESTRINGS_TABLE_START]              = sText_PkmnWasParalyzed,
    [STRINGID_PKMNWASPARALYZEDBY - BATTLESTRINGS_TABLE_START]            = sText_PkmnWasParalyzedBy,
    [STRINGID_PKMNISPARALYZED - BATTLESTRINGS_TABLE_START]               = sText_PkmnIsParalyzed,
    [STRINGID_PKMNISALREADYPARALYZED - BATTLESTRINGS_TABLE_START]        = sText_PkmnIsAlreadyParalyzed,
    [STRINGID_PKMNHEALEDPARALYSIS - BATTLESTRINGS_TABLE_START]           = sText_PkmnHealedParalysis,
    [STRINGID_PKMNDREAMEATEN - BATTLESTRINGS_TABLE_START]                = sText_PkmnDreamEaten,
    [STRINGID_STATSWONTINCREASE - BATTLESTRINGS_TABLE_START]             = sText_StatsWontIncrease,
    [STRINGID_STATSWONTDECREASE - BATTLESTRINGS_TABLE_START]             = sText_StatsWontDecrease,
    [STRINGID_TEAMSTOPPEDWORKING - BATTLESTRINGS_TABLE_START]            = sText_TeamStoppedWorking,
    [STRINGID_FOESTOPPEDWORKING - BATTLESTRINGS_TABLE_START]             = sText_FoeStoppedWorking,
    [STRINGID_PKMNISCONFUSED - BATTLESTRINGS_TABLE_START]                = sText_PkmnIsConfused,
    [STRINGID_PKMNHEALEDCONFUSION - BATTLESTRINGS_TABLE_START]           = sText_PkmnHealedConfusion,
    [STRINGID_PKMNWASCONFUSED - BATTLESTRINGS_TABLE_START]               = sText_PkmnWasConfused,
    [STRINGID_PKMNALREADYCONFUSED - BATTLESTRINGS_TABLE_START]           = sText_PkmnAlreadyConfused,
    [STRINGID_PKMNFELLINLOVE - BATTLESTRINGS_TABLE_START]                = sText_PkmnFellInLove,
    [STRINGID_PKMNINLOVE - BATTLESTRINGS_TABLE_START]                    = sText_PkmnInLove,
    [STRINGID_PKMNIMMOBILIZEDBYLOVE - BATTLESTRINGS_TABLE_START]         = sText_PkmnImmobilizedByLove,
    [STRINGID_PKMNBLOWNAWAY - BATTLESTRINGS_TABLE_START]                 = sText_PkmnBlownAway,
    [STRINGID_PKMNCHANGEDTYPE - BATTLESTRINGS_TABLE_START]               = sText_PkmnChangedType,
    [STRINGID_PKMNFLINCHED - BATTLESTRINGS_TABLE_START]                  = sText_PkmnFlinched,
    [STRINGID_PKMNREGAINEDHEALTH - BATTLESTRINGS_TABLE_START]            = sText_PkmnRegainedHealth,
    [STRINGID_PKMNHPFULL - BATTLESTRINGS_TABLE_START]                    = sText_PkmnHPFull,
    [STRINGID_PKMNRAISEDSPDEF - BATTLESTRINGS_TABLE_START]               = sText_PkmnRaisedSpDef,
    [STRINGID_PKMNRAISEDDEF - BATTLESTRINGS_TABLE_START]                 = sText_PkmnRaisedDef,
    [STRINGID_PKMNCOVEREDBYVEIL - BATTLESTRINGS_TABLE_START]             = sText_PkmnCoveredByVeil,
    [STRINGID_PKMNUSEDSAFEGUARD - BATTLESTRINGS_TABLE_START]             = sText_PkmnUsedSafeguard,
    [STRINGID_PKMNSAFEGUARDEXPIRED - BATTLESTRINGS_TABLE_START]          = sText_PkmnSafeguardExpired,
    [STRINGID_PKMNWENTTOSLEEP - BATTLESTRINGS_TABLE_START]               = sText_PkmnWentToSleep,
    [STRINGID_PKMNSLEPTHEALTHY - BATTLESTRINGS_TABLE_START]              = sText_PkmnSleptHealthy,
    [STRINGID_PKMNWHIPPEDWHIRLWIND - BATTLESTRINGS_TABLE_START]          = sText_PkmnWhippedWhirlwind,
    [STRINGID_PKMNTOOKSUNLIGHT - BATTLESTRINGS_TABLE_START]              = sText_PkmnTookSunlight,
    [STRINGID_PKMNLOWEREDHEAD - BATTLESTRINGS_TABLE_START]               = sText_PkmnLoweredHead,
    [STRINGID_PKMNISGLOWING - BATTLESTRINGS_TABLE_START]                 = sText_PkmnIsGlowing,
    [STRINGID_PKMNFLEWHIGH - BATTLESTRINGS_TABLE_START]                  = sText_PkmnFlewHigh,
    [STRINGID_PKMNDUGHOLE - BATTLESTRINGS_TABLE_START]                   = sText_PkmnDugHole,
    [STRINGID_PKMNSQUEEZEDBYBIND - BATTLESTRINGS_TABLE_START]            = sText_PkmnSqueezedByBind,
    [STRINGID_PKMNTRAPPEDINVORTEX - BATTLESTRINGS_TABLE_START]           = sText_PkmnTrappedInVortex,
    [STRINGID_PKMNWRAPPEDBY - BATTLESTRINGS_TABLE_START]                 = sText_PkmnWrappedBy,
    [STRINGID_PKMNCLAMPED - BATTLESTRINGS_TABLE_START]                   = sText_PkmnClamped,
    [STRINGID_PKMNHURTBY - BATTLESTRINGS_TABLE_START]                    = sText_PkmnHurtBy,
    [STRINGID_PKMNFREEDFROM - BATTLESTRINGS_TABLE_START]                 = sText_PkmnFreedFrom,
    [STRINGID_PKMNCRASHED - BATTLESTRINGS_TABLE_START]                   = sText_PkmnCrashed,
    [STRINGID_PKMNSHROUDEDINMIST - BATTLESTRINGS_TABLE_START]            = gBattleText_MistShroud,
    [STRINGID_PKMNPROTECTEDBYMIST - BATTLESTRINGS_TABLE_START]           = sText_PkmnProtectedByMist,
    [STRINGID_PKMNGETTINGPUMPED - BATTLESTRINGS_TABLE_START]             = gBattleText_GetPumped,
    [STRINGID_PKMNHITWITHRECOIL - BATTLESTRINGS_TABLE_START]             = sText_PkmnHitWithRecoil,
    [STRINGID_PKMNPROTECTEDITSELF2 - BATTLESTRINGS_TABLE_START]          = sText_PkmnProtectedItself2,
    [STRINGID_PKMNBUFFETEDBYSANDSTORM - BATTLESTRINGS_TABLE_START]       = sText_PkmnBuffetedBySandstorm,
    [STRINGID_PKMNPELTEDBYHAIL - BATTLESTRINGS_TABLE_START]              = sText_PkmnPeltedByHail,
    [STRINGID_PKMNSEEDED - BATTLESTRINGS_TABLE_START]                    = sText_PkmnSeeded,
    [STRINGID_PKMNEVADEDATTACK - BATTLESTRINGS_TABLE_START]              = sText_PkmnEvadedAttack,
    [STRINGID_PKMNSAPPEDBYLEECHSEED - BATTLESTRINGS_TABLE_START]         = sText_PkmnSappedByLeechSeed,
    [STRINGID_PKMNFASTASLEEP - BATTLESTRINGS_TABLE_START]                = sText_PkmnFastAsleep,
    [STRINGID_PKMNWOKEUP - BATTLESTRINGS_TABLE_START]                    = sText_PkmnWokeUp,
    [STRINGID_PKMNUPROARKEPTAWAKE - BATTLESTRINGS_TABLE_START]           = sText_PkmnUproarKeptAwake,
    [STRINGID_PKMNWOKEUPINUPROAR - BATTLESTRINGS_TABLE_START]            = sText_PkmnWokeUpInUproar,
    [STRINGID_PKMNCAUSEDUPROAR - BATTLESTRINGS_TABLE_START]              = sText_PkmnCausedUproar,
    [STRINGID_PKMNMAKINGUPROAR - BATTLESTRINGS_TABLE_START]              = sText_PkmnMakingUproar,
    [STRINGID_PKMNCALMEDDOWN - BATTLESTRINGS_TABLE_START]                = sText_PkmnCalmedDown,
    [STRINGID_PKMNCANTSLEEPINUPROAR - BATTLESTRINGS_TABLE_START]         = sText_PkmnCantSleepInUproar,
    [STRINGID_PKMNSTOCKPILED - BATTLESTRINGS_TABLE_START]                = sText_PkmnStockpiled,
    [STRINGID_PKMNCANTSTOCKPILE - BATTLESTRINGS_TABLE_START]             = sText_PkmnCantStockpile,
    [STRINGID_PKMNCANTSLEEPINUPROAR2 - BATTLESTRINGS_TABLE_START]        = sText_PkmnCantSleepInUproar2,
    [STRINGID_UPROARKEPTPKMNAWAKE - BATTLESTRINGS_TABLE_START]           = sText_UproarKeptPkmnAwake,
    [STRINGID_PKMNSTAYEDAWAKEUSING - BATTLESTRINGS_TABLE_START]          = sText_PkmnStayedAwakeUsing,
    [STRINGID_PKMNSTORINGENERGY - BATTLESTRINGS_TABLE_START]             = sText_PkmnStoringEnergy,
    [STRINGID_PKMNUNLEASHEDENERGY - BATTLESTRINGS_TABLE_START]           = sText_PkmnUnleashedEnergy,
    [STRINGID_PKMNFATIGUECONFUSION - BATTLESTRINGS_TABLE_START]          = sText_PkmnFatigueConfusion,
    [STRINGID_PLAYERPICKEDUPMONEY - BATTLESTRINGS_TABLE_START]           = sText_PkmnPickedUpItem,
    [STRINGID_PKMNUNAFFECTED - BATTLESTRINGS_TABLE_START]                = sText_PkmnUnaffected,
    [STRINGID_PKMNTRANSFORMEDINTO - BATTLESTRINGS_TABLE_START]           = sText_PkmnTransformedInto,
    [STRINGID_PKMNMADESUBSTITUTE - BATTLESTRINGS_TABLE_START]            = sText_PkmnMadeSubstitute,
    [STRINGID_PKMNHASSUBSTITUTE - BATTLESTRINGS_TABLE_START]             = sText_PkmnHasSubstitute,
    [STRINGID_SUBSTITUTEDAMAGED - BATTLESTRINGS_TABLE_START]             = sText_SubstituteDamaged,
    [STRINGID_PKMNSUBSTITUTEFADED - BATTLESTRINGS_TABLE_START]           = sText_PkmnSubstituteFaded,
    [STRINGID_PKMNMUSTRECHARGE - BATTLESTRINGS_TABLE_START]              = sText_PkmnMustRecharge,
    [STRINGID_PKMNRAGEBUILDING - BATTLESTRINGS_TABLE_START]              = sText_PkmnRageBuilding,
    [STRINGID_PKMNMOVEWASDISABLED - BATTLESTRINGS_TABLE_START]           = sText_PkmnMoveWasDisabled,
    [STRINGID_PKMNMOVEISDISABLED - BATTLESTRINGS_TABLE_START]            = sText_PkmnMoveIsDisabled,
    [STRINGID_PKMNMOVEDISABLEDNOMORE - BATTLESTRINGS_TABLE_START]        = sText_PkmnMoveDisabledNoMore,
    [STRINGID_PKMNGOTENCORE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnGotEncore,
    [STRINGID_PKMNENCOREENDED - BATTLESTRINGS_TABLE_START]               = sText_PkmnEncoreEnded,
    [STRINGID_PKMNTOOKAIM - BATTLESTRINGS_TABLE_START]                   = sText_PkmnTookAim,
    [STRINGID_PKMNSKETCHEDMOVE - BATTLESTRINGS_TABLE_START]              = sText_PkmnSketchedMove,
    [STRINGID_PKMNTRYINGTOTAKEFOE - BATTLESTRINGS_TABLE_START]           = sText_PkmnTryingToTakeFoe,
    [STRINGID_PKMNTOOKFOE - BATTLESTRINGS_TABLE_START]                   = sText_PkmnTookFoe,
    [STRINGID_PKMNREDUCEDPP - BATTLESTRINGS_TABLE_START]                 = sText_PkmnReducedPP,
    [STRINGID_PKMNSTOLEITEM - BATTLESTRINGS_TABLE_START]                 = sText_PkmnStoleItem,
    [STRINGID_TARGETCANTESCAPENOW - BATTLESTRINGS_TABLE_START]           = sText_TargetCantEscapeNow,
    [STRINGID_PKMNFELLINTONIGHTMARE - BATTLESTRINGS_TABLE_START]         = sText_PkmnFellIntoNightmare,
    [STRINGID_PKMNLOCKEDINNIGHTMARE - BATTLESTRINGS_TABLE_START]         = sText_PkmnLockedInNightmare,
    [STRINGID_PKMNLAIDCURSE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnLaidCurse,
    [STRINGID_PKMNAFFLICTEDBYCURSE - BATTLESTRINGS_TABLE_START]          = sText_PkmnAfflictedByCurse,
    [STRINGID_SPIKESSCATTERED - BATTLESTRINGS_TABLE_START]               = sText_SpikesScattered,
    [STRINGID_PKMNHURTBYSPIKES - BATTLESTRINGS_TABLE_START]              = sText_PkmnHurtBySpikes,
    [STRINGID_PKMNIDENTIFIED - BATTLESTRINGS_TABLE_START]                = sText_PkmnIdentified,
    [STRINGID_PKMNPERISHCOUNTFELL - BATTLESTRINGS_TABLE_START]           = sText_PkmnPerishCountFell,
    [STRINGID_PKMNBRACEDITSELF - BATTLESTRINGS_TABLE_START]              = sText_PkmnBracedItself,
    [STRINGID_PKMNENDUREDHIT - BATTLESTRINGS_TABLE_START]                = sText_PkmnEnduredHit,
    [STRINGID_MAGNITUDESTRENGTH - BATTLESTRINGS_TABLE_START]             = sText_MagnitudeStrength,
    [STRINGID_PKMNCUTHPMAXEDATTACK - BATTLESTRINGS_TABLE_START]          = sText_PkmnCutHPMaxedAttack,
    [STRINGID_PKMNCOPIEDSTATCHANGES - BATTLESTRINGS_TABLE_START]         = sText_PkmnCopiedStatChanges,
    [STRINGID_PKMNGOTFREE - BATTLESTRINGS_TABLE_START]                   = sText_PkmnGotFree,
    [STRINGID_PKMNSHEDLEECHSEED - BATTLESTRINGS_TABLE_START]             = sText_PkmnShedLeechSeed,
    [STRINGID_PKMNBLEWAWAYSPIKES - BATTLESTRINGS_TABLE_START]            = sText_PkmnBlewAwaySpikes,
    [STRINGID_PKMNFLEDFROMBATTLE - BATTLESTRINGS_TABLE_START]            = sText_PkmnFledFromBattle,
    [STRINGID_PKMNFORESAWATTACK - BATTLESTRINGS_TABLE_START]             = sText_PkmnForesawAttack,
    [STRINGID_PKMNTOOKATTACK - BATTLESTRINGS_TABLE_START]                = sText_PkmnTookAttack,
    [STRINGID_PKMNATTACK - BATTLESTRINGS_TABLE_START]                    = sText_PkmnAttack,
    [STRINGID_PKMNCENTERATTENTION - BATTLESTRINGS_TABLE_START]           = sText_PkmnCenterAttention,
    [STRINGID_PKMNCHARGINGPOWER - BATTLESTRINGS_TABLE_START]             = sText_PkmnChargingPower,
    [STRINGID_NATUREPOWERTURNEDINTO - BATTLESTRINGS_TABLE_START]         = sText_NaturePowerTurnedInto,
    [STRINGID_PKMNSTATUSNORMAL - BATTLESTRINGS_TABLE_START]              = sText_PkmnStatusNormal,
    [STRINGID_PKMNHASNOMOVESLEFT - BATTLESTRINGS_TABLE_START]            = sText_PkmnHasNoMovesLeft,
    [STRINGID_PKMNSUBJECTEDTOTORMENT - BATTLESTRINGS_TABLE_START]        = sText_PkmnSubjectedToTorment,
    [STRINGID_PKMNCANTUSEMOVETORMENT - BATTLESTRINGS_TABLE_START]        = sText_PkmnCantUseMoveTorment,
    [STRINGID_PKMNTIGHTENINGFOCUS - BATTLESTRINGS_TABLE_START]           = sText_PkmnTighteningFocus,
    [STRINGID_PKMNFELLFORTAUNT - BATTLESTRINGS_TABLE_START]              = sText_PkmnFellForTaunt,
    [STRINGID_PKMNCANTUSEMOVETAUNT - BATTLESTRINGS_TABLE_START]          = sText_PkmnCantUseMoveTaunt,
    [STRINGID_PKMNREADYTOHELP - BATTLESTRINGS_TABLE_START]               = sText_PkmnReadyToHelp,
    [STRINGID_PKMNSWITCHEDITEMS - BATTLESTRINGS_TABLE_START]             = sText_PkmnSwitchedItems,
    [STRINGID_PKMNCOPIEDFOE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnCopiedFoe,
    [STRINGID_PKMNMADEWISH - BATTLESTRINGS_TABLE_START]                  = sText_PkmnMadeWish,
    [STRINGID_PKMNWISHCAMETRUE - BATTLESTRINGS_TABLE_START]              = sText_PkmnWishCameTrue,
    [STRINGID_PKMNPLANTEDROOTS - BATTLESTRINGS_TABLE_START]              = sText_PkmnPlantedRoots,
    [STRINGID_PKMNABSORBEDNUTRIENTS - BATTLESTRINGS_TABLE_START]         = sText_PkmnAbsorbedNutrients,
    [STRINGID_PKMNANCHOREDITSELF - BATTLESTRINGS_TABLE_START]            = sText_PkmnAnchoredItself,
    [STRINGID_PKMNWASMADEDROWSY - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasMadeDrowsy,
    [STRINGID_PKMNKNOCKEDOFF - BATTLESTRINGS_TABLE_START]                = sText_PkmnKnockedOff,
    [STRINGID_PKMNSWAPPEDABILITIES - BATTLESTRINGS_TABLE_START]          = sText_PkmnSwappedAbilities,
    [STRINGID_PKMNSEALEDOPPONENTMOVE - BATTLESTRINGS_TABLE_START]        = sText_PkmnSealedOpponentMove,
    [STRINGID_PKMNCANTUSEMOVESEALED - BATTLESTRINGS_TABLE_START]         = sText_PkmnCantUseMoveSealed,
    [STRINGID_PKMNWANTSGRUDGE - BATTLESTRINGS_TABLE_START]               = sText_PkmnWantsGrudge,
    [STRINGID_PKMNLOSTPPGRUDGE - BATTLESTRINGS_TABLE_START]              = sText_PkmnLostPPGrudge,
    [STRINGID_PKMNSHROUDEDITSELF - BATTLESTRINGS_TABLE_START]            = sText_PkmnShroudedItself,
    [STRINGID_PKMNMOVEBOUNCED - BATTLESTRINGS_TABLE_START]               = sText_PkmnMoveBounced,
    [STRINGID_PKMNWAITSFORTARGET - BATTLESTRINGS_TABLE_START]            = sText_PkmnWaitsForTarget,
    [STRINGID_PKMNSNATCHEDMOVE - BATTLESTRINGS_TABLE_START]              = sText_PkmnSnatchedMove,
    [STRINGID_PKMNMADEITRAIN - BATTLESTRINGS_TABLE_START]                = sText_PkmnMadeItRain,
    [STRINGID_PKMNRAISEDSPEED - BATTLESTRINGS_TABLE_START]               = sText_PkmnRaisedSpeed,
    [STRINGID_PKMNPROTECTEDBY - BATTLESTRINGS_TABLE_START]               = sText_PkmnProtectedBy,
    [STRINGID_PKMNPREVENTSUSAGE - BATTLESTRINGS_TABLE_START]             = sText_PkmnPreventsUsage,
    [STRINGID_PKMNRESTOREDHPUSING - BATTLESTRINGS_TABLE_START]           = sText_PkmnRestoredHPUsing,
    [STRINGID_PKMNCHANGEDTYPEWITH - BATTLESTRINGS_TABLE_START]           = sText_PkmnChangedTypeWith,
    [STRINGID_PKMNPREVENTSPARALYSISWITH - BATTLESTRINGS_TABLE_START]     = sText_PkmnPreventsParalysisWith,
    [STRINGID_PKMNPREVENTSROMANCEWITH - BATTLESTRINGS_TABLE_START]       = sText_PkmnPreventsRomanceWith,
    [STRINGID_PKMNPREVENTSPOISONINGWITH - BATTLESTRINGS_TABLE_START]     = sText_PkmnPreventsPoisoningWith,
    [STRINGID_PKMNPREVENTSCONFUSIONWITH - BATTLESTRINGS_TABLE_START]     = sText_PkmnPreventsConfusionWith,
    [STRINGID_PKMNRAISEDFIREPOWERWITH - BATTLESTRINGS_TABLE_START]       = sText_PkmnRaisedFirePowerWith,
    [STRINGID_PKMNANCHORSITSELFWITH - BATTLESTRINGS_TABLE_START]         = sText_PkmnAnchorsItselfWith,
    [STRINGID_PKMNCUTSATTACKWITH - BATTLESTRINGS_TABLE_START]            = sText_PkmnCutsAttackWith,
    [STRINGID_PKMNPREVENTSSTATLOSSWITH - BATTLESTRINGS_TABLE_START]      = sText_PkmnPreventsStatLossWith,
    [STRINGID_PKMNHURTSWITH - BATTLESTRINGS_TABLE_START]                 = sText_PkmnHurtsWith,
    [STRINGID_PKMNTRACED - BATTLESTRINGS_TABLE_START]                    = sText_PkmnTraced,
    [STRINGID_STATSHARPLY - BATTLESTRINGS_TABLE_START]                   = sText_StatSharply,
    [STRINGID_STATROSE - BATTLESTRINGS_TABLE_START]                      = gBattleText_Rose,
    [STRINGID_STATHARSHLY - BATTLESTRINGS_TABLE_START]                   = sText_StatHarshly,
    [STRINGID_STATFELL - BATTLESTRINGS_TABLE_START]                      = sText_StatFell,
    [STRINGID_ATTACKERSSTATROSE - BATTLESTRINGS_TABLE_START]             = sText_AttackersStatRose,
    [STRINGID_DEFENDERSSTATROSE - BATTLESTRINGS_TABLE_START]             = gText_DefendersStatRose,
    [STRINGID_ATTACKERSSTATFELL - BATTLESTRINGS_TABLE_START]             = sText_AttackersStatFell,
    [STRINGID_DEFENDERSSTATFELL - BATTLESTRINGS_TABLE_START]             = sText_DefendersStatFell,
    [STRINGID_CRITICALHIT - BATTLESTRINGS_TABLE_START]                   = sText_CriticalHit,
    [STRINGID_ONEHITKO - BATTLESTRINGS_TABLE_START]                      = sText_OneHitKO,
    [STRINGID_123POOF - BATTLESTRINGS_TABLE_START]                       = sText_123Poof,
    [STRINGID_ANDELLIPSIS - BATTLESTRINGS_TABLE_START]                   = sText_AndEllipsis,
    [STRINGID_NOTVERYEFFECTIVE - BATTLESTRINGS_TABLE_START]              = sText_NotVeryEffective,
    [STRINGID_SUPEREFFECTIVE - BATTLESTRINGS_TABLE_START]                = sText_SuperEffective,
    [STRINGID_GOTAWAYSAFELY - BATTLESTRINGS_TABLE_START]                 = sText_GotAwaySafely,
    [STRINGID_WILDPKMNFLED - BATTLESTRINGS_TABLE_START]                  = sText_WildPkmnFled,
    [STRINGID_NORUNNINGFROMTRAINERS - BATTLESTRINGS_TABLE_START]         = sText_NoRunningFromTrainers,
    [STRINGID_CANTESCAPE - BATTLESTRINGS_TABLE_START]                    = sText_CantEscape,
    [STRINGID_DONTLEAVEBIRCH - BATTLESTRINGS_TABLE_START]                = sText_DontLeaveBirch,
    [STRINGID_BUTNOTHINGHAPPENED - BATTLESTRINGS_TABLE_START]            = sText_ButNothingHappened,
    [STRINGID_BUTITFAILED - BATTLESTRINGS_TABLE_START]                   = sText_ButItFailed,
    [STRINGID_ITHURTCONFUSION - BATTLESTRINGS_TABLE_START]               = sText_ItHurtConfusion,
    [STRINGID_MIRRORMOVEFAILED - BATTLESTRINGS_TABLE_START]              = sText_MirrorMoveFailed,
    [STRINGID_STARTEDTORAIN - BATTLESTRINGS_TABLE_START]                 = sText_StartedToRain,
    [STRINGID_DOWNPOURSTARTED - BATTLESTRINGS_TABLE_START]               = sText_DownpourStarted,
    [STRINGID_RAINCONTINUES - BATTLESTRINGS_TABLE_START]                 = sText_RainContinues,
    [STRINGID_DOWNPOURCONTINUES - BATTLESTRINGS_TABLE_START]             = sText_DownpourContinues,
    [STRINGID_RAINSTOPPED - BATTLESTRINGS_TABLE_START]                   = sText_RainStopped,
    [STRINGID_SANDSTORMBREWED - BATTLESTRINGS_TABLE_START]               = sText_SandstormBrewed,
    [STRINGID_SANDSTORMRAGES - BATTLESTRINGS_TABLE_START]                = sText_SandstormRages,
    [STRINGID_SANDSTORMSUBSIDED - BATTLESTRINGS_TABLE_START]             = sText_SandstormSubsided,
    [STRINGID_SUNLIGHTGOTBRIGHT - BATTLESTRINGS_TABLE_START]             = sText_SunlightGotBright,
    [STRINGID_SUNLIGHTSTRONG - BATTLESTRINGS_TABLE_START]                = sText_SunlightStrong,
    [STRINGID_SUNLIGHTFADED - BATTLESTRINGS_TABLE_START]                 = sText_SunlightFaded,
    [STRINGID_STARTEDHAIL - BATTLESTRINGS_TABLE_START]                   = sText_StartedHail,
    [STRINGID_HAILCONTINUES - BATTLESTRINGS_TABLE_START]                 = sText_HailContinues,
    [STRINGID_HAILSTOPPED - BATTLESTRINGS_TABLE_START]                   = sText_HailStopped,
    [STRINGID_FAILEDTOSPITUP - BATTLESTRINGS_TABLE_START]                = sText_FailedToSpitUp,
    [STRINGID_FAILEDTOSWALLOW - BATTLESTRINGS_TABLE_START]               = sText_FailedToSwallow,
    [STRINGID_WINDBECAMEHEATWAVE - BATTLESTRINGS_TABLE_START]            = sText_WindBecameHeatWave,
    [STRINGID_STATCHANGESGONE - BATTLESTRINGS_TABLE_START]               = sText_StatChangesGone,
    [STRINGID_COINSSCATTERED - BATTLESTRINGS_TABLE_START]                = sText_CoinsScattered,
    [STRINGID_TOOWEAKFORSUBSTITUTE - BATTLESTRINGS_TABLE_START]          = sText_TooWeakForSubstitute,
    [STRINGID_SHAREDPAIN - BATTLESTRINGS_TABLE_START]                    = sText_SharedPain,
    [STRINGID_BELLCHIMED - BATTLESTRINGS_TABLE_START]                    = sText_BellChimed,
    [STRINGID_FAINTINTHREE - BATTLESTRINGS_TABLE_START]                  = sText_FaintInThree,
    [STRINGID_NOPPLEFT - BATTLESTRINGS_TABLE_START]                      = sText_NoPPLeft,
    [STRINGID_BUTNOPPLEFT - BATTLESTRINGS_TABLE_START]                   = sText_ButNoPPLeft,
    [STRINGID_PLAYERUSEDITEM - BATTLESTRINGS_TABLE_START]                = sText_PlayerUsedItem,
    [STRINGID_OLDMANUSEDITEM - BATTLESTRINGS_TABLE_START]                = sText_OldManUsedItem,
    [STRINGID_TRAINERBLOCKEDBALL - BATTLESTRINGS_TABLE_START]            = sText_TrainerBlockedBall,
    [STRINGID_DONTBEATHIEF - BATTLESTRINGS_TABLE_START]                  = sText_DontBeAThief,
    [STRINGID_ITDODGEDBALL - BATTLESTRINGS_TABLE_START]                  = sText_ItDodgedBall,
    [STRINGID_YOUMISSEDPKMN - BATTLESTRINGS_TABLE_START]                 = sText_YouMissedPkmn,
    [STRINGID_PKMNBROKEFREE - BATTLESTRINGS_TABLE_START]                 = sText_PkmnBrokeFree,
    [STRINGID_ITAPPEAREDCAUGHT - BATTLESTRINGS_TABLE_START]              = sText_ItAppearedCaught,
    [STRINGID_AARGHALMOSTHADIT - BATTLESTRINGS_TABLE_START]              = sText_AarghAlmostHadIt,
    [STRINGID_SHOOTSOCLOSE - BATTLESTRINGS_TABLE_START]                  = sText_ShootSoClose,
    [STRINGID_GOTCHAPKMNCAUGHT - BATTLESTRINGS_TABLE_START]              = sText_GotchaPkmnCaught,
    [STRINGID_GOTCHAPKMNCAUGHT2 - BATTLESTRINGS_TABLE_START]             = sText_GotchaPkmnCaught2,
    [STRINGID_GIVENICKNAMECAPTURED - BATTLESTRINGS_TABLE_START]          = sText_GiveNicknameCaptured,
    [STRINGID_PKMNSENTTOPC - BATTLESTRINGS_TABLE_START]                  = sText_PkmnSentToPC,
    [STRINGID_PKMNDATAADDEDTODEX - BATTLESTRINGS_TABLE_START]            = sText_PkmnDataAddedToDex,
    [STRINGID_ITISRAINING - BATTLESTRINGS_TABLE_START]                   = sText_ItIsRaining,
    [STRINGID_SANDSTORMISRAGING - BATTLESTRINGS_TABLE_START]             = sText_SandstormIsRaging,
    [STRINGID_CANTESCAPE2 - BATTLESTRINGS_TABLE_START]                   = sText_CantEscape2,
    [STRINGID_PKMNIGNORESASLEEP - BATTLESTRINGS_TABLE_START]             = sText_PkmnIgnoresAsleep,
    [STRINGID_PKMNIGNOREDORDERS - BATTLESTRINGS_TABLE_START]             = sText_PkmnIgnoredOrders,
    [STRINGID_PKMNBEGANTONAP - BATTLESTRINGS_TABLE_START]                = sText_PkmnBeganToNap,
    [STRINGID_PKMNLOAFING - BATTLESTRINGS_TABLE_START]                   = sText_PkmnLoafing,
    [STRINGID_PKMNWONTOBEY - BATTLESTRINGS_TABLE_START]                  = sText_PkmnWontObey,
    [STRINGID_PKMNTURNEDAWAY - BATTLESTRINGS_TABLE_START]                = sText_PkmnTurnedAway,
    [STRINGID_PKMNPRETENDNOTNOTICE - BATTLESTRINGS_TABLE_START]          = sText_PkmnPretendNotNotice,
    [STRINGID_ENEMYABOUTTOSWITCHPKMN - BATTLESTRINGS_TABLE_START]        = sText_EnemyAboutToSwitchPkmn,
    [STRINGID_THREWROCK - BATTLESTRINGS_TABLE_START]                     = sText_ThrewARock,
    [STRINGID_THREWBAIT - BATTLESTRINGS_TABLE_START]                     = sText_ThrewSomeBait,
    [STRINGID_PKMNWATCHINGCAREFULLY - BATTLESTRINGS_TABLE_START]         = sText_PkmnWatchingCarefully,
    [STRINGID_PKMNANGRY - BATTLESTRINGS_TABLE_START]                     = sText_PkmnIsAngry,
    [STRINGID_PKMNEATING - BATTLESTRINGS_TABLE_START]                    = sText_PkmnIsEating,
    [STRINGID_DUMMY288 - BATTLESTRINGS_TABLE_START]                      = sText_Empty1,
    [STRINGID_DUMMY289 - BATTLESTRINGS_TABLE_START]                      = sText_Empty1,
    [STRINGID_OUTOFSAFARIBALLS - BATTLESTRINGS_TABLE_START]              = sText_OutOfSafariBalls,
    [STRINGID_PKMNSITEMCUREDPARALYSIS - BATTLESTRINGS_TABLE_START]       = sText_PkmnsItemCuredParalysis,
    [STRINGID_PKMNSITEMCUREDPOISON - BATTLESTRINGS_TABLE_START]          = sText_PkmnsItemCuredPoison,
    [STRINGID_PKMNSITEMHEALEDBURN - BATTLESTRINGS_TABLE_START]           = sText_PkmnsItemHealedBurn,
    [STRINGID_PKMNSITEMDEFROSTEDIT - BATTLESTRINGS_TABLE_START]          = sText_PkmnsItemDefrostedIt,
    [STRINGID_PKMNSITEMWOKEIT - BATTLESTRINGS_TABLE_START]               = sText_PkmnsItemWokeIt,
    [STRINGID_PKMNSITEMSNAPPEDOUT - BATTLESTRINGS_TABLE_START]           = sText_PkmnsItemSnappedOut,
    [STRINGID_PKMNSITEMCUREDPROBLEM - BATTLESTRINGS_TABLE_START]         = sText_PkmnsItemCuredProblem,
    [STRINGID_PKMNSITEMRESTOREDHEALTH - BATTLESTRINGS_TABLE_START]       = sText_PkmnsItemRestoredHealth,
    [STRINGID_PKMNSITEMRESTOREDPP - BATTLESTRINGS_TABLE_START]           = sText_PkmnsItemRestoredPP,
    [STRINGID_PKMNSITEMRESTOREDSTATUS - BATTLESTRINGS_TABLE_START]       = sText_PkmnsItemRestoredStatus,
    [STRINGID_PKMNSITEMRESTOREDHPALITTLE - BATTLESTRINGS_TABLE_START]    = sText_PkmnsItemRestoredHPALittle,
    [STRINGID_ITEMALLOWSONLYYMOVE - BATTLESTRINGS_TABLE_START]           = sText_ItemAllowsOnlyYMove,
    [STRINGID_PKMNHUNGONWITHX - BATTLESTRINGS_TABLE_START]               = sText_PkmnHungOnWithX,
    [STRINGID_EMPTYSTRING3 - BATTLESTRINGS_TABLE_START]                  = gText_EmptyString3,
    [STRINGID_PKMNSXPREVENTSBURNS - BATTLESTRINGS_TABLE_START]           = sText_PkmnsXPreventsBurns,
    [STRINGID_PKMNSXBLOCKSY - BATTLESTRINGS_TABLE_START]                 = sText_PkmnsXBlocksY,
    [STRINGID_PKMNSXRESTOREDHPALITTLE2 - BATTLESTRINGS_TABLE_START]      = sText_PkmnsXRestoredHPALittle2,
    [STRINGID_PKMNSXWHIPPEDUPSANDSTORM - BATTLESTRINGS_TABLE_START]      = sText_PkmnsXWhippedUpSandstorm,
    [STRINGID_PKMNSXPREVENTSYLOSS - BATTLESTRINGS_TABLE_START]           = sText_PkmnsXPreventsYLoss,
    [STRINGID_PKMNSXINFATUATEDY - BATTLESTRINGS_TABLE_START]             = sText_PkmnsXInfatuatedY,
    [STRINGID_PKMNSXMADEYINEFFECTIVE - BATTLESTRINGS_TABLE_START]        = sText_PkmnsXMadeYIneffective,
    [STRINGID_PKMNSXCUREDYPROBLEM - BATTLESTRINGS_TABLE_START]           = sText_PkmnsXCuredYProblem,
    [STRINGID_ITSUCKEDLIQUIDOOZE - BATTLESTRINGS_TABLE_START]            = sText_ItSuckedLiquidOoze,
    [STRINGID_PKMNTRANSFORMED - BATTLESTRINGS_TABLE_START]               = sText_PkmnTransformed,
    [STRINGID_ELECTRICITYWEAKENED - BATTLESTRINGS_TABLE_START]           = sText_ElectricityWeakened,
    [STRINGID_FIREWEAKENED - BATTLESTRINGS_TABLE_START]                  = sText_FireWeakened,
    [STRINGID_PKMNHIDUNDERWATER - BATTLESTRINGS_TABLE_START]             = sText_PkmnHidUnderwater,
    [STRINGID_PKMNSPRANGUP - BATTLESTRINGS_TABLE_START]                  = sText_PkmnSprangUp,
    [STRINGID_HMMOVESCANTBEFORGOTTEN - BATTLESTRINGS_TABLE_START]        = sText_HMMovesCantBeForgotten,
    [STRINGID_XFOUNDONEY - BATTLESTRINGS_TABLE_START]                    = sText_XFoundOneY,
    [STRINGID_PLAYERDEFEATEDTRAINER1 - BATTLESTRINGS_TABLE_START]        = sText_PlayerDefeatedLinkTrainerTrainer1,
    [STRINGID_SOOTHINGAROMA - BATTLESTRINGS_TABLE_START]                 = sText_SoothingAroma,
    [STRINGID_ITEMSCANTBEUSEDNOW - BATTLESTRINGS_TABLE_START]            = sText_ItemsCantBeUsedNow,
    [STRINGID_FORXCOMMAYZ - BATTLESTRINGS_TABLE_START]                   = sText_ForXCommaYZ,
    [STRINGID_USINGITEMSTATOFPKMNROSE - BATTLESTRINGS_TABLE_START]       = sText_UsingItemTheStatOfPkmnRose,
    [STRINGID_PKMNUSEDXTOGETPUMPED - BATTLESTRINGS_TABLE_START]          = sText_PkmnUsedXToGetPumped,
    [STRINGID_PKMNSXMADEYUSELESS - BATTLESTRINGS_TABLE_START]            = sText_PkmnsXMadeYUseless,
    [STRINGID_PKMNTRAPPEDBYSANDTOMB - BATTLESTRINGS_TABLE_START]         = sText_PkmnTrappedBySandTomb,
    [STRINGID_EMPTYSTRING4 - BATTLESTRINGS_TABLE_START]                  = sText_EmptyString4,
    [STRINGID_ABOOSTED - BATTLESTRINGS_TABLE_START]                      = sText_ABoosted,
    [STRINGID_PKMNSXINTENSIFIEDSUN - BATTLESTRINGS_TABLE_START]          = sText_PkmnsXIntensifiedSun,
    [STRINGID_PKMNMAKESGROUNDMISS - BATTLESTRINGS_TABLE_START]           = sText_PkmnMakesGroundMiss,
    [STRINGID_YOUTHROWABALLNOWRIGHT - BATTLESTRINGS_TABLE_START]         = sText_YouThrowABallNowRight,
    [STRINGID_PKMNSXTOOKATTACK - BATTLESTRINGS_TABLE_START]              = sText_PkmnsXTookAttack,
    [STRINGID_PKMNCHOSEXASDESTINY - BATTLESTRINGS_TABLE_START]           = sText_PkmnChoseXAsDestiny,
    [STRINGID_PKMNLOSTFOCUS - BATTLESTRINGS_TABLE_START]                 = sText_PkmnLostFocus,
    [STRINGID_USENEXTPKMN - BATTLESTRINGS_TABLE_START]                   = sText_UseNextPkmn,
    [STRINGID_PKMNFLEDUSINGITS - BATTLESTRINGS_TABLE_START]              = sText_PkmnFledUsingIts,
    [STRINGID_PKMNFLEDUSING - BATTLESTRINGS_TABLE_START]                 = sText_PkmnFledUsing,
    [STRINGID_PKMNWASDRAGGEDOUT - BATTLESTRINGS_TABLE_START]             = sText_PkmnWasDraggedOut,
    [STRINGID_PREVENTEDFROMWORKING - BATTLESTRINGS_TABLE_START]          = sText_PreventedFromWorking,
    [STRINGID_PKMNSITEMNORMALIZEDSTATUS - BATTLESTRINGS_TABLE_START]     = sText_PkmnsItemNormalizedStatus,
    [STRINGID_TRAINER1USEDITEM - BATTLESTRINGS_TABLE_START]              = sText_Trainer1UsedItem,
    [STRINGID_BOXISFULL - BATTLESTRINGS_TABLE_START]                     = sText_BoxIsFull,
    [STRINGID_PKMNAVOIDEDATTACK - BATTLESTRINGS_TABLE_START]             = sText_PkmnAvoidedAttack,
    [STRINGID_PKMNSXMADEITINEFFECTIVE - BATTLESTRINGS_TABLE_START]       = sText_PkmnsXMadeItIneffective,
    [STRINGID_PKMNSXPREVENTSFLINCHING - BATTLESTRINGS_TABLE_START]       = sText_PkmnsXPreventsFlinching,
    [STRINGID_PKMNALREADYHASBURN - BATTLESTRINGS_TABLE_START]            = sText_PkmnAlreadyHasBurn,
    [STRINGID_STATSWONTDECREASE2 - BATTLESTRINGS_TABLE_START]            = sText_StatsWontDecrease2,
    [STRINGID_PKMNSXBLOCKSY2 - BATTLESTRINGS_TABLE_START]                = sText_PkmnsXBlocksY2,
    [STRINGID_PKMNSXWOREOFF - BATTLESTRINGS_TABLE_START]                 = sText_PkmnsXWoreOff,
    [STRINGID_PKMNRAISEDDEFALITTLE - BATTLESTRINGS_TABLE_START]          = sText_PkmnRaisedDefALittle,
    [STRINGID_PKMNRAISEDSPDEFALITTLE - BATTLESTRINGS_TABLE_START]        = sText_PkmnRaisedSpDefALittle,
    [STRINGID_THEWALLSHATTERED - BATTLESTRINGS_TABLE_START]              = sText_TheWallShattered,
    [STRINGID_PKMNSXPREVENTSYSZ - BATTLESTRINGS_TABLE_START]             = sText_PkmnsXPreventsYsZ,
    [STRINGID_PKMNSXCUREDITSYPROBLEM - BATTLESTRINGS_TABLE_START]        = sText_PkmnsXCuredItsYProblem,
    [STRINGID_ATTACKERCANTESCAPE - BATTLESTRINGS_TABLE_START]            = sText_AttackerCantEscape,
    [STRINGID_PKMNOBTAINEDX - BATTLESTRINGS_TABLE_START]                 = sText_PkmnObtainedX,
    [STRINGID_PKMNOBTAINEDX2 - BATTLESTRINGS_TABLE_START]                = sText_PkmnObtainedX2,
    [STRINGID_PKMNOBTAINEDXYOBTAINEDZ - BATTLESTRINGS_TABLE_START]       = sText_PkmnObtainedXYObtainedZ,
    [STRINGID_BUTNOEFFECT - BATTLESTRINGS_TABLE_START]                   = sText_ButNoEffect,
    [STRINGID_PKMNSXHADNOEFFECTONY - BATTLESTRINGS_TABLE_START]          = sText_PkmnsXHadNoEffectOnY,
    [STRINGID_OAKPLAYERWON - BATTLESTRINGS_TABLE_START]                  = gText_WinEarnsPrizeMoney,
    [STRINGID_OAKPLAYERLOST - BATTLESTRINGS_TABLE_START]                 = gText_HowDissapointing,
    [STRINGID_PLAYERLOSTAGAINSTENEMYTRAINER - BATTLESTRINGS_TABLE_START] = sText_PlayerWhiteoutAgainstTrainer,
    [STRINGID_PLAYERPAIDPRIZEMONEY - BATTLESTRINGS_TABLE_START]          = sText_PlayerPaidAsPrizeMoney,
    [STRINGID_PKMNTRANSFERREDSOMEONESPC - BATTLESTRINGS_TABLE_START]     = Text_MonSentToBoxInSomeonesPC,
    [STRINGID_PKMNTRANSFERREDBILLSPC - BATTLESTRINGS_TABLE_START]        = Text_MonSentToBoxInBillsPC,
    [STRINGID_PKMNBOXSOMEONESPCFULL - BATTLESTRINGS_TABLE_START]         = Text_MonSentToBoxSomeonesBoxFull,
    [STRINGID_PKMNBOXBILLSPCFULL - BATTLESTRINGS_TABLE_START]            = Text_MonSentToBoxBillsBoxFull,
    [STRINGID_POKEDUDEUSED - BATTLESTRINGS_TABLE_START]                  = sText_PokedudeUsedItem,
    [STRINGID_POKEFLUTECATCHY - BATTLESTRINGS_TABLE_START]               = sText_PlayedFluteCatchyTune,
    [STRINGID_POKEFLUTE - BATTLESTRINGS_TABLE_START]                     = sText_PlayedThe,
    [STRINGID_MONHEARINGFLUTEAWOKE - BATTLESTRINGS_TABLE_START]          = sText_PkmnHearingFluteAwoke,
    [STRINGID_TRAINER2LOSETEXT - BATTLESTRINGS_TABLE_START]              = sText_Trainer2LoseText,
    [STRINGID_TRAINER2WINTEXT - BATTLESTRINGS_TABLE_START]               = sText_Trainer2WinText,
    [STRINGID_PLAYERWHITEDOUT - BATTLESTRINGS_TABLE_START]               = sText_PlayerWhiteout2,
    [STRINGID_MONTOOSCAREDTOMOVE - BATTLESTRINGS_TABLE_START]            = sText_TooScaredToMove,
    [STRINGID_GHOSTGETOUTGETOUT - BATTLESTRINGS_TABLE_START]             = sText_GetOutGetOut,
    [STRINGID_SILPHSCOPEUNVEILED - BATTLESTRINGS_TABLE_START]            = sText_SilphScopeUnveil,
    [STRINGID_GHOSTWASMAROWAK - BATTLESTRINGS_TABLE_START]               = sText_TheGhostWas,
    [STRINGID_TRAINER1MON1COMEBACK - BATTLESTRINGS_TABLE_START]          = sText_Trainer1RecallPkmn1,
    [STRINGID_TRAINER1WINTEXT - BATTLESTRINGS_TABLE_START]               = sText_Trainer1WinText,
    [STRINGID_TRAINER1MON2COMEBACK - BATTLESTRINGS_TABLE_START]          = sText_Trainer1RecallPkmn2,
    [STRINGID_TRAINER1MON1AND2COMEBACK - BATTLESTRINGS_TABLE_START]      = sText_Trainer1RecallBoth
};

const u16 gMissStringIds[] =
{
    [B_MSG_MISSED]      = STRINGID_ATTACKMISSED,
    [B_MSG_PROTECTED]   = STRINGID_PKMNPROTECTEDITSELF,
    [B_MSG_AVOIDED_ATK] = STRINGID_PKMNAVOIDEDATTACK,
    [B_MSG_AVOIDED_DMG] = STRINGID_AVOIDEDDAMAGE,
    [B_MSG_GROUND_MISS] = STRINGID_PKMNMAKESGROUNDMISS
};

const u16 gNoEscapeStringIds[] =
{
    [B_MSG_CANT_ESCAPE]          = STRINGID_CANTESCAPE,
    [B_MSG_DONT_LEAVE_BIRCH]     = STRINGID_DONTLEAVEBIRCH,
    [B_MSG_PREVENTS_ESCAPE]      = STRINGID_PREVENTSESCAPE,
    [B_MSG_CANT_ESCAPE_2]        = STRINGID_CANTESCAPE2,
    [B_MSG_ATTACKER_CANT_ESCAPE] = STRINGID_ATTACKERCANTESCAPE
};

const u16 gMoveWeatherChangeStringIds[] =
{
    [B_MSG_STARTED_RAIN]      = STRINGID_STARTEDTORAIN,
    [B_MSG_STARTED_DOWNPOUR]  = STRINGID_DOWNPOURSTARTED,
    [B_MSG_WEATHER_FAILED]    = STRINGID_BUTITFAILED,
    [B_MSG_STARTED_SANDSTORM] = STRINGID_SANDSTORMBREWED,
    [B_MSG_STARTED_SUNLIGHT]  = STRINGID_SUNLIGHTGOTBRIGHT,
    [B_MSG_STARTED_HAIL]      = STRINGID_STARTEDHAIL
};

const u16 gSandstormHailContinuesStringIds[] =
{
    [B_MSG_SANDSTORM] = STRINGID_SANDSTORMRAGES,
    [B_MSG_HAIL]      = STRINGID_HAILCONTINUES
};

const u16 gSandstormHailDmgStringIds[] =
{
    [B_MSG_SANDSTORM] = STRINGID_PKMNBUFFETEDBYSANDSTORM,
    [B_MSG_HAIL]      = STRINGID_PKMNPELTEDBYHAIL
};

const u16 gSandstormHailEndStringIds[] =
{
    [B_MSG_SANDSTORM] = STRINGID_SANDSTORMSUBSIDED,
    [B_MSG_HAIL]      = STRINGID_HAILSTOPPED
};

const u16 gRainContinuesStringIds[] =
{
    [B_MSG_RAIN_CONTINUES]     = STRINGID_RAINCONTINUES,
    [B_MSG_DOWNPOUR_CONTINUES] = STRINGID_DOWNPOURCONTINUES,
    [B_MSG_RAIN_STOPPED]       = STRINGID_RAINSTOPPED
};

const u16 gProtectLikeUsedStringIds[] =
{
    [B_MSG_PROTECTED_ITSELF] = STRINGID_PKMNPROTECTEDITSELF2,
    [B_MSG_BRACED_ITSELF]    = STRINGID_PKMNBRACEDITSELF,
    [B_MSG_PROTECT_FAILED]   = STRINGID_BUTITFAILED
};

const u16 gReflectLightScreenSafeguardStringIds[] =
{
    [B_MSG_SIDE_STATUS_FAILED]     = STRINGID_BUTITFAILED,
    [B_MSG_SET_REFLECT_SINGLE]     = STRINGID_PKMNRAISEDDEF,
    [B_MSG_SET_REFLECT_DOUBLE]     = STRINGID_PKMNRAISEDDEFALITTLE,
    [B_MSG_SET_LIGHTSCREEN_SINGLE] = STRINGID_PKMNRAISEDSPDEF,
    [B_MSG_SET_LIGHTSCREEN_DOUBLE] = STRINGID_PKMNRAISEDSPDEFALITTLE,
    [B_MSG_SET_SAFEGUARD]          = STRINGID_PKMNCOVEREDBYVEIL
};

const u16 gLeechSeedStringIds[] =
{
    [B_MSG_LEECH_SEED_SET]   = STRINGID_PKMNSEEDED,
    [B_MSG_LEECH_SEED_MISS]  = STRINGID_PKMNEVADEDATTACK,
    [B_MSG_LEECH_SEED_FAIL]  = STRINGID_ITDOESNTAFFECT,
    [B_MSG_LEECH_SEED_DRAIN] = STRINGID_PKMNSAPPEDBYLEECHSEED,
    [B_MSG_LEECH_SEED_OOZE]  = STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gRestUsedStringIds[] =
{
    [B_MSG_REST]          = STRINGID_PKMNWENTTOSLEEP,
    [B_MSG_REST_STATUSED] = STRINGID_PKMNSLEPTHEALTHY
};

const u16 gUproarOverTurnStringIds[] =
{
    [B_MSG_UPROAR_CONTINUES] = STRINGID_PKMNMAKINGUPROAR,
    [B_MSG_UPROAR_ENDS]      = STRINGID_PKMNCALMEDDOWN
};

const u16 gStockpileUsedStringIds[] =
{
    [B_MSG_STOCKPILED]     = STRINGID_PKMNSTOCKPILED,
    [B_MSG_CANT_STOCKPILE] = STRINGID_PKMNCANTSTOCKPILE
};

const u16 gWokeUpStringIds[] =
{
    [B_MSG_WOKE_UP]        = STRINGID_PKMNWOKEUP,
    [B_MSG_WOKE_UP_UPROAR] = STRINGID_PKMNWOKEUPINUPROAR
};

const u16 gSwallowFailStringIds[] =
{
    [B_MSG_SWALLOW_FAILED]  = STRINGID_FAILEDTOSWALLOW,
    [B_MSG_SWALLOW_FULL_HP] = STRINGID_PKMNHPFULL
};

const u16 gUproarAwakeStringIds[] =
{
    [B_MSG_CANT_SLEEP_UPROAR]  = STRINGID_PKMNCANTSLEEPINUPROAR2,
    [B_MSG_UPROAR_KEPT_AWAKE]  = STRINGID_UPROARKEPTPKMNAWAKE,
    [B_MSG_STAYED_AWAKE_USING] = STRINGID_PKMNSTAYEDAWAKEUSING
};

const u16 gStatUpStringIds[] =
{
    [B_MSG_ATTACKER_STAT_ROSE] = STRINGID_ATTACKERSSTATROSE,
    [B_MSG_DEFENDER_STAT_ROSE] = STRINGID_DEFENDERSSTATROSE,
    [B_MSG_STAT_WONT_INCREASE] = STRINGID_STATSWONTINCREASE,
    [B_MSG_STAT_ROSE_EMPTY]    = STRINGID_EMPTYSTRING3,
    [B_MSG_STAT_ROSE_ITEM]     = STRINGID_USINGITEMSTATOFPKMNROSE,
    [B_MSG_USED_DIRE_HIT]      = STRINGID_PKMNUSEDXTOGETPUMPED,
};

const u16 gStatDownStringIds[] =
{
    [B_MSG_ATTACKER_STAT_FELL] = STRINGID_ATTACKERSSTATFELL,
    [B_MSG_DEFENDER_STAT_FELL] = STRINGID_DEFENDERSSTATFELL,
    [B_MSG_STAT_WONT_DECREASE] = STRINGID_STATSWONTDECREASE,
    [B_MSG_STAT_FELL_EMPTY]    = STRINGID_EMPTYSTRING3
};

// Index read from sTWOTURN_STRINGID
const u16 gFirstTurnOfTwoStringIds[] =
{
    [B_MSG_TURN1_RAZOR_WIND] = STRINGID_PKMNWHIPPEDWHIRLWIND,
    [B_MSG_TURN1_SOLAR_BEAM] = STRINGID_PKMNTOOKSUNLIGHT,
    [B_MSG_TURN1_SKULL_BASH] = STRINGID_PKMNLOWEREDHEAD,
    [B_MSG_TURN1_SKY_ATTACK] = STRINGID_PKMNISGLOWING,
    [B_MSG_TURN1_FLY]        = STRINGID_PKMNFLEWHIGH,
    [B_MSG_TURN1_DIG]        = STRINGID_PKMNDUGHOLE,
    [B_MSG_TURN1_DIVE]       = STRINGID_PKMNHIDUNDERWATER,
    [B_MSG_TURN1_BOUNCE]     = STRINGID_PKMNSPRANGUP
};

// Index copied from move's index in gTrappingMoves
const u16 gWrappedStringIds[] =
{
    STRINGID_PKMNSQUEEZEDBYBIND,   // MOVE_BIND
    STRINGID_PKMNWRAPPEDBY,        // MOVE_WRAP
    STRINGID_PKMNTRAPPEDINVORTEX,  // MOVE_FIRE_SPIN
    STRINGID_PKMNCLAMPED,          // MOVE_CLAMP
    STRINGID_PKMNTRAPPEDINVORTEX,  // MOVE_WHIRLPOOL
    STRINGID_PKMNTRAPPEDBYSANDTOMB // MOVE_SAND_TOMB
};

const u16 gMistUsedStringIds[] =
{
    [B_MSG_SET_MIST]    = STRINGID_PKMNSHROUDEDINMIST,
    [B_MSG_MIST_FAILED] = STRINGID_BUTITFAILED
};

const u16 gFocusEnergyUsedStringIds[] =
{
    [B_MSG_GETTING_PUMPED]      = STRINGID_PKMNGETTINGPUMPED,
    [B_MSG_FOCUS_ENERGY_FAILED] = STRINGID_BUTITFAILED
};

const u16 gTransformUsedStringIds[] =
{
    [B_MSG_TRANSFORMED]      = STRINGID_PKMNTRANSFORMEDINTO,
    [B_MSG_TRANSFORM_FAILED] = STRINGID_BUTITFAILED
};

const u16 gSubstituteUsedStringIds[] =
{
    [B_MSG_SET_SUBSTITUTE]    = STRINGID_PKMNMADESUBSTITUTE,
    [B_MSG_SUBSTITUTE_FAILED] = STRINGID_TOOWEAKFORSUBSTITUTE
};

const u16 gGotPoisonedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPOISONED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNPOISONEDBY
};

const u16 gGotParalyzedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASPARALYZED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNWASPARALYZEDBY
};

const u16 gFellAsleepStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLASLEEP,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNMADESLEEP
};

const u16 gGotBurnedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASBURNED,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNBURNEDBY
};

const u16 gGotFrozenStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNWASFROZEN,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNFROZENBY
};

const u16 gGotDefrostedStringIds[] =
{
    [B_MSG_DEFROSTED]         = STRINGID_PKMNWASDEFROSTED2,
    [B_MSG_DEFROSTED_BY_MOVE] = STRINGID_PKMNWASDEFROSTEDBY
};

const u16 gKOFailedStringIds[] =
{
    [B_MSG_KO_MISS]       = STRINGID_ATTACKMISSED,
    [B_MSG_KO_UNAFFECTED] = STRINGID_PKMNUNAFFECTED
};

const u16 gAttractUsedStringIds[] =
{
    [B_MSG_STATUSED]            = STRINGID_PKMNFELLINLOVE,
    [B_MSG_STATUSED_BY_ABILITY] = STRINGID_PKMNSXINFATUATEDY
};

const u16 gAbsorbDrainStringIds[] =
{
    [B_MSG_ABSORB]      = STRINGID_PKMNENERGYDRAINED,
    [B_MSG_ABSORB_OOZE] = STRINGID_ITSUCKEDLIQUIDOOZE
};

const u16 gSportsUsedStringIds[] =
{
    [B_MSG_WEAKEN_ELECTRIC] = STRINGID_ELECTRICITYWEAKENED,
    [B_MSG_WEAKEN_FIRE]     = STRINGID_FIREWEAKENED
};

const u16 gPartyStatusHealStringIds[] =
{
    [B_MSG_BELL]                     = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_ATTACKER] = STRINGID_BELLCHIMED,
    [B_MSG_BELL_SOUNDPROOF_PARTNER]  = STRINGID_BELLCHIMED,
    [B_MSG_BELL_BOTH_SOUNDPROOF]     = STRINGID_BELLCHIMED,
    [B_MSG_SOOTHING_AROMA]           = STRINGID_SOOTHINGAROMA
};

const u16 gFutureMoveUsedStringIds[] =
{
    [B_MSG_FUTURE_SIGHT] = STRINGID_PKMNFORESAWATTACK,
    [B_MSG_DOOM_DESIRE]  = STRINGID_PKMNCHOSEXASDESTINY
};

const u16 gBallEscapeStringIds[] =
{
    [BALL_NO_SHAKES]     = STRINGID_PKMNBROKEFREE,
    [BALL_1_SHAKE]       = STRINGID_ITAPPEAREDCAUGHT,
    [BALL_2_SHAKES]      = STRINGID_AARGHALMOSTHADIT,
    [BALL_3_SHAKES_FAIL] = STRINGID_SHOOTSOCLOSE
};

// Overworld weathers that don't have an associated battle weather default to "It is raining."
const u16 gWeatherStartsStringIds[] =
{
    [WEATHER_NONE]               = STRINGID_ITISRAINING,
    [WEATHER_SUNNY_CLOUDS]       = STRINGID_ITISRAINING,
    [WEATHER_SUNNY]              = STRINGID_ITISRAINING,
    [WEATHER_RAIN]               = STRINGID_ITISRAINING,
    [WEATHER_SNOW]               = STRINGID_ITISRAINING,
    [WEATHER_RAIN_THUNDERSTORM]  = STRINGID_ITISRAINING,
    [WEATHER_FOG_HORIZONTAL]     = STRINGID_ITISRAINING,
    [WEATHER_VOLCANIC_ASH]       = STRINGID_ITISRAINING,
    [WEATHER_SANDSTORM]          = STRINGID_SANDSTORMISRAGING,
    [WEATHER_FOG_DIAGONAL]       = STRINGID_ITISRAINING,
    [WEATHER_UNDERWATER]         = STRINGID_ITISRAINING,
    [WEATHER_SHADE]              = STRINGID_ITISRAINING,
    [WEATHER_DROUGHT]            = STRINGID_SUNLIGHTSTRONG,
    [WEATHER_DOWNPOUR]           = STRINGID_ITISRAINING,
    [WEATHER_UNDERWATER_BUBBLES] = STRINGID_ITISRAINING,
    [WEATHER_ABNORMAL]           = STRINGID_ITISRAINING
};

const u16 gInobedientStringIds[] =
{
    [B_MSG_LOAFING]            = STRINGID_PKMNLOAFING,
    [B_MSG_WONT_OBEY]          = STRINGID_PKMNWONTOBEY,
    [B_MSG_TURNED_AWAY]        = STRINGID_PKMNTURNEDAWAY,
    [B_MSG_PRETEND_NOT_NOTICE] = STRINGID_PKMNPRETENDNOTNOTICE
};

const u16 gSafariReactionStringIds[NUM_SAFARI_REACTIONS] =
{
    [B_MSG_MON_WATCHING] = STRINGID_PKMNWATCHINGCAREFULLY,
    [B_MSG_MON_ANGRY]    = STRINGID_PKMNANGRY,
    [B_MSG_MON_EATING]   = STRINGID_PKMNEATING
};

const u16 gTrainerItemCuredStatusStringIds[] =
{
    [AI_HEAL_CONFUSION] = STRINGID_PKMNSITEMSNAPPEDOUT,
    [AI_HEAL_PARALYSIS] = STRINGID_PKMNSITEMCUREDPARALYSIS,
    [AI_HEAL_FREEZE]    = STRINGID_PKMNSITEMDEFROSTEDIT,
    [AI_HEAL_BURN]      = STRINGID_PKMNSITEMHEALEDBURN,
    [AI_HEAL_POISON]    = STRINGID_PKMNSITEMCUREDPOISON,
    [AI_HEAL_SLEEP]     = STRINGID_PKMNSITEMWOKEIT
};

const u16 gBerryEffectStringIds[] =
{
    [B_MSG_CURED_PROBLEM]     = STRINGID_PKMNSITEMCUREDPROBLEM,
    [B_MSG_NORMALIZED_STATUS] = STRINGID_PKMNSITEMNORMALIZEDSTATUS
};

const u16 gBRNPreventionStringIds[] =
{
    [B_MSG_ABILITY_PREVENTS_MOVE_STATUS]    = STRINGID_PKMNSXPREVENTSBURNS,
    [B_MSG_ABILITY_PREVENTS_ABILITY_STATUS] = STRINGID_PKMNSXPREVENTSYSZ,
    [B_MSG_STATUS_HAD_NO_EFFECT]            = STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPRLZPreventionStringIds[] =
{
    [B_MSG_ABILITY_PREVENTS_MOVE_STATUS]    = STRINGID_PKMNPREVENTSPARALYSISWITH,
    [B_MSG_ABILITY_PREVENTS_ABILITY_STATUS] = STRINGID_PKMNSXPREVENTSYSZ,
    [B_MSG_STATUS_HAD_NO_EFFECT]            = STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gPSNPreventionStringIds[] =
{
    [B_MSG_ABILITY_PREVENTS_MOVE_STATUS]    = STRINGID_PKMNPREVENTSPOISONINGWITH,
    [B_MSG_ABILITY_PREVENTS_ABILITY_STATUS] = STRINGID_PKMNSXPREVENTSYSZ,
    [B_MSG_STATUS_HAD_NO_EFFECT]            = STRINGID_PKMNSXHADNOEFFECTONY
};

const u16 gItemSwapStringIds[] =
{
    [B_MSG_ITEM_SWAP_TAKEN] = STRINGID_PKMNOBTAINEDX,
    [B_MSG_ITEM_SWAP_GIVEN] = STRINGID_PKMNOBTAINEDX2,
    [B_MSG_ITEM_SWAP_BOTH]  = STRINGID_PKMNOBTAINEDXYOBTAINEDZ
};

const u16 gFlashFireStringIds[] =
{
    [B_MSG_FLASH_FIRE_BOOST]    = STRINGID_PKMNRAISEDFIREPOWERWITH,
    [B_MSG_FLASH_FIRE_NO_BOOST] = STRINGID_PKMNSXMADEYINEFFECTIVE
};

const u16 gCaughtMonStringIds[] =
{
    [B_MSG_SENT_SOMEONES_PC]  = STRINGID_PKMNTRANSFERREDSOMEONESPC,
    [B_MSG_SENT_BILLS_PC]     = STRINGID_PKMNTRANSFERREDBILLSPC,
    [B_MSG_SOMEONES_BOX_FULL] = STRINGID_PKMNBOXSOMEONESPCFULL,
    [B_MSG_BILLS_BOX_FULL]    = STRINGID_PKMNBOXBILLSPCFULL
};

// Index is determined in VARIOUS_GET_BATTLERS_FOR_RECALL by ORing flags for each present battler on the losing side.
// No battlers (0) is skipped.
const u16 gDoubleBattleRecallStrings[1 << (MAX_BATTLERS_COUNT / 2)] =
{
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON1COMEBACK,
    STRINGID_TRAINER1MON2COMEBACK,
    STRINGID_TRAINER1MON1AND2COMEBACK
};

const u16 gTrappingMoves[NUM_TRAPPING_MOVES + 1] =
{
    MOVE_BIND,
    MOVE_WRAP,
    MOVE_FIRE_SPIN,
    MOVE_CLAMP,
    MOVE_WHIRLPOOL,
    MOVE_SAND_TOMB,
    0xFFFF // Never read
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_PkmnIsEvolving[] = _("Cosa?\n"
    "{STR_VAR_1} si sta evolvendo!");
#else
const u8 gText_PkmnIsEvolving[] = _("What?\n{STR_VAR_1} is evolving!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_CongratsPkmnEvolved[] = _("Complimenti! Il tuo {STR_VAR_1}\n"
    "si è evoluto in {STR_VAR_2}!{WAIT_SE}\p");
#else
const u8 gText_CongratsPkmnEvolved[] = _("Congratulations! Your {STR_VAR_1}\nevolved into {STR_VAR_2}!{WAIT_SE}\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_PkmnStoppedEvolving[] = _("Bloccata evoluzione\n"
    "di {STR_VAR_1}!\p");
#else
const u8 gText_PkmnStoppedEvolving[] = _("Huh? {STR_VAR_1}\nstopped evolving!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_EllipsisQuestionMark[] = _("… …?\p");
#else
const u8 gText_EllipsisQuestionMark[] = _("……?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_WhatWillPkmnDo[] = _("Cosa deve fare\n"
    "{B_ACTIVE_NAME_WITH_PREFIX}?");
#else
const u8 gText_WhatWillPkmnDo[] = _("What will\n{B_ACTIVE_NAME_WITH_PREFIX} do?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_WhatWillPlayerThrow[] = _("Cosa farà {B_PLAYER_NAME}?");
#else
const u8 gText_WhatWillPlayerThrow[] = _("What will {B_PLAYER_NAME}\nthrow?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_WhatWillOldManDo[] = _("Cosa deve fare\n"
    "il vecchietto?");
#else
const u8 gText_WhatWillOldManDo[] = _("What will the\nold man do?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_LinkStandby[] = _("{PAUSE 16}Un momento…");
#else
const u8 gText_LinkStandby[] = _("{PAUSE 16}Link standby…");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_BattleMenu[] = _("{PALETTE}È{COLOR_HIGHLIGHT_SHADOW}ÒÓÔLOTTA{CLEAR_TO 56}ZAINO\n"
    "POKéMON{CLEAR_TO 56}FUGA");
#else
const u8 gText_BattleMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}FIGHT{CLEAR_TO 56}BAG\nPOKéMON{CLEAR_TO 56}RUN");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_SafariZoneMenu[] = _("{PALETTE}È{COLOR_HIGHLIGHT_SHADOW}ÒÓÔBALL{CLEAR_TO 56}ESCA\n"
    "SASSO{CLEAR_TO 56}FUGA");
#else
const u8 gText_SafariZoneMenu[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}BALL{CLEAR_TO 56}BAIT\nROCK{CLEAR_TO 56}RUN");
#endif
const u8 gText_MoveInterfacePP[] = _("PP ");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_MoveInterfaceType[] = _("TIPO/");
#else
const u8 gText_MoveInterfaceType[] = _("TYPE/");
#endif
const u8 gText_MoveInterfaceDynamicColors[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
const u8 gText_WhichMoveToForget_Unused[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}どの わざを\nわすれさせたい?");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_BattleYesNoChoice[] = _("{PALETTE}È{COLOR_HIGHLIGHT_SHADOW}ÒÓÔSÌ\n"
    "NO");
#else
const u8 gText_BattleYesNoChoice[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Yes\nNo");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_BattleSwitchWhich[] = _("{PALETTE}È{COLOR_HIGHLIGHT_SHADOW}ÒÓÔSposta\n"
    "quale?");
#else
const u8 gText_BattleSwitchWhich[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}Switch\nwhich?");
#endif
static const u8 sText_UnusedColors[] = _("{PALETTE 5}{COLOR_HIGHLIGHT_SHADOW 13 14 15}");
static const u8 sText_RightArrow2[] = _("{RIGHT_ARROW_2}");
static const u8 sText_Plus[] = _("{PLUS}");
static const u8 sText_Dash[] = _("-");

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_MaxHP[] = _("{FONT_SMALL}PS{FONT_NORMAL} max");
#else
static const u8 sText_MaxHP[] = _("{FONT_SMALL}Max{FONT_NORMAL} HP");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Attack[] = _("ATTACCO");
#else
static const u8 sText_Attack[] = _("ATTACK ");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Defense[] = _("DIFESA");
#else
static const u8 sText_Defense[] = _("DEFENSE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SpAtk[] = _("ATT. SPEC.");
#else
static const u8 sText_SpAtk[] = _("SP. ATK");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SpDef[] = _("DIF. SPEC.");
#else
static const u8 sText_SpDef[] = _("SP. DEF");
#endif

// Unused
static const u8 *const sStatNamesTable2[] =
{
    sText_MaxHP,
    sText_SpAtk,
    sText_Attack,
    sText_SpDef,
    sText_Defense,
    sText_Speed
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_SafariBalls[] = _("{HIGHLIGHT DARK_GRAY}SAFARI BALL");
#else
const u8 gText_SafariBalls[] = _("{HIGHLIGHT 2}SAFARI BALLS"); //
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_HighlightRed_Left[] = _("{HIGHLIGHT DARK_GRAY}Ancora ");
#else
const u8 gText_HighlightRed_Left[] = _("{HIGHLIGHT 2}Left: ");
#endif
const u8 gText_HighlightRed[] = _("{HIGHLIGHT 2}");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Sleep[] = _("sonno");
#else
const u8 gText_Sleep[] = _("sleep");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Poison[] = _("avvelenamento");
#else
const u8 gText_Poison[] = _("poison");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Burn[] = _("scottatura");
#else
const u8 gText_Burn[] = _("burn");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Paralysis[] = _("paralisi");
#else
const u8 gText_Paralysis[] = _("paralysis");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Ice[] = _("congelamento");
#else
const u8 gText_Ice[] = _("ice");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Confusion[] = _("confusione");
#else
const u8 gText_Confusion[] = _("confusion");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Love[] = _("amore");
#else
const u8 gText_Love[] = _("love");
#endif
const u8 gText_BattleTowerBan_Space[] = _("  ");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_BattleTowerBan_Newline1[] = _("\l");
#else
const u8 gText_BattleTowerBan_Newline1[] = _("\n");
#endif
const u8 gText_BattleTowerBan_Newline2[] = _("\n");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_BattleTowerBan_Is1[] = _(" sono");
#else
const u8 gText_BattleTowerBan_Is1[] = _(" is");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_BattleTowerBan_Is2[] = _(" sono");
#else
const u8 gText_BattleTowerBan_Is2[] = _(" is");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_BadEgg[] = _("UOVO peste");
#else
const u8 gText_BadEgg[] = _("Bad EGG");
#endif
const u8 gText_BattleWallyName[] = _("ミツル");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Win[] = _("{HIGHLIGHT TRANSPARENT}Vinto");
#else
const u8 gText_Win[] = _("{HIGHLIGHT 0}Win");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Loss[] = _("{HIGHLIGHT TRANSPARENT}Perso");
#else
const u8 gText_Loss[] = _("{HIGHLIGHT 0}Loss");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Draw[] = _("{HIGHLIGHT TRANSPARENT}Pari");
#else
const u8 gText_Draw[] = _("{HIGHLIGHT 0}Draw");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SpaceIs[] = _(" è");
#else
static const u8 sText_SpaceIs[] = _(" is");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ApostropheS[] = _("di ");
#else
static const u8 sText_ApostropheS[] = _("'s");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ANormalMove[] = _("una mossa NORMALE");
#else
const u8 gText_ANormalMove[] = _("a NORMAL move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AFightingMove[] = _("una mossa LOTTA");
#else
const u8 gText_AFightingMove[] = _("a FIGHTING move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AFlyingMove[] = _("una mossa VOLANTE");
#else
const u8 gText_AFlyingMove[] = _("a FLYING move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_APoisonMove[] = _("una mossa VELENO");
#else
const u8 gText_APoisonMove[] = _("a POISON move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AGroundMove[] = _("una mossa TERRA");
#else
const u8 gText_AGroundMove[] = _("a GROUND move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ARockMove[] = _("una mossa ROCCIA");
#else
const u8 gText_ARockMove[] = _("a ROCK move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ABugMove[] = _("una mossa COLEOTTERO");
#else
const u8 gText_ABugMove[] = _("a BUG move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AGhostMove[] = _("una mossa SPETTRO");
#else
const u8 gText_AGhostMove[] = _("a GHOST move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ASteelMove[] = _("una mossa ACCIAIO");
#else
const u8 gText_ASteelMove[] = _("a STEEL move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AMysteryMove[] = _("una mossa ???");
#else
const u8 gText_AMysteryMove[] = _("a ??? move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AFireMove[] = _("una mossa FUOCO");
#else
const u8 gText_AFireMove[] = _("a FIRE move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AWaterMove[] = _("una mossa ACQUA");
#else
const u8 gText_AWaterMove[] = _("a WATER move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AGrassMove[] = _("una mossa ERBA");
#else
const u8 gText_AGrassMove[] = _("a GRASS move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AnElectricMove[] = _("una mossa ELETTRO");
#else
const u8 gText_AnElectricMove[] = _("an ELECTRIC move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_APsychicMove[] = _("una mossa PSICO");
#else
const u8 gText_APsychicMove[] = _("a PSYCHIC move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_AnIceMove[] = _("una mossa GHIACCIO");
#else
const u8 gText_AnIceMove[] = _("an ICE move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ADragonMove[] = _("una mossa DRAGO");
#else
const u8 gText_ADragonMove[] = _("a DRAGON move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ADarkMove[] = _("una mossa BUIO");
#else
const u8 gText_ADarkMove[] = _("a DARK move");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_TimeBoard[] = _("LAVAGNA RECORD");
#else
const u8 gText_TimeBoard[] = _("TIME BOARD");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_ClearTime[] = _("TEMPO");
#else
const u8 gText_ClearTime[] = _("CLEAR TIME"); // Unused
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_XMinYZSec[] = _("{STR_VAR_1} min. {STR_VAR_2},{STR_VAR_3} sec.");
#else
const u8 gText_XMinYZSec[] = _("{STR_VAR_1}MIN. {STR_VAR_2}.{STR_VAR_3}SEC.");
#endif
const u8 gText_Unused_1F[] = _("1F");
const u8 gText_Unused_2F[] = _("2F");
const u8 gText_Unused_3F[] = _("3F");
const u8 gText_Unused_4F[] = _("4F");
const u8 gText_Unused_5F[] = _("5F");
const u8 gText_Unused_6F[] = _("6F");
const u8 gText_Unused_7F[] = _("7F");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_Unused_8F[] = _("8F");
#else
const u8 gText_Unused_8F[] = _("8F");
#endif

const u8 *const gTrainerTowerChallengeTypeTexts[NUM_TOWER_CHALLENGE_TYPES] =
{
    gOtherText_Single,
    gOtherText_Double,
    gOtherText_Knockout,
    gOtherText_Mixed
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Trainer1Fled[] = _("{PLAY_SE}Ù {B_TRAINER1_NAME}, {B_TRAINER1_CLASS}, fugge!");
#else
static const u8 sText_Trainer1Fled[] = _("{PLAY_SE SE_FLEE}{B_TRAINER1_CLASS} {B_TRAINER1_NAME} fled!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerLostAgainstTrainer1[] = _("La sfida è vinta da\n"
    "{B_TRAINER1_NAME}, {B_TRAINER1_CLASS}!");
#else
static const u8 sText_PlayerLostAgainstTrainer1[] = _("Player lost against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("La sfida contro {B_TRAINER1_NAME},\n"
    "{B_TRAINER1_CLASS}, si è conclusa\l"
    "in parità!");
#else
static const u8 sText_PlayerBattledToDrawTrainer1[] = _("Player battled to a draw against\n{B_TRAINER1_CLASS} {B_TRAINER1_NAME}!");
#endif

static const u8 *const sATypeMove_Table[NUMBER_OF_MON_TYPES] =
{
    [TYPE_NORMAL]   = gText_ANormalMove,
    [TYPE_FIGHTING] = gText_AFightingMove,
    [TYPE_FLYING]   = gText_AFlyingMove,
    [TYPE_POISON]   = gText_APoisonMove,
    [TYPE_GROUND]   = gText_AGroundMove,
    [TYPE_ROCK]     = gText_ARockMove,
    [TYPE_BUG]      = gText_ABugMove,
    [TYPE_GHOST]    = gText_AGhostMove,
    [TYPE_STEEL]    = gText_ASteelMove,
    [TYPE_MYSTERY]  = gText_AMysteryMove,
    [TYPE_FIRE]     = gText_AFireMove,
    [TYPE_WATER]    = gText_AWaterMove,
    [TYPE_GRASS]    = gText_AGrassMove,
    [TYPE_ELECTRIC] = gText_AnElectricMove,
    [TYPE_PSYCHIC]  = gText_APsychicMove,
    [TYPE_ICE]      = gText_AnIceMove,
    [TYPE_DRAGON]   = gText_ADragonMove,
    [TYPE_DARK]     = gText_ADarkMove
};

static const u16 sGrammarMoveUsedTable[] =
{
    MOVE_SWORDS_DANCE,
    MOVE_STRENGTH,
    MOVE_GROWTH,
    MOVE_HARDEN,
    MOVE_MINIMIZE,
    MOVE_SMOKESCREEN,
    MOVE_WITHDRAW,
    MOVE_DEFENSE_CURL,
    MOVE_EGG_BOMB,
    MOVE_SMOG,
    MOVE_BONE_CLUB,
    MOVE_FLASH,
    MOVE_SPLASH,
    MOVE_ACID_ARMOR,
    MOVE_BONEMERANG,
    MOVE_REST,
    MOVE_SHARPEN,
    MOVE_SUBSTITUTE,
    MOVE_MIND_READER,
    MOVE_SNORE,
    MOVE_PROTECT,
    MOVE_SPIKES,
    MOVE_ENDURE,
    MOVE_ROLLOUT,
    MOVE_SWAGGER,
    MOVE_SLEEP_TALK,
    MOVE_HIDDEN_POWER,
    MOVE_PSYCH_UP,
    MOVE_EXTREME_SPEED,
    MOVE_FOLLOW_ME,
    MOVE_TRICK,
    MOVE_ASSIST,
    MOVE_INGRAIN,
    MOVE_KNOCK_OFF,
    MOVE_CAMOUFLAGE,
    MOVE_ASTONISH,
    MOVE_ODOR_SLEUTH,
    MOVE_GRASS_WHISTLE,
    MOVE_SHEER_COLD,
    MOVE_MUDDY_WATER,
    MOVE_IRON_DEFENSE,
    MOVE_BOUNCE,
    MOVE_NONE,

    MOVE_TELEPORT,
    MOVE_RECOVER,
    MOVE_BIDE,
    MOVE_AMNESIA,
    MOVE_FLAIL,
    MOVE_TAUNT,
    MOVE_BULK_UP,
    MOVE_NONE,

    MOVE_MEDITATE,
    MOVE_AGILITY,
    MOVE_MIMIC,
    MOVE_DOUBLE_TEAM,
    MOVE_BARRAGE,
    MOVE_TRANSFORM,
    MOVE_STRUGGLE,
    MOVE_SCARY_FACE,
    MOVE_CHARGE,
    MOVE_WISH,
    MOVE_BRICK_BREAK,
    MOVE_YAWN,
    MOVE_FEATHER_DANCE,
    MOVE_TEETER_DANCE,
    MOVE_MUD_SPORT,
    MOVE_FAKE_TEARS,
    MOVE_WATER_SPORT,
    MOVE_CALM_MIND,
    MOVE_NONE,

    MOVE_POUND,
    MOVE_SCRATCH,
    MOVE_VICE_GRIP,
    MOVE_WING_ATTACK,
    MOVE_FLY,
    MOVE_BIND,
    MOVE_SLAM,
    MOVE_HORN_ATTACK,
    MOVE_WRAP,
    MOVE_THRASH,
    MOVE_TAIL_WHIP,
    MOVE_LEER,
    MOVE_BITE,
    MOVE_GROWL,
    MOVE_ROAR,
    MOVE_SING,
    MOVE_PECK,
    MOVE_ABSORB,
    MOVE_STRING_SHOT,
    MOVE_EARTHQUAKE,
    MOVE_FISSURE,
    MOVE_DIG,
    MOVE_TOXIC,
    MOVE_SCREECH,
    MOVE_METRONOME,
    MOVE_LICK,
    MOVE_CLAMP,
    MOVE_CONSTRICT,
    MOVE_POISON_GAS,
    MOVE_BUBBLE,
    MOVE_SLASH,
    MOVE_SPIDER_WEB,
    MOVE_NIGHTMARE,
    MOVE_CURSE,
    MOVE_FORESIGHT,
    MOVE_CHARM,
    MOVE_ATTRACT,
    MOVE_ROCK_SMASH,
    MOVE_UPROAR,
    MOVE_SPIT_UP,
    MOVE_SWALLOW,
    MOVE_TORMENT,
    MOVE_FLATTER,
    MOVE_ROLE_PLAY,
    MOVE_ENDEAVOR,
    MOVE_TICKLE,
    MOVE_COVET,
    MOVE_NONE
};

void BufferStringBattle(u16 stringId)
{
    s32 i;
    const u8 *stringPtr = NULL;

    sBattleMsgDataPtr = (struct BattleMsgData *)(&gBattleBufferA[gActiveBattler][4]);
    gLastUsedItem = sBattleMsgDataPtr->lastItem;
    gLastUsedAbility = sBattleMsgDataPtr->lastAbility;
    gBattleScripting.battler = sBattleMsgDataPtr->scrActive;
    *(&gBattleStruct->scriptPartyIdx) = sBattleMsgDataPtr->bakScriptPartyIdx;
    *(&gBattleStruct->hpScale) = sBattleMsgDataPtr->hpScale;
    gPotentialItemEffectBattler = sBattleMsgDataPtr->itemEffectBattler;
    *(&gBattleStruct->stringMoveType) = sBattleMsgDataPtr->moveType;

    for (i = 0; i < MAX_BATTLERS_COUNT; i++)
    {
        sBattlerAbilities[i] = sBattleMsgDataPtr->abilities[i];
    }
    for (i = 0; i < TEXT_BUFF_ARRAY_COUNT; i++)
    {
        gBattleTextBuff1[i] = sBattleMsgDataPtr->textBuffs[0][i];
        gBattleTextBuff2[i] = sBattleMsgDataPtr->textBuffs[1][i];
        gBattleTextBuff3[i] = sBattleMsgDataPtr->textBuffs[2][i];
    }

    switch (stringId)
    {
    case STRINGID_INTROMSG: // first battle msg
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                {
                    stringPtr = sText_TwoLinkTrainersWantToBattle;
                }
                else
                {
                    if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                        stringPtr = sText_Trainer1WantsToBattle;
                    else
                        stringPtr = sText_LinkTrainerWantsToBattle;
                }
            }
            else
            {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                stringPtr = DECOMPRESS_BATTLE_STRING(sText_Trainer1WantsToBattle);
#else
                stringPtr = sText_Trainer1WantsToBattle;
#endif
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_GHOST)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_GHOST_UNVEILED)
                    stringPtr = sText_TheGhostAppeared;
                else
                    stringPtr = sText_GhostAppearedCantId;
            }
            else if (gBattleTypeFlags & BATTLE_TYPE_LEGENDARY)
                stringPtr = sText_WildPkmnAppeared2;
            else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) // interesting, looks like they had something planned for wild double battles
                stringPtr = sText_TwoWildPkmnAppeared;
            else if (gBattleTypeFlags & BATTLE_TYPE_OLD_MAN_TUTORIAL)
                stringPtr = sText_WildPkmnAppearedPause;
            else
                stringPtr = sText_WildPkmnAppeared;
        }
        break;
    case STRINGID_INTROSENDOUT: // poke first send-out
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkPartnerSentOutPkmnGoPkmn;
                else
                    stringPtr = sText_GoTwoPkmn;
            }
            else
            {
                stringPtr = sText_GoPkmn;
            }
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_TwoLinkTrainersSentOutPkmn;
                else if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                    stringPtr = sText_LinkTrainerSentOutTwoPkmn;
                else
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                    stringPtr = DECOMPRESS_BATTLE_STRING(sText_Trainer1SentOutTwoPkmn);
#else
                    stringPtr = sText_Trainer1SentOutTwoPkmn;
#endif
            }
            else
            {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                {
                    if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                        stringPtr = sText_Trainer1SentOutPkmn;
                    else
                        stringPtr = sText_LinkTrainerSentOutPkmn;
                }
                else
                {
                    stringPtr = DECOMPRESS_BATTLE_STRING(sText_Trainer1SentOutPkmn);
                }
#else
                if (!(gBattleTypeFlags & BATTLE_TYPE_LINK))
                    stringPtr = sText_Trainer1SentOutPkmn;
                else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn;
#endif
            }
        }
        break;
    case STRINGID_RETURNMON: // sending poke to ball msg
        if (GetBattlerSide(gActiveBattler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0)
                stringPtr = sText_PkmnThatsEnough;
            else if (*(&gBattleStruct->hpScale) == 1 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_PkmnComeBack;
            else if (*(&gBattleStruct->hpScale) == 2)
                stringPtr = sText_PkmnOkComeBack;
            else
                stringPtr = sText_PkmnGoodComeBack;
        }
        else
        {
            if (gTrainerBattleOpponent_A == TRAINER_LINK_OPPONENT)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainer2WithdrewPkmn;
                else
                    stringPtr = sText_LinkTrainer1WithdrewPkmn;
            }
            else
            {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                stringPtr = DECOMPRESS_BATTLE_STRING(sText_Trainer1WithdrewPkmn);
#else
                stringPtr = sText_Trainer1WithdrewPkmn;
#endif
            }
        }
        break;
    case STRINGID_SWITCHINMON: // switch-in msg
        if (GetBattlerSide(gBattleScripting.battler) == B_SIDE_PLAYER)
        {
            if (*(&gBattleStruct->hpScale) == 0 || gBattleTypeFlags & BATTLE_TYPE_DOUBLE)
                stringPtr = sText_GoPkmn2;
            else if (*(&gBattleStruct->hpScale) == 1)
                stringPtr = sText_DoItPkmn;
            else if (*(&gBattleStruct->hpScale) == 2)
                stringPtr = sText_GoForItPkmn;
            else
                stringPtr = sText_YourFoesWeakGetEmPkmn;
        }
        else
        {
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                    stringPtr = sText_LinkTrainerMultiSentOutPkmn;
                else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                    stringPtr = sText_Trainer1SentOutPkmn2;
                else
                    stringPtr = sText_LinkTrainerSentOutPkmn2;
            }
            else
            {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                stringPtr = DECOMPRESS_BATTLE_STRING(sText_Trainer1SentOutPkmn2);
#else
                stringPtr = sText_Trainer1SentOutPkmn2;
#endif
            }
        }
        break;
    case STRINGID_USEDMOVE: // pokemon used a move msg
        ChooseMoveUsedParticle(gBattleTextBuff1); // buff1 doesn't appear in the string, leftover from japanese move names

        if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
            StringCopy(gBattleTextBuff2, sATypeMove_Table[*(&gBattleStruct->stringMoveType)]);
        else
            StringCopy(gBattleTextBuff2, gMoveNames[sBattleMsgDataPtr->currentMove]);

        ChooseTypeOfMoveUsedString(gBattleTextBuff2);
        stringPtr = sText_AttackerUsedX;
        break;
    case STRINGID_BATTLEEND: // battle end
        if (gBattleTextBuff1[0] & B_OUTCOME_LINK_BATTLE_RAN)
        {
            gBattleTextBuff1[0] &= ~(B_OUTCOME_LINK_BATTLE_RAN);
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTextBuff1[0] == B_OUTCOME_LOST || gBattleTextBuff1[0] == B_OUTCOME_DREW)
                stringPtr = sText_GotAwaySafely;
            else if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
                stringPtr = sText_TwoWildFled;
            else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                stringPtr = sText_Trainer1Fled;
            else
                stringPtr = sText_WildFled;
        }
        else
        {
            if (GetBattlerSide(gActiveBattler) == B_SIDE_OPPONENT && gBattleTextBuff1[0] != B_OUTCOME_DREW)
                gBattleTextBuff1[0] ^= (B_OUTCOME_LOST | B_OUTCOME_WON);

            if (gBattleTypeFlags & BATTLE_TYPE_MULTI)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_TwoLinkTrainersDefeated;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostToTwo;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawVsTwo;
                    break;
                }
            }
            else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainerTrainer1;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstTrainer1;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawTrainer1;
                    break;
                }
            }
            else
            {
                switch (gBattleTextBuff1[0])
                {
                case B_OUTCOME_WON:
                    stringPtr = sText_PlayerDefeatedLinkTrainer;
                    break;
                case B_OUTCOME_LOST:
                    stringPtr = sText_PlayerLostAgainstLinkTrainer;
                    break;
                case B_OUTCOME_DREW:
                    stringPtr = sText_PlayerBattledToDrawLinkTrainer;
                    break;
                }
            }
        }
        break;
    default: // load a string from the table
        if (stringId >= BATTLESTRINGS_COUNT)
        {
            gDisplayedStringBattle[0] = EOS;
            return;
        }
        else
        {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
            stringPtr = DECOMPRESS_BATTLE_STRING(gBattleStringsTable[stringId - BATTLESTRINGS_TABLE_START]);
#else
            stringPtr = gBattleStringsTable[stringId - BATTLESTRINGS_TABLE_START];
#endif
        }
        break;
    }

    BattleStringExpandPlaceholdersToDisplayedString(stringPtr);
}

u32 BattleStringExpandPlaceholdersToDisplayedString(const u8 *src)
{
    return BattleStringExpandPlaceholders(src, gDisplayedStringBattle);
}

static const u8 *TryGetStatusString(u8 *src)
{
    u32 i;
    u8 status[] = _("$$$$$$$");
    u32 chars1, chars2, *cmp;
    u8 *statusPtr;

    statusPtr = status;
    for (i = 0; i < 8 && *src != EOS; i++)
        *statusPtr++ = *src++;

    chars1 = *(u32 *)status;
    chars2 = *((u32 *)status + 1);

    for (i = 0; i < NELEMS(gStatusConditionStringsTable); i++)
    {
        cmp = (u32 *)gStatusConditionStringsTable[i][0];
        if (chars1 == cmp[0] && chars2 == cmp[1])
            return gStatusConditionStringsTable[i][1];
    }
    return NULL;
}

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 *GetSchoolKidName(u32 gender)
{
    const u8 *name = gText_Scolara;
    if (gender == 0)
        name = gTrainerClassNames[TRAINER_CLASS_SCHOOL_KID];
    return name;
}

static const u8 *GetPkmnTrainerName(u32 dummy)
{
    return gTrainerClassNames[TRAINER_CLASS_PKMN_TRAINER];
}

static const u8 *GetLeaderName(u32 doubleBattle)
{
    const u8 *name = gText_Capipalestra;
    if (doubleBattle == 0)
        name = gTrainerClassNames[TRAINER_CLASS_LEADER];
    return name;
}

static const u8 *GetItalianTrainerClassName(s32 type, u32 trainerId)
{
    u8 trainerClass;

    switch (type)
    {
    case TRAINER_SECRET_BASE:
        trainerClass = GetSecretBaseTrainerNameIndex();
        return gTrainerClassNames[trainerClass];
    case TRAINER_UNION_ROOM:
        trainerClass = GetUnionRoomTrainerClass();
        return gTrainerClassNames[trainerClass];
    case BATTLE_TYPE_BATTLE_TOWER:
        trainerClass = GetBattleTowerTrainerClassNameId();
        return gTrainerClassNames[trainerClass];
    case BATTLE_TYPE_TRAINER_TOWER:
        trainerClass = GetTrainerTowerOpponentClass();
        return gTrainerClassNames[trainerClass];
    case BATTLE_TYPE_EREADER_TRAINER:
        trainerClass = GetEreaderTrainerClassId();
        return gTrainerClassNames[trainerClass];
    default:
        trainerClass = gTrainers[trainerId].trainerClass;
        {
            u32 gender = GetTrainerEncounterMusicId(trainerId);

            if (trainerClass == TRAINER_CLASS_SCHOOL_KID)
                return GetSchoolKidName(gender);
            if (trainerClass == TRAINER_CLASS_PKMN_TRAINER && gender == 1)
                return GetPkmnTrainerName(1);
            if (trainerClass == TRAINER_CLASS_LEADER)
            {
                bool8 isDouble = gTrainers[trainerId].doubleBattle;
                return GetLeaderName(isDouble == 1);
            }
            break;
        }
    }
    return gTrainerClassNames[trainerClass];
}

#define HANDLE_NICKNAME_STRING_CASE(battlerId, monIndex)                \
    if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)                     \
    {                                                                   \
        GetMonData(&gEnemyParty[monIndex], MON_DATA_NICKNAME, text);    \
        StringGet_Nickname(text);                                       \
        toCpy = text;                                                   \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstId] = *toCpy;                                        \
            dstId++;                                                    \
            toCpy++;                                                    \
        }                                                               \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
    }                                                                   \
    else                                                                \
    {                                                                   \
        GetMonData(&gPlayerParty[monIndex], MON_DATA_NICKNAME, text);   \
        StringGet_Nickname(text);                                       \
        toCpy = text;                                                   \
    }
#else
#define HANDLE_NICKNAME_STRING_CASE(battlerId, monIndex)                \
    if (GetBattlerSide(battlerId) != B_SIDE_PLAYER)                     \
    {                                                                   \
        if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)                     \
            toCpy = sText_FoePkmnPrefix;                                \
        else                                                            \
            toCpy = sText_WildPkmnPrefix;                               \
        while (*toCpy != EOS)                                           \
        {                                                               \
            dst[dstId] = *toCpy;                                        \
            dstId++;                                                    \
            toCpy++;                                                    \
        }                                                               \
        GetMonData(&gEnemyParty[monIndex], MON_DATA_NICKNAME, text);    \
    }                                                                   \
    else                                                                \
    {                                                                   \
        GetMonData(&gPlayerParty[monIndex], MON_DATA_NICKNAME, text);   \
    }                                                                   \
    StringGet_Nickname(text);                                           \
    toCpy = text;
#endif

u32 BattleStringExpandPlaceholders(const u8 *src, u8 *dst)
{
    u32 dstId = 0; // if they used dstId, why not use srcId as well?
    const u8 *toCpy = NULL;
    u8 text[30];
    u8 multiplayerId;
    s32 i;

    multiplayerId = GetMultiplayerId();

    while (*src != EOS)
    {
        if (*src == PLACEHOLDER_BEGIN)
        {
            src++;
            switch (*src)
            {
            case B_TXT_BUFF1:
                if (gBattleTextBuff1[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff1, gStringVar1);
                    toCpy = gStringVar1;
                }
                else
                {
                    toCpy = TryGetStatusString(gBattleTextBuff1);
                    if (toCpy == NULL)
                        toCpy = gBattleTextBuff1;
                }
                break;
            case B_TXT_BUFF2:
                if (gBattleTextBuff2[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff2, gStringVar2);
                    toCpy = gStringVar2;
                }
                else
                    toCpy = gBattleTextBuff2;
                break;
            case B_TXT_BUFF3:
                if (gBattleTextBuff3[0] == B_BUFF_PLACEHOLDER_BEGIN)
                {
                    ExpandBattleTextBuffPlaceholders(gBattleTextBuff3, gStringVar3);
                    toCpy = gStringVar3;
                }
                else
                    toCpy = gBattleTextBuff3;
                break;
            case B_TXT_COPY_VAR_1:
                toCpy = gStringVar1;
                break;
            case B_TXT_COPY_VAR_2:
                toCpy = gStringVar2;
                break;
            case B_TXT_COPY_VAR_3:
                toCpy = gStringVar3;
                break;
            case B_TXT_PLAYER_MON1_NAME: // first player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON1_NAME: // first enemy poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_PLAYER_MON2_NAME: // second player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_OPPONENT_MON2_NAME: // second enemy poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON1_NAME: // link first player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON1_NAME: // link first opponent poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 1]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_LINK_PLAYER_MON2_NAME: // link second player poke name
                GetMonData(&gPlayerParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 2]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_LINK_OPPONENT_MON2_NAME: // link second opponent poke name
                GetMonData(&gEnemyParty[gBattlerPartyIndexes[gLinkPlayers[multiplayerId].id ^ 3]],
                           MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX_MON1: // attacker name with prefix, only battlerId 0/1
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker,
                                            gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker))])
                break;
            case B_TXT_ATK_PARTNER_NAME: // attacker partner name
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    GetMonData(
                        &gPlayerParty[gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker)) +
                                                           2]], MON_DATA_NICKNAME, text);
                else
                    GetMonData(
                        &gEnemyParty[gBattlerPartyIndexes[GetBattlerAtPosition(GET_BATTLER_SIDE(gBattlerAttacker)) +
                                                          2]], MON_DATA_NICKNAME, text);

                StringGet_Nickname(text);
                toCpy = text;
                break;
            case B_TXT_ATK_NAME_WITH_PREFIX: // attacker name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerAttacker, gBattlerPartyIndexes[gBattlerAttacker])
                break;
            case B_TXT_DEF_NAME_WITH_PREFIX: // target name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattlerTarget, gBattlerPartyIndexes[gBattlerTarget])
                break;
            case B_TXT_EFF_NAME_WITH_PREFIX: // effect battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gEffectBattler, gBattlerPartyIndexes[gEffectBattler])
                break;
            case B_TXT_ACTIVE_NAME_WITH_PREFIX: // active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gActiveBattler, gBattlerPartyIndexes[gActiveBattler])
                break;
            case B_TXT_SCR_ACTIVE_NAME_WITH_PREFIX: // scripting active battlerId name with prefix
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, gBattlerPartyIndexes[gBattleScripting.battler])
                break;
            case B_TXT_CURRENT_MOVE: // current move name
                if (sBattleMsgDataPtr->currentMove >= MOVES_COUNT)
                    toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                else
                    toCpy = gMoveNames[sBattleMsgDataPtr->currentMove];
                break;
            case B_TXT_LAST_MOVE: // originally used move name
                if (sBattleMsgDataPtr->originallyUsedMove >= MOVES_COUNT)
                    toCpy = (const u8 *)&sATypeMove_Table[gBattleStruct->stringMoveType];
                else
                    toCpy = gMoveNames[sBattleMsgDataPtr->originallyUsedMove];
                break;
            case B_TXT_LAST_ITEM: // last used item
                if (gBattleTypeFlags & BATTLE_TYPE_LINK)
                {
                    if (gLastUsedItem == ITEM_ENIGMA_BERRY)
                    {
                        if (!(gBattleTypeFlags & BATTLE_TYPE_MULTI))
                        {
                            if ((gBattleStruct->multiplayerId != 0 && (gPotentialItemEffectBattler & BIT_SIDE))
                                || (gBattleStruct->multiplayerId == 0 && !(gPotentialItemEffectBattler & BIT_SIDE)))
                            {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                                toCpy = (const u8 *)StringCopy(gStringVar3, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                toCpy++;
                                StringExpandPlaceholders((u8 *)toCpy, sText_BerrySuffix);
#else
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
#endif
                            }
                            else
                            {
                                toCpy = sText_EnigmaBerry;
                            }
                        }
                        else
                        {
                            if (gLinkPlayers[gBattleStruct->multiplayerId].id == gPotentialItemEffectBattler)
                            {
                                StringCopy(text, gEnigmaBerries[gPotentialItemEffectBattler].name);
                                StringAppend(text, sText_BerrySuffix);
                                toCpy = text;
                            }
                            else
                                toCpy = sText_EnigmaBerry;
                        }
                    }
                    else
                    {
                        CopyItemName(gLastUsedItem, text);
                        toCpy = text;
                    }
                }
                else
                {
                    CopyItemName(gLastUsedItem, text);
                    toCpy = text;
                }
                break;
            case B_TXT_LAST_ABILITY: // last used ability
                toCpy = gAbilityNames[gLastUsedAbility];
                break;
            case B_TXT_ATK_ABILITY: // attacker ability
                toCpy = gAbilityNames[sBattlerAbilities[gBattlerAttacker]];
                break;
            case B_TXT_DEF_ABILITY: // target ability
                toCpy = gAbilityNames[sBattlerAbilities[gBattlerTarget]];
                break;
            case B_TXT_SCR_ACTIVE_ABILITY: // scripting active ability
                toCpy = gAbilityNames[sBattlerAbilities[gBattleScripting.battler]];
                break;
            case B_TXT_EFF_ABILITY: // effect battlerId ability
                toCpy = gAbilityNames[sBattlerAbilities[gEffectBattler]];
                break;
            case B_TXT_TRAINER1_CLASS: // trainer class name
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE)
                    toCpy = GetItalianTrainerClassName(TRAINER_SECRET_BASE, 0);
                else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                    toCpy = GetItalianTrainerClassName(TRAINER_UNION_ROOM, 0);
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                    toCpy = GetItalianTrainerClassName(BATTLE_TYPE_BATTLE_TOWER, 0);
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                    toCpy = GetItalianTrainerClassName(BATTLE_TYPE_TRAINER_TOWER, 0);
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                    toCpy = GetItalianTrainerClassName(BATTLE_TYPE_EREADER_TRAINER, 0);
                else
                    toCpy = GetItalianTrainerClassName(0, gTrainerBattleOpponent_A);
                break;
#else
                if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE)
                    toCpy = gTrainerClassNames[GetSecretBaseTrainerNameIndex()];
                else if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                    toCpy = gTrainerClassNames[GetUnionRoomTrainerClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                    toCpy = gTrainerClassNames[GetBattleTowerTrainerClassNameId()];
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                    toCpy = gTrainerClassNames[GetTrainerTowerOpponentClass()];
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                    toCpy = gTrainerClassNames[GetEreaderTrainerClassId()];
                else
                    toCpy = gTrainerClassNames[gTrainers[gTrainerBattleOpponent_A].trainerClass];
                break;
#endif
            case B_TXT_TRAINER1_NAME: // trainer1 name
                if (gTrainerBattleOpponent_A == TRAINER_SECRET_BASE)
                {
                    for (i = 0; i < (s32)NELEMS(gBattleResources->secretBase->trainerName); i++)
                        text[i] = gBattleResources->secretBase->trainerName[i];
                    text[i] = EOS;
                    toCpy = text;
                }
                if (gTrainerBattleOpponent_A == TRAINER_UNION_ROOM)
                {
                    toCpy = gLinkPlayers[multiplayerId ^ BIT_SIDE].name;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_BATTLE_TOWER)
                {
                    GetBattleTowerTrainerName(text);
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentName(text);
                    toCpy = text;
                }
                else if (gBattleTypeFlags & BATTLE_TYPE_EREADER_TRAINER)
                {
                    CopyEReaderTrainerName5(text);
                    toCpy = text;
                }
                else
                {
                    if (gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_EARLY
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_RIVAL_LATE
                     || gTrainers[gTrainerBattleOpponent_A].trainerClass == TRAINER_CLASS_CHAMPION)
                        toCpy = GetExpandedPlaceholder(PLACEHOLDER_ID_RIVAL);
                    else
                        toCpy = gTrainers[gTrainerBattleOpponent_A].trainerName;
                }
                break;
            case B_TXT_LINK_PLAYER_NAME: // link player name
                toCpy = gLinkPlayers[multiplayerId].name;
                break;
            case B_TXT_LINK_PARTNER_NAME: // link partner name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_PARTNER(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT1_NAME: // link opponent 1 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id))].name;
                break;
            case B_TXT_LINK_OPPONENT2_NAME: // link opponent 2 name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(
                    BATTLE_PARTNER(BATTLE_OPPOSITE(gLinkPlayers[multiplayerId].id)))].name;
                break;
            case B_TXT_LINK_SCR_TRAINER_NAME: // link scripting active name
                toCpy = gLinkPlayers[GetBattlerMultiplayerId(gBattleScripting.battler)].name;
                break;
            case B_TXT_PLAYER_NAME: // player name
                toCpy = gSaveBlock2Ptr->playerName;
                break;
            case B_TXT_TRAINER1_LOSE_TEXT: // trainerA lose text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentLoseText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerALoseText();
                }
                break;
            case B_TXT_TRAINER1_WIN_TEXT: // trainerA win text
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER_TOWER)
                {
                    GetTrainerTowerOpponentWinText(gStringVar4, 0);
                    toCpy = gStringVar4;
                }
                else
                {
                    toCpy = GetTrainerWonSpeech();
                }
                break;
            case B_TXT_TRAINER2_LOSE_TEXT:
                GetTrainerTowerOpponentLoseText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_TRAINER2_WIN_TEXT:
                GetTrainerTowerOpponentWinText(gStringVar4, 1);
                toCpy = gStringVar4;
                break;
            case B_TXT_26: // ?
                HANDLE_NICKNAME_STRING_CASE(gBattleScripting.battler, *(&gBattleStruct->scriptPartyIdx))
                break;
            case B_TXT_PC_CREATOR_NAME: // lanette pc
                if (FlagGet(FLAG_SYS_NOT_SOMEONES_PC))
                    toCpy = sText_Bills;
                else
                    toCpy = sText_Someones;
                break;
            case B_TXT_ATK_PREFIX2:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_DEF_PREFIX2:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix2;
                else
                    toCpy = sText_FoePkmnPrefix3;
                break;
            case B_TXT_ATK_PREFIX1:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_DEF_PREFIX1:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix;
                else
                    toCpy = sText_FoePkmnPrefix2;
                break;
            case B_TXT_ATK_PREFIX3:
                if (GetBattlerSide(gBattlerAttacker) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            case B_TXT_DEF_PREFIX3:
                if (GetBattlerSide(gBattlerTarget) == B_SIDE_PLAYER)
                    toCpy = sText_AllyPkmnPrefix3;
                else
                    toCpy = sText_FoePkmnPrefix4;
                break;
            }

            // missing if (toCpy != NULL) check
            while (*toCpy != EOS)
            {
                dst[dstId++] = *toCpy;
                toCpy++;
            }
            if (*src == B_TXT_TRAINER1_LOSE_TEXT || *src == B_TXT_TRAINER1_WIN_TEXT
             || *src == B_TXT_TRAINER2_LOSE_TEXT || *src == B_TXT_TRAINER2_WIN_TEXT)
            {
                dst[dstId++] = EXT_CTRL_CODE_BEGIN;
                dst[dstId++] = EXT_CTRL_CODE_PAUSE_UNTIL_PRESS;
            }
        }
        else
        {
            dst[dstId++] = *src;
        }
        src++;
    }

    dst[dstId++] = *src;

    return dstId;
}

static void ExpandBattleTextBuffPlaceholders(const u8 *src, u8 *dst)
{
    u32 srcId = 1;
    u32 value = 0;
    u8 text[12];
    u16 hword;

    *dst = EOS;
    while (src[srcId] != B_BUFF_EOS)
    {
        switch (src[srcId])
        {
        case B_BUFF_STRING: // battle string
            hword = T1_READ_16(&src[srcId + 1]);
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
            if (hword == STRINGID_STATSHARPLY || hword == STRINGID_STATHARSHLY)
                srcId += 3;
#endif
            StringAppend(dst, gBattleStringsTable[hword - BATTLESTRINGS_TABLE_START]);
            srcId += 3;
            break;
        case B_BUFF_NUMBER: // int to string
            switch (src[srcId + 1])
            {
            case 1:
                value = src[srcId + 3];
                break;
            case 2:
                value = T1_READ_16(&src[srcId + 3]);
                break;
            case 4:
                value = T1_READ_32(&src[srcId + 3]);
                break;
            }
            ConvertIntToDecimalStringN(dst, value, STR_CONV_MODE_LEFT_ALIGN, src[srcId + 2]);
            srcId += src[srcId + 1] + 3;
            break;
        case B_BUFF_MOVE: // move name
            StringAppend(dst, gMoveNames[T1_READ_16(&src[srcId + 1])]);
            srcId += 3;
            break;
        case B_BUFF_TYPE: // type name
            StringAppend(dst, gTypeNames[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_MON_NICK_WITH_PREFIX: // poke nick with prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
            {
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
            }
            else
            {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
                StringGet_Nickname(text);
                StringAppend(dst, text);

                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                    StringAppend(dst, sText_FoePkmnPrefix);
                else
                    StringAppend(dst, sText_WildPkmnPrefix);

                srcId += 3;
                break;
#else
                if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
                    StringAppend(dst, sText_FoePkmnPrefix);
                else
                    StringAppend(dst, sText_WildPkmnPrefix);

                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, text);
#endif
            }
            StringGet_Nickname(text);
            StringAppend(dst, text);
            srcId += 3;
            break;
        case B_BUFF_STAT: // stats
            StringAppend(dst, gStatNamesTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_SPECIES: // species name
            GetSpeciesName(dst, T1_READ_16(&src[srcId + 1]));
            srcId += 3;
            break;
        case B_BUFF_MON_NICK: // poke nick without prefix
            if (GetBattlerSide(src[srcId + 1]) == B_SIDE_PLAYER)
                GetMonData(&gPlayerParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            else
                GetMonData(&gEnemyParty[src[srcId + 2]], MON_DATA_NICKNAME, dst);
            StringGet_Nickname(dst);
            srcId += 3;
            break;
        case B_BUFF_NEGATIVE_FLAVOR: // flavor table
            StringAppend(dst, gPokeblockWasTooXStringTable[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ABILITY: // ability names
            StringAppend(dst, gAbilityNames[src[srcId + 1]]);
            srcId += 2;
            break;
        case B_BUFF_ITEM: // item name
            hword = T1_READ_16(&src[srcId + 1]);
            if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            {
                if (hword == ITEM_ENIGMA_BERRY)
                {
                    if (gLinkPlayers[gBattleStruct->multiplayerId].id == gPotentialItemEffectBattler)
                    {
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
                        StringCopy(gStringVar3, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringExpandPlaceholders(dst, sText_BerrySuffix);
#else
                        StringCopy(dst, gEnigmaBerries[gPotentialItemEffectBattler].name);
                        StringAppend(dst, sText_BerrySuffix);
#endif
                    }
                    else
                    {
                        StringAppend(dst, sText_EnigmaBerry);
                    }
                }
                else
                {
                    CopyItemName(hword, dst);
                }
            }
            else
            {
                CopyItemName(hword, dst);
            }
            srcId += 3;
            break;
        }
    }
}

// Loads one of two text strings into the provided buffer. This is functionally
// unused, since the value loaded into the buffer is not read; it loaded one of
// two particles (either "は" or "の") which works in tandem with ChooseTypeOfMoveUsedString
// below to effect changes in the meaning of the line.
static void ChooseMoveUsedParticle(u8 *textBuff)
{
    s32 counter = 0;
    u32 i = 0;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == 0)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    if (counter >= 0)
    {
        if (counter <= 2)
            StringCopy(textBuff, sText_SpaceIs); // is
        else if (counter <= MAX_MON_MOVES)
            StringCopy(textBuff, sText_ApostropheS); // 's
    }
}

// Appends "!" to the text buffer `dst`. In the original Japanese this looked
// into the table of moves at sGrammarMoveUsedTable and varied the line accordingly.
//
// sText_ExclamationMark was a plain "!", used for any attack not on the list.
// It resulted in the translation "<NAME>'s <ATTACK>!".
//
// sText_ExclamationMark2 was "を つかった！". This resulted in the translation
// "<NAME> used <ATTACK>!", which was used for all attacks in English.
//
// sText_ExclamationMark3 was "した！". This was used for those moves whose
// names were verbs, such as Recover, and resulted in translations like "<NAME>
// recovered itself!".
//
// sText_ExclamationMark4 was "を した！" This resulted in a translation of
// "<NAME> did an <ATTACK>!".
//
// sText_ExclamationMark5 was " こうげき！" This resulted in a translation of
// "<NAME>'s <ATTACK> attack!".
static void ChooseTypeOfMoveUsedString(u8 *dst)
{
    s32 counter = 0;
    s32 i = 0;

    while (*dst != EOS)
        dst++;

    while (counter != MAX_MON_MOVES)
    {
        if (sGrammarMoveUsedTable[i] == MOVE_NONE)
            counter++;
        if (sGrammarMoveUsedTable[i++] == sBattleMsgDataPtr->currentMove)
            break;
    }

    switch (counter)
    {
    case 0:
        StringCopy(dst, sText_ExclamationMark);
        break;
    case 1:
        StringCopy(dst, sText_ExclamationMark2);
        break;
    case 2:
        StringCopy(dst, sText_ExclamationMark3);
        break;
    case 3:
        StringCopy(dst, sText_ExclamationMark4);
        break;
    case 4:
        StringCopy(dst, sText_ExclamationMark5);
        break;
    }
}

static const struct BattleWindowText sTextOnWindowsInfo_Normal[] = {
    [B_WIN_MSG] = {
        .fillValue = PIXEL_FILL(0xf),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 1,
        .fgColor = 1,
        .bgColor = 15,
        .shadowColor = 6,
    },
    [B_WIN_ACTION_PROMPT] = {
        .fillValue = PIXEL_FILL(0xf),
        .fontId = FONT_NORMAL,
        .x = 2,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 15,
        .shadowColor = 6,
    },
    [B_WIN_ACTION_MENU] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL_COPY_1,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_1] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_SMALL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_2] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_SMALL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_3] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_SMALL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_MOVE_NAME_4] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_SMALL,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_PP] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_SMALL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 12,
        .bgColor = 14,
        .shadowColor = 11,
    },
    [B_WIN_MOVE_TYPE] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_SMALL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_PP_REMAINING] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL_COPY_1,
        .x = 10,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 12,
        .bgColor = 14,
        .shadowColor = 11,
    },
    [B_WIN_DUMMY] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL_COPY_1,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_SWITCH_PROMPT] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL_COPY_1,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_LEVEL_UP_BOX] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 0,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_LEVEL_UP_BANNER] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_SMALL,
        .x = 0x20,
        .y = 0,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 2,
    },
    [B_WIN_YESNO] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 1,
        .lineSpacing = 2,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_PLAYER] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_OPPONENT] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_1] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_2] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_3] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_MULTI_PLAYER_4] = {
        .fillValue = PIXEL_FILL(0xe),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 13,
        .bgColor = 14,
        .shadowColor = 15,
    },
    [B_WIN_VS_OUTCOME_DRAW] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 6,
    },
    [B_WIN_VS_OUTCOME_LEFT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 6,
    },
    [B_WIN_VS_OUTCOME_RIGHT] = {
        .fillValue = PIXEL_FILL(0x0),
        .fontId = FONT_NORMAL,
        .x = 0,
        .y = 2,
        .letterSpacing = 0,
        .lineSpacing = 0,
        .speed = 0,
        .fgColor = 1,
        .bgColor = 0,
        .shadowColor = 6,
    },
    [B_WIN_OAK_OLD_MAN] = {
        .fillValue = PIXEL_FILL(0x1),
        .fontId = FONT_MALE,
        .x = 0,
        .y = 1,
        .letterSpacing = 0,
        .lineSpacing = 1,
        .speed = 1,
        .fgColor = 2,
        .bgColor = 1,
        .shadowColor = 3,
    }
};

static const u8 sNpcTextColorToFont[] = 
{
    [NPC_TEXT_COLOR_MALE]    = FONT_MALE, 
    [NPC_TEXT_COLOR_FEMALE]  = FONT_FEMALE, 
    [NPC_TEXT_COLOR_MON]     = FONT_NORMAL, 
    [NPC_TEXT_COLOR_NEUTRAL] = FONT_NORMAL,
};

// windowId: Upper 2 bits are text flags
//   x40: Use NPC context-defined font
//   x80: Inhibit window clear
void BattlePutTextOnWindow(const u8 *text, u8 windowId) {
    bool32 copyToVram;
    struct TextPrinterTemplate printerTemplate;
    u8 speed;
    int x;
    u8 color;

    u8 textFlags = windowId & 0xC0;
    windowId &= 0x3F;
    if (!(textFlags & 0x80))
        FillWindowPixelBuffer(windowId, sTextOnWindowsInfo_Normal[windowId].fillValue);
    if (textFlags & 0x40) {
        color = ContextNpcGetTextColor();
        printerTemplate.fontId = sNpcTextColorToFont[color];
    }
    else {
        printerTemplate.fontId = sTextOnWindowsInfo_Normal[windowId].fontId;
    }
    switch (windowId)
    {
    case B_WIN_VS_PLAYER:
    case B_WIN_VS_OPPONENT:
    case B_WIN_VS_MULTI_PLAYER_1:
    case B_WIN_VS_MULTI_PLAYER_2:
    case B_WIN_VS_MULTI_PLAYER_3:
    case B_WIN_VS_MULTI_PLAYER_4:
        x = (48 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    case B_WIN_VS_OUTCOME_DRAW:
    case B_WIN_VS_OUTCOME_LEFT:
    case B_WIN_VS_OUTCOME_RIGHT:
        x = (64 - GetStringWidth(sTextOnWindowsInfo_Normal[windowId].fontId, text,
                                 sTextOnWindowsInfo_Normal[windowId].letterSpacing)) / 2;
        break;
    default:
        x = sTextOnWindowsInfo_Normal[windowId].x;
        break;
    }
    if (x < 0)
        x = 0;
    printerTemplate.currentChar = text;
    printerTemplate.windowId = windowId;
    printerTemplate.x = x;
    printerTemplate.y = sTextOnWindowsInfo_Normal[windowId].y;
    printerTemplate.currentX = printerTemplate.x;
    printerTemplate.currentY = printerTemplate.y;
    printerTemplate.letterSpacing = sTextOnWindowsInfo_Normal[windowId].letterSpacing;
    printerTemplate.lineSpacing = sTextOnWindowsInfo_Normal[windowId].lineSpacing;
    printerTemplate.unk = 0;
    printerTemplate.fgColor = sTextOnWindowsInfo_Normal[windowId].fgColor;
    printerTemplate.bgColor = sTextOnWindowsInfo_Normal[windowId].bgColor;
    printerTemplate.shadowColor = sTextOnWindowsInfo_Normal[windowId].shadowColor;
    if (windowId == B_WIN_OAK_OLD_MAN)
        gTextFlags.useAlternateDownArrow = FALSE;
    else
        gTextFlags.useAlternateDownArrow = TRUE;

    if ((gBattleTypeFlags & BATTLE_TYPE_LINK) || ((gBattleTypeFlags & BATTLE_TYPE_POKEDUDE) && windowId != B_WIN_OAK_OLD_MAN))
        gTextFlags.autoScroll = TRUE;
    else
        gTextFlags.autoScroll = FALSE;

    if (windowId == B_WIN_MSG || windowId == B_WIN_OAK_OLD_MAN)
    {
        if (gBattleTypeFlags & BATTLE_TYPE_LINK)
            speed = 1;
        else
            speed = GetTextSpeedSetting();
        gTextFlags.canABSpeedUpPrint = TRUE;
    }
    else
    {
        speed = sTextOnWindowsInfo_Normal[windowId].speed;
        gTextFlags.canABSpeedUpPrint = FALSE;
    }

    AddTextPrinter(&printerTemplate, speed, NULL);
    if (!(textFlags & 0x80))
    {
        PutWindowTilemap(windowId);
        CopyWindowToVram(windowId, COPYWIN_FULL);
    }
}

bool8 BattleStringShouldBeColored(u16 stringId)
{
    if (stringId == STRINGID_TRAINER1LOSETEXT
     || stringId == STRINGID_TRAINER2LOSETEXT
     || stringId == STRINGID_TRAINER1WINTEXT
     || stringId == STRINGID_TRAINER2WINTEXT)
        return TRUE;
    return FALSE;
}

void SetPpNumbersPaletteInMoveSelection(void)
{
    struct ChooseMoveStruct *chooseMoveStruct = (struct ChooseMoveStruct *)(&gBattleBufferA[gActiveBattler][4]);
    const u16 *palPtr = gPPTextPalette;
    u8 var = GetCurrentPpToMaxPpState(chooseMoveStruct->currentPp[gMoveSelectionCursor[gActiveBattler]],
                                      chooseMoveStruct->maxPp[gMoveSelectionCursor[gActiveBattler]]);

    gPlttBufferUnfaded[BG_PLTT_ID(5) + 12] = palPtr[(var * 2) + 0];
    gPlttBufferUnfaded[BG_PLTT_ID(5) + 11] = palPtr[(var * 2) + 1];

    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 12], &gPlttBufferFaded[BG_PLTT_ID(5) + 12], PLTT_SIZEOF(1));
    CpuCopy16(&gPlttBufferUnfaded[BG_PLTT_ID(5) + 11], &gPlttBufferFaded[BG_PLTT_ID(5) + 11], PLTT_SIZEOF(1));
}

u8 GetCurrentPpToMaxPpState(u8 currentPp, u8 maxPp)
{
    if (maxPp == currentPp)
    {
        return 3;
    }
    else if (maxPp <= 2)
    {
        if (currentPp > 1)
            return 3;
        else
            return 2 - currentPp;
    }
    else if (maxPp <= 7)
    {
        if (currentPp > 2)
            return 3;
        else
            return 2 - currentPp;
    }
    else
    {
        if (currentPp == 0)
            return 2;
        if (currentPp <= maxPp / 4)
            return 1;
        if (currentPp > maxPp / 2)
            return 3;
    }

    return 0;
}
