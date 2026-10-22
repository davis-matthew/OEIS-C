/*
 * A389924  Standard composition numbers of integer compositions with first
 *          part = length.
 * https://oeis.org/A389924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   clz64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389924
   The n-th standard composition number whose composition satisfies the
   property "first part = length".
   The property is equivalent to: for the binary representation of k
   (with 1‑based bit positions), let m be the number of 1‑bits,
   let p_m be the largest and p_{m-1} the second largest 1‑bit position.
   Then the composition has first part p_m - p_{m-1}.  The
   condition first part = length is m-1 == p_m - p_{m-1} (for m > 1),
   and only k = 1 when m = 1.  Thus a(n) is the n‑th k >= 1 that
   satisfies this test.

   Implementation: simply enumerate k from 1 upward, test the
   condition above, and return the n-th qualifying k.  The test
   requires only popcount, clz, and simple bit operations. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/clz64.h"

/* popcount of a 64‑bit integer */
static inline int popcnt64(uint64_t x)
{
    return __builtin_popcountll(x);
}

/* Test whether k satisfies first_part == length. */
static inline bool good(uint64_t k)
{
    int m = popcnt64(k);
    if (m == 1)            /* only (1) is good */
        return k == 1;

    /* position of the most significant 1‑bit (1‑based) */
    int msb = 64 - clz64(k);
    uint64_t msb_mask = 1ULL << (msb - 1);
    uint64_t rest = k & ~msb_mask;               /* remove the highest bit */

    /* position of the second highest 1‑bit */
    int second_msb = 64 - clz64(rest);

    /* condition: difference between top two bit positions equals popcount */
    return (msb - second_msb) == m;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 1; ; ++k) {
        if (good((uint64_t)k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}
