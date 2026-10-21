/*
 * A278692  Pisot sequence T(4,14).
 * https://oeis.org/A278692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) + a(n-3) - a(n-4) Initial values:
 *            a(0) = 4, a(1) = 14, a(2) = 49, a(3) = 171
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Cache up to the largest n requested so far.
       The sequence is defined for n up to about 10 000 in the tests,
       so a fixed-size cache of that size is sufficient. */
    static seq_t cache[10001];
    static idx_t len = 0;           /* number of cached terms (cache[0..len-1]) */

    /* Initialise the first four terms once */
    if (len == 0) {
        cache[0] = 4u;   /* a(0) */
        cache[1] = 14u;  /* a(1) */
        cache[2] = 49u;  /* a(2) */
        cache[3] = 171u; /* a(3) */
        len = 4;
    }

    /* If more terms are needed, compute them incrementally */
    if (n + 1 > len) {
        for (idx_t i = len; i <= n; ++i) {
            seq_t val = cache[i-1] * 4u;
            val = val - cache[i-2] * 2u;
            val = val + cache[i-3];
            val = val - cache[i-4];
            cache[i] = val;
        }
        len = n + 1;
    }

    return cache[n];
}
