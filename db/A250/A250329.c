/*
 * A250329  Number of length n+5 0..1 arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + 2*a(n-6) - 2*a(n-9) - a(n-10) - a(n-12)
 *            + a(n-15) Initial values for 1 ≤ n ≤ 15:
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 42, u64 n <= 93, u128 n <= 193,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=44, a(2)=68, a(3)=108, a(4)=172, a(5)=272, a(6)=424,
   a(7)=648, a(8)=996, a(9)=1544, a(10)=2404, a(11)=3746,
   a(12)=5830, a(13)=9048, a(14)=14024, a(15)=21746 */

seq_t a(idx_t n)
{
    /* Store the first fifteen terms in a static array.
       Index 0 is unused to keep the indices 1‑based. */
    static const seq_t init[16] = {
        0ULL,  /* a(0) – unused */
        44ULL, 68ULL, 108ULL, 172ULL, 272ULL,
        424ULL, 648ULL, 996ULL, 1544ULL, 2404ULL,
        3746ULL, 5830ULL, 9048ULL, 14024ULL, 21746ULL
    };

    if (n == 0)
        return (seq_t)0;           /* offset 1 – undefined below 1 */
    if (n <= 15)
        return init[n];

    /* Use a fixed‑size ring buffer to hold the last 15 values
       needed to compute the next one.  buf[0] holds a(n-15),
       buf[14] holds a(n-1).  Initially fill it with a(1)…a(15). */
    seq_t buf[15];
    for (idx_t i = 0; i < 15; ++i)
        buf[i] = init[i + 1];      /* a(1) → buf[0], …, a(15) → buf[14] */

    seq_t next = 0;
    for (idx_t i = 16; i <= n; ++i) {
        /* Recurrence: a(i) = a(i-1) + a(i-3) + 2*a(i-6)
                         - 2*a(i-9) - a(i-10) - a(i-12) + a(i-15) */
        next = buf[14]                 /* a(i-1) */
             + buf[12]                 /* a(i-3) */
             + (seq_t)2 * buf[9]       /* 2*a(i-6) */
             - (seq_t)2 * buf[6]       /* -2*a(i-9) */
             - buf[5]                  /* -a(i-10) */
             - buf[3]                  /* -a(i-12) */
             + buf[0];                 /* +a(i-15) */

        /* Shift the buffer left by one position to make room for the new term */
        for (idx_t k = 0; k < 14; ++k)
            buf[k] = buf[k + 1];
        buf[14] = next;
    }

    return next;
}
