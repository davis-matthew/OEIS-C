/*
 * A304762  Number of nX3 0..1 arrays with every element unequal to 2, 3, 4, 5
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Recurrence for n > 15 a(n) = 5*a(n-1) + 8*a(n-2) - a(n-3) -
 *            51*a(n-4) - 45*a(n-5) + 30*a(n-6) + 33*a(n-7) + 7*a(n-8) -
 *            13*a(n-9) - 2*a(n-10) + 5*a(n-11) + 8*a(n-12) + 2*a(n-13);
 *            Initial values (n = 1 … 15): a(1) = 0 a(2) = 5 a(3) = 16 a(4) =
 *            103 a(5) = 609 a(6) = 3680 a(7) = 22187 a(8) = 133917 a...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 1‑based index; init[0] is unused to keep indices simple */
    static const seq_t init[16] = {
        0ULL,            /* dummy 0 */
        0ULL,            /* a(1) */
        5ULL,            /* a(2) */
        16ULL,           /* a(3) */
        103ULL,          /* a(4) */
        609ULL,          /* a(5) */
        3680ULL,         /* a(6) */
        22187ULL,        /* a(7) */
        133917ULL,       /* a(8) */
        808316ULL,       /* a(9) */
        4879291ULL,      /* a(10) */
        29453651ULL,     /* a(11) */
        177797086ULL,    /* a(12) */
        1073274929ULL,   /* a(13) */
        6478847051ULL,   /* a(14) */
        39109708948ULL  /* a(15) */
    };

    if (n <= 15)
        return init[n];

    /* buffer holds a(i-13) … a(i-1) for current i */
    seq_t buf[13];
    for (idx_t j = 0; j < 13; ++j)
        buf[j] = init[j + 3];   /* j+3 goes from 3 to 15 => a(3)…a(15) */

    seq_t cur = 0;
    for (idx_t i = 16; i <= n; ++i) {
        cur = 5 * buf[12] + 8 * buf[11] - buf[10] - 51 * buf[9]
            - 45 * buf[8] + 30 * buf[7] + 33 * buf[6]
            + 7 * buf[5] - 13 * buf[4] - 2 * buf[3]
            + 5 * buf[2] + 8 * buf[1] + 2 * buf[0];

        /* shift buffer left and insert new value at the end */
        for (idx_t k = 0; k < 12; ++k)
            buf[k] = buf[k + 1];
        buf[12] = cur;
    }
    return cur;
}
