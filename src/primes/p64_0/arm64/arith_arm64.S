; Format function and variable names for Mac OS X
#if defined(__APPLE__)
    #define fmt(f)    _##f
#else
    #define fmt(f)    f
#endif

.text
.align 2

; static const digit_t p[WORDS_FIELD]         = {0x1FFFFFFFFFFFFFFF}; // Field order p
; static const digit_t Mont_one[WORDS_FIELD]  = {0x0000000000000008}; // R  =  2^{NBITS_PRIME} (mod p)
; static const digit_t R2[WORDS_FIELD]        = {0x0000000000000040}; // R2 = (2^{NBITS_PRIME})^2 (mod p)
; static const digit_t iR[WORDS_FIELD]        = {0x4000000000000000}; // iR =  R^(-1) (mod p)
; static const digit_t pp[WORDS_FIELD]        = {0x2000000000000001}; // pp = -p^(-1) mod R
; static const digit_t ip[WORDS_FIELD]        = {0xdFFFFFFFFFFFFFFF}; // ip =  p^(-1) mod R    
; static const digit_t Zero[WORDS_FIELD]      = {0x0000000000000000}; // Zero = 0
; static const digit_t One[WORDS_FIELD]       = {0x0000000000000001}; // One = 1


; FIELD CONSTANTS

; Field characterstics
p64:
.quad   0x1FFFFFFFFFFFFFFF

; Montgomery one = R = 2^64 % p 
Rmp:
.quad   0x0000000000000008

; R squared mod p
R2mp:
.quad   0x0000000000000040

; Inverse of R mod p 
iRmp:
.quad   0x4000000000000000

; Inverse of -p mod R
impmR:
.quad   0x2000000000000001

; Inverse of p mod R
ipmR:
.quad   0xdFFFFFFFFFFFFFFF

; Zero ; Not actually used
Zero:
.quad   0x0000000000000000

; One
One:
.quad   0x0000000000000001



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Modular reduction
;  Input: a[x0] 1 word < R
;  Output: c[x1] 1 words < p
;  Operation: c [x1] =  a [x0] mod p
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
.global fmt(f_red_asm)
fmt(f_red_asm):
    ldr     x2, [x0]

    lsr     x5, x2, #61
    and     x2, x2, #0x1FFFFFFFFFFFFFFF

    add     x2, x2, x5

    mov     x4, #0x1FFFFFFFFFFFFFFF

    subs    x2, x2, x4

    sbc     x3, xzr, xzr
    and     x3, x3, x4

    add     x2, x2, x3

    str     x2, [x0]
    ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Field addition
;  Input: a[x0] 1 word < p; b[x1] 1 word < p
;  Output: c[x2] 1 words
;  Operation: c [x2] = a [x0] + b [x1] mod p
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
.global fmt(f_add_asm)
fmt(f_add_asm):
    ldr     x3, [x0]
    ldr     x4, [x1]

    mov     x5, #0x1FFFFFFFFFFFFFFF

    add     x3, x3, x4

    subs    x3, x3, x5

    sbc     x6, xzr, xzr
    and     x5, x5, x6
    add     x3, x3, x5

    str     x3, [x2]
    ret






;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Field negation
;  Input: a[x0] 1 word < p
;  Output: c[x1] 1 words
;  Operation: c [x1] =  -a [x0] mod p
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
.global fmt(f_neg_asm)
fmt(f_neg_asm):

    ldr     x2, [x0]
    mov     x3, #0x1FFFFFFFFFFFFFFF

    sub     x2, x3, x2
    
    subs    x2, x2, x3

    sbc     x4, xzr, xzr
    and     x4, x4, x3

    add     x2, x2, x4

    str     x2, [x1]
    ret

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Field subtraction
;  Input: a[x0] 1 word < p; b[x1] 1 word < p
;  Output: c[x2] 1 words
;  Operation: c [x2] = a [x0] - b [x1] mod p
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
.global fmt(f_sub_asm)
fmt(f_sub_asm):
    ldr     x3, [x0]
    ldr     x4, [x1]
    mov     x5, #0x1FFFFFFFFFFFFFFF

    subs    x3, x3, x4

    sbc     x6, xzr, xzr
    and     x5, x6, x5

    add     x3, x3, x5

    str     x3, [x2]
    ret



