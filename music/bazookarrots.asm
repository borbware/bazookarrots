; Bazookarrots, Song part, encoded in the AKM (minimalist) format V0.

	org 16384
Bazookarrots_Start
Bazookarrots_StartDisarkGenerateExternalLabel

Bazookarrots_DisarkPointerRegionStart0
	dw Bazookarrots_InstrumentIndexes	; Index table for the Instruments.
Bazookarrots_DisarkForceNonReferenceDuring2_1
	dw 0	; Index table for the Arpeggios.
Bazookarrots_DisarkForceNonReferenceDuring2_2
	dw 0	; Index table for the Pitches.

; The subsongs references.
	dw Bazookarrots_Subsong0
	dw Bazookarrots_Subsong1
Bazookarrots_DisarkPointerRegionEnd0

; The Instrument indexes.
Bazookarrots_InstrumentIndexes
Bazookarrots_DisarkPointerRegionStart3
	dw Bazookarrots_Instrument0
	dw Bazookarrots_Instrument1
	dw Bazookarrots_Instrument2
	dw Bazookarrots_Instrument3
Bazookarrots_DisarkPointerRegionEnd3

; The Instrument.
Bazookarrots_DisarkByteRegionStart4
Bazookarrots_Instrument0
	db 255	; Speed.

Bazookarrots_Instrument0Loop	db 0	; Volume: 0.

	db 4	; End the instrument.
Bazookarrots_DisarkPointerRegionStart5
	dw Bazookarrots_Instrument0Loop	; Loops.
Bazookarrots_DisarkPointerRegionEnd5

Bazookarrots_Instrument1
	db 0	; Speed.

	db 61	; Volume: 15.

	db 57	; Volume: 14.

	db 53	; Volume: 13.

	db 49	; Volume: 12.

	db 45	; Volume: 11.

	db 41	; Volume: 10.

	db 37	; Volume: 9.

	db 33	; Volume: 8.

	db 29	; Volume: 7.

	db 25	; Volume: 6.

	db 21	; Volume: 5.

	db 17	; Volume: 4.

	db 13	; Volume: 3.

	db 9	; Volume: 2.

	db 5	; Volume: 1.

	db 4	; End the instrument.
Bazookarrots_DisarkPointerRegionStart6
	dw Bazookarrots_Instrument0Loop	; Loop to silence.
Bazookarrots_DisarkPointerRegionEnd6

Bazookarrots_Instrument2
	db 0	; Speed.

	db 248	; Volume: 15.
	db 1	; Noise.

	db 121	; Volume: 14.
	dw 150	; Pitch: 150.

	db 117	; Volume: 13.
	dw 300	; Pitch: 300.

	db 113	; Volume: 12.
	dw 400	; Pitch: 400.

	db 109	; Volume: 11.
	dw 500	; Pitch: 500.

	db 105	; Volume: 10.
	dw 600	; Pitch: 600.

	db 4	; End the instrument.
Bazookarrots_DisarkPointerRegionStart7
	dw Bazookarrots_Instrument0Loop	; Loop to silence.
Bazookarrots_DisarkPointerRegionEnd7

Bazookarrots_Instrument3
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
Bazookarrots_DisarkPointerRegionStart8
	dw Bazookarrots_Instrument0Loop	; Loop to silence.
Bazookarrots_DisarkPointerRegionEnd8

Bazookarrots_DisarkByteRegionEnd4
Bazookarrots_ArpeggioIndexes
Bazookarrots_DisarkPointerRegionStart9
Bazookarrots_DisarkPointerRegionEnd9

Bazookarrots_DisarkByteRegionStart10
Bazookarrots_DisarkByteRegionEnd10

Bazookarrots_PitchIndexes
Bazookarrots_DisarkPointerRegionStart11
Bazookarrots_DisarkPointerRegionEnd11

Bazookarrots_DisarkByteRegionStart12
Bazookarrots_DisarkByteRegionEnd12

; Bazookarrots, Subsong 0.
; ----------------------------------

Bazookarrots_Subsong0
Bazookarrots_Subsong0DisarkPointerRegionStart0
	dw Bazookarrots_Subsong0_NoteIndexes	; Index table for the notes.
	dw Bazookarrots_Subsong0_TrackIndexes	; Index table for the Tracks.
Bazookarrots_Subsong0DisarkPointerRegionEnd0

Bazookarrots_Subsong0DisarkByteRegionStart1
	db 6	; Initial speed.

	db 1	; Most used instrument.
	db 2	; Second most used instrument.

	db 0	; Most used wait.
	db 1	; Second most used wait.

	db 55	; Default start note in tracks.
	db 0	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 13	; Are there effects? 12 if yes, 13 if not. Don't ask.
