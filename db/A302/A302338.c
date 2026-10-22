/*
 * A302338  a(n) = 3*n + 2^v(n) where v(n) denotes the 2-adic valuation of n.
 * https://oeis.org/A302338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n + 2^v(n) (v(n) = exponent of 2 dividing n)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n >= 1 due to offset; but guard against 0 just in case */
    if (n == 0)
        return 0;

    /* count trailing zero bits of n (2‑adic valuation) */
    unsigned int v = 0;
    uint64_t t = n;
    while ((t & 1ULL) == 0) {
        ++v;
        t >>= 1;
    }

    /* compute a(n) = 3*n + 2^v */
    seq_t result = (seq_t)n * 3 + ((seq_t)1 << v);
    return result;
}
