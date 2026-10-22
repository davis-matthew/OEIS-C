/*
 * A332923  2*a(n) are the perimeters of distinct triangles with integer sides
 *          i <= j <= k, whose area equals 3 times their perimeter. Terms
 *          occurring more than once belong to different triangles.
 * https://oeis.org/A332923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   cmp_seq_t_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by enumerating integer solutions to 36*(x+y+z) = x*y*z.
   For each solution (x,y,z) the value of the sequence is a(n) = x+y+z. */

#include <stdlib.h>
#include "../utils/cmp_seq_t_custom.h"

/* static storage for all solutions (45 known triangles) */
static seq_t solutions[100];
static size_t solcnt = 0;

static void init_solutions(void)
{
    if (solcnt != 0) return;          /* already initialized */

    /* search for all integer triples x <= y <= z with 36*(x+y+z) = x*y*z */
    for (idx_t x = 1; x <= 10; ++x) {            /* x is bounded by sqrt(108) ≈ 10 */
        for (idx_t y = x; y <= 200; ++y) {
            idx_t prod_xy = x * y;
            if (prod_xy <= 36) continue;        /* denominator would be zero or negative */

            idx_t denom = prod_xy - 36;
            idx_t num   = 36 * (x + y);
            if (num % denom != 0) continue;

            idx_t z = num / denom;
            if (z < y) continue;                /* maintain x <= y <= z */

            /* verify the Diophantine equation (sanity check) */
            seq_t prod_xyz = (seq_t)x * y * z;
            seq_t sum_xyz  = (seq_t)(x + y + z);
            if (prod_xyz != 36 * sum_xyz) continue;

            /* sides of the triangle are: a = y+z, b = x+z, c = x+y */
            seq_t a = y + z;
            seq_t b = x + z;
            seq_t c = x + y;

            /* sort sides to maintain i <= j <= k (not strictly needed for a(n)) */
            seq_t sides[3] = {a, b, c};
            for (int i = 0; i < 3; ++i) {
                for (int j = i + 1; j < 3; ++j) {
                    if (sides[j] < sides[i]) {
                        seq_t tmp = sides[i];
                        sides[i] = sides[j];
                        sides[j] = tmp;
                    }
                }
            }

            /* a(n) = (i + j + k) / 2 = x + y + z = sum_xyz */
            solutions[solcnt++] = sum_xyz;
        }
    }

    /* sort the values; duplicates are kept exactly as many times as they
       occur (different triangles with same perimeter) */
    qsort(solutions, solcnt, sizeof(seq_t), cmp_seq_t_custom);
}

/* public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but never called */

    init_solutions();                /* ensure table is ready */

    if (n <= solcnt) return solutions[n - 1];
    /* beyond the known finite sequence */
    return 0;
}