Bazookarrots_Subsong0DisarkByteRegionEnd1

; The Linker.
Bazookarrots_Subsong0DisarkByteRegionStart2
; Pattern 0
Bazookarrots_Subsong0_Loop
	db 170	; State byte.
	db 63	; New height.
	db ((Bazookarrots_Subsong0_Track0 - ($ + 2)) & #ff00) / 256	; New track (0) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Bazookarrots_Subsong0_Track0 - ($ + 1)) & 255)
	db ((Bazookarrots_Subsong0_Track1 - ($ + 2)) & #ff00) / 256	; New track (1) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Bazookarrots_Subsong0_Track1 - ($ + 1)) & 255)
	db ((Bazookarrots_Subsong0_Track2 - ($ + 2)) & #ff00) / 256	; New track (2) for channel 3, as an offset. Offset MSB, then LSB.
	db ((Bazookarrots_Subsong0_Track2 - ($ + 1)) & 255)

; Pattern 1
	db 40	; State byte.
	db ((Bazookarrots_Subsong0_Track3 - ($ + 2)) & #ff00) / 256	; New track (3) for channel 1, as an offset. Offset MSB, then LSB.
	db ((Bazookarrots_Subsong0_Track3 - ($ + 1)) & 255)
	db ((Bazookarrots_Subsong0_Track4 - ($ + 2)) & #ff00) / 256	; New track (4) for channel 2, as an offset. Offset MSB, then LSB.
	db ((Bazookarrots_Subsong0_Track4 - ($ + 1)) & 255)

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Bazookarrots_Subsong0DisarkByteRegionEnd2
Bazookarrots_Subsong0DisarkPointerRegionStart3
	dw Bazookarrots_Subsong0_Loop

Bazookarrots_Subsong0DisarkPointerRegionEnd3
; The indexes of the tracks.
Bazookarrots_Subsong0_TrackIndexes
Bazookarrots_Subsong0DisarkPointerRegionStart4
Bazookarrots_Subsong0DisarkPointerRegionEnd4

Bazookarrots_Subsong0DisarkByteRegionStart5
Bazookarrots_Subsong0_Track0
	db 159	; Primary instrument (1). Same escaped note: 55. Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 57. Secondary wait (1).
	db 57	;   Escape note value.
	db 149	; Primary instrument (1). Note reference (5). Secondary wait (1).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 148	; Primary instrument (1). Note reference (4). Secondary wait (1).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 88	; Primary instrument (1). Note reference (8). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 88	; Primary instrument (1). Note reference (8). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 216	; Primary instrument (1). Note reference (8). New wait (7).
	db 7	;   Escape wait value.
	db 92	; Primary instrument (1). Note reference (12). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 92	; Primary instrument (1). Note reference (12). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 92	; Primary instrument (1). Note reference (12). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 92	; Primary instrument (1). Note reference (12). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 92	; Primary instrument (1). Note reference (12). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 150	; Primary instrument (1). Note reference (6). Secondary wait (1).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 212	; Primary instrument (1). Note reference (4). New wait (127).
	db 127	;   Escape wait value.

Bazookarrots_Subsong0_Track1
	db 158	; Primary instrument (1). New escaped note: 19. Secondary wait (1).
	db 19	;   Escape note value.
	db 155	; Primary instrument (1). Note reference (11). Secondary wait (1).
	db 151	; Primary instrument (1). Note reference (7). Secondary wait (1).
	db 217	; Primary instrument (1). Note reference (9). New wait (5).
	db 5	;   Escape wait value.
	db 154	; Primary instrument (1). Note reference (10). Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 28. Secondary wait (1).
	db 28	;   Escape note value.
	db 154	; Primary instrument (1). Note reference (10). Secondary wait (1).
	db 153	; Primary instrument (1). Note reference (9). Secondary wait (1).
	db 151	; Primary instrument (1). Note reference (7). Secondary wait (1).
	db 27	; Primary instrument (1). Note reference (11). 
	db 151	; Primary instrument (1). Note reference (7). Secondary wait (1).
	db 153	; Primary instrument (1). Note reference (9). Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 19. Secondary wait (1).
	db 19	;   Escape note value.
	db 155	; Primary instrument (1). Note reference (11). Secondary wait (1).
	db 151	; Primary instrument (1). Note reference (7). Secondary wait (1).
	db 26	; Primary instrument (1). Note reference (10). 
	db 158	; Primary instrument (1). New escaped note: 28. Secondary wait (1).
	db 28	;   Escape note value.
	db 154	; Primary instrument (1). Note reference (10). Secondary wait (1).
	db 153	; Primary instrument (1). Note reference (9). Secondary wait (1).
	db 151	; Primary instrument (1). Note reference (7). Secondary wait (1).
	db 155	; Primary instrument (1). Note reference (11). Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 19. Secondary wait (1).
	db 19	;   Escape note value.
	db 159	; Primary instrument (1). Same escaped note: 19. Secondary wait (1).
	db 155	; Primary instrument (1). Note reference (11). Secondary wait (1).
	db 151	; Primary instrument (1). Note reference (7). Secondary wait (1).
	db 217	; Primary instrument (1). Note reference (9). New wait (127).
	db 127	;   Escape wait value.

Bazookarrots_Subsong0_Track2
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 240	; New instrument (3). Note reference (0). New wait (2).
	db 3	;   Escape instrument value.
	db 2	;   Escape wait value.
	db 98	; Secondary instrument (2). Note reference (2). Primary wait (0).
	db 128	; Note reference (0). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 192	; Note reference (0). New wait (3).
	db 3	;   Escape wait value.
	db 128	; Note reference (0). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 192	; Note reference (0). New wait (2).
	db 2	;   Escape wait value.
	db 98	; Secondary instrument (2). Note reference (2). Primary wait (0).
	db 128	; Note reference (0). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 0	; Note reference (0). 
	db 98	; Secondary instrument (2). Note reference (2). Primary wait (0).
	db 128	; Note reference (0). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 0	; Note reference (0). 
	db 98	; Secondary instrument (2). Note reference (2). Primary wait (0).
	db 128	; Note reference (0). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 0	; Note reference (0). 
	db 98	; Secondary instrument (2). Note reference (2). Primary wait (0).
	db 128	; Note reference (0). Secondary wait (1).
	db 162	; Secondary instrument (2). Note reference (2). Secondary wait (1).
	db 0	; Note reference (0). 
	db 98	; Secondary instrument (2). Note reference (2). Primary wait (0).
	db 128	; Note reference (0). Secondary wait (1).
	db 226	; Secondary instrument (2). Note reference (2). New wait (127).
	db 127	;   Escape wait value.

Bazookarrots_Subsong0_Track3
	db 159	; Primary instrument (1). Same escaped note: 55. Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 57. Secondary wait (1).
	db 57	;   Escape note value.
	db 149	; Primary instrument (1). Note reference (5). Secondary wait (1).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 94	; Primary instrument (1). New escaped note: 61. Primary wait (0).
	db 61	;   Escape note value.
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 95	; Primary instrument (1). Same escaped note: 61. Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 95	; Primary instrument (1). Same escaped note: 61. Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 88	; Primary instrument (1). Note reference (8). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 88	; Primary instrument (1). Note reference (8). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 152	; Primary instrument (1). Note reference (8). Secondary wait (1).
	db 152	; Primary instrument (1). Note reference (8). Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 71. Secondary wait (1).
	db 71	;   Escape note value.
	db 152	; Primary instrument (1). Note reference (8). Secondary wait (1).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 84	; Primary instrument (1). Note reference (4). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 80	; Primary instrument (1). Note reference (0). Primary wait (0).
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 94	; Primary instrument (1). New escaped note: 55. Primary wait (0).
	db 55	;   Escape note value.
	db 83	; Primary instrument (1). Note reference (3). Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 85	; Primary instrument (1). Note reference (5). Primary wait (0).
	db 95	; Primary instrument (1). Same escaped note: 55. Primary wait (0).
	db 86	; Primary instrument (1). Note reference (6). Primary wait (0).
	db 145	; Primary instrument (1). Note reference (1). Secondary wait (1).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 81	; Primary instrument (1). Note reference (1). Primary wait (0).
	db 145	; Primary instrument (1). Note reference (1). Secondary wait (1).
	db 149	; Primary instrument (1). Note reference (5). Secondary wait (1).
	db 209	; Primary instrument (1). Note reference (1). New wait (127).
	db 127	;   Escape wait value.

Bazookarrots_Subsong0_Track4
	db 158	; Primary instrument (1). New escaped note: 19. Secondary wait (1).
	db 19	;   Escape note value.
	db 155	; Primary instrument (1). Note reference (11). Secondary wait (1).
	db 151	; Primary instrument (1). Note reference (7). Secondary wait (1).
	db 217	; Primary instrument (1). Note reference (9). New wait (5).
	db 5	;   Escape wait value.
	db 154	; Primary instrument (1). Note reference (10). Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 28. Secondary wait (1).
	db 28	;   Escape note value.
	db 154	; Primary instrument (1). Note reference (10). Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 27. Secondary wait (1).
	db 27	;   Escape note value.
	db 158	; Primary instrument (1). New escaped note: 28. Secondary wait (1).
	db 28	;   Escape note value.
	db 146	; Primary instrument (1). Note reference (2). Secondary wait (1).
	db 159	; Primary instrument (1). Same escaped note: 28. Secondary wait (1).
	db 146	; Primary instrument (1). Note reference (2). Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 31. Secondary wait (1).
	db 31	;   Escape note value.
	db 158	; Primary instrument (1). New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 159	; Primary instrument (1). Same escaped note: 33. Secondary wait (1).
	db 158	; Primary instrument (1). New escaped note: 35. Secondary wait (1).
	db 35	;   Escape note value.
	db 158	; Primary instrument (1). New escaped note: 33. Secondary wait (1).
	db 33	;   Escape note value.
	db 222	; Primary instrument (1). New escaped note: 31. New wait (3).
	db 31	;   Escape note value.
	db 3	;   Escape wait value.
	db 23	; Primary instrument (1). Note reference (7). 
	db 26	; Primary instrument (1). Note reference (10). 
	db 23	; Primary instrument (1). Note reference (7). 
	db 25	; Primary instrument (1). Note reference (9). 
	db 31	; Primary instrument (1). Same escaped note: 31. 
	db 222	; Primary instrument (1). New escaped note: 36. New wait (127).
	db 36	;   Escape note value.
	db 127	;   Escape wait value.

Bazookarrots_Subsong0DisarkByteRegionEnd5
; The note indexes.
Bazookarrots_Subsong0_NoteIndexes
Bazookarrots_Subsong0DisarkByteRegionStart6
	db 65	; Note for index 0.
	db 60	; Note for index 1.
	db 29	; Note for index 2.
	db 64	; Note for index 3.
	db 67	; Note for index 4.
	db 59	; Note for index 5.
	db 62	; Note for index 6.
	db 23	; Note for index 7.
	db 69	; Note for index 8.
	db 24	; Note for index 9.
	db 26	; Note for index 10.
	db 21	; Note for index 11.
	db 53	; Note for index 12.
Bazookarrots_Subsong0DisarkByteRegionEnd6

; Bazookarrots, Subsong 1.
; ----------------------------------

Bazookarrots_Subsong1
Bazookarrots_Subsong1DisarkPointerRegionStart0
	dw Bazookarrots_Subsong1_NoteIndexes	; Index table for the notes.
	dw Bazookarrots_Subsong1_TrackIndexes	; Index table for the Tracks.
Bazookarrots_Subsong1DisarkPointerRegionEnd0

Bazookarrots_Subsong1DisarkByteRegionStart1
	db 6	; Initial speed.

	db 1	; Most used instrument.
	db 2	; Second most used instrument.

	db 0	; Most used wait.
	db 1	; Second most used wait.

	db 0	; Default start note in tracks.
	db 0	; Default start instrument in tracks.
	db 0	; Default start wait in tracks.

	db 13	; Are there effects? 12 if yes, 13 if not. Don't ask.
Bazookarrots_Subsong1DisarkByteRegionEnd1

; The Linker.
Bazookarrots_Subsong1DisarkByteRegionStart2
; Pattern 0
Bazookarrots_Subsong1_Loop
	db 170	; State byte.
	db 63	; New height.
	db 128	; New track (0) for channel 1, as a reference (index 0).
	db 128	; New track (0) for channel 2, as a reference (index 0).
	db 128	; New track (0) for channel 3, as a reference (index 0).

	db 1	; End of the Song.
	db 0	; Speed to 0, meaning "end of song".
Bazookarrots_Subsong1DisarkByteRegionEnd2
Bazookarrots_Subsong1DisarkPointerRegionStart3
	dw Bazookarrots_Subsong1_Loop

Bazookarrots_Subsong1DisarkPointerRegionEnd3
; The indexes of the tracks.
Bazookarrots_Subsong1_TrackIndexes
Bazookarrots_Subsong1DisarkPointerRegionStart4
	dw Bazookarrots_Subsong1_Track0	; Track 0, index 0.
Bazookarrots_Subsong1DisarkPointerRegionEnd4

Bazookarrots_Subsong1DisarkByteRegionStart5
Bazookarrots_Subsong1_Track0
	db 205	; New wait (127).
	db 127	;   Escape wait value.

Bazookarrots_Subsong1DisarkByteRegionEnd5
; The note indexes.
Bazookarrots_Subsong1_NoteIndexes
Bazookarrots_Subsong1DisarkByteRegionStart6
	db 65	; Note for index 0.
	db 60	; Note for index 1.
	db 29	; Note for index 2.
	db 64	; Note for index 3.
	db 67	; Note for index 4.
	db 59	; Note for index 5.
	db 62	; Note for index 6.
	db 23	; Note for index 7.
	db 69	; Note for index 8.
	db 24	; Note for index 9.
	db 26	; Note for index 10.
	db 21	; Note for index 11.
	db 53	; Note for index 12.
Bazookarrots_Subsong1DisarkByteRegionEnd6

