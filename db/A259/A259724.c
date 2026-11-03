/*
 * A259724  Numbers k such that [r[s*k]] < [s[r*k]], where r = sqrt(2),
 *          s=sqrt(3), and [ ] = floor.
 * https://oeis.org/A259724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 72 terms through n = 760 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A259724:
   a(n) = the n‑th positive integer k for which
          floor(√3 * floor(√2 * k)) > floor(√2 * floor(√3 * k)).  */

/* Utility: floor√(x) for arbitrary precision or builtin types.  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Compute successive terms on demand, caching results for later
   (allows calls in arbitrary order). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined for n < 1 */

    /* static cache */
    static seq_t *cache    = NULL;
    static size_t  cachelen = 0;
    static size_t  cachecap = 0;
    static idx_t   last_k   = 0;   /* largest k examined so far */

    /* generate terms until we have at least n entries */
    while (cachelen < n) {
        ++last_k;
        idx_t     kidx = last_k;          /* integer index */
        seq_t     k    = (seq_t)kidx;      /* promote to seq_t */

        /* A = floor(sqrt(2) * k) */
        seq_t A = floor_sqrt( ((seq_t)2) * k * k );

        /* B = floor(sqrt(3) * k) */
        seq_t B = floor_sqrt( ((seq_t)3) * k * k );

        /* left  = floor( sqrt(2) * B ) = floor_sqrt(2 * B^2) */
        seq_t left  = floor_sqrt( ((seq_t)2) * B * B );

        /* right = floor( sqrt(3) * A ) = floor_sqrt(3 * A^2) */
        seq_t right = floor_sqrt( ((seq_t)3) * A * A );

        if (left < right) {   /* k belongs to our sequence */
            if (cachelen == cachecap) {
                size_t newcap = cachecap ? cachecap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, newcap);
                cache = tmp;
                cachecap = newcap;
            }
            cache[cachelen++] = k;
        }
    }

    return cache[n - 1];
}
