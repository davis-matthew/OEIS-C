/*
 * A283306  List points (x,y) having integer coordinates with x >= 0, y >= 0,
 *          sorted first by x^2+y^2 and in case of a tie, by x-coordinate.
 *          Sequence gives y-coordinates.
 * https://oeis.org/A283306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence lists all lattice points (x,y) with x>=0, y>=0.
   Points are sorted first by the sum of squares r = x² + y²,
   and in case of a tie by the smaller x.  The function returns
   the y-coordinate of the n-th point (1‑based).  The algorithm
   generates points sum‑by‑sum, storing the y values in a
   dynamically enlarging cache.  No floating point arithmetic
   is used, only integer square‑root helper _floor_isqrt_u64. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"

static idx_t *ycache = NULL;  /* cached y values (as idx_t) */
static size_t  ylen   = 0;    /* number of cached values */
static size_t  ycap   = 0;    /* allocated capacity */
static idx_t   next_s = 0;    /* next sum of squares to process */

/* Ensure that at least 'needed' y values are cached. */
static void generate_until(idx_t needed)
{
    while (ylen < needed) {
        idx_t x_limit = _floor_isqrt_u64(next_s);
        for (idx_t x = 0; x <= x_limit; ++x) {
            idx_t x2   = x * x;
            idx_t diff = next_s - x2;              /* >=0 */
            idx_t y    = _floor_isqrt_u64(diff);   /* integer sqrt */
            if (y * y == diff) {                   /* perfect square */
                if (ylen >= ycap) {
                    ycap = ycap ? ycap * 2 : 128;
                    idx_t *tmp = (idx_t *)realloc(ycache, ycap * sizeof(idx_t));
                    if (!tmp) abort();
                    ycache = tmp;
                }
                ycache[ylen++] = y;
            }
        }
        ++next_s;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined, return 0 */
    if (n > ylen) generate_until(n);
    return (seq_t)ycache[n - 1];
}
