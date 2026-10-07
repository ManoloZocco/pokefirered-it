#include "global.h"
#include "gpu_regs.h"
#include "multiboot.h"
#include "graphics.h"
#include "main.h"
#include "sprite.h"
#include "task.h"
#include "scanline_effect.h"
#include "help_system.h"
#include "m4a.h"

#if GAME_LANGUAGE == LANGUAGE_ITALIAN

#include "malloc.h"
#include "menu.h"
#include "bg.h"
#include "window.h"
#include "text.h"

enum {
    SCENE_ENSURE_CONNECT,
    SCENE_TURN_OFF_POWER,
    SCENE_TRANSMITTING,
    SCENE_FOLLOW_INSTRUCT,
    SCENE_TRANSMIT_FAILED,
    SCENE_BEGIN,
};

enum {
    STATE_BEGIN,
    STATE_CONNECT,
    STATE_TURN_OFF_POWER,
    STATE_UNUSED,
    STATE_INIT_MULTIBOOT,
    STATE_MULTIBOOT,
    STATE_TRANSMIT,
    STATE_SUCCEEDED,
    STATE_EXIT,
    STATE_FAILED,
    STATE_RETRY,
};

struct BerryFix
{
    u8 state;
    u8 scene;
    u16 timer;
    struct MultiBootParam mbParam;
};

static struct BerryFix *sBerryFix;
extern const u8 gMultiBootProgram_BerryGlitchFix_Start[];
extern const u8 gMultiBootProgram_BerryGlitchFix_End[];

static void CB2_BerryFix(void);
static void InitBerryFixBgAndWindows(void);
static int SetBerryFixScene(int scene);
static void LoadBerryFixScene(int scene);
static void HideBerryFixBgs(void);

static const void *const sBerryFixGraphics[][3] = {
    [SCENE_ENSURE_CONNECT] = {
        gBerryFixGameboy_Gfx,
        gBerryFixGameboy_Tilemap,
        gBerryFixGameboy_Pal
    },
    [SCENE_TURN_OFF_POWER] = {
        gBerryFixGameboyLogo_Gfx,
        gBerryFixGameboyLogo_Tilemap,
        gBerryFixGameboyLogo_Pal
    },
    [SCENE_TRANSMITTING] = {
        gBerryFixGbaTransfer_Gfx,
        gBerryFixGbaTransfer_Tilemap,
        gBerryFixGbaTransfer_Pal
    },
    [SCENE_FOLLOW_INSTRUCT] = {
        gBerryFixGbaTransferHighlight_Gfx,
        gBerryFixGbaTransferHighlight_Tilemap,
        gBerryFixGbaTransferHighlight_Pal
    },
    [SCENE_TRANSMIT_FAILED] = {
        gBerryFixGbaTransferError_Gfx,
        gBerryFixGbaTransferError_Tilemap,
        gBerryFixGbaTransferError_Pal
    },
    [SCENE_BEGIN] = {
        gBerryFixWindow_Gfx,
        gBerryFixWindow_Tilemap,
        gBerryFixWindow_Pal
    },
};

static const u8 sText_AggiornamentoProgrammaBacche[] = {
    0xBB, 0xDB, 0xDB, 0xDD, 0xE3, 0xE6, 0xE2, 0xD5, 0xE1, 0xD9, 0xE2, 0xE8, 0xE3, 0x00, 0xCA, 0xE6,
    0xE3, 0xDB, 0xE6, 0xD5, 0xE1, 0xE1, 0xD5, 0x00, 0xBC, 0xD5, 0xD7, 0xD7, 0xDC, 0xD9, 0xFF
};

static const u8 sText_RubinoZaffiro[] = {
    0xCC, 0xE9, 0xD6, 0xDD, 0xE2, 0xE3, 0xBA, 0xD4, 0xD5, 0xDA, 0xDA, 0xDD, 0xE6, 0xE3, 0xFF
};

static const u8 sText_RossoFuocoVerdeFoglia[] = {
    0xCC, 0xE3, 0xE7, 0xE7, 0xE3, 0x00, 0xC0, 0xE9, 0xE3, 0xD7, 0xE3, 0xBA, 0xD0, 0xD9, 0xE6, 0xD8,
    0xD9, 0x00, 0xC0, 0xE3, 0xDB, 0xE0, 0xDD, 0xD5, 0xFF
};

