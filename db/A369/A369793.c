/*
 * A369793  a(n) is the number of occurrences of n in A063655.
 * https://oeis.org/A369793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of occurrences of n in A063655
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A063655(n) = minimal semiperimeter of an integer‑rectangular partition of area n */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Static cache for precomputed values */
static idx_t  cache_N          = 0;      /* last maximum n that was precomputed */
static seq_t *cache_counts     = NULL;   /* array of counts, size cache_N + 1 */
static uint16_t *cache_min_s   = NULL;   /* minimal semiperimeter for each area */
static idx_t  cache_area_max   = 0;      /* maximum area that cache_min_s holds */

/* Recompute a(n) array for all n <= N */
static void recompute(idx_t N)
{
    if (N == 0) return;

    /* compute maximum area that could appear with i + j <= N */
    idx_t half   = N / 2;
    idx_t other  = (N + 1) / 2;
    idx_t area_lim = half * other;

    /* reallocate counts array */
    seq_t *temp_counts = (seq_t *)realloc(cache_counts, (N + 1) * sizeof(seq_t));
    if (!temp_counts) abort();          /* allocation failure */
    cache_counts = temp_counts;
    memset(cache_counts, 0, (N + 1) * sizeof(seq_t));

    /* reallocate min_s array */
    uint16_t *temp_min_s = (uint16_t *)realloc(cache_min_s, (area_lim + 1) * sizeof(uint16_t));
    if (!temp_min_s) abort();
    cache_min_s = temp_min_s;
    /* initialise with sentinel value > N for all areas */
    const uint16_t sentinel = (uint16_t)(N + 1);
    for (idx_t k = 0; k <= area_lim; ++k) cache_min_s[k] = sentinel;

    /* iterate over all unordered pairs (i,j) with i + j <= N */
    for (idx_t i = 1; i <= N - 1; ++i) {
        idx_t max_j = N - i;
        for (idx_t j = i; j <= max_j; ++j) {
            idx_t area = i * j;
            uint16_t sum = (uint16_t)(i + j);
            uint16_t old = cache_min_s[area];
            if (sum < old) {
                cache_min_s[area] = sum;
                if (old <= N) cache_counts[old]--;
                cache_counts[sum]++;
            }
        }
    }

    cache_N        = N;
    cache_area_max = area_lim;
}

/* Public function required by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                     /* offset is 1 */
    if (n > cache_N) recompute(n);                  /* compute if not cached yet */
    return cache_counts[n];
}
