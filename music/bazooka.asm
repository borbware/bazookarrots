; Bazooka, AKG format, v1.0.

; Generated by Arkos Tracker 2.

Bazooka_Start
Bazooka_StartDisarkGenerateExternalLabel

Bazooka_DisarkByteRegionStart0
	db "AT20"
Bazooka_DisarkPointerRegionStart1
	dw Bazooka_ArpeggioTable	; The address of the Arpeggio table.
	dw Bazooka_PitchTable	; The address of the Pitch table.
	dw Bazooka_InstrumentTable	; The address of the Instrument table.
	dw Bazooka_EffectBlockTable	; The address of the Effect Block table.
Bazooka_DisarkPointerRegionEnd1


; The addresses of each Subsong:
Bazooka_DisarkPointerRegionStart2
	dw Bazooka_Subsong0_Start
Bazooka_DisarkPointerRegionEnd2

; Declares all the Arpeggios.
Bazooka_ArpeggioTable
Bazooka_DisarkPointerRegionStart3
Bazooka_DisarkPointerRegionEnd3

; Declares all the Pitches.
Bazooka_PitchTable
Bazooka_DisarkPointerRegionStart4
Bazooka_DisarkPointerRegionEnd4

; Declares all the Instruments.
Bazooka_InstrumentTable
Bazooka_DisarkPointerRegionStart5
	dw Bazooka_EmptyInstrument
	dw Bazooka_Instrument1
	dw Bazooka_Instrument2
Bazooka_DisarkPointerRegionEnd5

Bazooka_EmptyInstrument
	db 0	; The speed (>0, 0 for 256).
Bazooka_EmptyInstrument_Loop	db 0	; No Soft no Hard. Volume: 0. Noise? false.

	db 6	; Loop to silence.

Bazooka_Instrument1
	db 3	; The speed (>0, 0 for 256).
	db 248	; No Soft no Hard. Volume: 15. Noise? true.
	db 1	; Noise: 1.

	db 240	; No Soft no Hard. Volume: 14. Noise? true.
	db 1	; Noise: 1.

	db 232	; No Soft no Hard. Volume: 13. Noise? true.
	db 1	; Noise: 1.

	db 224	; No Soft no Hard. Volume: 12. Noise? true.
	db 1	; Noise: 1.

	db 216	; No Soft no Hard. Volume: 11. Noise? true.
	db 1	; Noise: 1.

	db 200	; No Soft no Hard. Volume: 9. Noise? true.
	db 1	; Noise: 1.

	db 192	; No Soft no Hard. Volume: 8. Noise? true.
	db 1	; Noise: 1.

	db 184	; No Soft no Hard. Volume: 7. Noise? true.
	db 1	; Noise: 1.

	db 176	; No Soft no Hard. Volume: 6. Noise? true.
	db 1	; Noise: 1.

	db 168	; No Soft no Hard. Volume: 5. Noise? true.
	db 1	; Noise: 1.

	db 160	; No Soft no Hard. Volume: 4. Noise? true.
	db 1	; Noise: 1.

	db 152	; No Soft no Hard. Volume: 3. Noise? true.
	db 1	; Noise: 1.

	db 144	; No Soft no Hard. Volume: 2. Noise? true.
	db 1	; Noise: 1.

	db 136	; No Soft no Hard. Volume: 1. Noise? true.
	db 1	; Noise: 1.

	db 6	; Loop to silence.

Bazooka_Instrument2
	db 1	; The speed (>0, 0 for 256).
	db 248	; No Soft no Hard. Volume: 15. Noise? true.
	db 1	; Noise: 1.

	db 113	; Soft only. Volume: 14.
	db 34	; Additional data. Noise: 2. Pitch? true. Arp? false. Period? false.
	dw -10	; Pitch.

	db 97	; Soft only. Volume: 12.
	db 35	; Additional data. Noise: 3. Pitch? true. Arp? false. Period? false.
	dw -30	; Pitch.

	db 216	; No Soft no Hard. Volume: 11. Noise? true.
	db 1	; Noise: 1.

	db 80	; No Soft no Hard. Volume: 10. Noise? false.

	db 208	; No Soft no Hard. Volume: 10. Noise? true.
	db 1	; Noise: 1.

	db 200	; No Soft no Hard. Volume: 9. Noise? true.
	db 1	; Noise: 1.

	db 192	; No Soft no Hard. Volume: 8. Noise? true.
	db 1	; Noise: 1.

	db 184	; No Soft no Hard. Volume: 7. Noise? true.
	db 1	; Noise: 1.

	db 176	; No Soft no Hard. Volume: 6. Noise? true.
	db 1	; Noise: 1.

	db 168	; No Soft no Hard. Volume: 5. Noise? true.
	db 1	; Noise: 1.

	db 160	; No Soft no Hard. Volume: 4. Noise? true.
	db 1	; Noise: 1.

	db 144	; No Soft no Hard. Volume: 2. Noise? true.
	db 1	; Noise: 1.

	db 136	; No Soft no Hard. Volume: 1. Noise? true.
	db 1	; Noise: 1.

	db 6	; Loop to silence.


; The indexes of the effect blocks used by this song.
Bazooka_EffectBlockTable

Bazooka_DisarkByteRegionEnd0

; Subsong 0
; ----------------------
Bazooka_Subsong0_DisarkByteRegionStart0
Bazooka_Subsong0_Start
	db 2	; ReplayFrequency (0=12.5hz, 1=25, 2=50, 3=100, 4=150, 5=300).
	db 1	; Digichannel (0-2).
	db 1	; PSG count (>0).
	db 0	; Loop start index (>=0).
	db 0	; End index (>=0).
	db 6	; Initial speed (>=0).
	db 0	; Base note index (>=0).

Bazooka_Subsong0_Linker
Bazooka_Subsong0_DisarkPointerRegionStart1
; Position 0
Bazooka_Subsong0_Linker_Loop
	dw Bazooka_Subsong0_Track0
	dw Bazooka_Subsong0_Track1
	dw Bazooka_Subsong0_Track1
	dw Bazooka_Subsong0_LinkerBlock0

Bazooka_Subsong0_DisarkPointerRegionEnd1
	dw 0	; Loop.
Bazooka_Subsong0_DisarkWordForceReference2
	dw Bazooka_Subsong0_Linker_Loop

Bazooka_Subsong0_LinkerBlock0
	db 5	; Height.
	db 0	; Transposition 0.
	db 0	; Transposition 1.
	db 0	; Transposition 2.
Bazooka_Subsong0_DisarkWordForceReference3
	dw Bazooka_Subsong0_SpeedTrack0	; SpeedTrack address.
Bazooka_Subsong0_DisarkWordForceReference4
	dw Bazooka_Subsong0_EventTrack0	; EventTrack address.

Bazooka_Subsong0_Track0
	db 152
	db 1	; New Instrument (1).
	db 61, 127	; Waits for 128 lines.


Bazooka_Subsong0_Track1
	db 152
	db 2	; New Instrument (2).
	db 61, 127	; Waits for 128 lines.


; The speed tracks
Bazooka_Subsong0_SpeedTrack0
	db 255	; Wait for 128 lines.

; The event tracks
Bazooka_Subsong0_EventTrack0
	db 255	; Wait for 128 lines.

Bazooka_Subsong0_DisarkByteRegionEnd0