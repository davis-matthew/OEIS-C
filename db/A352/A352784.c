/*
 * A352784  a(n) = w(n - w(n)), where w(n) is the binary weight of n,
 *          A000120(n).
 * https://oeis.org/A352784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A000120(n - A000120(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of 1‑bits in an idx_t value.
   The result is small (≤ 64), so it fits in seq_t safely. */
static inline seq_t popcount_idx(idx_t x)
{
    seq_t count = 0;
    while (x)
    {
        count++;
        x &= x - 1;           /* clear the lowest set bit */
    }
    return count;
}

seq_t a(idx_t n)
{
    /* w = A000120(n) – binary weight of n */
    seq_t w = popcount_idx(n);

    /* m = n - w(n)  (fits in idx_t) */
    idx_t m = n - (idx_t)w;

    /* a(n) = A000120(m) */
    return popcount_idx(m);
}
