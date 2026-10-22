/*
 * A396652  a(n) = (2*n-3-(-1)^n)*(6*n+1-(-1)^n)/8.
 * https://oeis.org/A396652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n - 3 - (-1)^n)*(6*n + 1 - (-1)^n)/8 Alternate form
 *            used for computation: a(2*k) = 6*k*(k-1) a(2*k+1) = 2*k*(3*k+2)
 *            for k >= 0 No memoization needed; each call is O(1)
 * Range:     u8 n <= 14, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* k = floor(n/2) */
    seq_t k = (seq_t)(n >> 1);

    if ((n & 1) == 0) {            /* n even  -> n = 2*k */
        if (k == 0)                /* a(0) = 0 */
            return 0;
        return (seq_t)6 * k * (k - (seq_t)1);
    } else {                       /* n odd   -> n = 2*k+1 */
        return (seq_t)2 * k * ((seq_t)3 * k + (seq_t)2);
    }
}
