/*
 * A362577  Number of vertex cuts in the n-trapezohedral graph.
 * https://oeis.org/A362577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=5, a(2)=15, a(3)=88, a(4)=435, a(5)=1957, a(6)=8394,
 *            a(7)=35273, a(8)=146795 (initial values); recurrence: a(n) =
 *            13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 179*a(n-4) + 69*a(n-5) +
 *            37*a(n-6) - 38*a(n-7) + 8*a(n-8) , n>8
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial values for n=1..8 */
    const seq_t init[9] = {
        0, /* unused */
        (seq_t)5ULL,          /* a(1) */
        (seq_t)15ULL,         /* a(2) */
        (seq_t)88ULL,         /* a(3) */
        (seq_t)435ULL,        /* a(4) */
        (seq_t)1957ULL,       /* a(5) */
        (seq_t)8394ULL,       /* a(6) */
        (seq_t)35273ULL,      /* a(7) */
        (seq_t)146795ULL      /* a(8) */
    };

    if (n <= 8) {
        return init[n];
    }

    /* circular buffer to hold the last 8 values: a(i-8)...a(i-1) */
    seq_t buf[8];
    for (int i = 0; i < 8; ++i) {
        buf[i] = init[i + 1];
    }

    for (idx_t i = 9; i <= n; ++i) {
        seq_t val =
            (seq_t)13 * buf[7]    /* 13*a(n-1) */
          - (seq_t)65 * buf[6]    /* -65*a(n-2) */
          + (seq_t)156 * buf[5]  /* +156*a(n-3) */
          - (seq_t)179 * buf[4]  /* -179*a(n-4) */
          + (seq_t)69 * buf[3]   /* +69*a(n-5) */
          + (seq_t)37 * buf[2]   /* +37*a(n-6) */
          - (seq_t)38 * buf[1]   /* -38*a(n-7) */
          + (seq_t)8 * buf[0];   /* +8*a(n-8) */

        /* rotate buffer left */
        for (int j = 0; j < 7; ++j) {
            buf[j] = buf[j + 1];
        }
        buf[7] = val;
    }

    return buf[7];
}
