/*
 * A373972  a(n) = 1 if n can be represented as a sum of 2 nonnegative cubes,
 *          otherwise 0.
 * https://oeis.org/A373972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if ∃x,y≥0 : x³ + y³ = n; otherwise 0
 * Range:     u8 n <= 1095, u16 n <= 8680, u32 n <= 8680, u64 n <= 8680,
 *            u128 n <= 8680, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes by brute‑force checking all cubes ≤ n. */

#include <stdbool.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* Upper bound for cube roots that we need to test. */
    idx_t max_a = int_cube_root(n);

    for (idx_t a = 0; a <= max_a; ++a)
    {
        /* Compute a³.  All intermediate values fit in seq_t for n ≤ 10000. */
        seq_t ca = (seq_t)a * a * a;

        /* Remaining part to reach n. */
        seq_t rem = (seq_t)n - ca;
        /* rem is guaranteed to be ≤ n and non‑negative. */

        /* Candidate cube root of the remaining part. */
        idx_t b = int_cube_root((idx_t)rem);

        /* Verify that b³ equals the remainder. */
        if ((seq_t)b * b * b == rem)
            return (seq_t)1;
    }

    return (seq_t)0;
}
