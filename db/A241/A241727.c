/*
 * A241727  Number of compositions of n into distinct parts with exactly eight
 *          descents.
 * https://oeis.org/A241727
 *
 * Offset:    45    (a(45) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 65, u32 n <= 94, u64 n <= 197,
 *            u128 n <= 595, big any n
 * Helpers:   kmax_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241727: Number of compositions of n into distinct parts with exactly eight descents.
   Compute a(n) as the sum over k of Eulerian(k,8) * p_distinct_k(n),
   where p_distinct_k(n) counts partitions of n into k distinct parts.
   The function memoizes a DP table for all n up to the largest requested value. */
#include <stdlib.h>
#include "../utils/kmax_for_n.h"

static seq_t *cache_ways = NULL;      /* Flattened table [k][s] => ways[k*stride+s] */
static seq_t *cache_eulerian = NULL;  /* cache_eulerian[k] = Eulerian(k,8) */
static idx_t cache_max_n = 0;         /* maximum n for which cache is valid   */
static idx_t cache_kmax = 0;          /* number of k values currently stored   */

/* Rebuild the DP tables for all n up to mx */
static void rebuild_cache(idx_t mx)
{
    /* Free any previous cache */
    if (cache_ways) {
        free(cache_ways);
        cache_ways = NULL;
    }
    if (cache_eulerian) {
        free(cache_eulerian);
        cache_eulerian = NULL;
    }

    /* New limits */
    cache_kmax = kmax_for_n(mx);
    cache_max_n = mx;

    /* Allocate ways table: (kmax+1) × (mx+1) */
    size_t stride = (size_t)(mx + 1);
    size_t ways_len = (size_t)(cache_kmax + 1) * stride;
    seq_t *ways = (seq_t *)calloc(ways_len, sizeof(seq_t));
    if (!ways) exit(1);          /* allocation failure */

    cache_ways = ways;

    /* Base: ways[0][0] = 1 */
    ways[0 * stride + 0] = 1;

    /* DP for partitions into distinct parts with exactly k parts */
    for (idx_t k = 1; k <= cache_kmax; ++k) {
        idx_t min_s = k * (k + 1) / 2;
        for (idx_t n = min_s; n <= mx; ++n) {
            seq_t val = 0;
            if (n >= k) {
                val += ways[k * stride + (n - k)];          /* add part k */
                val += ways[(k - 1) * stride + (n - k)];    /* add part k and increase part count */
            }
            ways[k * stride + n] = val;
        }
    }

    /* Compute Eulerian numbers a(k,8) for k up to kmax */
    seq_t *eul = (seq_t *)calloc((size_t)(cache_kmax + 1), sizeof(seq_t));
    if (!eul) exit(1);

    /* We keep two rows of the recurrence for m = 0..8 */
    seq_t prev[9] = {0}, cur[9] = {0};
    prev[0] = 1;          /* a[0][0] = 1 */

    for (idx_t n = 1; n <= cache_kmax; ++n) {
        for (int m = 0; m <= 8; ++m) {
            seq_t term1 = (seq_t)(m + 1) * prev[m];
            seq_t term2 = 0;
            if (m > 0 && n > (idx_t)m)
                term2 = (seq_t)(n - m) * prev[m - 1];
            cur[m] = term1 + term2;
        }
        eul[n] = cur[8];
        /* Prepare for next n */
        for (int m = 0; m <= 8; ++m)
            prev[m] = cur[m];
    }

    cache_eulerian = eul;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n < 45) return 0;   /* behaviour below the offset is irrelevant */

    if (n > cache_max_n)
        rebuild_cache(n);

    const idx_t stride = cache_max_n + 1;
    seq_t res = 0;
    for (idx_t k = 0; k <= cache_kmax; ++k) {
        seq_t e = cache_eulerian[k];
        if (!e) continue;
        seq_t w = cache_ways[k * stride + n];
        res += e * w;
    }
    return res;
}