static const u8 sText_Scene5[] = {
    0xC3, 0xE0, 0x00, 0xCA, 0xE6, 0xE3, 0xDB, 0xE6, 0xD5, 0xE1, 0xE1, 0xD5, 0x00, 0xBC, 0xD5, 0xD7,
    0xD7, 0xDC, 0xD9, 0x00, 0xD8, 0xD9, 0xE0, 0xE0, 0xD5, 0x00, 0xD7, 0xD5, 0xE7, 0xE7, 0xD9, 0xE8,
    0xE8, 0xD5, 0x00, 0xD8, 0xDD, 0xFE, 0xDB, 0xDD, 0xE3, 0xD7, 0xE3, 0x00, 0xCA, 0xE3, 0xDF, 0x1B,
    0xE1, 0xE3, 0xE2, 0x00, 0xCC, 0xE9, 0xD6, 0xDD, 0xE2, 0xE3, 0x00, 0xE3, 0x00, 0xD4, 0xD5, 0xDA,
    0xDA, 0xDD, 0xE6, 0xE3, 0x00, 0xEA, 0xD9, 0xE6, 0xE6, 0x16, 0xFE, 0xD5, 0xDB, 0xDB, 0xDD, 0xE3,
    0xE6, 0xE2, 0xD5, 0xE8, 0xE3, 0xAD, 0xFE, 0xFC, 0x01, 0x04, 0xFC, 0x03, 0x05, 0xCA, 0xE6, 0xD9,
    0xE1, 0xDD, 0x00, 0xDD, 0xE0, 0x00, 0xE4, 0xE9, 0xE0, 0xE7, 0xD5, 0xE2, 0xE8, 0xD9, 0x00, 0xBB,
    0xAD, 0xFF
};

static const u8 sText_Scene0[] = {
    0xC3, 0x00, 0xC1, 0xD5, 0xE1, 0xD9, 0x00, 0xBC, 0xE3, 0xED, 0x00, 0xBB, 0xD8, 0xEA, 0xD5, 0xE2,
    0xD7, 0xD9, 0x00, 0xE7, 0xE3, 0xE2, 0xE3, 0x00, 0xD7, 0xE3, 0xE0, 0xE0, 0xD9, 0xDB, 0xD5, 0xE8,
    0xDD, 0xFE, 0xD7, 0xE3, 0xE1, 0xD9, 0x00, 0xE1, 0xE3, 0xE7, 0xE8, 0xE6, 0xD5, 0xE8, 0xE3, 0x00,
    0xDD, 0xE2, 0x00, 0xDA, 0xDD, 0xDB, 0xE9, 0xE6, 0xD5, 0xAC, 0xFE, 0xFC, 0x01, 0x04, 0xFC, 0x03,
    0x05, 0xCD, 0x09, 0xF0, 0x00, 0xE4, 0xE6, 0xD9, 0xE1, 0xDD, 0x00, 0xDD, 0xE0, 0x00, 0xE4, 0xE9,
    0xE0, 0xE7, 0xD5, 0xE2, 0xE8, 0xD9, 0x00, 0xBB, 0xAD, 0xFE, 0xC8, 0xC9, 0xF0, 0x00, 0xE7, 0xE4,
    0xD9, 0xDB, 0xE2, 0xDD, 0x00, 0x5C, 0xC9, 0xC0, 0xC0, 0x5D, 0x00, 0xD9, 0x00, 0xE6, 0xDD, 0xE4,
    0xE6, 0xE3, 0xEA, 0xD5, 0xAD, 0xFF
};

