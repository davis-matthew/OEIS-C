/*
 * A308490  a(0) = 1, a(n) = Sum_{k=1..n} stirling2(n,k) * k^(2*k).
 * https://oeis.org/A308490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A308490:  a(n) = Σ_{k=1..n} Stirling2(n,k) · k^(2k)
   Offset : 0  (a(0) = 1)                                */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#ifdef __cplusplus
#include <cstdlib>
using std::realloc;
#else
#include <stdlib.h>
#endif

#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */

static seq_t *stirling = NULL;  /* S(n,k) for current row (0..n) */
static size_t stir_cap = 0;

static seq_t *a_cache = NULL;   /* cached a(n) values */
static size_t cache_cap = 0;    /* number of cached values */

/* Ensure array *arr has capacity >= new_cap.  Reallocate if necessary. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t new_cap)
{
    if (*cap < new_cap) {
        size_t newsize = (*cap == 0) ? 1 : (*cap << 1);
        if (newsize < new_cap) newsize = new_cap;
        #ifdef __cplusplus
            *arr = reinterpret_cast<seq_t*>(realloc(*arr, newsize * sizeof(seq_t)));
        #else
            *arr = (seq_t*)realloc(*arr, newsize * sizeof(seq_t));
        #endif
        *cap = newsize;
    }
}

/* Compute a(n) for all n up to the maximum requested. */
seq_t a(idx_t n)
{
    /* initialise memoisation structures */
    if (stirling == NULL) {
        ensure_cap(&stirling, &stir_cap, 1);
        stirling[0] = (seq_t)1;          /* S(0,0) = 1 */
    }
    if (a_cache == NULL) {
        ensure_cap(&a_cache, &cache_cap, 1);
        a_cache[0] = (seq_t)1;           /* a(0) = 1 */
    }

    /* compute values not yet cached */
    for (idx_t cur = cache_cap; cur <= n; ++cur) {
        /* extend Stirling array to hold row cur */
        ensure_cap(&stirling, &stir_cap, (size_t)cur + 1);

        /* update Stirling numbers for row cur from previous row */
        if (cur > 0) {
            for (idx_t k = cur; k >= 1; --k) {
                /* S(cur,k) = k*S(cur-1,k) + S(cur-1,k-1) */
                stirling[k] = (seq_t)((seq_t)k * stirling[k] + stirling[k - 1]);
            }
            stirling[0] = (seq_t)0; /* S(cur,0) = 0 for cur>0 */
        }

        /* compute a(cur) = Σ_{k=1..cur} S(cur,k) * k^(2k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t pow = big_pow((seq_t)k, (seq_t)(2 * k));
            sum += stirling[k] * pow;
        }

        ensure_cap(&a_cache, &cache_cap, (size_t)cur + 1);
        a_cache[cur] = sum;
        cache_cap = (size_t)cur + 1;
    }

    return a_cache[n];
}
