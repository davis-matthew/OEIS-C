/*
 * A383109  Consider the isosceles triangle whose vertices are the Gaussian
 *          integers z1 = 0, z2 = x+i*y, z3 = x-i*y. The sequence lists the
 *          pairs of positive integer foci (f_i, f_j), f_i < f_j of Steiner
 *          inellipse for some z2, z3.
 * https://oeis.org/A383109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_sqrt, seq_alloc, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A383109:  foci of the Steiner in‑ellipse of an isosceles Gaussian
   triangle with vertices 0, x+i⋅y, x-i⋅y.

   The pair of foci is
        f₁ = (2x – k)/3 ,   f₂ = (2x + k)/3
   where x is a multiple of 3 and k = √(x² – 3y²) is an integer that is
   also a multiple of 3.  The sequence lists the foci in increasing
   order of x; for each x the y‑values increase.  Every valid pair
   contributes two consecutive terms: f₁ followed by f₂.
   The program enumerates all admissible (x,y) pairs, caching the
   foci so that an arbitrary a(n) can be returned in O(1) after
   the necessary terms have been generated.
*/

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"

/* Cached sequence values. */
static seq_t *term_cache = NULL;     /* array of seq_t values */
static idx_t  cache_len   = 0;       /* number of stored terms  */
static size_t cache_cap   = 0;       /* allocated capacity      */

/* Generation state. */
static idx_t  next_x  = 3;       /* current x (≡0 mod 3)      */
static idx_t  next_y  = 1;       /* current y for this x      */
static bool   pending_f2 = false;/* pending second term ready */
static seq_t  pending_f2_val;

/* Ensure the cache can hold at least <need> terms. */
static void ensure_capacity(idx_t need)
{
    if (cache_cap < need) {
        size_t newcap = cache_cap ? cache_cap << 1 : 64;
        while (newcap < (size_t)need) newcap <<= 1;
        seq_t *newptr = seq_realloc(term_cache, cache_cap, newcap);
        if (!newptr) { exit(1); }   /* allocation failure */
        term_cache = newptr;
        cache_cap  = newcap;
    }
}

/* Generate terms until at least <up_to> entries are stored. */
static void generate_up_to(idx_t up_to)
{
    if (cache_len >= up_to) return;
    ensure_capacity(up_to);

    while (cache_len < up_to) {
        /* First output a pending second member of a pair, if any. */
        if (pending_f2) {
            term_cache[cache_len++] = pending_f2_val;
            pending_f2 = false;
            continue;               /* continue with current x/y */
        }

        seq_t  x_seq   = (seq_t)next_x;                 /* x  (≥3, ≡0 mod3) */
        seq_t  two_x   = seq_mul((seq_t)2, x_seq);      /* 2x          */

        for (idx_t y = next_y; ; ++y) {
            seq_t  y_seq = (seq_t)y;
            seq_t  y2    = seq_mul(y_seq, y_seq);
            seq_t  lhs   = seq_mul((seq_t)3, y2);        /* 3⋅y²          */
            seq_t  x2    = seq_mul(x_seq, x_seq);       /* x²             */
            if (lhs > x2) {                             /* no more y for this x */
                next_x += 3;
                next_y  = 1;
                break;
            }

            seq_t  D   = x2 - lhs;                      /* D = x² – 3y²   */
            seq_t  k   = floor_sqrt(D);                 /* k = ⌊√D⌋        */
            if (seq_mul(k, k) != D) continue;           /* not a perfect square */
            if (k % (seq_t)3 != 0) continue;           /* k must be multiple of 3 */

            seq_t  f1  = (two_x - k) / 3;              /* first focus   */
            seq_t  f2  = (two_x + k) / 3;              /* second focus  */

            /* Store f1. */
            term_cache[cache_len++] = f1;
            if (cache_len == up_to) {                  /* need only one more term */
                pending_f2      = true;
                pending_f2_val  = f2;
                next_y          = y + 1;               /* resume later   */
                return;
            }

            /* Store f2. */
            term_cache[cache_len++] = f2;
            next_y = y + 1;                            /* next y value   */
            if (cache_len == up_to) return;
        }
    }
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n==0 */
    if (cache_len < n) {                  /* generate as needed */
        generate_up_to(n);
    }
    return term_cache[n - 1];
}
