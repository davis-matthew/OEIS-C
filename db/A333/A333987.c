/*
 * A333987  Integers m for which b(m) < b(m-1) where b(k) = Min_{sqrt(k) -
 *          sqrt(x) - sqrt(y) > 0 with x, y distinct integers}.
 * https://oeis.org/A333987
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A333987 – Integers m for which the minimal positive
    difference b(m) is less than b(m‑1),
    where b(k) = Min{ sqrt(k) - sqrt(x) - sqrt(y) :
                      sqrt(k)-sqrt(x)-sqrt(y) > 0, x≠y }   */

/*  The function scans k = 1,2,… and records every k whose b(k) is
    strictly smaller than the best difference seen so far.
    The results are cached so that any call can be answered in O(1).  */

#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --------------------------------------------------------------
   Compute the minimal positive difference b(k)
   -------------------------------------------------------------- */
static double best_difference_for_k(idx_t k)
{
    double sqrt_k = sqrt((double)k);
    double best_sum = -1.0;           /* maximum sqrt(x)+sqrt(y) found   */
    unsigned x_start = 0U;             /* allow x=0 for first few k       */

    for (unsigned x = x_start; x < (unsigned)k - 1U; ++x) {
        double sqrt_x = sqrt((double)x);
        double min_sum = sqrt_x + sqrt((double)(x + 1));
        if (min_sum >= sqrt_k) break;  /* any larger x cannot satisfy */

        double diff_x = sqrt_k - sqrt_x;
        if (diff_x <= 0.0) break;      /* no valid y for this x           */

        double y_max_d = diff_x * diff_x - 1e-12;
        if (y_max_d < 0.0) continue;
        unsigned y_max = (unsigned)floor(y_max_d);

        if (y_max > (unsigned)k - 1U) y_max = (unsigned)k - 1U;
        if (y_max <= x) continue;      /* need distinct and y > x */

        double sum = sqrt_x + sqrt((double)y_max);
        if (sum > best_sum) best_sum = sum;
    }

    if (best_sum < 0.0) return HUGE_VALF;   /* no admissible pair */
    return sqrt_k - best_sum;               /* minimal positive difference */
}

/*  Function prototype required by the build system.  */
seq_t a(idx_t n);

static seq_t *cache = NULL;          /* cache[0] = a(1), cache[1] = a(2), …   */
static size_t cache_len   = 0;       /* number of terms currently stored     */
static size_t cache_cap   = 0;       /* allocated capacity of cache          */
static double prev_diff   = HUGE_VALF;/* last recorded difference b(m)         */
static idx_t next_k       = 2;       /* next integer k to evaluate            */

/* --------------------------------------------------------------
   Grow the cache if needed
   -------------------------------------------------------------- */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;

    seq_t *p;
#ifdef __cplusplus
    p = new seq_t[new_cap];
#else
    p = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!p) abort();
    memset(p, 0, new_cap * sizeof(seq_t));
#endif

    if (cache) {
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }
    cache = p;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------
   Ensure that the cache contains at least n terms
   -------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    if (cache_len == 0) {          /* initialise with a(1)=1 */
        grow_cache(1);
        cache[0] = (seq_t)1;
        cache_len = 1;
        prev_diff = HUGE_VALF;
        next_k = 2;
    }

    while (cache_len < (size_t)n) {
        double diff = best_difference_for_k(next_k);
        if (diff < prev_diff - 1e-12) { /* strict decrease */
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)next_k;
            prev_diff = diff;
        }
        ++next_k;
    }
}

/* --------------------------------------------------------------
   Public function required by the specification
   -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for n<1 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
