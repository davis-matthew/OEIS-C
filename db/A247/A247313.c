/*
 * A247313  a(n) = 5*a(n-1) - 2^n for n>0, a(0)=1.
 * https://oeis.org/A247313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2^n, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t res  = (seq_t)1;      /* a(0) */
    seq_t pow2 = (seq_t)2;      /* 2^1 */

    for (idx_t i = 1; i <= n; ++i) {
        res = res * (seq_t)5 - pow2;  /* a(i) = 5*a(i-1) - 2^i */
        pow2 = pow2 * (seq_t)2;       /* next power of two */
    }
    return res;
}
