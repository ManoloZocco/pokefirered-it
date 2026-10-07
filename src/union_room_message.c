#include "global.h"
#include "link_rfu.h"
#include "mystery_gift_server.h"
#include "mystery_gift_client.h"
#include "constants/union_room.h"

ALIGNED(4) const u8 gText_UR_EmptyString[] = _("");
ALIGNED(4) const u8 gText_UR_Colon[] = _(":");
ALIGNED(4) const u8 gText_UR_ID[] = _("{ID}");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PleaseStartOver[] = _("Inizia daccapo.");
#else
ALIGNED(4) const u8 gText_UR_PleaseStartOver[] = _("Please start over from the beginning.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_WirelessSearchCanceled[] = _("SISTEMA COMUNICAZIONE WIRELESS:\n"
    "la ricerca è stata annullata.");
#else
ALIGNED(4) const u8 gText_UR_WirelessSearchCanceled[] = _("The WIRELESS COMMUNICATION\nSYSTEM search has been canceled.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_AwaitingCommunucation2[] = _("  ÑóだÙÉúç º+úË&\n"
    "まっÛÁまÒ");
#else
ALIGNED(4) static const u8 sText_AwaitingCommunucation2[] = _("ともだちからの れんらくを\nまっています");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AwaitingCommunication[] = _("{STR_VAR_1}! In attesa\n"
    "comunicazione da altro giocatore.");
#else
ALIGNED(4) const u8 gText_UR_AwaitingCommunication[] = _("{STR_VAR_1}! Awaiting\ncommunication from another player.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AwaitingLinkPressStart[] = _("{STR_VAR_1}! Quando siete\n"
    "tutti pronti, premi START.");
