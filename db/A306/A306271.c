/*
 * A306271  a(n) is the smallest positive integer x such that x^2 mod n is a
 *          square, with x^2 >= n.
 * https://oeis.org/A306271
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer x such that x² mod n is a
 *            perfect square and x² ≥ n
 * Range:     u8 n <= 273, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for unsigned 64‑bit indices.
   For the range of n used in this sequence (≤ 10 000) the
   simple loop is fast and safe. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= v) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n < 1 */

    /* Start with the smallest x whose square is ≥ n. */
    idx_t x = isqrt_u64(n);
    if (x * x < n) ++x;

    for (;;) {
        /* r = x² mod n */
        idx_t r = (x * x) % n;

        /* Check if r is a perfect square. */
        idx_t t = isqrt_u64(r);
        if (t * t == r) {
            return (seq_t)x;          /* minimal x found */
        }
        ++x;
    }
}
