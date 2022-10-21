.pos 0x100
                 ld   $0, r0              # r0 = 0 = i
                 ld   $n, r1              # r1 = &n
                 ld   (r1), r1            # r1 = n
                 not  r1                  # r1 = !n
                 inc  r1                  # r1 = -n
                 ld   $a, r2              # r2 = &a[0] = a
                 ld   $b, r3              # r3 = &b[0]
                 ld   $c, r7              # r7 = &c
                 ld   (r7), r7            # r7 = c
loop:            mov  r0, r4              # r4 = i
                 add  r1, r4              # r4 = i-n
                 beq  r4, end_loop        # if i-n == 0 goto end_loop
                 ld   (r2, r0, 4), r5     # r5 = a[i]
                 ld   (r3, r0, 4), r6     # r6 = b[i]
                 not  r6                  # r6 = !b[i]
                 inc  r6                  # r6 = -b[i]
                 add  r5, r6              # r6 = a[i] - b[i]
                 bgt  r6, then            # if a[i] - b[i] > 0, goto then
else:            br   end_if              # if else, goto end_if
then:            inc  r7                  # c++
end_if:          inc  r0                  # i++
                 br   loop                # goto loop
end_loop:        ld   $c, r1              # r1 = &c
                 st   r7, (r1)            # c = r7
                 ld   $i, r2              # r2 = &i
                 st   r0, (r2)            # i = r0
                 halt



.pos 0x2000
i:               .long -1         # i
n:               .long 5          # n
a:               .long 10         # a[0]
                 .long 20         # a[1]
                 .long 30         # a[2]
                 .long 40         # a[3]
                 .long 50         # a[4]
b:               .long 11         # b[0]
                 .long 20         # b[1]
                 .long 28         # b[2]
                 .long 44         # b[3]
                 .long 48         # b[4]
c:               .long 0          # c