#else
ALIGNED(4) const u8 gText_UR_AwaitingLinkPressStart[] = _("{STR_VAR_1}! Awaiting link!\nPress START when everyone's ready.");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SingleBattle[] = _("(ングルバトル& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_SingleBattle[] = _("シングルバトルを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DoubleBattle[] = _("ダブルバトル& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_DoubleBattle[] = _("ダブルバトルを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_MultiBattle[] = _(" íルチバトル& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_MultiBattle[] = _("マルチバトルを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TradePokemon[] = _(" ポケモンこÂÉ+& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_TradePokemon[] = _("ポケモンこうかんを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_Chat[] = _("   チャット& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_Chat[] = _("チャットを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DistWonderCard[] = _("   êÏぎßカ-ド&Ëばñ");
#else
ALIGNED(4) static const u8 sText_DistWonderCard[] = _("ふしぎなカードをくばる");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DistWonderNews[] = _("êÏぎßニ<-)&Ëばñ");
#else
ALIGNED(4) static const u8 sText_DistWonderNews[] = _("ふしぎなニュースをくばる");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_DistMysteryEvent[] = _("   êÏぎßでÊごÑ& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_DistMysteryEvent[] = _("ふしぎなできごとを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HoldPokemonJump[] = _("   ßわÑび& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_HoldPokemonJump[] = _("なわとびを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HoldBerryCrush[] = _("   Êçîíッ(ャ-& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_HoldBerryCrush[] = _("きのみマッシャーを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HoldBerryPicking[] = _("   Êçîどû& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_HoldBerryPicking[] = _("きのみどりを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HoldSpinTrade[] = _("  ぐñぐñこÂÉ+& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_HoldSpinTrade[] = _("ぐるぐるこうかんを かいさいする");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_HoldSpinShop[] = _("   ぐñぐñ(>ップ& ÉÁÎÁÒñ");
#else
ALIGNED(4) static const u8 sText_HoldSpinShop[] = _("ぐるぐるショップを かいさいする");
#endif

// Unused
static const u8 *const sLinkGroupActionTexts[] = {
    sText_SingleBattle,
    sText_DoubleBattle,
    sText_MultiBattle,
    sText_TradePokemon,
    sText_Chat,
    sText_DistWonderCard,
    sText_DistWonderNews,
    sText_DistWonderCard,
    sText_HoldPokemonJump,
    sText_HoldBerryCrush,
    sText_HoldBerryPicking,
    sText_HoldBerryPicking,
    sText_HoldSpinTrade,
    sText_HoldSpinShop
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_1PlayerNeeded[] = _("Manca 1\n"
    "giocatore.");
#else
static const u8 sText_1PlayerNeeded[] = _("1 player\nneeded.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_2PlayersNeeded[] = _("Mancano 2\n"
    "giocatori.");
#else
static const u8 sText_2PlayersNeeded[] = _("2 players\nneeded.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_3PlayersNeeded[] = _("Mancano 3\n"
    "giocatori.");
#else
static const u8 sText_3PlayersNeeded[] = _("3 players\nneeded.");
#endif
static const u8 sText_4PlayersNeeded[] = _("あと4にん\nひつよう");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_2PlayerMode[] = _("MODALITÀ\n"
    "2 GIOC.");
#else
static const u8 sText_2PlayerMode[] = _("2-PLAYER\nMODE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_3PlayerMode[] = _("MODALITÀ\n"
    "3 GIOC.");
#else
static const u8 sText_3PlayerMode[] = _("3-PLAYER\nMODE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_4PlayerMode[] = _("MODALITÀ\n"
    "4 GIOC.");
#else
static const u8 sText_4PlayerMode[] = _("4-PLAYER\nMODE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_5PlayerMode[] = _("MODALITÀ\n"
    "5 GIOC.");
#else
static const u8 sText_5PlayerMode[] = _("5-PLAYER\nMODE");
#endif

const u8 *const gTexts_UR_PlayersNeededOrMode[][5] = {
    { // 2 players required
        sText_1PlayerNeeded,
        sText_2PlayerMode
    },
    { // 4 players required
        sText_3PlayersNeeded,
        sText_2PlayersNeeded,
        sText_1PlayerNeeded,
        sText_4PlayerMode
    },
    { // 2-5 players required
        sText_1PlayerNeeded,
        sText_2PlayerMode,
        sText_3PlayerMode,
        sText_4PlayerMode,
        sText_5PlayerMode
    },
    { // 3-5 players required
        sText_2PlayersNeeded,
        sText_1PlayerNeeded,
        sText_3PlayerMode,
        sText_4PlayerMode,
        sText_5PlayerMode
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_BButtonCancel[] = _("{B_BUTTON}ESCI");
#else
ALIGNED(4) const u8 gText_UR_BButtonCancel[] = _("{B_BUTTON}CANCEL");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_SearchingForParticipants[] = _(" Œò\n"
    "Î+ÉÏゃ ぼÏ=ÂÙ=Â でÒ!");
#else
ALIGNED(4) static const u8 sText_SearchingForParticipants[] = _("ため\nさんかしゃ ぼしゅうちゅう です！");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PlayerContactedYouForXAccept[] = _("{STR_VAR_2} ti ha contattato per\n"
    "“{STR_VAR_1}”. Accetti?");
#else
ALIGNED(4) const u8 gText_UR_PlayerContactedYouForXAccept[] = _("{STR_VAR_2} contacted you for\n{STR_VAR_1}. Accept?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PlayerContactedYouShareX[] = _("{STR_VAR_2} ti ha contattato.\n"
    "Condividi le {STR_VAR_1}?");
#else
ALIGNED(4) const u8 gText_UR_PlayerContactedYouShareX[] = _("{STR_VAR_2} contacted you.\nWill you share {STR_VAR_1}?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PlayerContactedYouAddToMembers[] = _("{STR_VAR_2} ti ha contattato.\n"
    "Aggiungi ai partecipanti?");
#else
ALIGNED(4) const u8 gText_UR_PlayerContactedYouAddToMembers[] = _("{STR_VAR_2} contacted you.\nAdd to the members?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AreTheseMembersOK[] = _("{STR_VAR_1}!\n"
    "Vanno bene questi partecipanti?");
#else
ALIGNED(4) const u8 gText_UR_AreTheseMembersOK[] = _("{STR_VAR_1}!\nAre these members OK?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_CancelModeWithTheseMembers[] = _("Annulli MODALITÀ {STR_VAR_1}\n"
    "con questi partecipanti?");
#else
ALIGNED(4) const u8 gText_UR_CancelModeWithTheseMembers[] = _("Cancel {STR_VAR_1} MODE\nwith these members?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AnOKWasSentToPlayer[] = _("Inviato “OK”\n"
    "a {STR_VAR_1}.");
#else
ALIGNED(4) const u8 gText_UR_AnOKWasSentToPlayer[] = _("An “OK” was sent\nto {STR_VAR_1}.");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_OtherTrainerUnavailableNow[] = _("L’altro ALLENATORE non è\n"
    "disponibile al momento.\p");
#else
ALIGNED(4) static const u8 sText_OtherTrainerUnavailableNow[] = _("The other TRAINER doesn't appear\nto be available now…\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_CantTransmitTrainerTooFar[] = _("Non puoi comunicare con un\n"
    "ALLENATORE troppo distante.\p");
#else
ALIGNED(4) static const u8 sText_CantTransmitTrainerTooFar[] = _("You can't transmit with a TRAINER\nwho is too far away.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_TrainersNotReadyYet[] = _("L’altro ALLENATORE o gli altri\n"
    "ALLENATORI non sono disponibili.\p");
#else
ALIGNED(4) static const u8 sText_TrainersNotReadyYet[] = _("The other TRAINER(S) is/are not\nready yet.\p");
#endif

const u8 *const gTexts_UR_CantTransmitToTrainer[] = {
    sText_CantTransmitTrainerTooFar,
    sText_TrainersNotReadyYet
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ModeWithTheseMembersWillBeCanceled[] = _("La MODALITÀ {STR_VAR_1} con\n"
    "questi partecipanti sarà annullata.{PAUSE 90}");
#else
ALIGNED(4) const u8 gText_UR_ModeWithTheseMembersWillBeCanceled[] = _("The {STR_VAR_1} MODE with\nthese members will be canceled.{PAUSE 90}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_MemberNoLongerAvailable[] = _("Uno dei partecipanti non può\n"
    "continuare.\p");
#else
ALIGNED(4) static const u8 sText_MemberNoLongerAvailable[] = _("There is a member who can no\nlonger remain available.\p");
#endif

const u8 *const gTexts_UR_PlayerUnavailable[] = {
    sText_OtherTrainerUnavailableNow,
    sText_MemberNoLongerAvailable
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_TrainerAppearsUnavailable[] = _("L’altro ALLENATORE non è\n"
    "disponibile.\p");
#else
ALIGNED(4) static const u8 sText_TrainerAppearsUnavailable[] = _("The other TRAINER appears\nunavailable…\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PlayerSentBackOK[] = _("{STR_VAR_1} ha risposto con un\n"
    "“OK”!");
#else
ALIGNED(4) const u8 gText_UR_PlayerSentBackOK[] = _("{STR_VAR_1} sent back an “OK”!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PlayerOKdRegistration[] = _("{STR_VAR_1} ti ha accettato tra i\n"
    "partecipanti.");
#else
ALIGNED(4) const u8 gText_UR_PlayerOKdRegistration[] = _("{STR_VAR_1} OK'd your registration as\na member.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_PlayerRepliedNo[] = _("{STR_VAR_1} ha risposto con un\n"
    "“no”!\p");
#else
ALIGNED(4) static const u8 sText_PlayerRepliedNo[] = _("{STR_VAR_1} replied, “No…”\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AwaitingOtherMembers[] = _("{STR_VAR_1}!\n"
    "In attesa di altri partecipanti…");
#else
ALIGNED(4) const u8 gText_UR_AwaitingOtherMembers[] = _("{STR_VAR_1}!\nAwaiting other members!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_QuitBeingMember[] = _("Vuoi uscire?");
#else
ALIGNED(4) const u8 gText_UR_QuitBeingMember[] = _("Quit being a member?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_StoppedBeingMember[] = _("Non sei più tra i partecipanti.\p");
#else
ALIGNED(4) static const u8 sText_StoppedBeingMember[] = _("You stopped being a member.\p");
#endif

const u8 *const gTexts_UR_PlayerDisconnected[] = {
    [RFU_STATUS_OK]                  = NULL,
    [RFU_STATUS_FATAL_ERROR]         = sText_MemberNoLongerAvailable,
    [RFU_STATUS_CONNECTION_ERROR]    = sText_TrainerAppearsUnavailable,
    [RFU_STATUS_CHILD_SEND_COMPLETE] = NULL,
    [RFU_STATUS_NEW_CHILD_DETECTED]  = NULL,
    [RFU_STATUS_JOIN_GROUP_OK]       = NULL,
    [RFU_STATUS_JOIN_GROUP_NO]       = sText_PlayerRepliedNo,
    [RFU_STATUS_WAIT_ACK_JOIN_GROUP] = NULL,
    [RFU_STATUS_LEAVE_GROUP_NOTICE]  = NULL,
    [RFU_STATUS_LEAVE_GROUP]         = sText_StoppedBeingMember
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_WirelessLinkEstablished[] = _("Stabilito collegamento SISTEMA\n"
    "COMUNICAZIONE WIRELESS.");
#else
ALIGNED(4) const u8 gText_UR_WirelessLinkEstablished[] = _("The WIRELESS COMMUNICATION\nSYSTEM link has been established.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_WirelessLinkDropped[] = _("Interrotto collegamento SISTEMA\n"
    "COMUNICAZIONE WIRELESS.");
#else
ALIGNED(4) const u8 gText_UR_WirelessLinkDropped[] = _("The WIRELESS COMMUNICATION\nSYSTEM link has been dropped…");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_LinkWithFriendDropped[] = _("Il collegamento è stato interrotto.");
#else
ALIGNED(4) const u8 gText_UR_LinkWithFriendDropped[] = _("The link with your friend has been\ndropped…");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_PlayerRepliedNo2[] = _("{STR_VAR_1} ha risposto\n"
    "con un “no…”");
#else
ALIGNED(4) static const u8 sText_PlayerRepliedNo2[] = _("{STR_VAR_1} replied, “No…”");
#endif

const u8 *const gTexts_UR_LinkDropped[] = {
    [RFU_STATUS_OK]                  = NULL,
    [RFU_STATUS_FATAL_ERROR]         = gText_UR_LinkWithFriendDropped,
    [RFU_STATUS_CONNECTION_ERROR]    = gText_UR_LinkWithFriendDropped,
    [RFU_STATUS_CHILD_SEND_COMPLETE] = NULL,
    [RFU_STATUS_NEW_CHILD_DETECTED]  = NULL,
    [RFU_STATUS_JOIN_GROUP_OK]       = NULL,
    [RFU_STATUS_JOIN_GROUP_NO]       = sText_PlayerRepliedNo2,
    [RFU_STATUS_WAIT_ACK_JOIN_GROUP] = NULL,
    [RFU_STATUS_LEAVE_GROUP_NOTICE]  = NULL,
    [RFU_STATUS_LEAVE_GROUP]         = NULL
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoYouWantXMode[] = _("Vuoi la MODALITÀ\n"
    "{STR_VAR_2}?");
#else
ALIGNED(4) static const u8 sText_DoYouWantXMode[] = _("Do you want the {STR_VAR_2}\nMODE?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoYouWantXMode2[] = _("Vuoi la MODALITÀ\n"
    "{STR_VAR_2}?");
#else
ALIGNED(4) static const u8 sText_DoYouWantXMode2[] = _("Do you want the {STR_VAR_2}\nMODE?");
#endif

// Unused
static const u8 *const sDoYouWantModeTexts[] = {
    sText_DoYouWantXMode,
    sText_DoYouWantXMode2
};

ALIGNED(4) static const u8 sText_CommunicatingPleaseWait[] = _("はなしかけています…\nしょうしょう おまちください"); // Unused
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AwaitingPlayersResponseAboutTrade[] = _("In attesa di risposta da {STR_VAR_1}\n"
    "sullo scambio…");
#else
ALIGNED(4) const u8 gText_UR_AwaitingPlayersResponseAboutTrade[] = _("Awaiting {STR_VAR_1}'s response about\nthe trade…");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_Communicating[] = _("Comunicazione in corso{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n"
    "{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
#else
ALIGNED(4) static const u8 sText_Communicating[] = _("Communicating{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_CommunicatingWithPlayer[] = _("Comunicazione con {STR_VAR_1}{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n"
    "{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
#else
ALIGNED(4) static const u8 sText_CommunicatingWithPlayer[] = _("Communicating with {STR_VAR_1}{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_PleaseWaitAWhile[] = _("Attendi{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n"
    "{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
#else
ALIGNED(4) static const u8 sText_PleaseWaitAWhile[] = _("Please wait a while{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.\n{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.{PAUSE 15}.");
#endif

const u8 *const gTexts_UR_CommunicatingWait[] = {
    sText_Communicating,
    sText_CommunicatingWithPlayer,
    sText_PleaseWaitAWhile
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_HiDoSomethingMale[] = _("Ciao! Che cosa vuoi fare?");
#else
ALIGNED(4) static const u8 sText_HiDoSomethingMale[] = _("Hiya! Is there something that you\nwanted to do?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_HiDoSomethingFemale[] = _("Ehi, ciao! Che cosa vuoi fare?");
#else
ALIGNED(4) static const u8 sText_HiDoSomethingFemale[] = _("Hello!\nWould you like to do something?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_HiDoSomethingAgainMale[] = _("{STR_VAR_1}: Ciao, ci si rincontra!\n"
    "Cosa vuoi fare questa volta?");
#else
ALIGNED(4) static const u8 sText_HiDoSomethingAgainMale[] = _("{STR_VAR_1}: Hiya, we meet again!\nWhat are you up for this time?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_HiDoSomethingAgainFemale[] = _("{STR_VAR_1}: Ciao, {PLAYER}!\n"
    "Che cosa vuoi fare?");
#else
ALIGNED(4) static const u8 sText_HiDoSomethingAgainFemale[] = _("{STR_VAR_1}: Oh! {PLAYER}, hello!\nWould you like to do something?");
#endif

const u8 *const gTexts_UR_HiDoSomething[][GENDER_COUNT] = {
    {
        sText_HiDoSomethingMale,
        sText_HiDoSomethingFemale
    }, {
        sText_HiDoSomethingAgainMale,
        sText_HiDoSomethingAgainFemale
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoSomethingMale[] = _("Che cosa vuoi fare?");
#else
ALIGNED(4) static const u8 sText_DoSomethingMale[] = _("Want to do something?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoSomethingFemale[] = _("Che cosa vuoi fare?");
#else
ALIGNED(4) static const u8 sText_DoSomethingFemale[] = _("Would you like to do something?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoSomethingAgainMale[] = _("{STR_VAR_1}: Che cosa vuoi fare ora?");
#else
ALIGNED(4) static const u8 sText_DoSomethingAgainMale[] = _("{STR_VAR_1}: What would you like to\ndo now?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoSomethingAgainFemale[] = _("{STR_VAR_1}‘まŒ ßàÉÒñ?");
#else
ALIGNED(4) static const u8 sText_DoSomethingAgainFemale[] = _("{STR_VAR_1}‘また なにかする？");
#endif

// Unused
static const u8 *const sDoSomethingTexts[][GENDER_COUNT] = {
    {
        sText_DoSomethingMale,
        sText_DoSomethingFemale
    }, {
        sText_DoSomethingAgainMale,
        sText_DoSomethingAgainMale // was probably supposed to be sText_DoSomethingAgainFemale
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_SomebodyHasContactedYou[] = _("Qualcuno ti ha contattato.{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_SomebodyHasContactedYou[] = _("Somebody has contacted you.{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_PlayerHasContactedYou[] = _("{STR_VAR_1} ti ha contattato.{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_PlayerHasContactedYou[] = _("{STR_VAR_1} has contacted you.{PAUSE 60}");
#endif

const u8 *const gTexts_UR_PlayerContactedYou[] = {
    sText_SomebodyHasContactedYou,
    sText_PlayerHasContactedYou
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_AwaitingResponseFromTrainer[] = _("In attesa di risposta\n"
    "dall’altro ALLENATORE…");
#else
ALIGNED(4) static const u8 sText_AwaitingResponseFromTrainer[] = _("Awaiting a response from\nthe other TRAINER…");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_AwaitingResponseFromPlayer[] = _("In attesa di risposta\n"
    "da {STR_VAR_1}…");
#else
ALIGNED(4) static const u8 sText_AwaitingResponseFromPlayer[] = _("Awaiting a response from\n{STR_VAR_1}…");
#endif

const u8 *const gTexts_UR_AwaitingResponse[] = {
    sText_AwaitingResponseFromTrainer,
    sText_AwaitingResponseFromPlayer
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_AwaitingResponseCancelBButton[] = _("ÀÁÛç ÛÁÀ+& まっÛÁまÒ\n"
    "ビ-ボタンで キャンセル");
#else
ALIGNED(4) static const u8 sText_AwaitingResponseCancelBButton[] = _("あいての ていあんを まっています\nビーボタンで キャンセル");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ShowTrainerCard[] = _("L’altro ALLENATORE ti ha mostrato\n"
    "la sua SCHEDA ALLENATORE.\p"
    "Vuoi mostrargli la tua SCHEDA\n"
    "ALLENATORE?");
#else
ALIGNED(4) const u8 gText_UR_ShowTrainerCard[] = _("The other TRAINER showed\nyou their TRAINER CARD.\pWould you like to show your\nTRAINER CARD?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_BattleChallenge[] = _("L’altro ALLENATORE ti sfida\n"
    "a lottare.\p"
    "Accetti la sfida?");
#else
ALIGNED(4) const u8 gText_UR_BattleChallenge[] = _("The other TRAINER challenges you\nto battle.\pWill you accept the battle\nchallenge?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ChatInvitation[] = _("L’altro ALLENATORE ti invita\n"
    "a chattare.\p"
    "Accetti l’invito?");
#else
ALIGNED(4) const u8 gText_UR_ChatInvitation[] = _("The other TRAINER invites you\nto chat.\pWill you accept the chat\ninvitation?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_OfferToTradeMon[] = _("Ricevuta offerta di scambio: il tuo\n"
    "{DYNAMIC 0x01} del L. {DYNAMIC 0x00} registrato\p"
    "per {DYNAMIC 0x03} del L. {DYNAMIC 0x02}.\p"
    "Accetti l’offerta?");
#else
ALIGNED(4) const u8 gText_UR_OfferToTradeMon[] = _("There is an offer to trade your\nregistered Lv. {DYNAMIC 0} {DYNAMIC 1}\pin exchange for a\nLv. {DYNAMIC 2} {DYNAMIC 3}.\pWill you accept this trade\noffer?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_OfferToTradeEgg[] = _("Ricevuta offerta di scambio\n"
    "per il tuo UOVO registrato.\l"
    "Accetti l’offerta?");
#else
ALIGNED(4) const u8 gText_UR_OfferToTradeEgg[] = _("There is an offer to trade your\nregistered EGG.\lWill you accept this trade offer?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ChatDropped[] = _("La chat è stata interrotta.\p");
#else
ALIGNED(4) const u8 gText_UR_ChatDropped[] = _("The chat has been dropped.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_OfferDeclined1[] = _("Hai rifiutato l’offerta.\p");
#else
ALIGNED(4) const u8 gText_UR_OfferDeclined1[] = _("You declined the offer.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_OfferDeclined2[] = _("Hai rifiutato l’offerta.\p");
#else
ALIGNED(4) const u8 gText_UR_OfferDeclined2[] = _("You declined the offer.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ChatEnded[] = _("La chat è stata conclusa.\p");
#else
ALIGNED(4) const u8 gText_UR_ChatEnded[] = _("The chat was ended.\p");
#endif

// Unused
static const u8 *const sInvitationTexts[] = {
    gText_UR_ShowTrainerCard,
    gText_UR_BattleChallenge,
    gText_UR_ChatInvitation,
    gText_UR_OfferToTradeMon
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_JoinChatMale[] = _("Ehi, ciao! Siamo in chat!\n"
    "Vuoi partecipare?");
#else
ALIGNED(4) static const u8 sText_JoinChatMale[] = _("Oh, hey! We're in a chat right now.\nWant to join us?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_PlayerJoinChatMale[] = _("{STR_VAR_1}: Ehi, {PLAYER}!\n"
    "Siamo in chat!\l"
    "Vuoi partecipare?");
#else
ALIGNED(4) static const u8 sText_PlayerJoinChatMale[] = _("{STR_VAR_1}: Hey, {PLAYER}!\nWe're having a chat right now.\lWant to join us?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_JoinChatFemale[] = _("Ciao! Siamo in chat!\n"
    "Vuoi partecipare?");
#else
ALIGNED(4) static const u8 sText_JoinChatFemale[] = _("Oh, hi! We're having a chat now.\nWould you like to join us?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_PlayerJoinChatFemale[] = _("{STR_VAR_1}: Ciao, {PLAYER}!\n"
    "Siamo in chat!\l"
    "Vuoi partecipare?");
#else
ALIGNED(4) static const u8 sText_PlayerJoinChatFemale[] = _("{STR_VAR_1}: Oh, hi, {PLAYER}!\nWe're having a chat now.\lWould you like to join us?");
#endif

const u8 *const gTexts_UR_JoinChat[][GENDER_COUNT] = {
    {
        sText_JoinChatMale,
        sText_JoinChatFemale
    }, {
        sText_PlayerJoinChatMale,
        sText_PlayerJoinChatFemale
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_TrainerAppearsBusy[] = _("……\n"
    "L’ALLENATORE è\l"
    "occupato…\p");
#else
ALIGNED(4) const u8 gText_UR_TrainerAppearsBusy[] = _("……\nThe TRAINER appears to be busy…\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_WaitForBattleMale[] = _("Una lotta?\n"
    "Va bene! Dammi solo un momento!");
#else
ALIGNED(4) static const u8 sText_WaitForBattleMale[] = _("A battle, huh?\nAll right, just give me some time.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_WaitForChatMale[] = _("Vuoi chattare?\n"
    "Benissimo! Dammi solo un momento!");
#else
ALIGNED(4) static const u8 sText_WaitForChatMale[] = _("You want to chat, huh?\nSure, just wait a little.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowTrainerCardMale[] = _("Senz’altro! Eccoti la mia SCHEDA\n"
    "ALLENATORE, come saluto!");
#else
ALIGNED(4) static const u8 sText_ShowTrainerCardMale[] = _("Sure thing! As my “Greetings,”\nhere's my TRAINER CARD.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_WaitForBattleFemale[] = _("Una lotta?\n"
    "Va bene! Aspetta solo un momento!");
#else
ALIGNED(4) static const u8 sText_WaitForBattleFemale[] = _("A battle? Of course, but I need\ntime to get ready.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_WaitForChatFemale[] = _("Vuoi chattare?\n"
    "OK, aspetta un momento.");
#else
ALIGNED(4) static const u8 sText_WaitForChatFemale[] = _("Did you want to chat?\nOkay, but please wait a moment.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowTrainerCardFemale[] = _("Per presentarmi ti mostro la\n"
    "mia SCHEDA ALLENATORE!");
#else
ALIGNED(4) static const u8 sText_ShowTrainerCardFemale[] = _("As my introduction, I'll show you\nmy TRAINER CARD.");
#endif

const u8 *const gTexts_UR_WaitOrShowCard[GENDER_COUNT][4] = {
    {
        sText_WaitForBattleMale,
        sText_WaitForChatMale,
        NULL,
        sText_ShowTrainerCardMale
    }, {
        sText_WaitForBattleFemale,
        sText_WaitForChatFemale,
        NULL,
        sText_ShowTrainerCardFemale
    }
};

ALIGNED(4) static const u8 sText_WaitForChatMale2[] = _("チャットだね！\nわかった ちょっと まってて！");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoneWaitingBattleMale[] = _("Scusa, t’ho fatto attendere!\n"
    "Diamo inizio alla lotta!{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_DoneWaitingBattleMale[] = _("Thanks for waiting!\nLet's get our battle started!{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoneWaitingChatMale[] = _("Bene!\n"
    "Dai, chattiamo un po’!{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_DoneWaitingChatMale[] = _("All right!\nLet's chat!{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoneWaitingBattleFemale[] = _("Scusa, t’ho fatto attendere.\n"
    "Dai, iniziamo!{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_DoneWaitingBattleFemale[] = _("Sorry I made you wait!\nLet's get started!{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DoneWaitingChatFemale[] = _("Scusa, t’ho fatto attendere.\n"
    "Dai, chattiamo!{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_DoneWaitingChatFemale[] = _("Sorry I made you wait!\nLet's chat.{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_TradeWillBeStarted[] = _("Ha inizio lo scambio.{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_TradeWillBeStarted[] = _("The trade will be started.{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_BattleWillBeStarted[] = _("Ha inizio la lotta.{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_BattleWillBeStarted[] = _("The battle will be started.{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_EnteringChat[] = _("Ha inizio la chat.{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_EnteringChat[] = _("Entering the chat…{PAUSE 60}");
#endif

const u8 *const gTexts_UR_StartActivity[][GENDER_COUNT][3] = {
    {
        {
            sText_BattleWillBeStarted,
            sText_EnteringChat,
            sText_TradeWillBeStarted
        }, {
            sText_BattleWillBeStarted,
            sText_EnteringChat,
            sText_TradeWillBeStarted
        }
    }, {
        {
            sText_DoneWaitingBattleMale,
            sText_DoneWaitingChatMale,
            sText_TradeWillBeStarted
        }, {
            sText_DoneWaitingBattleFemale,
            sText_DoneWaitingChatFemale,
            sText_TradeWillBeStarted
        }
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_BattleDeclinedMale[] = _("Scusa, ma i miei POKéMON non si\n"
    "sentono molto bene al momento.\l"
    "Lotteremo un’altra volta!\p");
#else
ALIGNED(4) static const u8 sText_BattleDeclinedMale[] = _("Sorry! My POKéMON don't seem to\nbe feeling too well right now.\lLet me battle you another time.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_BattleDeclinedFemale[] = _("Mi spiace, ma i miei POKéMON non\n"
    "si sentono bene in questo momento.\l"
    "Sarà per un’altra volta!\p");
#else
ALIGNED(4) static const u8 sText_BattleDeclinedFemale[] = _("I'm terribly sorry, but my POKéMON\naren't feeling well…\pLet's battle another time.\p");
#endif

const u8 *const gTexts_UR_BattleDeclined[GENDER_COUNT] = {
    sText_BattleDeclinedMale,
    sText_BattleDeclinedFemale
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedMale[] = _("Dov’è finita la mia SCHEDA\n"
    "ALLENATORE?! Scusa, te la\l"
    "mostrerò un’altra volta.\p");
#else
ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedMale[] = _("Huh? My TRAINER CARD…\nWhere'd it go now?\lSorry! I'll show you another time!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedFemale[] = _("Ma dove avrò messo la mia SCHEDA\n"
    "ALLENATORE?! Scusa ma non\l"
    "te la posso mostrare.\p");
#else
ALIGNED(4) static const u8 sText_ShowTrainerCardDeclinedFemale[] = _("Oh? Now where did I put my\nTRAINER CARD?…\lSorry! I'll show you later!\p");
#endif

const u8 *const gTexts_UR_ShowTrainerCardDeclined[GENDER_COUNT] = {
    sText_ShowTrainerCardDeclinedMale,
    sText_ShowTrainerCardDeclinedFemale
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingMale[] = _("Se vuoi chattare, scambiare o\n"
    "lottare con me, fammi un fischio.\p");
#else
ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingMale[] = _("If you want to do something with\nme, just give me a shout!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingFemale[] = _("Se vuoi chattare, scambiare o\n"
    "lottare con me, mi trovi qui.\p");
#else
ALIGNED(4) static const u8 sText_IfYouWantToDoSomethingFemale[] = _("If you want to do something with\nme, don't be shy.\p");
#endif

const u8 *const gTexts_UR_IfYouWantToDoSomething[GENDER_COUNT] = {
    sText_IfYouWantToDoSomethingMale,
    sText_IfYouWantToDoSomethingFemale
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_TrainerBattleBusy[] = _("Scusa ma ho qualcos’altro da\n"
    "fare. Sarà per la prossima volta!\p");
#else
ALIGNED(4) const u8 gText_UR_TrainerBattleBusy[] = _("Whoops! Sorry, but I have to do\nsomething else.\lAnother time, okay?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_NeedTwoMonsOfLevel30OrLower1[] = _("Per lottare, devi avere due\n"
    "POKéMON sotto il L. 30.\p");
#else
ALIGNED(4) const u8 gText_UR_NeedTwoMonsOfLevel30OrLower1[] = _("If you want to battle, you need\ntwo POKéMON that are below\lLv. 30.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_NeedTwoMonsOfLevel30OrLower2[] = _("Puoi lottare se hai due\n"
    "POKéMON sotto il L. 30.\p");
#else
ALIGNED(4) const u8 gText_UR_NeedTwoMonsOfLevel30OrLower2[] = _("For a battle, you need two\nPOKéMON that are below Lv. 30.\p");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_DeclineChatMale[] = _("Oh… Va bene.\n"
    "Torna a trovarmi quando vuoi!");
#else
ALIGNED(4) static const u8 sText_DeclineChatMale[] = _("Oh, all right.\nCome see me anytime, okay?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 stext_DeclineChatFemale[] = _("Oh…\n"
    "Beh, torna quando vuoi!");
#else
ALIGNED(4) static const u8 stext_DeclineChatFemale[] = _("Oh…\nPlease come by anytime.\p");
#endif

// Response from partner when player declines chat
const u8 *const gTexts_UR_DeclineChat[GENDER_COUNT] = {
    sText_DeclineChatMale,
    stext_DeclineChatFemale
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChatDeclinedMale[] = _("Scusa ma non posso in questo\n"
    "momento.\l"
    "Chatteremo un’altra volta.\p");
#else
ALIGNED(4) static const u8 sText_ChatDeclinedMale[] = _("Oh, sorry!\nI just can't right this instant.\lLet's chat another time.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChatDeclinedFemale[] = _("Scusa, in questo momento non\n"
    "posso proprio. Avremo altre\l"
    "occasioni per chattare.\p");
#else
ALIGNED(4) static const u8 sText_ChatDeclinedFemale[] = _("Oh, I'm sorry.\nI have too much to do right now.\lLet's chat some other time.\p");
#endif

// Response from partner when they decline chat
const u8 *const gTexts_UR_ChatDeclined[GENDER_COUNT] = {
    sText_ChatDeclinedMale,
    sText_ChatDeclinedFemale
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_YoureToughMale[] = _("Ti ho visto: sei forte!");
#else
ALIGNED(4) static const u8 sText_YoureToughMale[] = _("Whoa!\nI can tell you're pretty tough!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_UsedGoodMoveMale[] = _("Ho visto la mossa che hai\n"
    "usato!\l"
    "Grande strategia!\p");
#else
ALIGNED(4) static const u8 sText_UsedGoodMoveMale[] = _("You used that move?\nThat's good strategy!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_BattleSurpriseMale[] = _("Così si fa!\n"
    "Sei stupefacente!\p");
#else
ALIGNED(4) static const u8 sText_BattleSurpriseMale[] = _("Way to go!\nThat was an eye-opener!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_SwitchedMonsMale[] = _("Come fai a guidare così bene\n"
    "i tuoi POKéMON nella lotta?\p");
#else
ALIGNED(4) static const u8 sText_SwitchedMonsMale[] = _("Oh! How could you use that\nPOKéMON in that situation?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_YoureToughFemale[] = _("Quel tuo POKéMON…\n"
    "…è davvero notevole!\p");
#else
ALIGNED(4) static const u8 sText_YoureToughFemale[] = _("That POKéMON…\nIt's been raised really well!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_UsedGoodMoveFemale[] = _("Proprio la mossa giusta!\p");
#else
ALIGNED(4) static const u8 sText_UsedGoodMoveFemale[] = _("That's it!\nThis is the right move now!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_BattleSurpriseFemale[] = _("Incredibile!\n"
    "Come fai a lottare in quel modo?\p");
#else
ALIGNED(4) static const u8 sText_BattleSurpriseFemale[] = _("That's awesome!\nYou can battle that way?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_SwitchedMonsFemale[] = _("Sai perfettamente quando sostituire\n"
    "i POKéMON!\p");
#else
ALIGNED(4) static const u8 sText_SwitchedMonsFemale[] = _("You have exquisite timing for\nswitching POKéMON!\p");
#endif

const u8 *const gTexts_UR_BattleReaction[GENDER_COUNT][4] = {
    {
        sText_YoureToughMale,
        sText_UsedGoodMoveMale,
        sText_BattleSurpriseMale,
        sText_SwitchedMonsMale
    }, {
        sText_YoureToughFemale,
        sText_UsedGoodMoveFemale,
        sText_BattleSurpriseFemale,
        sText_SwitchedMonsFemale
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_LearnedSomethingMale[] = _("Ah, davvero? Non lo sapevo!\p");
#else
ALIGNED(4) static const u8 sText_LearnedSomethingMale[] = _("Oh, I see!\nThis is educational!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ThatsFunnyMale[] = _("Sei troppo divertente! Mi sto\n"
    "sbellicando dalle risate!\p");
#else
ALIGNED(4) static const u8 sText_ThatsFunnyMale[] = _("Don't say anything funny anymore!\nI'm sore from laughing!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_RandomChatMale1[] = _("Eh?\n"
    "Non ci credo!\p");
#else
ALIGNED(4) static const u8 sText_RandomChatMale1[] = _("Oh?\nSomething like that happened.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_RandomChatMale2[] = _("Dici davvero?\p");
#else
ALIGNED(4) static const u8 sText_RandomChatMale2[] = _("Hmhm… What?\nSo is this what you're saying?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_LearnedSomethingFemale[] = _("Ma va’?\p");
#else
ALIGNED(4) static const u8 sText_LearnedSomethingFemale[] = _("Is that right?\nI didn't know that.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ThatsFunnyFemale[] = _("Non ho capito. Puoi ripetere?\p");
#else
ALIGNED(4) static const u8 sText_ThatsFunnyFemale[] = _("Ahaha!\nWhat is that about?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_RandomChatFemale1[] = _("Mi hai tolto le parole di bocca!\p");
#else
ALIGNED(4) static const u8 sText_RandomChatFemale1[] = _("Yes, that's exactly it!\nThat's what I meant.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_RandomChatFemale2[] = _("Certo! Hai ragione!\p");
#else
ALIGNED(4) static const u8 sText_RandomChatFemale2[] = _("In other words…\nYes! That's right!\p");
#endif

const u8 *const gTexts_UR_ChatReaction[GENDER_COUNT][4] = {
    {
        sText_LearnedSomethingMale,
        sText_ThatsFunnyMale,
        sText_RandomChatMale1,
        sText_RandomChatMale2
    }, {
        sText_LearnedSomethingFemale,
        sText_ThatsFunnyFemale,
        sText_RandomChatFemale1,
        sText_RandomChatFemale2
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowedTrainerCardMale1[] = _("Questa è la mia SCHEDA\n"
    "ALLENATORE, per presentarmi.\p");
#else
ALIGNED(4) static const u8 sText_ShowedTrainerCardMale1[] = _("I'm just showing my TRAINER CARD\nas my way of greeting.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowedTrainerCardMale2[] = _("Spero che faremo amicizia!\p");
#else
ALIGNED(4) static const u8 sText_ShowedTrainerCardMale2[] = _("I hope I get to know you better!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale1[] = _("Mostriamoci le SCHEDE ALLENATORE,\n"
    "così ci conosciamo!\p");
#else
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale1[] = _("We're showing each other our\nTRAINER CARDS to get acquainted.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale2[] = _("Piacere di conoscerti.\n"
    "Dico davvero!\p");
#else
ALIGNED(4) static const u8 sText_ShowedTrainerCardFemale2[] = _("Glad to meet you.\nPlease don't be a stranger!\p");
#endif

const u8 *const gTexts_UR_TrainerCardReaction[GENDER_COUNT][2] = {
    {
        sText_ShowedTrainerCardMale1,
        sText_ShowedTrainerCardMale2
    }, {
        sText_ShowedTrainerCardFemale1,
        sText_ShowedTrainerCardFemale2
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_MaleTraded1[] = _("Grandioso!\n"
    "Proprio il POKéMON che volevo!\p");
#else
ALIGNED(4) static const u8 sText_MaleTraded1[] = _("Yeahah!\nI really wanted this POKéMON!\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_MaleTraded2[] = _("Era tanto che volevo ottenere\n"
    "questo POKéMON! Ottimo scambio!\p");
#else
ALIGNED(4) static const u8 sText_MaleTraded2[] = _("Finally, a trade got me that\nPOKéMON I'd wanted a long time.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_FemaleTraded1[] = _("Sto facendo uno scambio di\n"
    "POKéMON.\p");
#else
ALIGNED(4) static const u8 sText_FemaleTraded1[] = _("I'm trading POKéMON right now.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_FemaleTraded2[] = _("Ecco il POKéMON che cercavo!\n"
    "Sono felice dello scambio!\p");
#else
ALIGNED(4) static const u8 sText_FemaleTraded2[] = _("I finally got that POKéMON I\nwanted in a trade!\p");
#endif

const u8 *const gTexts_UR_TradeReaction[GENDER_COUNT][4] = {
    {
        sText_MaleTraded1,
        sText_MaleTraded2
    }, {
        sText_FemaleTraded1,
        sText_FemaleTraded2
    }
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
const u8 gText_UR_XCheckedTradingBoard[] = _("{STR_VAR_1} visita\n"
    "l’AREA SCAMBI.\p");
#else
const u8 gText_UR_XCheckedTradingBoard[] = _("{STR_VAR_1} checked the\nTRADING BOARD.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_RegisterMonAtTradingBoard[] = _("Questa è l’AREA SCAMBI.\p"
    "Qui puoi registrare i tuoi POKéMON,\n"
    "offrendoli per uno scambio.\p"
    "Vuoi registrare uno dei tuoi\n"
    "POKéMON?");
#else
ALIGNED(4) const u8 gText_UR_RegisterMonAtTradingBoard[] = _("Welcome to the TRADING BOARD.\pYou may register your POKéMON\nand offer it up for a trade.\pWould you like to register one of\nyour POKéMON?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_TradingBoardInfo[] = _("Nell’AREA SCAMBI si può offrire un\n"
    "POKéMON per uno scambio.\p"
    "Non devi fare altro che registrare\n"
    "il POKéMON che vuoi scambiare.\p"
    "Un altro ALLENATORE potrebbe\n"
    "offrire uno dei suoi POKéMON per\l"
    "lo scambio.\p"
    "Speriamo che tu possa fare\n"
    "tantissimi scambi in questo\l"
    "modo!\p"
    "Vuoi registrare uno dei tuoi\n"
    "POKéMON?");
#else
ALIGNED(4) const u8 gText_UR_TradingBoardInfo[] = _("This TRADING BOARD is used for\n"
                                                    "offering a POKéMON for a trade.\p"
                                                    "All you need to do is register a\n"
                                                    "POKéMON for a trade.\p"
                                                    "Another TRAINER may offer a party\n"
                                                    "POKéMON in return for the trade.\p"
                                                    "We hope you will register POKéMON\n"
                                                    "and trade them with many, many\l"
                                                    "other TRAINERS.\p"
                                                    "Would you like to register one of\n"
                                                    "your POKéMON?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ThankYouForRegistering[] = _(" こÂÉ+ÌÁじば+ ç ÑÂªËが\n"
    "É+û;Â ÏまÏŒ\p"
    "ごûùÂ ÀûがÑÂ\n"
    "ござÁまÏŒ!\p");
#else
ALIGNED(4) static const u8 sText_ThankYouForRegistering[] = _("こうかんけいじばん の とうろくが\nかんりょう しました\pごりよう ありがとう\nございました！\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_NobodyHasRegistered[] = _("   ÌÁじば+à だºó ポケモン&\n"
    "ÑÂªË ÏÛÁまÓ+\p"
    "\n");
#else
ALIGNED(4) static const u8 sText_NobodyHasRegistered[] = _("けいじばんに だれも ポケモンを\nとうろく していません\p\n");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ChooseRequestedMonType[] = _("Scegli il tipo del POKéMON che\n"
    "cerchi.\n"
    "\n");
#else
ALIGNED(4) const u8 gText_UR_ChooseRequestedMonType[] = _("Please choose the type of POKéMON\nthat you would like in the trade.\n");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_WhichMonWillYouOffer[] = _("Quale POKéMON della tua squadra\n"
    "vuoi offrire per uno scambio?\p");
#else
ALIGNED(4) const u8 gText_UR_WhichMonWillYouOffer[] = _("Which of your party POKéMON will\nyou offer in trade?\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_RegistrationCanceled[] = _("La registrazione è stato annullata.\p");
#else
ALIGNED(4) const u8 gText_UR_RegistrationCanceled[] = _("Registration has been canceled.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_RegistraionCompleted[] = _("La registrazione è stata\n"
    "completata.\p");
#else
ALIGNED(4) const u8 gText_UR_RegistraionCompleted[] = _("Registration has been completed.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_TradeCanceled[] = _("Lo scambio è stato annullato.\p");
#else
ALIGNED(4) const u8 gText_UR_TradeCanceled[] = _("The trade has been canceled.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_CancelRegistrationOfMon[] = _("Annulli la registrazione di\n"
    "{STR_VAR_1} del L. {STR_VAR_2}?");
#else
ALIGNED(4) const u8 gText_UR_CancelRegistrationOfMon[] = _("Cancel the registration of your\nLv. {STR_VAR_2} {STR_VAR_1}?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_CancelRegistrationOfEgg[] = _("Annulli la registrazione\n"
    "dell’UOVO?");
#else
ALIGNED(4) const u8 gText_UR_CancelRegistrationOfEgg[] = _("Cancel the registration of your\nEGG?");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_RegistrationCanceled2[] = _("La registrazione è stata annullata.\p");
#else
ALIGNED(4) const u8 gText_UR_RegistrationCanceled2[] = _("The registration has been canceled.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_TradeTrainersWillBeListed[] = _("   こÂÉ+& ÊぼÂÏÛÁñéÑ&\n"
    "é;ÂじÏまÒ");
#else
ALIGNED(4) static const u8 sText_TradeTrainersWillBeListed[] = _("こうかんを きぼうしているひとを\nひょうじします");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_ChooseTrainerToTradeWith2[] = _("   こÂÉ+ ÏŒÁ トレ-ナ-&\n"
    "Çú+で ËだÎÁ");
#else
ALIGNED(4) static const u8 sText_ChooseTrainerToTradeWith2[] = _("こうかん したい トレーナーを\nえらんで ください");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AskTrainerToMakeTrade[] = _("Vuoi chiedere a {STR_VAR_1} di fare\n"
    "uno scambio?");
#else
ALIGNED(4) const u8 gText_UR_AskTrainerToMakeTrade[] = _("Would you like to ask {STR_VAR_1} to\nmake a trade?");
#endif
ALIGNED(4) static const u8 sText_AwaitingResponseFromTrainer2[] = _("……\nあいての へんじを まっています");
ALIGNED(4) static const u8 sText_NotRegisteredAMonForTrade[] = _("あなたが こうかんにだす\nポケモンが とうろくされていません\p");
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_DontHaveTypeTrainerWants[] = _("{STR_VAR_1} cerca un POKéMON di tipo\n"
    "{STR_VAR_2}, che tu non hai.\p");
#else
ALIGNED(4) const u8 gText_UR_DontHaveTypeTrainerWants[] = _("You don't have a {STR_VAR_2}-type\nPOKéMON that {STR_VAR_1} wants.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_DontHaveEggTrainerWants[] = _("{STR_VAR_1} cerca un UOVO, che tu non\n"
    "hai.\p");
#else
ALIGNED(4) const u8 gText_UR_DontHaveEggTrainerWants[] = _("You don't have an EGG that\n{STR_VAR_1} wants.\p");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_PlayerCantTradeForYourMon[] = _("{STR_VAR_1} non può fare uno scambio\n"
    "per il tuo POKéMON al momento.\p");
#else
ALIGNED(4) static const u8 sText_PlayerCantTradeForYourMon[] = _("{STR_VAR_1} can't make a trade for\nyour POKéMON right now.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_CantTradeForPartnersMon[] = _("Non puoi fare uno scambio per\n"
    "il POKéMON di {STR_VAR_1} al momento.\p");
#else
ALIGNED(4) static const u8 sText_CantTradeForPartnersMon[] = _("You can't make a trade for\n{STR_VAR_1}'s POKéMON right now.\p");
#endif

// Unused
static const u8 *const sCantTradeMonTexts[] = {
    sText_PlayerCantTradeForYourMon,
    sText_CantTradeForPartnersMon
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_TradeOfferRejected[] = _("La tua offerta di scambio è stata\n"
    "rifiutata.\p");
#else
ALIGNED(4) const u8 gText_UR_TradeOfferRejected[] = _("Your trade offer was rejected.\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_EggTrade[] = _("SCAMBIO UOVA");
#else
ALIGNED(4) const u8 gText_UR_EggTrade[] = _("EGG TRADE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ChooseJoinCancel[] = _("{DPAD_UPDOWN}SCEGLI  {A_BUTTON}PARTECIPA  {B_BUTTON}ANNULLA");
#else
ALIGNED(4) const u8 gText_UR_ChooseJoinCancel[] = _("{DPAD_UPDOWN}CHOOSE  {A_BUTTON}JOIN  {B_BUTTON}CANCEL");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ChooseTrainer[] = _("Scegli un ALLENATORE.");
#else
ALIGNED(4) const u8 gText_UR_ChooseTrainer[] = _("Please choose a TRAINER.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseTrainerSingleBattle[] = _("Scegli un ALLENATORE per\n"
    "una LOTTA in SINGOLO.");
#else
ALIGNED(4) static const u8 sText_ChooseTrainerSingleBattle[] = _("Please choose a TRAINER for\na SINGLE BATTLE.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseTrainerDoubleBattle[] = _("Scegli un ALLENATORE per\n"
    "una LOTTA in DOPPIO.");
#else
ALIGNED(4) static const u8 sText_ChooseTrainerDoubleBattle[] = _("Please choose a TRAINER for\na DOUBLE BATTLE.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseLeaderMultiBattle[] = _("Scegli un ALLENATORE per\n"
    "una LOTTA MULTIPLA.");
#else
ALIGNED(4) static const u8 sText_ChooseLeaderMultiBattle[] = _("Please choose the LEADER\nfor a MULTI BATTLE.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseTrainerToTradeWith[] = _("Scegli un ALLENATORE per\n"
    "fare scambi.");
#else
ALIGNED(4) static const u8 sText_ChooseTrainerToTradeWith[] = _("Please choose the TRAINER to\ntrade with.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderCards[] = _("Scegli un ALLENATORE che\n"
    "condivide SCHEDE SEGRETE.");
#else
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderCards[] = _("Please choose the TRAINER who is\nsharing WONDER CARDS.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderNews[] = _("Scegli un ALLENATORE che\n"
    "condivide le NOTIZIE SEGRETE.");
#else
ALIGNED(4) static const u8 sText_ChooseTrainerToShareWonderNews[] = _("Please choose the TRAINER who is\nsharing WONDER NEWS.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseLeaderPokemonJump[] = _("Salta con piccoli POKéMON!\n"
    "Scegli il CAPOGRUPPO.");
#else
ALIGNED(4) static const u8 sText_ChooseLeaderPokemonJump[] = _("Jump with mini POKéMON!\nPlease choose the LEADER.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseLeaderBerryCrush[] = _("MACINABACCHE!\n"
    "Scegli il CAPOGRUPPO.");
#else
ALIGNED(4) static const u8 sText_ChooseLeaderBerryCrush[] = _("BERRY CRUSH!\nPlease choose the LEADER.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ChooseLeaderBerryPicking[] = _("DODRIO PIGLIABACCHE!\n"
    "Scegli il CAPOGRUPPO.");
#else
ALIGNED(4) static const u8 sText_ChooseLeaderBerryPicking[] = _("DODRIO BERRY-PICKING!\nPlease choose the LEADER.");
#endif

const u8 *const gTexts_UR_ChooseTrainer[] = {
    [LINK_GROUP_SINGLE_BATTLE] = sText_ChooseTrainerSingleBattle,
    [LINK_GROUP_DOUBLE_BATTLE] = sText_ChooseTrainerDoubleBattle,
    [LINK_GROUP_MULTI_BATTLE]  = sText_ChooseLeaderMultiBattle,
    [LINK_GROUP_TRADE]         = sText_ChooseTrainerToTradeWith,
    [LINK_GROUP_POKEMON_JUMP]  = sText_ChooseLeaderPokemonJump,
    [LINK_GROUP_BERRY_CRUSH]   = sText_ChooseLeaderBerryCrush,
    [LINK_GROUP_BERRY_PICKING] = sText_ChooseLeaderBerryPicking,
    [LINK_GROUP_WONDER_CARD]   = sText_ChooseTrainerToShareWonderCards,
    [LINK_GROUP_WONDER_NEWS]   = sText_ChooseTrainerToShareWonderNews
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_SearchingForWirelessSystemWait[] = _("Ricerca di un SISTEMA\n"
    "COMUNICAZIONE WIRELESS. Attendi…");
#else
ALIGNED(4) const u8 gText_UR_SearchingForWirelessSystemWait[] = _("Searching for a WIRELESS\nCOMMUNICATION SYSTEM. Wait...");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_MustHaveTwoMonsForDoubleBattle[] = _(" ダブルバトルでè 2éÊ Áじ;Âç\n"
    "ポケモンが éÚùÂでÒ\p");
#else
ALIGNED(4) static const u8 sText_MustHaveTwoMonsForDoubleBattle[] = _("ダブルバトルでは 2ひき いじょうの\nポケモンが ひつようです\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AwaitingPlayersResponse[] = _("In attesa di risposta da {STR_VAR_1}.");
#else
ALIGNED(4) const u8 gText_UR_AwaitingPlayersResponse[] = _("Awaiting {STR_VAR_1}'s response…");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PlayerHasBeenAskedToRegisterYouPleaseWait[] = _("{STR_VAR_1} ha ricevuto la tua\n"
    "richiesta di partecipare. Attendi…");
#else
ALIGNED(4) const u8 gText_UR_PlayerHasBeenAskedToRegisterYouPleaseWait[] = _("{STR_VAR_1} has been asked to register\nyou as a member. Please wait.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_AwaitingResponseFromWirelessSystem[] = _("In attesa di risposta dal SISTEMA\n"
    "COMUNICAZIONE WIRELESS.");
#else
ALIGNED(4) const u8 gText_UR_AwaitingResponseFromWirelessSystem[] = _("Awaiting a response from the\nWIRELESS COMMUNICATION SYSTEM.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
static const u8 sText_PleaseWaitForOtherTrainersToGather[] = _("  ìÉç Î+ÉÏゃが ÔªÂまで\n"
    "Ï;ÂÏ;Â ÈまÙËだÎÁ");
#else
ALIGNED(4) static const u8 sText_PleaseWaitForOtherTrainersToGather[] = _("ほかの さんかしゃが そろうまで\nしょうしょう おまちください");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_NoCardsSharedRightNow[] = _("Non ci sono SCHEDE condivise\n"
    "al momento.");
#else
ALIGNED(4) static const u8 sText_NoCardsSharedRightNow[] = _("No CARDS appear to be shared \nright now.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_NoNewsSharedRightNow[] = _("Non ci sono NOTIZIE condivise\n"
    "al momento.");
#else
ALIGNED(4) static const u8 sText_NoNewsSharedRightNow[] = _("No NEWS appears to be shared\nright now.");
#endif

const u8 *const gTexts_UR_NoWonderShared[] = {
    sText_NoCardsSharedRightNow,
    sText_NoNewsSharedRightNow
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Battle[] = _("LOTTA");
#else
ALIGNED(4) const u8 gText_UR_Battle[] = _("BATTLE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Chat2[] = _("CHAT");
#else
ALIGNED(4) const u8 gText_UR_Chat2[] = _("CHAT");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Greetings[] = _("SALUTI");
#else
ALIGNED(4) const u8 gText_UR_Greetings[] = _("GREETINGS");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Exit[] = _("ESCI");
#else
ALIGNED(4) const u8 gText_UR_Exit[] = _("EXIT");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Exit2[] = _("ESCI");
#else
ALIGNED(4) const u8 gText_UR_Exit2[] = _("EXIT");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Info[] = _("INFO");
#else
ALIGNED(4) const u8 gText_UR_Info[] = _("INFO");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_NameWantedOfferLv[] = _("NOME{CLEAR_TO 60}CERCO{CLEAR_TO 110}OFFRO{CLEAR_TO 198}L.");
#else
ALIGNED(4) const u8 gText_UR_NameWantedOfferLv[] = _("NAME{CLEAR_TO 0x3C}WANTED{CLEAR_TO 0x6E}OFFER{CLEAR_TO 0xC6}LV.");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_SingleBattle[] = _("LOTTA in SINGOLO");
#else
ALIGNED(4) const u8 gText_UR_SingleBattle[] = _("SINGLE BATTLE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_DoubleBattle[] = _("LOTTA in DOPPIO");
#else
ALIGNED(4) const u8 gText_UR_DoubleBattle[] = _("DOUBLE BATTLE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_MultiBattle[] = _("LOTTA MULTIPLA");
#else
ALIGNED(4) const u8 gText_UR_MultiBattle[] = _("MULTI BATTLE");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PokemonTrades[] = _("SCAMBI POKéMON");
#else
ALIGNED(4) const u8 gText_UR_PokemonTrades[] = _("POKéMON TRADES");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Chat[] = _("CHAT");
#else
ALIGNED(4) const u8 gText_UR_Chat[] = _("CHAT");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Cards[] = _("SCHEDE");
#else
ALIGNED(4) const u8 gText_UR_Cards[] = _("CARDS");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_WonderCards[] = _("SCHEDE SEGRETE");
#else
ALIGNED(4) const u8 gText_UR_WonderCards[] = _("WONDER CARDS");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_WonderNews[] = _("NOTIZIE SEGRETE");
#else
ALIGNED(4) const u8 gText_UR_WonderNews[] = _("WONDER NEWS");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_PokemonJump[] = _("POKéSALTI");
#else
ALIGNED(4) const u8 gText_UR_PokemonJump[] = _("POKéMON JUMP");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_BerryCrush[] = _("MACINABACCHE");
#else
ALIGNED(4) const u8 gText_UR_BerryCrush[] = _("BERRY CRUSH");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_BerryPicking[] = _("PIGLIABACCHE");
#else
ALIGNED(4) const u8 gText_UR_BerryPicking[] = _("BERRY-PICKING");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_Search[] = _("RICERCA");
#else
ALIGNED(4) const u8 gText_UR_Search[] = _("SEARCH");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_SpinTrade[] = _("ぐñぐñこÂÉ+");
#else
ALIGNED(4) const u8 gText_UR_SpinTrade[] = _("ぐるぐるこうかん");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_ItemTrade[] = _("¿¡テムトレ-ド");
#else
ALIGNED(4) const u8 gText_UR_ItemTrade[] = _("アイテムトレード");
#endif

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ItsNormalCard[] = _("È una SCHEDA NORMALE!");
#else
ALIGNED(4) static const u8 sText_ItsNormalCard[] = _("It's a NORMAL CARD.");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ItsBronzeCard[] = _("È una SCHEDA di BRONZO!");
#else
ALIGNED(4) static const u8 sText_ItsBronzeCard[] = _("It's a BRONZE CARD!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ItsCopperCard[] = _("È una SCHEDA di RAME!");
#else
ALIGNED(4) static const u8 sText_ItsCopperCard[] = _("It's a COPPER CARD!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ItsSilverCard[] = _("È una SCHEDA d’ARGENTO!");
#else
ALIGNED(4) static const u8 sText_ItsSilverCard[] = _("It's a SILVER CARD!");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_ItsGoldCard[] = _("È una SCHEDA d’ORO!");
#else
ALIGNED(4) static const u8 sText_ItsGoldCard[] = _("It's a GOLD CARD!");
#endif

const u8 *const gTexts_UR_CardColor[] = {
    sText_ItsNormalCard,
    sText_ItsBronzeCard,
    sText_ItsCopperCard,
    sText_ItsSilverCard,
    sText_ItsGoldCard
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_TrainerCardInfoPage1[] = _("Questa è la SCHEDA\n"
    "ALLENATORE di\l"
    "{DYNAMIC 0x01}, {DYNAMIC 0x00}.\p"
    "{DYNAMIC 0x02}\p"
    "POKéDEX: {DYNAMIC 0x03}\n"
    "TEMPO: {DYNAMIC 0x04}:{DYNAMIC 0x05}\p");
#else
ALIGNED(4) const u8 gText_UR_TrainerCardInfoPage1[] = _("This is {DYNAMIC 0} {DYNAMIC 1}'s\nTRAINER CARD…\l{DYNAMIC 2}\pPOKéDEX: {DYNAMIC 3}\nTIME:    {DYNAMIC 4}:{DYNAMIC 5}\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_TrainerCardInfoPage2[] = _("LOTTE: {DYNAMIC 0x00} VINTE  {DYNAMIC 0x02} PERSE\n"
    "SCAMBI: {DYNAMIC 0x03} VOLTE\p"
    "“{DYNAMIC 0x04} {DYNAMIC 0x05}\n"
    "{DYNAMIC 0x06} {DYNAMIC 0x07}”\p");
#else
ALIGNED(4) const u8 gText_UR_TrainerCardInfoPage2[] = _("BATTLES: {DYNAMIC 0} WINS  {DYNAMIC 2} LOSSES\nTRADES:  {DYNAMIC 3} TIMES\p“{DYNAMIC 4} {DYNAMIC 5}\n{DYNAMIC 6} {DYNAMIC 7}”\p");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_GladToMeetYouMale[] = _("{DYNAMIC 0x01}: Piacere di conoscerti!{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_GladToMeetYouMale[] = _("{DYNAMIC 1}: Glad to have met you!{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_GladToMeetYouFemale[] = _("{DYNAMIC 0x01}: Piacere!{PAUSE 60}");
#else
ALIGNED(4) static const u8 sText_GladToMeetYouFemale[] = _("{DYNAMIC 1}: Glad to meet you!{PAUSE 60}");
#endif

const u8 *const gTexts_UR_GladToMeetYou[GENDER_COUNT] = {
    sText_GladToMeetYouMale,
    sText_GladToMeetYouFemale
};

#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) const u8 gText_UR_FinishedCheckingPlayersTrainerCard[] = _("Completata la lettura della SCHEDA\n"
    "ALLENATORE di {DYNAMIC 0x01}.{PAUSE 60}");
#else
ALIGNED(4) const u8 gText_UR_FinishedCheckingPlayersTrainerCard[] = _("Finished checking {DYNAMIC 1}'s\nTRAINER CARD.{PAUSE 60}");
#endif
#if GAME_LANGUAGE == LANGUAGE_ITALIAN
ALIGNED(4) static const u8 sText_CanceledReadingCard[] = _("Annullata la lettura della Scheda.");
#else
ALIGNED(4) static const u8 sText_CanceledReadingCard[] = _("Canceled reading the Card.");
#endif

static const struct MysteryGiftClientCmd sClientScript_DynamicError[] = {
    {CLI_RECV, MG_LINKID_DYNAMIC_MSG},
    {CLI_COPY_MSG},
    {CLI_SEND_READY_END},
    {CLI_RETURN, CLI_MSG_BUFFER_FAILURE}
};

const struct MysteryGiftServerCmd gServerScript_ClientCanceledCard[] = {
    {SVR_LOAD_CLIENT_SCRIPT, PTR_ARG(sClientScript_DynamicError)},
    {SVR_SEND},
    {SVR_LOAD_MSG, PTR_ARG(sText_CanceledReadingCard)},
    {SVR_SEND},
    {SVR_RECV, MG_LINKID_READY_END},
    {SVR_RETURN, SVR_MSG_CLIENT_CANCELED}
};
