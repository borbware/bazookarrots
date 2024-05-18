// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Sprite tool sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "bios.h"
#include "memory.h"
#include "fsm.h"
#include "sprite_fx.h"

//=============================================================================
// DEFINES
//=============================================================================

// Pattern enums
#define PATTERN_16_NUM		(u8)(6*2*4)

// Functions
typedef void (*fxFunc)(const u8*, u8*);

// 8-bits unsigned vector structure
typedef struct
{
	const u8* Name;
	fxFunc    Func;
} EffectInfo;

// Function prototypes
void Init16();
void Update16();


//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_mgl_sample8.h"
#include "font/font_mgl_symbol1.h"

// Sprite by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_16or.h"

// Character animation
const u8 g_CharAnim[] = { '|', '\\', '-', '/' };
//
const u8* g_RotAnim[] = { g_PatternData, g_PatternDataRotLeft, g_PatternDataRotHalf, g_PatternDataRotRight };

// states
const FSM_State g_State16 = { 0, Init16, Update16, NULL };

//=============================================================================
// MEMORY DATA
//=============================================================================

// Screen mode setting index
u8 g_VBlank = 0;
u8 g_Frame = 0;

// Sprite data
u8 g_PatternData[PATTERN_16_NUM * 8];
u8 g_PatternDataRotRight[PATTERN_16_NUM * 8];
u8 g_PatternDataRotLeft[PATTERN_16_NUM * 8];
u8 g_PatternDataRotHalf[PATTERN_16_NUM * 8];

// Sprite buffer
u8 g_Buffer1[32];
u8 g_Buffer2[32];
u8 g_Buffer3[32];
u8 g_Buffer4[32];

// Sprite X position
u8 g_PosX0;
u8 g_PosX1;

//
u8 g_FXIndex;

//=============================================================================
// HELPER FUNCTIONS
//=============================================================================

//-----------------------------------------------------------------------------
// H_TIMI interrupt hook
void VBlankHook()
{
	g_VBlank = 1;
}

//-----------------------------------------------------------------------------
// Wait for V-Blank period
void WaitVBlank()
{
	while(g_VBlank == 0) {}
	g_VBlank = 0;
	g_Frame++;
}

//-----------------------------------------------------------------------------
void Init16()
{
	// Setup screen
	VDP_SetMode(VDP_MODE_SCREEN4);
	VDP_SetColor(COLOR_DARK_BLUE);
	VDP_ClearVRAM();
	
	// Setup sprite
	VDP_EnableSprite(TRUE);
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);

	// Load 16x16 sprites (Pattern 96~143)
	u8* ptr = g_PatternData;
	for(u8 i5 = 0; i5 < 6; i5++)
	{
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  0) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 12) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 +  1) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i5 * 2 + 13) * 8, ptr, 8); ptr += 8;
	}
	for(u8 i6 = 0; i6 < 6; i6++)
	{
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 24) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 36) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 25) * 8, ptr, 8); ptr += 8;
		Mem_Copy((u8*)g_DataSprt16or + (i6 * 2 + 37) * 8, ptr, 8); ptr += 8;
	}

	// Initialize 16x16 OR sprites
	VDP_SetPaletteEntry(2, RGB16(7, 7, 7));
	VDP_SetPaletteEntry(3, RGB16(6, 4, 1));
	VDP_SetSpriteExUniColor(0, 0, 32-1, 0, 0x02);
	VDP_SetSpriteExUniColor(1, 0, 32-1, 4, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(2, 0, 80-1, 8, 0x02);
	VDP_SetSpriteExUniColor(3, 0, 80-1, 12, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(4, 0, 128-1, 16, 0x02);
	VDP_SetSpriteExUniColor(5, 0, 128-1, 20, VDP_SPRITE_CC + 0x01);
	VDP_SetSpriteExUniColor(6, 64, (u8)(176-1), 24, 0x02);
	VDP_SetSpriteExUniColor(7, 64, (u8)(176-1), 28, VDP_SPRITE_CC + 0x01);
	VDP_HideSpriteFrom(8);
	g_PosX0 = 0;
	g_PosX1 = 0;

	// Compute transformed sprite data
	loop(i, 6 * 2)
	{
		u16 idx = i * 4 * 8;
		SpriteFX_RotateRight16(&g_PatternData[idx], &g_PatternDataRotRight[idx]);
		SpriteFX_RotateLeft16(&g_PatternData[idx], &g_PatternDataRotLeft[idx]);
		SpriteFX_RotateHalfTurn16(&g_PatternData[idx], &g_PatternDataRotHalf[idx]);
	}

	// Setup print
	Print_SetTextFont(g_Font_MGL_Sample8, 0);
	Print_SetColor(0xF, 0x4);
	VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (0*256*8), 32*4*8);
	VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (1*256*8), 32*4*8);
	VDP_FillVRAM_16K(COLOR_MERGE(COLOR_LIGHT_BLUE, COLOR_DARK_BLUE), g_ScreenColorLow + (32*4*8) + (2*256*8), 32*4*8);

	Print_DrawTextAt(0, 0, "\x2\x3\x4\x5 Sprite FX sample (16x16)");
	Print_DrawCharXAt(0, 1, '\x17', 32);

	Print_DrawTextAt(1, 4, "Crop R/L");
	Print_DrawTextAt(15, 3, "\x9F\x9F");
	Print_DrawTextAt(15, 4, "\x9F\x9F");
	Print_DrawTextAt(15, 5, "\x9F\x9F");
	Print_DrawCharXAt(0, 6, '\x9F', 32);

	Print_DrawTextAt(1, 10, "Flip H");
	Print_DrawCharXAt(0, 12, '\x9F', 32);

	Print_DrawTextAt(1, 22, "Rotate");

	Print_DrawTextAt(31-7, 22, "\x82:Flip V");
	Print_DrawTextAt(31-7, 23, "\x83:8x8");
}