static const u8 sText_Scene1[] = {
    0xBB, 0xD7, 0xD7, 0xD9, 0xE2, 0xD8, 0xDD, 0x00, 0x5C, 0xC9, 0xC8, 0x5D, 0x00, 0xE0, 0xD5, 0x00,
    0xD7, 0xE3, 0xE2, 0xE7, 0xE3, 0xE0, 0xD9, 0x00, 0xD7, 0xE3, 0xE2, 0xE8, 0xD9, 0xE2, 0xD9, 0xE2,
    0xE8, 0xD9, 0xFE, 0xCA, 0xE3, 0xDF, 0x1B, 0xE1, 0xE3, 0xE2, 0x00, 0xCC, 0xE9, 0xD6, 0xDD, 0xE2,
    0xE3, 0x00, 0xE3, 0x00, 0xD4, 0xD5, 0xDA, 0xDA, 0xDD, 0xE6, 0xE3, 0x00, 0xE1, 0xD9, 0xE2, 0xE8,
    0xE6, 0xD9, 0x00, 0xE8, 0xDD, 0xD9, 0xE2, 0xDD, 0xFE, 0xE4, 0xE6, 0xD9, 0xE1, 0xE9, 0xE8, 0xDD,
    0x00, 0xCD, 0xCE, 0xBB, 0xCC, 0xCE, 0x00, 0xD9, 0x00, 0xCD, 0xBF, 0xC6, 0xBF, 0xBD, 0xCE, 0xAD,
    0x00, 0xBB, 0xE7, 0xE7, 0xDD, 0xD7, 0xE9, 0xE6, 0xD5, 0xE8, 0xDD, 0xFE, 0xD7, 0xDC, 0xD9, 0x00,
    0xD5, 0xE4, 0xE4, 0xD5, 0xDD, 0xD5, 0x00, 0xE0, 0xB4, 0xDD, 0xE1, 0xE1, 0xD5, 0xDB, 0xDD, 0xE2,
    0xD9, 0x00, 0xE5, 0xE9, 0xDD, 0x00, 0xE1, 0xE3, 0xE7, 0xE8, 0xE6, 0xD5, 0xE8, 0xD5, 0xAD, 0xFF
};

static const u8 sText_Scene2[] = {
    0xCE, 0xE6, 0xD5, 0xE7, 0xE1, 0xDD, 0xE7, 0xE7, 0xDD, 0xE3, 0xE2, 0xD9, 0x00, 0xDD, 0xE2, 0x00,
    0xD7, 0xE3, 0xE6, 0xE7, 0xE3, 0xAD, 0x00, 0xBB, 0xE8, 0xE8, 0xD9, 0xE2, 0xD8, 0xDD, 0xB0, 0xFE,
    0xFC, 0x01, 0x04, 0xFC, 0x03, 0x05, 0xC8, 0xE3, 0xE2, 0x00, 0xE7, 0xE4, 0xD9, 0xDB, 0xE2, 0xD9,
    0xE6, 0xD9, 0x00, 0x5C, 0xC9, 0xC0, 0xC0, 0x5D, 0x00, 0xDD, 0xE0, 0x00, 0xC1, 0xD5, 0xE1, 0xD9,
    0x00, 0xBC, 0xE3, 0xED, 0xFE, 0xBB, 0xD8, 0xEA, 0xD5, 0xE2, 0xD7, 0xD9, 0x00, 0xD9, 0x00, 0xE2,
    0xE3, 0xE2, 0x00, 0xE7, 0xD7, 0xE3, 0xE0, 0xE0, 0xD9, 0xDB, 0xD5, 0xE6, 0xD9, 0x00, 0xDD, 0xE0,
    0x00, 0xD7, 0xD5, 0xEA, 0xE3, 0xFE, 0xC1, 0xD5, 0xE1, 0xD9, 0x00, 0xC6, 0xDD, 0xE2, 0xDF, 0x00,
    0xE4, 0xD9, 0xE6, 0x00, 0xC1, 0xD5, 0xE1, 0xD9, 0x00, 0xBC, 0xE3, 0xED, 0x00, 0xBB, 0xD8, 0xEA,
    0xD5, 0xE2, 0xD7, 0xD9, 0xAD, 0xFF
};

