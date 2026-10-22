/*
 * A384166  a(n) = Product_{k=0..n-1} (3*n+4*k).
 * https://oeis.org/A384166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=0..n-1} (3*n + 4*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    seq_t base = (seq_t)3 * (seq_t)n;           /* 3*n */
    for (idx_t k = 0; k < n; ++k) {
        res *= base + (seq_t)4 * (seq_t)k;      /* 3*n + 4*k */
    }
    return res;
}
