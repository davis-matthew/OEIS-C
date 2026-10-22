/*
 * A394666  a(n) = n! mod (2*n - 1).
 * https://oeis.org/A394666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! mod (2*n - 1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the modulus is 2*n-1; if it equals 1 the result is obviously 0 */
    seq_t mod = (seq_t)(2*n - 1);
    if (mod == (seq_t)1) return (seq_t)0;

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        res = (res * (seq_t)i) % mod;
    }
    return res;
}
