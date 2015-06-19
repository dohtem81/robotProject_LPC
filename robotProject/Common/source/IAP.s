.text

.global IAPRead
.global jump_to_IAP

.set IAP_ENTRY, 0x7ffffff1


IAPRead:
 STMFD	SP!, {R0-R2, R14}

 MOV	R2, #0X36
 STR	R2, [R0]

 BL	jump_to_IAP

 LDMFD	SP!, {R0-R2, R14}
 MOV PC, LR

jump_to_IAP:
 LDR R12, =IAP_ENTRY

 BX R12

 .end
