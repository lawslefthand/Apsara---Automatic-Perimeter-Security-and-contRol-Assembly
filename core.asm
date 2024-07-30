;Apsara V1 made by Aryan Basnet 

;Stack pointer initialization
LDI R21,HIGH(RAMEND)
OUT SPH,R21                       - Initializing stack pointers to start memory addresses from 0x00
LDI R21,LOW(RAMEND)
OUT SPL,R21

;FOR STATUS LEDS
;LDI R21,0xFF
;OUT DDRB,R21
;CALL DELAYSHORT
;SBI PORTB,0
;CALL DELAYSHORT
;SBI PORTB,3
;CALL DELAYSHORT                     - Defunct code replaced with ; (comments)
;SBI PORTB,2
;CALL DELAYSHORT 
;CBI PORTB,0
;CBI PORTB,3
;CBI PORTB,2

;Making ir sensor pin input
LDI R16,0x00
OUT DDRC,R16
LDI R16,0xFF                      -Infrared pin configuration changed for DATA in
OUT DDRD,R16

;Main body

THERE:	CALL IR_LEFT                -Subroutines being called for executing different functions
      CALL IR_RIGHT
              CALL IR_LEFT 
		CALL PWM_SUBR
		RJMP THERE

;Subroutines

;ServoRotation PWM

PWM_SUBR:
		SBI DDRB,1
		LDI R21,0b10000010
		STS TCCR1A,R21
		LDI R21,0b00011011
		STS TCCR1B,R21
		LDI R21,HIGH(4999)
		STS ICR1H,R21
		LDI R21,LOW(4999)
		STS ICR1L,R21
		LDI R21,HIGH(97)
		STS OCR1AH,R21
		LDI R21,LOW(97)               -Subroutine created for generating Pwm (pulse width          
		STS OCR1AL,R21                 modulation) for connected servo
		CALL DELAYLONG
		LDI R21,HIGH(316)
		STS OCR1AH,R21
		LDI R21,LOW(316)             -Works by engaging TCCR1A and B which are timer
		STS OCR1AL,R21                control registers. OCR1A is basically output     
		CALL DELAYLONG                compare register and ICR1 is the input capture
		LDI R21,HIGH(535)             register
		STS OCR1AH,R21
		LDI R21,LOW(535)
		STS OCR1AL,R21
		RET
		

IR_RIGHT:
    	SBIS PINC,0
		SBI PORTD,2
		CALL DELAYSHORT                  -engages right motor on ir high
		CBI PORTD,2	
		RET

IR_LEFT:
         SBIS PINC,1
	     SBI PORTD,3
	     CALL DELAYSHORT                   -engages left motor on ir low
	     CBI PORTD,3
	     RET

;Periodic monitor alert subroutine for leds

LED_SUBR:
;SBI PORTB,0
;CALL DELAYLONG
;SBI PORTB,3
;;CALL DELAYLONG
;SBI PORTB,2                ;Defunct code
;CALL DELAYLONG
;CBI PORTB,2
;CALL DELAYLONG
;CBI PORTB,3
;CALL DELAYLONG
;CBI PORTB,0
;RET

;Delay subroutines

DELAYLONG:
           LDI R22,250
D2:        LDI R23,100
D4:	    LDI R24,50
D7:	    NOP 
	    NOP                         -Subroutine for creating long time delay before execution
	    DEC R24
	    BRNE D7
           DEC R23
	    BRNE D4
	    DEC R22
	    BRNE D2
	    RET

DELAYSHORT:
	    LDI R23,100
D1:	    LDI R24,50
D3:	    NOP 
	    NOP                        -Subroutine for creating short time delay before execution
	    DEC R23
	    BRNE D3
	    DEC R22
	    BRNE D1
	    RET

DELAYEXTRALONG:
	 LDI R16,250
D22:    LDI R17,250
D21:    LDI R22,250
D24:    LDI R23,100
D25:    LDI R24,50
D30:	 NOP                        -Subroutine for creating extra long time delay before execution
	 NOP
	 DEC R24
        BRNE D30
        DEC R23
	 BRNE D25
          DEC R22
        BRNE D24
DEC R17
BRNE D21
DEC R16
BRNE D22
RET




