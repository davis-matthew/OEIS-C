/*
 * A250581  Number of (n+1)X(6+1) 0..1 arrays with nondecreasing
 *          max(x(i,j),x(i,j-1)) in the i direction and nondecreasing
 *          min(x(i,j),x(i-1,j)) in the j direction.
 * https://oeis.org/A250581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 14*a(n-2) - 11*a(n-3) + 80*a(n-4) - 93*a(n-5)
 *            - 38*a(n-6) + 174*a(n-7) - 134*a(n-8) + 54*a(n-10) - 29*a(n-11)
 *            + 5*a(n-12) for n>15 Initial values: a(1)=635, a(2)=1743,
 *            a(3)=4077, a(4)=9055, a(5)=19806, a(6)=43202, a(7)=94623,
 *            a(8)=208525, a(9)=463050, a(10)=1035676, a(11)=233267...
 * Range:     u8 none, u16 n <= 6, u32 n <= 20, u64 n <= 46, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* first 15 terms */
    static const seq_t init[15] = {
        635ULL,   /* a(1) */
        1743ULL,  /* a(2) */
        4077ULL,  /* a(3) */
        9055ULL,  /* a(4) */
        19806ULL, /* a(5) */
        43202ULL, /* a(6) */
        94623ULL, /* a(7) */
        208525ULL,/* a(8) */
        463050ULL,/* a(9) */
        1035676ULL,/* a(10) */
        2332670ULL,/* a(11) */
        5285207ULL,/* a(12) */
        12037997ULL,/* a(13) */
        27534946ULL,/* a(14) */
        63206421ULL /* a(15) */
    };

    if (n <= 15) {
        return init[n - 1];
    }

    /* ring buffer holding a(n-12)…a(n-1)   */
    seq_t buf[12];
    for (idx_t i = 0; i < 12; ++i) {
        buf[i] = init[i + 3];   /* buf[0] = a(4), buf[11] = a(15) */
    }

    seq_t next = 0;
    for (idx_t k = 16; k <= n; ++k) {
        seq_t pos = 7 * buf[11]              /* a(k-1) */
                 + 80 * buf[8]               /* a(k-4) */
                 + 174 * buf[5]              /* a(k-7) */
                 + 54 * buf[2]               /* a(k-10) */
                 + 5 * buf[0];               /* a(k-12) */

        seq_t neg = 14 * buf[10]             /* a(k-2) */
                 + 11 * buf[9]               /* a(k-3) */
                 + 93 * buf[7]               /* a(k-5) */
                 + 38 * buf[6]               /* a(k-6) */
                 + 134 * buf[4]              /* a(k-8) */
                 + 29 * buf[1];              /* a(k-11) */

        next = pos - neg;

        /* shift buffer left, insert next at end */
        for (int i = 0; i < 11; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[11] = next;
    }

    return next;
}
