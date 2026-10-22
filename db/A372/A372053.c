/*
 * A372053  Irregular array read by rows: row n lists the factorizations of n
 *          into a product of nondecreasing integers >= 2.
 * https://oeis.org/A372053
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 2997, u16 n <= 9942, u32 n <= 9942, u64 n <= 9942,
 *            u128 n <= 9942, big any n
 * Helpers:   dfs_emit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th element of the flattened array of nondecreasing
   factorizations of integers ≥2.  a(2) is the first factor emitted.
   The algorithm enumerates the factorizations of successive integers
   in lexicographic order and decrements a counter until the desired
   factor is reached. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/dfs_emit.h"

seq_t a(idx_t n)
{
    if (n < (idx_t)2)          /* undefined for n<2, return 0 */
        return (seq_t)0;

    idx_t off = n - (idx_t)2;   /* offset into the flattened list */
    seq_t result;
    /* buffer big enough for the largest factor chain (≤64 for all
       practical n; larger for arbitrary‑precision types).          */
    seq_t buf[128];

    for (idx_t m = (idx_t)2; ; ++m) {
        if (dfs_emit(m, (idx_t)2, buf, (size_t)0, &off, &result))
            return result;
    }
}
