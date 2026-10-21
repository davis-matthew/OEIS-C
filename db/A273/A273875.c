/*
 * A273875  Number of ordered ways to write n as w^2 + x^2 + y^2 + z^2 with
 *          x*y + 2*y*z + 4*z*x a nonnegative cube, where w,x,y,z are integers
 *          with w >= 0 and x > 0.
 * https://oeis.org/A273875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of ordered quadruples (w,x,y,z) of integers with
   w >= 0, x > 0, such that
   w^2 + x^2 + y^2 + z^2 = n   and
   x*y + 2*y*z + 4*z*x is a non‑negative perfect cube. */

/* The function is memoised: the first call with a given n
   triggers a full recomputation of all counts up to that n
   and subsequent calls for smaller n simply fetch the
   previously computed value. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define MAX_CUBE 80000  /* maximum value of the cubic expression
                          for n <= 10000 (7*100^2 = 70 000) */

/* pre‑computed table: cube_small[v] is true iff v is a non‑negative cube */
static bool cube_small[MAX_CUBE + 1] = { false };
static bool cube_table_built = false;

static void build_cube_table(void)
{
    if (!cube_table_built) {
        for (int m = 0; ; ++m) {
            int cube = m * m * m;
            if (cube > MAX_CUBE) break;
            cube_small[cube] = true;
        }
        cube_table_built = true;
    }
}

seq_t a(idx_t n)
{
    static seq_t *counts = NULL;
    static idx_t max_n = 0;

    if (n == 0) return 0;      /* offset 1: a(1) is the first term */

    /* recompute everything if we need a larger maximum */
    if (n > max_n) {
        /* free any previously allocated array */
        free(counts);
        /* allocate array of size n+1, initialized to 0 */
        counts = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!counts) exit(1);   /* allocation failure */

        /* build the cube lookup table once */
        build_cube_table();

        /* maximum possible square root for w/x/y/z */
        int max_root = (int)floor(sqrt((double)n));

        for (idx_t w = 0; w <= (idx_t)max_root; ++w) {
            uint64_t w2 = (uint64_t)w * w;
            int   max_x = (int)floor(sqrt((double)(n - w2)));
            for (idx_t x = 1; x <= (idx_t)max_x; ++x) {
                uint64_t wx = w2 + (uint64_t)x * x;
                int   max_y = (int)floor(sqrt((double)(n - wx)));
                int   X = (int)x;
                for (int y = -max_y; y <= max_y; ++y) {
                    int Y = y;
                    uint64_t wy = wx + (uint64_t)(Y * Y);
                    if (wy > n) continue;   /* safety check, shouldn't happen */
                    int   max_z = (int)floor(sqrt((double)(n - wy)));
                    for (int z = -max_z; z <= max_z; ++z) {
                        int Z = z;
                        uint64_t total = wy + (uint64_t)(Z * Z);
                        /* compute the cubic expression */
                        int C = X * Y + 2 * Y * Z + 4 * Z * X;
                        if (C >= 0 && C <= MAX_CUBE && cube_small[C]) {
                            counts[total] += 1;
                        }
                    }
                }
            }
        }
        max_n = n;
    }

    return counts[(idx_t)n];
}
