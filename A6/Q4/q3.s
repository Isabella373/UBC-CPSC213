.pos 0x1000


average:
    ld    $n, r0      # r0 = &n
    ld    (r0), r0    # r0 = n
    ld    $s, r1      # r1 = &s
    ld    (r1), r1    # r1 = s
    ld    $24, r2     # r2 = 24
L0: beq   r0, sort    # goto sort if n == 0
    ld    4(r1), r3   # r3 = s->grade[0]
    ld    8(r1), r4   # r4 = s->grade[1]
    add   r4, r3      # r3 = s->grade[0] + s- grade[1]
    ld    12(r1), r4  # r4 = s ->grade[2]
    add   r4, r3      # r3 = s ->grade[0] + s- grade[1] + s ->grade[2]
    ld    16(r1), r4  # r4 = s ->grade[3]
    add   r3, r4      # r4 = s ->grade[0] + s- grade[1] + s ->grade[2] + s ->grade[3]
    shr   $2, r4      # r4 = (s ->grade[0] + s- grade[1] + s ->grade[2] + s ->grade[3]) / 4
    st    r4, 20(r1)  # s ->average = r4
    add   r2, r1      # s ++
    dec   r0          # n --
    br    L0          # goto L0

sort:
    ld    $n, r0      # r0 = &n
    ld    (r0), r0    # r0 = n = n 
    ld    $s, r7      # r7 = &s
    ld    (r7), r7    # r7 = s    
    dec   r0          # n --
L1: beq   r0, L6      # goto L6 if n  == 0
    ld    $1, r1      # r1 = j = 1   
L2: mov   r0, r3      # r3 = n 
    not   r3
    inc   r3          # r3 = -n 
    add   r1, r3      # r3 = j  - n 
    bgt   r3, L5      # goto L5 if j  ==  n
    mov   r1, r2      # r2 = j 
    mov   r1, r3      # r3 = j 
    shl   $4, r2      # r2 = j  * 16
    shl   $3, r3      # r3 = j  * 8
    add   r3, r2      # r2 = j  * 24
    add   r7, r2      # r2 = &s[j]
    mov   r2, r3      # r3 = &s[j]
    ld    $-24, r4    # r3 = -24
    add   r4, r2      # r2 = &s[j -1]]
    ld    20(r2), r4  # r4 = s[j-1].average
    ld    20(r3), r5  # r5 = s[j].average
    not   r4
    inc   r4          # r5 = -s[j-1].average
    add   r5, r4      # r4 = s[j].average - s[j-1].average
    beq   r4, L4      # goto L4 if s[j -1].average == s[j].average
    bgt   r4, L4      # goto L4 if s[j -1].average < s[j].average
    ld    $6, r4      # r4 = 6 = sizeof (struct Student) / 4 = k 
L3: beq   r4, L4      # goto L4 if k  == 0
    ld    (r3), r6    # r6  = *b 
    ld    (r2), r5    # r5  = *a 
    st    r6, (r2)    # *a  = *b 
    st    r5, (r3)    # *b  = *a 
    inca  r3          # b ++
    inca  r2          # a ++
    dec   r4          # k --
    br    L3          # goto L3
L4: inc   r1          # j ++
    br    L2          # goto L2
L5: dec   r0          # n --
    br    L1          # goto L1
L6: ld    $n, r0      # r0 = &n
    ld    (r0), r0    # r0 = n
    shr   $1, r0      # r0 = n/2
    mov   r0, r1      # r1 = n/2
    shl   $4, r0      # r0 = (n/2) * 16
    shl   $3, r1      # r1 = (n/2) * 8
    add   r1, r0      # r0 = (n/2) * 24
    ld    $s, r1      # r1 = &s
    ld    (r1), r1    # r1 = s
    add   r1, r0      # r0 = &s[n/2]
    ld    (r0), r0    # r0 = s[n/2].sid
    ld    $m, r1      # r1 = &m
    st    r0, (r1)    # m  = s[n/2].sid
    halt






.pos 0x2000
n:      .long 1       # just one student
m:      .long 0       # put the answer here
s:      .long base    # address of the array
base:   .long 1234    # student ID
        .long 80      # grade 0
        .long 60      # grade 1
        .long 78      # grade 2
        .long 90      # grade 3
        .long 0       # computed average  