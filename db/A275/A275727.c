/*
 * A275727  a(0) = 0, for n >= 1, a(n) = A275736(n) + 2*a(A257684(n)).
 * https://oeis.org/A275727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Σ_{i≥1, d_i>0} 2^{i-1} where d_i are the digits of n in
 *            factorial base (n = Σ d_i·i!, 0 ≤ d_i ≤ i)
 * Range:     u8 n <= 9876, u16 n <= 9876, u32 n <= 9876, u64 n <= 9876,
 *            u128 n <= 9876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    idx_t temp = n;
    unsigned i = 1;                   /* position of factorial digit (i!); start at 1! */

    while (temp != 0u) {
        unsigned base = i + 1;        /* next factorial base */
        unsigned digit = (unsigned)(temp % base);
        if (digit != 0u)
            res |= ((seq_t)1 << (i - 1));   /* set bit for this position */
        temp /= base;
        ++i;
    }
    return res;
}
