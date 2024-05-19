; Bazooka, Song part, encoded in the AKM (minimalist) format V0.

	org 16384
Bazooka_Start
Bazooka_StartDisarkGenerateExternalLabel

Bazooka_DisarkPointerRegionStart0
	dw Bazooka_InstrumentIndexes	; Index table for the Instruments.
Bazooka_DisarkForceNonReferenceDuring2_1
	dw 0	; Index table for the Arpeggios.
Bazooka_DisarkForceNonReferenceDuring2_2
	dw 0	; Index table for the Pitches.

; The subsongs references.
	dw Bazooka_Subsong0
Bazooka_DisarkPointerRegionEnd0

; The Instrument indexes.
Bazooka_InstrumentIndexes
Bazooka_DisarkPointerRegionStart3
	dw Bazooka_Instrument0
	dw Bazooka_Instrument1
	dw Bazooka_Instrument2
Bazooka_DisarkPointerRegionEnd3

; The Instrument.
Bazooka_DisarkByteRegionStart4
Bazooka_Instrument0
	db 255	; Speed.

Bazooka_Instrument0Loop	db 0	; Volume: 0.

	db 4	; End the instrument.
Bazooka_DisarkPointerRegionStart5
	dw Bazooka_Instrument0Loop	; Loops.
Bazooka_DisarkPointerRegionEnd5

Bazooka_Instrument1
	db 2	; Speed.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 240	; Volume: 14.
	db 1	; Noise.

	db 232	; Volume: 13.
	db 1	; Noise.

	db 224	; Volume: 12.
	db 1	; Noise.

	db 216	; Volume: 11.
	db 1	; Noise.

	db 200	; Volume: 9.
	db 1	; Noise.

	db 192	; Volume: 8.
	db 1	; Noise.

	db 184	; Volume: 7.
	db 1	; Noise.

	db 176	; Volume: 6.
	db 1	; Noise.

	db 168	; Volume: 5.
	db 1	; Noise.

	db 160	; Volume: 4.
	db 1	; Noise.

	db 152	; Volume: 3.
	db 1	; Noise.

	db 144	; Volume: 2.
	db 1	; Noise.

	db 136	; Volume: 1.
	db 1	; Noise.

	db 4	; End the instrument.
Bazooka_DisarkPointerRegionStart6
	dw Bazooka_Instrument0Loop	; Loop to silence.
Bazooka_DisarkPointerRegionEnd6

Bazooka_Instrument2
	db 0	; Speed.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 249	; Volume: 14.
	db 1	; Arpeggio: 0.
	db 2	; Noise: 2.
	dw -10	; Pitch: -10.

	db 241	; Volume: 12.
	db 1	; Arpeggio: 0.
	db 3	; Noise: 3.
	dw -30	; Pitch: -30.

	db 216	; Volume: 11.
	db 1	; Noise.

	db 80	; Volume: 10.

	db 208	; Volume: 10.
	db 1	; Noise.

	db 200	; Volume: 9.
	db 1	; Noise.

	db 192	; Volume: 8.
	db 1	; Noise.

	db 184	; Volume: 7.
	db 1	; Noise.

	db 176	; Volume: 6.
	db 1	; Noise.

	db 168	; Volume: 5.
	db 1	; Noise.

	db 160	; Volume: 4.
	db 1	; Noise.

	db 144	; Volume: 2.
	db 1	; Noise.

	db 136	; Volume: 1.
	db 1	; Noise.

	db 4	; End the instrument.
Bazooka_DisarkPointerRegionStart7
	dw Bazooka_Instrument0Loop	; Loop to silence.
Bazooka_DisarkPointerRegionEnd7

Bazooka_DisarkByteRegionEnd4
Bazooka_ArpeggioIndexes
Bazooka_DisarkPointerRegionStart8
Bazooka_DisarkPointerRegionEnd8

Bazooka_DisarkByteRegionStart9
Bazooka_DisarkByteRegionEnd9

Bazooka_PitchIndexes
Bazooka_DisarkPointerRegionStart10
Bazooka_DisarkPointerRegionEnd10

Bazooka_DisarkByteRegionStart11
Bazooka_DisarkByteRegionEnd11

; Bazooka, Subsong 0.
; ----------------------------------

Bazooka_Subsong0
Bazooka_Subsong0DisarkPointerRegionStart0
	dw Bazooka_Subsong0_NoteIndexes	; Index table for the notes.
	dw Bazooka_Subsong0_TrackIndexes	; Index table for the Tracks.
Bazooka_Subsong0DisarkPointerRegionEnd0

Bazooka_Subsong0DisarkByteRegionStart1
	db 6	; Initial speed.

	db 1	; Most used instrument.
	db 2	; Second most used instrument.

	db 0	; Most used wait.
	db 0	; Second most used wait.

	db 24	; Default start note in tracks.
	db 0	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 13	; Are there effects? 12 if yes, 13 if not. Don't ask.
Bazooka_Subsong0DisarkByteRegionEnd1

; The Linker.
Bazooka_Subsong0DisarkByteRegionStart2
; Pattern 0
Bazooka_Subsong0_Loop
	db 170	; State byte.
	db 4	; New height.
	db ((Bazooka_Subsong0_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Bazooka_Subsong0_Track0 - ($ + 1)) & 255)
	db ((Bazooka_Subsong0_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Bazooka_Subsong0_Track1 - ($ + 1)) & 255)
	db ((Bazooka_Subsong0_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Bazooka_Subsong0_Track1 - ($ + 1)) & 255)

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Bazooka_Subsong0DisarkByteRegionEnd2
Bazooka_Subsong0DisarkPointerRegionStart3
	dw Bazooka_Subsong0_Loop

Bazooka_Subsong0DisarkPointerRegionEnd3
; The indexes of the tracks.
Bazooka_Subsong0_TrackIndexes
Bazooka_Subsong0DisarkPointerRegionStart4
Bazooka_Subsong0DisarkPointerRegionEnd4

Bazooka_Subsong0DisarkByteRegionStart5
Bazooka_Subsong0_Track0
	db 223	; Primary instrument (1). Same escaped note: 24. New wait (127).
	db 127	;   Escape wait value.

Bazooka_Subsong0_Track1
	db 239	; Secondary instrument (2). Same escaped note: 24. New wait (127).
	db 127	;   Escape wait value.

Bazooka_Subsong0DisarkByteRegionEnd5
; The note indexes.
Bazooka_Subsong0_NoteIndexes
Bazooka_Subsong0DisarkByteRegionStart6
Bazooka_Subsong0DisarkByteRegionEnd6

