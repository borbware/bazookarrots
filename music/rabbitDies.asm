; RabbitDies, Song part, encoded in the AKM (minimalist) format V0.

	org 16384
RabbitDies_Start
RabbitDies_StartDisarkGenerateExternalLabel

RabbitDies_DisarkPointerRegionStart0
	dw RabbitDies_InstrumentIndexes	; Index table for the Instruments.
RabbitDies_DisarkForceNonReferenceDuring2_1
	dw 0	; Index table for the Arpeggios.
RabbitDies_DisarkForceNonReferenceDuring2_2
	dw 0	; Index table for the Pitches.

; The subsongs references.
	dw RabbitDies_Subsong0
RabbitDies_DisarkPointerRegionEnd0

; The Instrument indexes.
RabbitDies_InstrumentIndexes
RabbitDies_DisarkPointerRegionStart3
	dw RabbitDies_Instrument0
	dw RabbitDies_Instrument1
	dw RabbitDies_Instrument2
	dw RabbitDies_Instrument3
RabbitDies_DisarkPointerRegionEnd3

; The Instrument.
RabbitDies_DisarkByteRegionStart4
RabbitDies_Instrument0
	db 255	; Speed.

RabbitDies_Instrument0Loop	db 0	; Volume: 0.

	db 4	; End the instrument.
RabbitDies_DisarkPointerRegionStart5
	dw RabbitDies_Instrument0Loop	; Loops.
RabbitDies_DisarkPointerRegionEnd5

RabbitDies_Instrument1
	db 2	; Speed.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 4	; End the instrument.
RabbitDies_DisarkPointerRegionStart6
	dw RabbitDies_Instrument0Loop	; Loop to silence.
RabbitDies_DisarkPointerRegionEnd6

RabbitDies_Instrument2
	db 0	; Speed.

RabbitDies_Instrument2Loop	db 66
	db 4	; End the instrument.
RabbitDies_DisarkPointerRegionStart7
	dw RabbitDies_Instrument2Loop	; Loops.
RabbitDies_DisarkPointerRegionEnd7

RabbitDies_Instrument3
	db 0	; Speed.

RabbitDies_Instrument3Loop	db 74
	db 4	; End the instrument.
RabbitDies_DisarkPointerRegionStart8
	dw RabbitDies_Instrument3Loop	; Loops.
RabbitDies_DisarkPointerRegionEnd8

RabbitDies_DisarkByteRegionEnd4
RabbitDies_ArpeggioIndexes
RabbitDies_DisarkPointerRegionStart9
RabbitDies_DisarkPointerRegionEnd9

RabbitDies_DisarkByteRegionStart10
RabbitDies_DisarkByteRegionEnd10

RabbitDies_PitchIndexes
RabbitDies_DisarkPointerRegionStart11
RabbitDies_DisarkPointerRegionEnd11

RabbitDies_DisarkByteRegionStart12
RabbitDies_DisarkByteRegionEnd12

; RabbitDies, Subsong 0.
; ----------------------------------

RabbitDies_Subsong0
RabbitDies_Subsong0DisarkPointerRegionStart0
	dw RabbitDies_Subsong0_NoteIndexes	; Index table for the notes.
	dw RabbitDies_Subsong0_TrackIndexes	; Index table for the Tracks.
RabbitDies_Subsong0DisarkPointerRegionEnd0

RabbitDies_Subsong0DisarkByteRegionStart1
	db 6	; Initial speed.

	db 2	; Most used instrument.
	db 1	; Second most used instrument.

	db 0	; Most used wait.
	db 0	; Second most used wait.

	db 36	; Default start note in tracks.
	db 3	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 12	; Are there effects? 12 if yes, 13 if not. Don't ask.
RabbitDies_Subsong0DisarkByteRegionEnd1

; The Linker.
RabbitDies_Subsong0DisarkByteRegionStart2
; Pattern 0
RabbitDies_Subsong0_Loop
	db 170	; State byte.
	db 4	; New height.
	db ((RabbitDies_Subsong0_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((RabbitDies_Subsong0_Track0 - ($ + 1)) & 255)
	db ((RabbitDies_Subsong0_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((RabbitDies_Subsong0_Track1 - ($ + 1)) & 255)
	db ((RabbitDies_Subsong0_Track2 - ($ + 2)) & #ff00) / 256	; New track (2) for channel 3, as an offset. Offset MSB, then LSB.
	db ((RabbitDies_Subsong0_Track2 - ($ + 1)) & 255)

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
RabbitDies_Subsong0DisarkByteRegionEnd2
RabbitDies_Subsong0DisarkPointerRegionStart3
	dw RabbitDies_Subsong0_Loop

RabbitDies_Subsong0DisarkPointerRegionEnd3
; The indexes of the tracks.
RabbitDies_Subsong0_TrackIndexes
RabbitDies_Subsong0DisarkPointerRegionStart4
RabbitDies_Subsong0DisarkPointerRegionEnd4

RabbitDies_Subsong0DisarkByteRegionStart5
RabbitDies_Subsong0_Track0
	db 94	; Primary instrument (2). New escaped note: 84. Primary wait (0).
	db 84	;   Escape note value.
	db 221	; Effect only. New wait (127).
	db 127	;   Escape wait value.

RabbitDies_Subsong0_Track1
	db 239	; Secondary instrument (1). Same escaped note: 36. New wait (127).
	db 127	;   Escape wait value.

RabbitDies_Subsong0_Track2
	db 78	; New escaped note: 113. Primary wait (0).
	db 113	;   Escape note value.
	db 221	; Effect only. New wait (127).
	db 127	;   Escape wait value.

RabbitDies_Subsong0DisarkByteRegionEnd5
; The note indexes.
RabbitDies_Subsong0_NoteIndexes
RabbitDies_Subsong0DisarkByteRegionStart6
RabbitDies_Subsong0DisarkByteRegionEnd6

