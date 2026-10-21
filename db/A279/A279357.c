/*
 * A279357  a(n) = A005187(n) XOR A005187(n+1).
 * https://oeis.org/A279357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n - popcount(n)) XOR (2*(n+1) - popcount(n+1))
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of set bits in a 64‑bit unsigned integer. */
static inline unsigned int popcount_u64(idx_t x)
{
    unsigned int c = 0;
    while (x)
    {
        x &= x - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t t1 = ((seq_t)n << 1) - (seq_t)popcount_u64(n);
    idx_t np1 = n + 1;
    seq_t t2 = ((seq_t)np1 << 1) - (seq_t)popcount_u64(np1);
    return t1 ^ t2;
}
