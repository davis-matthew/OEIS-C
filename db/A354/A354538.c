/*
 * A354538  a(n) is the least k such that A322523(k) = n.
 * https://oeis.org/A354538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n + 7)/2 (for n ≥ 2), a(0)=1, a(1)=3
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Handle the first two values directly. */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;

    /* If seq_t is a primitive (≤64 bit) use a 128‑bit intermediate to
       avoid overflow while still producing the correct value for all
       indices that fit in seq_t.  For arbitrary‑precision seq_t the
       builtin pow3 works directly. */
    if (sizeof(seq_t) <= 8) {
        __uint128_t pow = 1;
        for (idx_t i = 0; i < n; ++i) pow *= 3;
        __uint128_t res = (pow + 7) / 2;
        return (seq_t)res;
    } else {
        seq_t p = pow3(n);
        return (p + 7) / 2;
    }
}