static const u8 sText_Scene3[] = {
    0xCD, 0xD9, 0xDB, 0xE9, 0xDD, 0x00, 0xE0, 0xD9, 0x00, 0xDD, 0xE7, 0xE8, 0xE6, 0xE9, 0xEE, 0xDD,
    0xE3, 0xE2, 0xDD, 0x00, 0xD5, 0x00, 0xE7, 0xD7, 0xDC, 0xD9, 0xE6, 0xE1, 0xE3, 0xFE, 0xD8, 0xDD,
    0x00, 0xCA, 0xE3, 0xDF, 0x1B, 0xE1, 0xE3, 0xE2, 0x00, 0xCC, 0xE9, 0xD6, 0xDD, 0xE2, 0xE3, 0x00,
    0xE3, 0x00, 0xD4, 0xD5, 0xDA, 0xDA, 0xDD, 0xE6, 0xE3, 0xAD, 0xFF
};

static const u8 sText_Scene4[] = {
    0xC6, 0xD5, 0x00, 0xE8, 0xE6, 0xD5, 0xE7, 0xE1, 0xDD, 0xE7, 0xE7, 0xDD, 0xE3, 0xE2, 0xD9, 0x00,
    0xE2, 0xE3, 0xE2, 0x00, 0x1A, 0x00, 0xE6, 0xDD, 0xE9, 0xE7, 0xD7, 0xDD, 0xE8, 0xD5, 0xAD, 0xFE,
    0xFC, 0x01, 0x04, 0xFC, 0x03, 0x05, 0xCC, 0xDD, 0xE4, 0xE6, 0xE3, 0xEA, 0xD5, 0xAD, 0xFF
};

static const struct BgTemplate sBgTemplates[] = {
    {
        .bg = 0,
        .charBaseIndex = 0,
        .mapBaseIndex = 30,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 0,
        .baseTile = 0
    },
    {
        .bg = 1,
        .charBaseIndex = 1,
        .mapBaseIndex = 31,
        .screenSize = 0,
        .paletteMode = 0,
        .priority = 1,
        .baseTile = 0
    }
};

static const struct WindowTemplate sWindowTemplates[] = {
    {
        .bg = 0,
        .tilemapLeft = 2,
        .tilemapTop = 4,
        .width = 26,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 1
    },
    {
        .bg = 0,
        .tilemapLeft = 1,
        .tilemapTop = 11,
        .width = 28,
        .height = 8,
        .paletteNum = 15,
        .baseBlock = 53
    },
    {
        .bg = 0,
        .tilemapLeft = 0,
        .tilemapTop = 8,
        .width = 30,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 277
    },
    {
        .bg = 0,
        .tilemapLeft = 8,
        .tilemapTop = 0,
        .width = 14,
        .height = 2,
        .paletteNum = 15,
        .baseBlock = 337
    },
    DUMMY_WIN_TEMPLATE
};

static const u16 sPalette[] = {
    0x7FFF, 0x7FFF, 0x318C, 0x675A, 0x043C, 0x3AFF, 0x0664, 0x4BD2,
    0x6546, 0x7B14, 0x7FFF, 0x318C, 0x675A, 0x0000, 0x0000, 0x0000
};

static const u8 sTextColor[] = { 0x0A, 0x0B, 0x0C, 0x00 };
static const u8 sTextColor2[] = { 0x0A, 0x0D, 0x00, 0x00 };

static const u8 *const sSceneTexts[] = {
    [SCENE_ENSURE_CONNECT] = sText_Scene0,
    [SCENE_TURN_OFF_POWER] = sText_Scene1,
    [SCENE_TRANSMITTING]   = sText_Scene2,
    [SCENE_FOLLOW_INSTRUCT] = sText_Scene3,
    [SCENE_TRANSMIT_FAILED] = sText_Scene4,
    [SCENE_BEGIN]          = sText_Scene5
};

