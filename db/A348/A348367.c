/*
 * A348367  a(n) = w(n + w(n)), where w(n) is the binary weight of n,
 *          A000120(n).
 * https://oeis.org/A348367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000120(n + A000120(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binary weight (popcount) of a number n */

#include <stdint.h>
#include <stdbool.h>

/* Portable popcount for 64‑bit unsigned integers */
static inline unsigned popcount_u64(uint64_t x)
{
    unsigned cnt = 0;
    while (x) {
        x &= x - 1;   /* drop lowest set bit */
        cnt++;
    }
    return cnt;
}

/* OEIS A348367: a(n) = binary weight of (n + binary weight of n) */
seq_t
a(idx_t n)
{
    unsigned w_n   = popcount_u64((uint64_t)n);       /* w(n) */
    uint64_t sum   = (uint64_t)n + w_n;               /* n + w(n) */
    unsigned w_sum = popcount_u64(sum);               /* w(n + w(n)) */
    return (seq_t)w_sum;
}
