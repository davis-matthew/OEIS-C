/*
 * A282562  Number of ways to write n as x^2 + y^2 + z^2 + w^2 with x,y,z,w
 *          nonnegative integers such that both 2*x - y and 9*z^2 + 666*z*w +
 *          w^2 are squares.
 * https://oeis.org/A282562
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute OEIS sequence A282562
 *  a(n) = number of quadruples (x,y,z,w) of nonnegative integers such that
 *         x^2 + y^2 + z^2 + w^2 = n,
 *         2*x - y is a perfect square, and
 *         9*z^2 + 666*z*w + w^2 is a perfect square.
 *  The implementation performs a direct enumeration over the variables
 *  and caches the counts of admissible (z,w) pairs for each possible sum.
 *  Complexity:  O(√n * √n) ≈ O(n) for the precomputation of the (z,w) table
 *  and O(√n) per query to sum over the (x,y) possibilities.
 *
 *  The routine uses only integer arithmetic and standard library calls
 *  (malloc, memset).  The helper function isqrt_u64() implements an
 *  integer square root using binary search; it returns floor(sqrt(n)).
 *
 *  The function is safe for any idx_t n ≥ 0 and works with any seq_t
 *  type supplied by the build.  It can be called in any order, as the
 *  data structures are automatically extended when larger n values
 *  are requested.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Integer square root (floor). Binary search implementation. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t low = 0;
    idx_t high = v + 1;           /* high is exclusive */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * mid <= v)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Caching structures for admissible (z,w) pairs */
static seq_t *z_counts = NULL;      /* counts of admissible (z,w) for each sum r */
static idx_t z_counts_size = 0;     /* size of the above array (max n + 1) */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Allocate or grow the z_counts array to cover all sums up to n */
    if (n + 1 > z_counts_size) {
        /* free previous allocation if any */
        if (z_counts)
            free(z_counts);
        z_counts = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!z_counts) {
            /* allocation failed – return 0 as safe fallback */
            return (seq_t)0;
        }
        memset(z_counts, 0, (n + 1) * sizeof(seq_t));
        z_counts_size = n + 1;

        /* Precompute admissible (z,w) pairs for all sums up to n */
        idx_t max_z = isqrt_u64(n);
        for (idx_t z = 0; z <= max_z; ++z) {
            idx_t z2 = z * z;
            idx_t max_w = isqrt_u64(n - z2);
            for (idx_t w = 0; w <= max_w; ++w) {
                idx_t r = z2 + w * w;              /* sum of squares */
                /* compute E(z,w) = 9*z^2 + 666*z*w + w^2 */
                idx_t E = 9 * z * z + 666 * z * w + w * w;
                idx_t sE = isqrt_u64(E);
                if (sE * sE == E)
                    z_counts[r] += (seq_t)1;
            }
        }
    }

    /* Count valid (x,y) pairs and add corresponding (z,w) counts */
    seq_t result = (seq_t)0;
    idx_t max_x = isqrt_u64(n);
    for (idx_t x = 0; x <= max_x; ++x) {
        idx_t x2 = x * x;
        idx_t remaining_after_x = n - x2;
        idx_t max_s1 = isqrt_u64(2 * x);
        for (idx_t s1 = 0; s1 <= max_s1; ++s1) {
            idx_t y = 2 * x - s1 * s1;           /* y is nonnegative by construction */
            idx_t y2 = y * y;
            if (y2 > remaining_after_x)
                continue;
            idx_t remaining = remaining_after_x - y2;   /* sum needed from z^2 + w^2 */
            if (remaining <= n)               /* guaranteed by construction */
                result += z_counts[remaining];
        }
    }
    return result;
}