static const void *const sBerryFixGraphics2[][3] = {
    [SCENE_ENSURE_CONNECT] = {
        gBerryFixGameboy_Gfx,
        gBerryFixGameboy_Tilemap,
        gBerryFixGameboy_Pal
    },
    [SCENE_TURN_OFF_POWER] = {
        gBerryFixGameboyLogo_Gfx,
        gBerryFixGameboyLogo_Tilemap,
        gBerryFixGameboyLogo_Pal
    },
    [SCENE_TRANSMITTING] = {
        gBerryFixGbaTransfer_Gfx,
        gBerryFixGbaTransfer_Tilemap,
        gBerryFixGbaTransfer_Pal
    },
    [SCENE_FOLLOW_INSTRUCT] = {
        gBerryFixGbaTransferHighlight_Gfx,
        gBerryFixGbaTransferHighlight_Tilemap,
        gBerryFixGbaTransferHighlight_Pal
    },
    [SCENE_TRANSMIT_FAILED] = {
        gBerryFixGbaTransferError_Gfx,
        gBerryFixGbaTransferError_Tilemap,
        gBerryFixGbaTransferError_Pal
    },
    [SCENE_BEGIN] = {
        gBerryFixWindow_Gfx,
        gBerryFixWindow_Tilemap,
        gBerryFixWindow_Pal
    },
};

static void SetScene(int scene)
{
    REG_DISPCNT = 0;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BLDCNT = 0;
    LZ77UnCompVram(sBerryFixGraphics[scene][0], (void *)BG_CHAR_ADDR(0));
    LZ77UnCompVram(sBerryFixGraphics[scene][1], (void *)BG_SCREEN_ADDR(31));
    CpuCopy16(sBerryFixGraphics[scene][2], (void *)BG_PLTT, 0x200);
    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_16COLOR | BGCNT_SCREENBASE(31) | BGCNT_TXT256x256;
    REG_DISPCNT = DISPCNT_BG0_ON;
}

void CB2_InitBerryFixProgram(void)
{
    DisableInterrupts(0xFFFF);
    EnableInterrupts(INTR_FLAG_VBLANK);
    m4aSoundVSyncOff();
    SetVBlankCallback(NULL);
    ResetSpriteData();
    ResetTasks();
    ScanlineEffect_Stop();
    HelpSystem_Disable();
    SetGpuReg(REG_OFFSET_DISPCNT, 0);
    sBerryFix = AllocZeroed(sizeof(struct BerryFix));
    sBerryFix->state = 0;
    sBerryFix->scene = 6;
    SetMainCallback2(CB2_BerryFix);
}

static void CB2_BerryFix(void)
{
    switch (sBerryFix->state)
    {
    case 0:
        InitBerryFixBgAndWindows();
        sBerryFix->state = 1;
        break;
    case 1:
        if (SetBerryFixScene(SCENE_BEGIN) == SCENE_BEGIN && JOY_NEW(A_BUTTON))
            sBerryFix->state = 2;
        break;
    case 2:
        if (SetBerryFixScene(SCENE_ENSURE_CONNECT) == SCENE_ENSURE_CONNECT && JOY_NEW(A_BUTTON))
            sBerryFix->state = 3;
        break;
    case 3:
        if (SetBerryFixScene(SCENE_TURN_OFF_POWER) == SCENE_TURN_OFF_POWER)
        {
            sBerryFix->mbParam.masterp = (void *)gMultiBootProgram_BerryGlitchFix_Start;
            sBerryFix->mbParam.server_type = MULTIBOOT_SERVER_TYPE_NORMAL;
            MultiBootInit(&sBerryFix->mbParam);
            sBerryFix->timer = 0;
            sBerryFix->state = 4;
        }
        break;
    case 4:
        MultiBootMain(&sBerryFix->mbParam);
        if (sBerryFix->mbParam.probe_count || !(sBerryFix->mbParam.response_bit & 2) || !(sBerryFix->mbParam.client_bit & 2))
        {
            sBerryFix->timer = 0;
            break;
        }
        sBerryFix->timer++;
        if (sBerryFix->timer > 180)
        {
            MultiBootStartMaster(&sBerryFix->mbParam, gMultiBootProgram_BerryGlitchFix_Start + MULTIBOOT_HEADER_SIZE, gMultiBootProgram_BerryGlitchFix_End - gMultiBootProgram_BerryGlitchFix_Start - MULTIBOOT_HEADER_SIZE, 4, 1);
            sBerryFix->state = 5;
        }
        break;
    case 5:
        if (SetBerryFixScene(SCENE_TRANSMITTING) == SCENE_TRANSMITTING)
        {
            MultiBootMain(&sBerryFix->mbParam);
            if (MultiBootCheckComplete(&sBerryFix->mbParam))
                sBerryFix->state = 6;
            else if (!(sBerryFix->mbParam.client_bit & SCENE_TRANSMITTING))
                sBerryFix->state = 7;
        }
        break;
    case 6:
        if (SetBerryFixScene(SCENE_FOLLOW_INSTRUCT) == SCENE_FOLLOW_INSTRUCT && JOY_NEW(A_BUTTON))
            DoSoftReset();
        break;
    case 7:
        if (SetBerryFixScene(SCENE_TRANSMIT_FAILED) == SCENE_TRANSMIT_FAILED && JOY_NEW(A_BUTTON))
            sBerryFix->state = 1;
        break;
    }
}

