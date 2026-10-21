/*
 * A275146  Number of 6Xn 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) -77*a(n-2) +286*a(n-3) -736*a(n-4) +1351*a(n-5)
 *            -1787*a(n-6) +1716*a(n-7) -1217*a(n-8) +652*a(n-9) -265*a(n-10)
 *            +78*a(n-11) -14*a(n-12) +a(n-13) for n>18; initial values
 *            a(1..18) are: 122, 7776, 10000, 25344, 41360, 165568, 663904,
 *            2695808, 10930304, 44604864, 182448928, 74542505...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 28, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* initial values a(1)..a(18) */
    static const seq_t init[] = {
        (seq_t)0ULL,      /* dummy 0 index */
        (seq_t)122ULL,
        (seq_t)7776ULL,
        (seq_t)10000ULL,
        (seq_t)25344ULL,
        (seq_t)41360ULL,
        (seq_t)165568ULL,
        (seq_t)663904ULL,
        (seq_t)2695808ULL,
        (seq_t)10930304ULL,
        (seq_t)44604864ULL,
        (seq_t)182448928ULL,
        (seq_t)745425056ULL,
        (seq_t)3044211024ULL,
        (seq_t)12443779056ULL,
        (seq_t)50916573248ULL,
        (seq_t)208404032016ULL,
        (seq_t)852924441248ULL,
        (seq_t)3490340288512ULL
    };

    if (n <= 18)
        return init[n];

    /* Buffer holds the last 13 computed values:
       buf[0] = a(current-1), buf[1] = a(current-2), ..., buf[12] = a(current-13) */
    seq_t buf[13];
    for (int i = 0; i < 13; ++i)
        buf[i] = init[18 - i];   /* buf[0] = a(18), buf[12] = a(6) */

    for (idx_t k = 19; k <= n; ++k) {
        seq_t pos = 0;
        pos += (seq_t)13 * buf[0];
        pos += (seq_t)286 * buf[2];
        pos += (seq_t)1351 * buf[4];
        pos += (seq_t)1716 * buf[6];
        pos += (seq_t)652 * buf[8];
        pos += (seq_t)78 * buf[10];
        pos += buf[12];            /* a(k-13) */

        seq_t neg = 0;
        neg += (seq_t)77 * buf[1];
        neg += (seq_t)736 * buf[3];
        neg += (seq_t)1787 * buf[5];
        neg += (seq_t)1217 * buf[7];
        neg += (seq_t)265 * buf[9];
        neg += (seq_t)14 * buf[11];

        seq_t val = pos;
        val -= neg;                 /* a(k) */

        /* shift buffer: new value becomes buf[0] */
        for (int i = 12; i > 0; --i)
            buf[i] = buf[i - 1];
        buf[0] = val;
    }
    return buf[0];                  /* a(n) */
}
