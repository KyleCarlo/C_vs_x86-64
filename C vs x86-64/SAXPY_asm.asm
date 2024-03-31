	segment .text
		bits 64
		default rel
		global saxpy_asm

	saxpy_asm:
	push rbp
	mov rbp, rsp
	add rbp, 16			; return address of call and push rbp

	; rcx - n
	; xmm1 - A
	; r8 - X
	; r9 - Y
	; [rbp + 32] - Z

	mov r10, [rbp + 32]

	XOR rbx, rbx				; i
	for_loop:
	movss xmm4, [r8 + 4 * rbx]	; X[i]
	movss xmm5, [r9 + 4 * rbx]	; Y[i]

	mulss xmm4, xmm1			; xmm4 = X[i] * A
	addss xmm4, xmm5			; xmm4 = X[i] * A + Y[i]
		
	movss [r10 + 4 * rbx], xmm4	; Z[i] = xmm4

	INC rbx						; i++
	CMP rbx, rcx				; i < n
	JL for_loop

	pop rbp
	ret