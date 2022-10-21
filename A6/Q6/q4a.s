.pos 0x0
start:
    ld   $sb, r5         # stack pointer(sp) = address of sb
    inca r5              # sp = address of word after stack
    gpc  $6, r6          # ra = pc + 6
    j    0x300           # call foo
    halt                 # halt

.pos 0x100
x:    .long 0x00001000     # int *x = array;

.pos 0x200
add:
    ld   (r5), r0        # r0 = a
    ld   4(r5), r1       # r1 = b
    ld   $0x100, r2      # r2 = 0x100 = &x
    ld   (r2), r2        # r2 = x
    ld   (r2, r1, 4), r3 # r3 = x[b]
    add  r3, r0          # r0 = a + x[b]
    st   r0, (r2, r1, 4) # x[b] = a + x[b]
    j    (r6)            # return

.pos 0x300
foo:
    ld   $-12, r0        # r0 = -12
    add  r0, r5          # sp - 12 (allocate frame on stack)
    st   r6, 8(r5)       # save ra on stack
    ld   $1, r0          # r0 = 1
    st   r0, (r5)        # local_a = 1
    ld   $2, r0          # r0 = 2
    st   r0, 4(r5)       # local_b = 2
    ld   $-8, r0         # r0 = -8
    add  r0, r5          # sp - 8
    ld   $3, r0          # r0 = 3
    st   r0, (r5)        # store value of first arg
    ld   $4, r0          # r0 = 4
    st   r0, 4(r5)       # store value of second arg
    gpc  $6, r6          # ra = pc + 6
    j    0x200           # call add(3, 4)
    ld   $8, r0          # r0 = 8
    add  r0, r5          # sp + 8
    ld   (r5), r1        # r1 = a
    ld   4(r5), r2       # r2 = b
    ld   $-8, r0         # r0 = -8
    add  r0, r5          # sp - 8
    st   r1, (r5)        # store a
    st   r2, 4(r5)       # store b
    gpc  $6, r6          # ra = pc + 6
    j    0x200           # call add(a, b)
    ld   $8, r0          # r0 = 8
    add  r0, r5          # sp + 8
    ld   8(r5), r6       # load ra from stack
    ld   $12, r0         # r0 = 12
    add  r0, r5          # sp + 12 (deallocate frame on stack)
    j    (r6)            # return

.pos 0x1000
array:
    .long 0.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
    # These are here so you can see (some of) the stack contents
    # while running the code on the simulator.
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