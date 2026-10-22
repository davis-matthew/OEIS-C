/*
 * A340978  Number of ways to write n as an ordered sum of 5 positive cubes.
 * https://oeis.org/A340978
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (∑_{k≥1} x^{k^3})^5
 * Range:     u8 n <= 1160, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP solution: convolve the list of positive cubes five times. */

#include <stddef.h>   /* size_t */
#include <string.h>   /* memcpy */
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* a(n) is defined for n ≥ 5.  Return 0 for lower indices. */
    if (n < 5)
        return 0;

    /* Maximum cube root needed for this n.  For n ≤ 10000 the result < 22. */
    idx_t kmax = int_cube_root(n);

    /* Store the cubes k^3 (k ≥ 1) up to n. */
    idx_t cube[22];
    idx_t num_cubes = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        cube[num_cubes++] = k * k * k;
    }

    /* Static arrays large enough for the maximum n needed (≈10000). */
    /* The size is fixed; only the first (n+1) elements are used. */
    static seq_t dpA[10001];
    static seq_t dpB[10001];
    static idx_t computed_up_to = 0;  /* highest n for which dpA is valid */

    /* Recompute if this call requests a larger n than previously processed. */
    if (n > computed_up_to) {
        /* Initialize the DP for 0 cubes: only sum 0 is possible. */
        dpA[0] = 1;
        for (idx_t i = 1; i <= n; ++i)
            dpA[i] = 0;

        /* Convolve the cube list five times. */
        for (idx_t iter = 0; iter < 5; ++iter) {
            /* Zero the next DP array. */
            for (idx_t i = 0; i <= n; ++i)
                dpB[i] = 0;

            /* Add contributions of each cube. */
            for (idx_t ci = 0; ci < num_cubes; ++ci) {
                idx_t c = cube[ci];
                for (idx_t i = c; i <= n; ++i)
                    dpB[i] += dpA[i - c];
            }

            /* Copy dpB back into dpA for the next iteration. */
            for (idx_t i = 0; i <= n; ++i)
                dpA[i] = dpB[i];
        }

        computed_up_to = n;
    }

    return dpA[n];
}
