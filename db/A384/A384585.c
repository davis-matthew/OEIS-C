/*
 * A384585  a(n) = 6 * (4*n)! / ((n+1)! * (3*n+1)!).
 * https://oeis.org/A384585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A384585
   Formula: a(n) = 6 * (4*n)! / ((n+1)! * (3*n+1)!)
   Offest: 0
   64‑bit values may overflow; for the arbitrary‑precision build the
   result is exact for any n.  The implementation uses a multiplicative
   recurrence:
   a(n) = a(n‑1) * (4n‑3)(4n‑2)(4n‑1)(4n) / ((n+1)(3n‑1)(3n)(3n+1)),
   with exact reductions performed by gcd to keep intermediates small. */

#include <stdlib.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */
#include "../utils/seq_alloc.h"   /* seq_t *seq_alloc(size_t cap); */
#include "../utils/seq_realloc.h" /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap); */

static seq_t *cache = NULL;
static idx_t  cache_len = 0;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initialise cache with a(0) */
    if (!cache) {
        cache_cap = 1;
        cache     = seq_alloc(cache_cap);
        cache[0]  = (seq_t)6;   /* a(0) = 6 */
        cache_len = 1;
    }

    /* Extend cache up to required index */
    while (cache_len <= n) {
        idx_t k       = cache_len;          /* compute a(k) */
        seq_t val     = cache[k-1];          /* a(k-1) */

        /* Numerator factors: (4k-3)(4k-2)(4k-1)(4k) */
        idx_t num[4] = {4*k-3, 4*k-2, 4*k-1, 4*k};
        /* Denominator factors: (k+1)(3k-1)(3k)(3k+1) */
        idx_t den[4] = {k+1, 3*k-1, 3*k, 3*k+1};

        /* Cross‑reduce numerators vs. denominators */
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                seq_t g = gcd((seq_t)num[i], (seq_t)den[j]);
                if (g > (seq_t)1) {
                    num[i] = (idx_t)((seq_t)num[i] / g);
                    den[j] = (idx_t)((seq_t)den[j] / g);
                }
            }
        }

        /* Reduce remaining denominators with current value */
        for (int j = 0; j < 4; ++j) {
            if (den[j] == 1) continue;
            seq_t g = gcd(val, (seq_t)den[j]);
            if (g > (seq_t)1) {
                val = val / g;
                den[j] = (idx_t)((seq_t)den[j] / g);
            }
        }

        /* Multiply by remaining numerators */
        for (int i = 0; i < 4; ++i) {
            if (num[i] == 1) continue;
            val = val * (seq_t)num[i];
        }

        /* Divide by remaining denominators (exact by construction) */
        for (int j = 0; j < 4; ++j) {
            if (den[j] == 1) continue;
            val = val / (seq_t)den[j];
        }

        /* Grow cache if necessary */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap <= cache_len) new_cap <<= 1;
            seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len++] = val;
    }

    return cache[n];
}
