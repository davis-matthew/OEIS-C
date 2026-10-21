/*
 * A271995  The Pnictogen sequence: a(n) = A018227(n)-3.
 * https://oeis.org/A271995
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 + 6*n^2 + 14*n - 18)/6 for n even * a(n) = (n^3 +
 *            6*n^2 + 11*n - 24)/6 for n odd * (This is equivalent to the
 *            given closed form with (-1)^n.)
 * Range:     u8 n <= 9, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only defined for n >= 2. Below offset, return 0. */
    if (n < 2)
        return 0;

    /* Use 128‑bit arithmetic to avoid overflow for intermediate values. */
    __int128 n128 = ( __int128 ) n;
    __int128 num;

    if ((n & 1) == 0) {                 /* n even */
        num = n128 * n128 * n128        /* n^3   */
             + 6 * n128 * n128            /* +6 n^2*/
             + 14 * n128                  /* +14 n */
             - 18;                        /* -18   */
    } else {                            /* n odd  */
        num = n128 * n128 * n128
             + 6 * n128 * n128
             + 11 * n128
             - 24;
    }

    num /= 6;   /* division is exact for all n >= 2 */

    return (seq_t) num;
}
