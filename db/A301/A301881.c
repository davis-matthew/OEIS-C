/*
 * A301881  Number of nX5 0..1 arrays with every element equal to 0, 1 or 3
 *          horizontally or antidiagonally adjacent elements, with upper left
 *          element zero.
 * https://oeis.org/A301881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 14*a(n-2) - 34*a(n-3) - 56*a(n-4) + 120*a(n-5)
 *            + 64*a(n-6) - 128*a(n-7) (n > 9) Initial values: a(1)=8,
 *            a(2)=35, a(3)=125, a(4)=510, a(5)=1961, a(6)=7869, a(7)=31173,
 *            a(8)=124795, a(9)=497909
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* first nine values */
    static const seq_t init[9] = {
        8,      /* a(1) */
        35,     /* a(2) */
        125,    /* a(3) */
        510,    /* a(4) */
        1961,   /* a(5) */
        7869,   /* a(6) */
        31173,  /* a(7) */
        124795, /* a(8) */
        497909  /* a(9) */
    };
    if (n <= 9)
        return init[n - 1];

    /* buf[0] = a(n-7), buf[6] = a(n-1) */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i)          /* init with a(3)..a(9) */
        buf[i] = init[2 + i];

    for (idx_t k = 10; k <= n; ++k) {
        seq_t pos = buf[6] * 3;                 /* 3*a(n-1) */
        pos += buf[5] * 14;                    /* +14*a(n-2) */
        pos += buf[2] * 120;                   /* +120*a(n-5) */
        pos += buf[1] * 64;                    /* +64*a(n-6) */

        seq_t new_val = pos -
                        buf[4] * 34 -       /* -34*a(n-3) */
                        buf[3] * 56 -       /* -56*a(n-4) */
                        buf[0] * 128;       /* -128*a(n-7) */

        /* shift buffer left: drop a(n-7), add new a(n) */
        for (idx_t i = 0; i < 6; ++i)
            buf[i] = buf[i + 1];
        buf[6] = new_val;
    }
    return buf[6];
}