;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Multiprecision multiplication
;  Input: a[x0] 1 word; b[x1] 1 word
;  Output: c[x2] 2 words
;  Operation: c [x2] = a [x0] * b [x1]
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
.global fmt(mp_mul_asm)
fmt(mp_mul_asm):
    ldr     x3, [x0]
    ldr     x4, [x1]
    mul     x5, x3, x4
    umulh   x6, x3, x4
    stp     x5, x6, [x2]
    ret





;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Montgomery reduction
;  Input: a[x0] 2 words < p*R
;  Output: c[x1] 1 word < p
;  Operation: c[x1] = a [x0] * (R^(-1)) mod p
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
.global fmt(mont_redc_asm)
fmt(mont_redc_asm):

    ldp     x3, x4, [x0]
    
    mov     x8, #0xdFFFFFFFFFFFFFFF
    mov     x5, #0x1FFFFFFFFFFFFFFF 

    mul     x6, x3, x8
    umulh   x7, x6, x5

    subs    x4, x4, x7

    sbc     x7, xzr, xzr 
    and     x7, x7, x5

    add     x4, x4, x7
    str     x4, [x1]

    ret




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;  Field multiplication
;  Input: a[x0] 1 word < p; b[x1] 1 word < p
;  Output: c[x2] 1 word < p
;  Operation: c [x2] = a [x0] * b [x1] mod p
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; 
.global fmt(f_mul_asm)
fmt(f_mul_asm):
    ldr     x0, [x0]
    ldr     x1, [x1]

    mul     x3, x0, x1
    umulh   x4, x0, x1

    mov     x8, #0xdFFFFFFFFFFFFFFF
    mov     x5, #0x1FFFFFFFFFFFFFFF

    mul     x6, x3, x8
    umulh   x7, x6, x5

    subs    x4, x4, x7

    sbc     x7, xzr, xzr 
    and     x7, x7, x5

    add     x4, x4, x7
    str     x4, [x2]

    ret


// A = A^2 % 2p
.macro f_square     A, iP, P, \
                    T0, T1, T2, T3, T4

    mul     \T0, \A, \A
    umulh   \T1, \A, \A

    mul     \T2, \T0, \iP

    mul     \T3, \T2, \P
    umulh   \T4, \T2, \P

    adds    \T0, \T0, \T3
    adc     \A, \T1, \T4

.endm

.macro f_mul    A, B, iP, P, \
                T0, T1, T2, T3, T4

    mul     \T0, \A, \B
    umulh   \T1, \A, \B

    mul     \T2, \T0, \iP

    mul     \T3, \T2, \P
    umulh   \T4, \T2, \P

    adds    \T0, \T0, \T3
    adc     \A, \T1, \T4
.endm


.global fmt(f_leg_asm)
fmt(f_leg_asm):
    ldr     x2, [x0]

    ldr     x3, impmR
    mov     x4, #0x1FFFFFFFFFFFFFFF

    mov     x10, x2
    mov     x11, x2

    // x10 = x10^2 % (2p)  in [0, 2*p-1)
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    mov     x12, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    mov     x13, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9

    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9

    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x13, x3, x4, x5, x6, x7, x8, x9

    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x12, x3, x4, x5, x6, x7, x8, x9


    lsr     x10, x10, #4
    and     x10, x10, #0x01

    strb    w10, [x1]

    ret


.global fmt(f_inv_asm)
fmt(f_inv_asm):
    ldr     x2, [x0]

    ldr     x3, impmR
    mov     x4, #0x1FFFFFFFFFFFFFFF

    mov     x10, x2
    mov     x11, x2

    // x10 = x10^2 % (2p)  in [0, 2*p-1)
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    mov     x12, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    mov     x13, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9
    mov     x11, x10
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9

    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x11, x3, x4, x5, x6, x7, x8, x9

    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x13, x3, x4, x5, x6, x7, x8, x9

    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x12, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_square    x10, x3, x4, x5, x6, x7, x8, x9
    f_mul       x10, x2, x3, x4, x5, x6, x7, x8, x9

    subs    x10, x10, x4
    sbc     x5, xzr, xzr
    and     x6, x4, x5
    add     x10, x10, x6

    str     x10, [x1]
    ret



.global fmt(f_sqrt_asm)
fmt(f_sqrt_asm):
    ldr     x2, [x0]

    ldr     x3, impmR
    mov     x4, #0x1FFFFFFFFFFFFFFF

    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9
    f_square    x2, x3, x4, x5, x6, x7, x8, x9


    subs    x2, x2, x4
    sbc     x5, xzr, xzr
    and     x6, x4, x5
    add     x2, x2, x6

    str     x2, [x1]
    ret






