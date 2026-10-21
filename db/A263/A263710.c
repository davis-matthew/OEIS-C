/*
 * A263710  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -1 to 1 places and every four consecutive
 *          elements having its maximum within 4 of its minimum.
 * https://oeis.org/A263710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + 2*a(n-5) - a(n-6) + a(n-7),
 *            a(1)=1, a(2)=2, a(3)=3, a(4)=5, a(5)=8, a(6)=11, a(7)=17
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 109,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is the first term; anything below offset is unused */
    if (n == 0) return 0;

    /* known values for the first seven indices */
    static const seq_t init[7] = {1, 2, 3, 5, 8, 11, 17};

    if (n <= 7) return init[n - 1];

    /* buffer holding the last seven computed values:
       buf[0] = a(i-7), ..., buf[6] = a(i-1) for the next i. */
    seq_t buf[7];
    for (idx_t i = 0; i < 7; ++i) buf[i] = init[i];

    seq_t val;
    for (idx_t i = 8; i <= n; ++i) {
        val = buf[6]                           /* a(i-1) */
            + buf[4]                           /* a(i-3) */
            - buf[3]                           /* a(i-4) */
            + (buf[2] * 2)                     /* 2*a(i-5) */
            - buf[1]                           /* a(i-6) */
            + buf[0];                          /* a(i-7) */

        /* shift buffer left: drop buf[0], move others */
        for (idx_t k = 0; k < 6; ++k) buf[k] = buf[k + 1];
        buf[6] = val;                         /* new a(i) */
    }
    return buf[6];                             /* a(n) */
}