static void InitBerryFixBgAndWindows(void)
{
    int width;
    int x;

    SetGpuReg(REG_OFFSET_BG0CNT, 0);
    SetGpuReg(REG_OFFSET_BG1CNT, 0);
    SetGpuReg(REG_OFFSET_BG0HOFS, 0);
    SetGpuReg(REG_OFFSET_BG0VOFS, 0);
    SetGpuReg(REG_OFFSET_BG1HOFS, 0);
    SetGpuReg(REG_OFFSET_BG1VOFS, 0);
    SetGpuReg(REG_OFFSET_BLDY, 0);
    DmaFill32(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)PLTT, PLTT_SIZE);
    DmaFill32(3, 0, (void *)OAM, OAM_SIZE);
    ResetBgsAndClearDma3BusyFlags(0);
    InitBgsFromTemplates(0, sBgTemplates, ARRAY_COUNT(sBgTemplates));
    ChangeBgX(0, 0, BG_COORD_SET);
    ChangeBgY(0, 0, BG_COORD_SET);
    ChangeBgX(1, 0, BG_COORD_SET);
    ChangeBgY(1, 0, BG_COORD_SET);
    InitWindows(sWindowTemplates);
    DeactivateAllTextPrinters();
    DmaCopy32(3, sPalette, (void *)(BG_PLTT + 0x1E0), sizeof(sPalette));
    SetGpuReg(REG_OFFSET_DISPCNT, 0x40);
    FillWindowPixelBuffer(2, PIXEL_FILL(0));
    FillWindowPixelBuffer(3, PIXEL_FILL(0));
    FillWindowPixelBuffer(0, PIXEL_FILL(10));

    width = GetStringWidth(0, sText_RossoFuocoVerdeFoglia, 0);
    x = (120 - width) / 2;
    AddTextPrinterParameterized3(2, 0, x, 3, sTextColor2, -1, sText_RossoFuocoVerdeFoglia);

    width = GetStringWidth(0, sText_RubinoZaffiro, 0);
    x = (120 - width) / 2 + 120;
    AddTextPrinterParameterized3(2, 0, x, 3, sTextColor2, -1, sText_RubinoZaffiro);

    width = GetStringWidth(0, sText_RubinoZaffiro, 0);
    x = (112 - width) / 2;
    AddTextPrinterParameterized3(3, 0, x, 0, sTextColor2, -1, sText_RubinoZaffiro);

    width = GetStringWidth(2, sText_AggiornamentoProgrammaBacche, 0);
    x = (208 - width) / 2;
    AddTextPrinterParameterized3(0, 2, x, 2, sTextColor, -1, sText_AggiornamentoProgrammaBacche);

    CopyWindowToVram(2, 2);
    CopyWindowToVram(3, 2);
    CopyWindowToVram(0, 2);
}

static int SetBerryFixScene(int scene)
{
    if (sBerryFix->scene == scene)
        return scene;

    if (sBerryFix->scene == 6)
    {
        LoadBerryFixScene(scene);
        sBerryFix->scene = scene;
    }
    else
    {
        HideBerryFixBgs();
        sBerryFix->scene = 6;
    }

    return sBerryFix->scene;
}

