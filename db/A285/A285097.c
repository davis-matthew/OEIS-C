/*
 * A285097  a(n) = difference between the positions of two least significant
 *          1-bits in base-2 representation of n, or 0 if there are less than
 *          two 1-bits in n (when n is either zero or a power of 2).
 * https://oeis.org/A285097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = difference between the positions of the two least
 *            significant 1‑bits in the binary expansion of n. If n has fewer
 *            than two 1‑bits, a(n) = 0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any 0 <= n < 2^64.
   The implementation uses only integer types and operations allowed
   by the specification, avoiding built‑in intrinsics for portability. */
#include <stdint.h>
#include <stdbool.h>

/* External linkage as required.  The types seq_t and idx_t are supplied
   by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* zero has no 1‑bits */

    uint64_t x = n;

    /* Find position of the least significant 1‑bit. */
    uint64_t tz0 = 0;
    while ((x & 1ull) == 0) {
        tz0++;
        x >>= 1;
    }

    /* Skip that 1‑bit and look for the next 1‑bit. */
    x >>= 1;
    if (x == 0)
        return 0;                    /* only one 1‑bit in n */

    /* Find the position of the second least significant 1‑bit
       relative to the original LSB. */
    uint64_t tz1 = 0;
    while ((x & 1ull) == 0) {
        tz1++;
        x >>= 1;
    }

    /* The required difference is 1 + tz1. */
    return (seq_t)(1 + tz1);
}
