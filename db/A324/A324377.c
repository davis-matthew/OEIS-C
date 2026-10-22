/*
 * A324377  a(0) = 0; for n > 0, a(n) = A000265(A005187(n)).
 * https://oeis.org/A324377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000265(2*n - popcount(n)) where A000265(x) is the
 *            largest odd divisor of x. popcount(n) is the number of 1‑bits in
 *            the binary representation of n
 * Range:     u8 n <= 129, u16 n <= 9415, u32 n <= 9415, u64 n <= 9415,
 *            u128 n <= 9415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Portable popcount for 64‑bit unsigned integers. */
static inline idx_t popcount_u64(idx_t v)
{
    idx_t c = 0;
    while (v) {
        c += v & 1;
        v >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* Handle offset 0 explicitly. */
    if (n == 0)
        return 0;

    /* Compute t = 2*n - popcount(n). */
    seq_t t = (seq_t)n * 2 - (seq_t)popcount_u64(n);

    /* Return the largest odd divisor of t. */
    seq_t r = t;
    while ((r & 1) == 0)
        r >>= 1;
    return r;
}