static void LoadBerryFixScene(int scene)
{
    FillBgTilemapBufferRect_Palette0(0, 0, 0, 0, 32, 32);
    FillWindowPixelBuffer(1, PIXEL_FILL(10));
    AddTextPrinterParameterized3(1, 2, 0, 0, sTextColor, -1, sSceneTexts[scene]);
    PutWindowTilemap(1);
    CopyWindowToVram(1, 2);

    switch (scene)
    {
    case 0:
    case 2:
    case 3:
    case 4:
        PutWindowTilemap(2);
        break;
    case 1:
        PutWindowTilemap(3);
        break;
    case 5:
        PutWindowTilemap(0);
        break;
    }

    CopyBgTilemapBufferToVram(0);
    LZ77UnCompVram(sBerryFixGraphics[scene][0], (void *)BG_CHAR_ADDR(1));
    LZ77UnCompVram(sBerryFixGraphics[scene][1], (void *)BG_SCREEN_ADDR(31));
    CpuCopy32(sBerryFixGraphics[scene][2], (void *)BG_PLTT, 0x100);
    ShowBg(0);
    ShowBg(1);
}

static void HideBerryFixBgs(void)
{
    HideBg(0);
    HideBg(1);
}

#else // GAME_LANGUAGE != LANGUAGE_ITALIAN

enum {
    SCENE_ENSURE_CONNECT,
    SCENE_TURN_OFF_POWER,
    SCENE_TRANSMITTING,
    SCENE_FOLLOW_INSTRUCT,
    SCENE_TRANSMIT_FAILED,
    SCENE_BEGIN,
};

enum {
    STATE_BEGIN,
    STATE_CONNECT,
    STATE_TURN_OFF_POWER,
    STATE_UNUSED,
    STATE_INIT_MULTIBOOT,
    STATE_MULTIBOOT,
    STATE_TRANSMIT,
    STATE_SUCCEEDED,
    STATE_EXIT,
    STATE_FAILED,
    STATE_RETRY,
};

COMMON_DATA const void *gMultibootStart = NULL;
COMMON_DATA int gMultibootStatus = 0;
COMMON_DATA size_t gMultibootSize = 0;
COMMON_DATA struct MultiBootParam gMultibootParam = {0};

static void CB2_BerryFix(void);
static void Task_BerryFixMain(u8 taskId);

static const void *const sBerryFixGraphics[][3] = {
    [SCENE_ENSURE_CONNECT] = {
        gBerryFixGameboy_Gfx,
        gBerryFixGameboy_Tilemap,
        gBerryFixGameboy_Pal
    },
    [SCENE_TURN_OFF_POWER] = {
        gBerryFixGameboyLogo_Gfx,
        gBerryFixGameboyLogo_Tilemap,
        gBerryFixGameboyLogo_Pal
    },
    [SCENE_TRANSMITTING] = {
        gBerryFixGbaTransfer_Gfx,
        gBerryFixGbaTransfer_Tilemap,
        gBerryFixGbaTransfer_Pal
    },
    [SCENE_FOLLOW_INSTRUCT] = {
        gBerryFixGbaTransferHighlight_Gfx,
        gBerryFixGbaTransferHighlight_Tilemap,
        gBerryFixGbaTransferHighlight_Pal
    },
    [SCENE_TRANSMIT_FAILED] = {
        gBerryFixGbaTransferError_Gfx,
        gBerryFixGbaTransferError_Tilemap,
        gBerryFixGbaTransferError_Pal
    },
    [SCENE_BEGIN] = {
        gBerryFixWindow_Gfx,
        gBerryFixWindow_Tilemap,
        gBerryFixWindow_Pal
    },
};

extern const u8 gMultiBootProgram_BerryGlitchFix_Start[0x3BF4];
extern const u8 gMultiBootProgram_BerryGlitchFix_End[];

static void SetScene(int scene)
{
    REG_DISPCNT = 0;
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BLDCNT = 0;
    LZ77UnCompVram(sBerryFixGraphics[scene][0], (void *)BG_CHAR_ADDR(0));
    LZ77UnCompVram(sBerryFixGraphics[scene][1], (void *)BG_SCREEN_ADDR(31));
    CpuCopy16(sBerryFixGraphics[scene][2], (void *)BG_PLTT, 0x200);
    REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(0) | BGCNT_16COLOR | BGCNT_SCREENBASE(31) | BGCNT_TXT256x256;
    REG_DISPCNT = DISPCNT_BG0_ON;
}

