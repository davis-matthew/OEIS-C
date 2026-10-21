/*
 * A246484  Number of length 5+3 0..n arrays with no pair in any consecutive
 *          four terms totalling exactly n.
 * https://oeis.org/A246484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of length‑5+3 arrays consisting of integers 0..n with no
 *            pair of terms among any four consecutive terms whose sum is
 *            exactly n. Recurrence (OEIS A246484): a(n) = 3*a(n-1) + 3*a(n-2)
 *            - 17*a(n-3) + 3*a(n-4) + 39*a(n-5) - 25*a(n-6) - 45*a(n-7) +
 *            45*a(n-8) + 25*a(n-9) - 39*a(n-10) - 3*a(n-11)...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 17, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[15] = {
        2ULL,          /* a(1)  */
        38ULL,         /* a(2)  */
        1420ULL,       /* a(3)  */
        11020ULL,      /* a(4)  */
        92478ULL,      /* a(5)  */
        415650ULL,     /* a(6)  */
        1756472ULL,    /* a(7)  */
        5497304ULL,    /* a(8)  */
        16089370ULL,   /* a(9)  */
        39798430ULL,   /* a(10) */
        93260292ULL,   /* a(11) */
        196688868ULL,  /* a(12) */
        397729430ULL,  /* a(13) */
        748819610ULL,  /* a(14) */
        1364059888ULL /* a(15) */
    };

    if (n <= 15)
        return init[n - 1];

    /* buf holds the last 15 values: buf[0]=a(k-15), ..., buf[14]=a(k-1) */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i];

    for (idx_t k = 16; k <= n; ++k) {
        /* compute a(k) using the recurrence */
        seq_t pos = buf[14] * 3        /* a(k-1)  */
                 + buf[13] * 3        /* a(k-2)  */
                 + buf[11] * 3        /* a(k-4)  */
                 + buf[10] * 39       /* a(k-5)  */
                 + buf[7]  * 45       /* a(k-8)  */
                 + buf[6]  * 25       /* a(k-9)  */
                 + buf[3]  * 17       /* a(k-12) */
                 + buf[0];            /* a(k-15) */

        seq_t neg = buf[12] * 17       /* a(k-6)  */
                 + buf[9]  * 25       /* a(k-9)  */
                 + buf[8]  * 45       /* a(k-8)  */
                 + buf[5]  * 39       /* a(k-11) */
                 + buf[4]  * 3        /* a(k-10) */
                 + buf[2]  * 3        /* a(k-13) */
                 + buf[1]  * 3;       /* a(k-14) */

        seq_t new_val = pos - neg;     /* final value at index k */

        /* shift buf left and insert new_val */
        for (int j = 0; j < 14; ++j)
            buf[j] = buf[j + 1];
        buf[14] = new_val;
    }

    return buf[14];
}
