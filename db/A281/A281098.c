/*
 * A281098  a(n) is the GCD of the sequence d(n) = A261327(k+n) - A261327(k)
 *          for all k.
 * https://oeis.org/A281098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(4*k) = 4*k a(4*k+2) = 2*k+1 a(4*k+1) = 1 if k%3 ≠ 2 3 if k%3 =
 *            2 a(4*k+3) = 1 if k%3 ≠ 0 3 if k%3 = 0
 * Range:     u8 n <= 255, u16 n <= 9790, u32 n <= 9790, u64 n <= 9790,
 *            u128 n <= 9790, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 1ULL) == 0) {                    /* n is even */
        if ((n & 3ULL) == 0)                  /* n % 4 == 0 */
            return (seq_t)n;                  /* a(4k) = 4k */
        else                                  /* n % 4 == 2 */
            return (seq_t)(n >> 1);           /* a(4k+2) = n/2 */
    } else {                                  /* n is odd */
        if ((n & 3ULL) == 1) {                /* n % 4 == 1 */
            idx_t k = (n - 1) >> 2;            /* k = (n-1)/4 */
            return (seq_t)((k % 3ULL) == 2ULL ? 3 : 1);
        } else {                              /* n % 4 == 3 */
            idx_t k = (n - 3) >> 2;            /* k = (n-3)/4 */
            return (seq_t)((k % 3ULL) == 0ULL ? 3 : 1);
        }
    }
}