#define tState data[0]
#define tTimer data[1]

void CB2_InitBerryFixProgram(void)
{
    u8 taskId;
    DisableInterrupts(0xFFFF);
    EnableInterrupts(INTR_FLAG_VBLANK);
    m4aSoundVSyncOff();
    SetVBlankCallback(NULL);
    DmaFill32(3, 0, (void *)VRAM, VRAM_SIZE);
    DmaFill32(3, 0, (void *)PLTT, PLTT_SIZE);
    ResetSpriteData();
    ResetTasks();
    ScanlineEffect_Stop();
    gHelpSystemEnabled = FALSE;
    taskId = CreateTask(Task_BerryFixMain, 0);
    gTasks[taskId].tState = STATE_BEGIN;
    SetMainCallback2(CB2_BerryFix);
}

static void CB2_BerryFix(void)
{
    RunTasks();
}

static void Task_BerryFixMain(u8 taskId)
{
    s16 * data = gTasks[taskId].data;

    switch (tState)
    {
    case STATE_BEGIN:
        SetScene(SCENE_BEGIN);
        tState = STATE_CONNECT;
        break;
    case STATE_CONNECT:
        if (JOY_NEW(A_BUTTON))
        {
            SetScene(SCENE_ENSURE_CONNECT);
            tState = STATE_TURN_OFF_POWER;
        }
        break;
    case STATE_TURN_OFF_POWER:
        if (JOY_NEW(A_BUTTON))
        {
            SetScene(SCENE_TURN_OFF_POWER);
            tState = STATE_INIT_MULTIBOOT;
        }
        break;
    case STATE_INIT_MULTIBOOT:
        gMultibootStart = gMultiBootProgram_BerryGlitchFix_Start;
        gMultibootSize = gMultiBootProgram_BerryGlitchFix_End - gMultiBootProgram_BerryGlitchFix_Start;
        gMultibootParam.masterp = (void *)gMultiBootProgram_BerryGlitchFix_Start;
        gMultibootParam.server_type = MULTIBOOT_SERVER_TYPE_NORMAL;
        MultiBootInit(&gMultibootParam);
        tTimer = 0;
        tState = STATE_MULTIBOOT;
        break;
    case STATE_MULTIBOOT:
        if (gMultibootParam.probe_count == 0 && gMultibootParam.response_bit & 0x2 && gMultibootParam.client_bit & 0x2)
        {
            if (++tTimer > 180)
            {
                SetScene(SCENE_TRANSMITTING);
                MultiBootStartMaster(&gMultibootParam, gMultibootStart + MULTIBOOT_HEADER_SIZE, gMultibootSize - MULTIBOOT_HEADER_SIZE, 4, 1);
                tTimer = 0;
                tState = STATE_TRANSMIT;
            }
            else
            {
                gMultibootStatus = MultiBootMain(&gMultibootParam);
            }
        }
        else
        {
            tTimer = 0;
            gMultibootStatus = MultiBootMain(&gMultibootParam);
        }
        break;
    case STATE_TRANSMIT:
        gMultibootStatus = MultiBootMain(&gMultibootParam);
        if (MultiBootCheckComplete(&gMultibootParam))
        {
            SetScene(SCENE_FOLLOW_INSTRUCT);
            tState = STATE_SUCCEEDED;
        }
        else if (!(gMultibootParam.client_bit & 2))
        {
            tState = STATE_FAILED;
        }
        break;
    case STATE_SUCCEEDED:
        tState = STATE_EXIT;
        break;
    case STATE_EXIT:
        if (JOY_NEW(A_BUTTON))
        {
            DestroyTask(taskId);
            DoSoftReset();
        }
        break;
    case STATE_FAILED:
        SetScene(SCENE_TRANSMIT_FAILED);
        tState = STATE_RETRY;
        break;
    case STATE_RETRY:
        if (JOY_NEW(A_BUTTON))
            tState = STATE_BEGIN;
        break;
    }
}

#endif // GAME_LANGUAGE == LANGUAGE_ITALIAN
