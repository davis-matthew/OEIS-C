/*
 * A287833  Number of words of length n over the alphabet {0,1,...,10} such
 *          that no two consecutive terms have distance 2.
 * https://oeis.org/A287833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) - 2*a(n-2) - 37*a(n-3) + 16*a(n-4) + 19*a(n-5) + a(n-6)
 * Initial values:
 *   a(0) = 1
 *   a(1) = 11
 *   a(2) = 103
 *   a(3) = 967
 *   a(4) = 9079
 *   a(5) = 85243
 */
seq_t a(idx_t n)
{
    /* initial values a(0)..a(5) */
    static const seq_t init[6] = { 1, 11, 103, 967, 9079, 85243 };

    if (n < 6)
        return init[n];

    /* sliding window: v0..v5 correspond to a(n-6)..a(n-1) */
    seq_t v0 = init[0]; /* a(n-6) */
    seq_t v1 = init[1]; /* a(n-5) */
    seq_t v2 = init[2]; /* a(n-4) */
    seq_t v3 = init[3]; /* a(n-3) */
    seq_t v4 = init[4]; /* a(n-2) */
    seq_t v5 = init[5]; /* a(n-1) */
    seq_t v;            /* a(n) */

    for (idx_t i = 6; i <= n; ++i) {
        v = 10 * v5 - 2 * v4 - 37 * v3 + 16 * v2 + 19 * v1 + v0;
        /* shift the window for next iteration */
        v0 = v1;
        v1 = v2;
        v2 = v3;
        v3 = v4;
        v4 = v5;
        v5 = v;
    }

    return v;
}
