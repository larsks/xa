
#define test 45

huge    =$10000

        .text

	clc
	xce
        rep #$30
        .al
        .xl
        lda huge
        ldx long
        ora long,x
        sta @short
        ldx #long
        sta short
        lda !huge

        .data

long    .word $ff
short   .word $1000

