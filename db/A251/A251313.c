/*
 * A251313  Number of (n+1)X(4+1) 0..1 arrays with no 2X2 subblock having the
 *          maximum of its diagonal elements greater than the absolute
 *          difference of its antidiagonal elements.
 * https://oeis.org/A251313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A251313: Number of (n+1)×5 0..1 arrays with no 2×2 subblock
    having max(diagonal) > |antidiagonal| .
    The sequence is computed by a simple dynamic programming over rows.
    The DP state is the 5‑bit pattern of the last row; transitions
    are allowed if all four 2×2 blocks formed with the previous row
    satisfy the condition.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <new>
#endif

/* precomputed compatibility of consecutive rows */
static bool allowed[32][32];
static bool allowed_init = false;

/* initialise allowed[][] once */
static void init_allowed(void)
{
    if (allowed_init) return;
    for (int s = 0; s < 32; ++s) {
        for (int t = 0; t < 32; ++t) {
            bool ok = true;
            for (int j = 0; j < 4; ++j) {
                int a = (s >> j)       & 1;  /* top‑left   */
                int b = (s >> (j+1))   & 1;  /* top‑right  */
                int c = (t >> j)       & 1;  /* bottom‑left*/
                int d = (t >> (j+1))   & 1;  /* bottom‑right*/
                int max_diag = (a > d) ? a : d;
                int diff = b - c;
                if (diff < 0) diff = -diff;
                if (max_diag > diff) { ok = false; break; }
            }
            allowed[s][t] = ok;
        }
    }
    allowed_init = true;
}

/* dynamic programming arrays: dp_cur is counts for current row, dp_nxt for next */
static seq_t *dp_cur = NULL;
static seq_t *dp_nxt = NULL;

/* memoised cache of a(n) */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* maximum n for which cache holds a value */
static idx_t cache_max = 0;    /* largest computed index */

/* allocate seq_t array safely; used for dp and cache */
static seq_t *alloc_seq_array(size_t sz)
{
#if defined(__cplusplus)
    return new seq_t[sz];
#else
    return (seq_t*)calloc(sz, sizeof(seq_t));
#endif
}

/* free seq_t array */
static void free_seq_array(seq_t *p)
{
#if defined(__cplusplus)
    delete[] p;
#else
    free(p);
#endif
}

/* initialise DP arrays and cache once */
static void init_arrays(void)
{
    if (dp_cur) return;          /* already initialised */

    init_allowed();

    dp_cur   = alloc_seq_array(32);
    dp_nxt   = alloc_seq_array(32);
    for (int i = 0; i < 32; ++i) dp_cur[i] = (seq_t)1;  /* first row contributes 1 for every pattern */

    /* initial cache capacity for indices 0..16 */
    cache_cap = 16;
    cache = alloc_seq_array(cache_cap + 1);
    for (size_t i = 0; i <= cache_cap; ++i) cache[i] = (seq_t)0;
    cache[0] = (seq_t)0;          /* unused, but keeps index 0 valid */
    cache_max = 0;
}

/* grow cache to at least indices 0..new_cap (new_cap >= cache_cap+1) */
static void cache_grow(size_t new_cap)
{
    if (new_cap <= cache_cap) return;

    /* allocate new array with zeros */
    seq_t *tmp = alloc_seq_array(new_cap + 1);
    /* copy old values */
    for (size_t i = 0; i <= cache_cap; ++i) {
        tmp[i] = cache[i];
    }
    /* new entries are already zeroed by alloc_seq_array */

    /* free old cache */
    free_seq_array(cache);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (!dp_cur) init_arrays();
    if (n == 0) return (seq_t)0;   /* not used, offset is 1 */

    for (idx_t k = cache_max + 1; k <= n; ++k) {
        /* compute counts for next row pattern t */
        for (int t = 0; t < 32; ++t) {
            seq_t sum = (seq_t)0;
            for (int s = 0; s < 32; ++s) {
                if (allowed[s][t]) sum += dp_cur[s];
            }
            dp_nxt[t] = sum;
        }

        /* total number of arrays with height k+1 = k+1 rows (i.e., a(k)) */
        seq_t total = (seq_t)0;
        for (int t = 0; t < 32; ++t) total += dp_nxt[t];

        if ((size_t)k > cache_cap) cache_grow(k * 2);
        cache[k] = total;

        /* swap dp arrays for next iteration */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    cache_max = n;
    return cache[n];
}
