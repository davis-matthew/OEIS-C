/*
 * A297508  Number of 3 X n 0..1 arrays with every 1 horizontally, diagonally
 *          or antidiagonally adjacent to 1 or 2 neighboring 1s.
 * https://oeis.org/A297508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = 3*a(n-1)+a(n-2)+32*a(n-3)-15*a(n-4)-65*a(n-5)-74*a(n-6)+56*a(n-7)+24*a(n-8)-3*a(n-9)+12*a(n-10);
   initial values: a(1)=1, a(2)=29, a(3)=110, a(4)=314, a(5)=1829, a(6)=8803,
   a(7)=34757, a(8)=157673, a(9)=735144, a(10)=3208028. */

seq_t a(idx_t n)
{
    /* initial values */
    static const seq_t init[11] = {
        0,          /* unused index 0 */
        1,          /* a(1) */
        29,         /* a(2) */
        110,        /* a(3) */
        314,        /* a(4) */
        1829,       /* a(5) */
        8803,       /* a(6) */
        34757,      /* a(7) */
        157673,     /* a(8) */
        735144,     /* a(9) */
        3208028     /* a(10) */
    };

    /*  n <= 10: return cached value */
    if (n <= 10)
        return init[n];

    /*  recurrence for n > 10 */
    seq_t buf[10];
    /*  buf[0] = a(n-10), buf[1] = a(n-9), ..., buf[9] = a(n-1) for current n */
    buf[0] = init[1];
    buf[1] = init[2];
    buf[2] = init[3];
    buf[3] = init[4];
    buf[4] = init[5];
    buf[5] = init[6];
    buf[6] = init[7];
    buf[7] = init[8];
    buf[8] = init[9];
    buf[9] = init[10];

    seq_t val = 0;   /* will hold a(n) when loop ends */

    for (idx_t i = 11; i <= n; ++i) {
        /* compute a(i) using the recurrence */
        seq_t pos = 3 * buf[9] + buf[8] + 32 * buf[7] + 56 * buf[3] + 24 * buf[2] + 12 * buf[0];
        seq_t neg = 15 * buf[6] + 65 * buf[5] + 74 * buf[4] + 3 * buf[1];
        val = pos - neg;

        /* shift buffer left by one: drop oldest term, insert new term */
        for (int j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = val;
    }

    return val;
}