//-----------------------------------------------------------------------------
void Update16()
{
	bool bToggle = Keyboard_IsKeyPressed(KEY_UP) || Keyboard_IsKeyPressed(KEY_DOWN);

	u8 frame = (g_Frame >> 2) % 6;
	u8 pat = (frame * 8 * 4);

	// Crop right/left
	g_PosX0++;
	VDP_SetSpritePositionX(0, g_PosX0);
	VDP_SetSpritePositionX(1, g_PosX0);
	u8* pat1 = g_PatternData + pat;
	u8* pat2 = g_PatternData + pat + 24 * 8;
	if((g_PosX0 > 104) && (g_PosX0 <= 120))
	{
		u8 offset = g_PosX0 - 105;
		SpriteFX_CropRight16(pat1, g_Buffer1, offset);
		SpriteFX_CropRight16(pat2, g_Buffer2, offset);
		pat1 = g_Buffer1;
		pat2 = g_Buffer2;
	}
	else if((g_PosX0 >= 120) && (g_PosX0 < 136))
	{
		u8 offset = 15 - (g_PosX0 - 120);
		SpriteFX_CropLeft16(pat1, g_Buffer1, offset);
		SpriteFX_CropLeft16(pat2, g_Buffer2, offset);
		pat1 = g_Buffer1;
		pat2 = g_Buffer2;
	}
	if(bToggle)
	{
		SpriteFX_FlipVertical16(pat1, g_Buffer3);
		SpriteFX_FlipVertical16(pat2, g_Buffer4);
		pat1 = g_Buffer3;
		pat2 = g_Buffer4;
	}
	VDP_LoadSpritePattern(pat1, 0, 4);
	VDP_LoadSpritePattern(pat2, 4, 4);

	// Flip horizontal
	g_PosX1--;
	VDP_SetSpritePositionX(2, g_PosX1);
	VDP_SetSpritePositionX(3, g_PosX1);
	pat1 = g_PatternData + pat;
	pat2 = g_PatternData + pat + 24 * 8;
	if(bToggle)
	{
		SpriteFX_Rotate180_16(pat1, g_Buffer1);
		SpriteFX_Rotate180_16(pat2, g_Buffer2);
	}
	else
	{
		SpriteFX_FlipHorizontal16(pat1, g_Buffer1);
		SpriteFX_FlipHorizontal16(pat2, g_Buffer2);
	}
	VDP_LoadSpritePattern(g_Buffer1, 8, 4);
	VDP_LoadSpritePattern(g_Buffer2, 12, 4);

	// Rotate 90°
	u8 rot = (g_Frame >> 4) % 4;
	VDP_LoadSpritePattern(g_RotAnim[rot] + pat, 24, 4);
	VDP_LoadSpritePattern(g_RotAnim[rot] + pat + (24 * 8), 28, 4);

	// if(Keyboard_IsKeyPressed(KEY_SPACE))
	// 	FSM_SetState(&g_State8);
}

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	// Setup system
	Bios_SetKeyClick(FALSE);
	Bios_SetHookCallback(H_TIMI, VBlankHook);

	FSM_SetState(&g_State16);

	bool bContinue = TRUE;
	while(bContinue)
	{
		// VDP_SetColor(COLOR_DARK_BLUE);
		WaitVBlank();
		// VDP_SetColor(COLOR_LIGHT_BLUE);

		FSM_Update();

		// Sign-of-life	
		Print_DrawCharAt(31, 0, g_CharAnim[g_Frame & 0x03]);

		if(Keyboard_IsKeyPressed(KEY_ESC))
			bContinue = FALSE;
	}

	Bios_ClearHook(H_TIMI);
	Bios_Exit(0);
}


