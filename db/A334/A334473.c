/*
 * A334473  The n-cowboy shootout problem: a(3^k) = 3^k, a((3^k)+b) = b if
 *          (3^k)+b <= 2*3^k, otherwise a((3^k)+b) = 2*b-3^k, where b is a
 *          positive integer.
 * https://oeis.org/A334473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is a power of 3; otherwise let m = 3^⌊log₃(n)⌋, r
 *            = n - m; then a(n) = r if r ≤ m; else a(n) = 2*r - m
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 1;
    /* find largest power of 3 <= n */
    while (m <= n / 3)
        m *= 3;

    if (n == m)
        return (seq_t)n;            /* n is a power of 3 */

    idx_t r = n - m;
    if (r <= m)
        return (seq_t)r;
    return (seq_t)((seq_t)2 * r - m);
}
