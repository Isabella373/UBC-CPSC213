.pos 0x100
start:
    ld   $sb, r5         # sp = address of sb
    inca r5              # sp = address of word after stack
    gpc  $6, r6          # ra = pc + 6
    j    main            # call main
    halt

f:
    deca r5              # sp - 4
    ld   $0, r0          # r0 = a = 0 
    ld   4(r5), r1       # r1 = b
    ld   $0x80000000, r2 # r2 = 0x80000000
f_loop:
    beq  r1, f_end       # if b == 0, then goto f_end
    mov  r1, r3          # r3 = b
    and  r2, r3          # r3 = b & 0x80000000
    beq  r3, f_if1       # if (b & 0x80000000) == 0, then goto f_if1
    inc  r0              # a++
f_if1:
    shl  $1, r1          # r1 = b << 1 = b*2
    br   f_loop          # goto f_loop
f_end:
    inca r5              # sp + 4
    j    (r6)            # return

main:
    deca r5              # sp - 4
    deca r5              # sp - 4
    st   r6, 4(r5)       # save ra on stack
    ld   $8, r4          # r4 = i = 8
main_loop:
    beq  r4, main_end    # if i == 0, then goto main_end
    dec  r4              # i--
    ld   $x, r0          # r0 = &x
    ld   (r0, r4, 4), r0 # r0 = x[i]
    deca r5              # sp - 4
    st   r0, (r5)        # store value of parameter = x[i]
    gpc  $6, r6          # ra = pc + 6
    j    f               # call f(x[i])
    inca r5              # sp + 4
    ld   $y, r1          # r1 = &y
    st   r0, (r1, r4, 4) # y[i] = f(x[i])
    br   main_loop       # goto main_loop
main_end:
    ld   4(r5), r6       # load ra from stack
    inca r5              # sp + 4
    inca r5              # sp + 4
    j    (r6)            # return

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long -1
    .long -2
    .long 0
    .long 184
    .long 340057058

y:
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0