.pos 0x1000
code:
      # v0 = s.x[i]
      ld $i, r0               # r0 = &i
      ld (r0), r0          # r0 = i
      ld $s, r1               # r1 = &s
      ld (r1, r0, 4), r2      # r2 = s.x[i]
      ld $v0, r3              # r3 = &v0
      st r2, (r3)          # v0 = s.x[i]


      # v1 = s.y[i]
      ld $i, r0               # r0 = &i
      ld 0x0(r0), r0          # r0 = i
      ld $s, r1               # r1 = &s
      ld 8(r1), r2            # r2 = s.y
      ld (r2, r0, 4), r3      # r3 = s.y[i]
      ld $v1, r4              # r4 = &v1
      st r3, (r4)             # v1 = s.y[i]


      # v2 = s.z->x[i]
      ld $i, r0               # r0 = &i
      ld 0x0(r0), r0          # r0 = i
      ld $s, r1               # r1 = &s
      ld 12(r1), r2           # r2 = s.z
      ld (r2, r0, 4), r3      # r3 = s.z->x[i]
      ld $v2, r4              # r4 = &v2
      st r3, (r4)             # v2 = s.z->x[i]


      # v3 = s.z->z->y[i]
      ld $i, r0               # r0 = &i
      ld 0x0(r0), r0          # r0 = i
      ld $s, r1               # r1 = &s
      ld 12(r1), r2           # r2 = s.z
      ld 12(r2), r2           # r2 = s.z->z
      ld 8(r2), r2            # r2 = s.z->z->y
      ld (r2, r0, 4), r3      # r3 = s.z->z->y[i]
      ld $v3, r4              # r4 = &v3
      st r3, (r4)             # v3 = s.z->z->y[i]


      halt





.pos 0x2000
static:
i:      .long 0   # i
v0:     .long 0   # v0
v1:     .long 0   # v1
v2:     .long 0   # v2
v3:     .long 0   # v3
s:      .long 0   # s.x[0]
        .long 0   # s.x[1]
        .long s_y   # s.y
        .long s_z   # s.z
        

.pos 0x3000
heap:
s_y:    .long 0       # s.y[0]
        .long 0       # s.y[1]
s_z:    .long 0       # s.z.x[0]
        .long 0       # s.z.x[1]
        .long s_z_y   # s.z.y
        .long s_z_z   # s.z.z
s_z_y:  .long 0       # s.z.y[0]
        .long 0       # s.z.y[1]
s_z_z:  .long 0       # s.z.x[0]
        .long 0       # s.z.x[1]
        .long s_z_z_y # s.z.y
        .long 0       # s.z.z
s_z_z_y:.long 0       # s.z.z.y[0]
        .long 0       # s.z.z.y[1]