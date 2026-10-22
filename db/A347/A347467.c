/*
 * A347467  Numbers h such that floor(k*sqrt(3)) = floor(h*sqrt(2)) for some
 *          k.
 * https://oeis.org/A347467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A347467:  a(n) = the n-th integer h such that 
   floor(k * sqrt(3)) = floor(h * sqrt(2)) for some k.  
   Each h occurs exactly once.
   The function computes terms on demand and memoises them in a static
   cache so that calls are O(1) apart from generation time.
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- integer square‑root for an unsigned type seq_t ---------- */
static seq_t
sqrt_floor(seq_t x)
{
    /* binary search in [0, x] – works for seq_t up to 128 bits */
    seq_t lo = 0;
    seq_t hi = x;
    while (lo < hi) {
        seq_t mid = lo + ((hi - lo + 1) >> 1);   /* ceil((lo+hi)/2) */
        if ((unsigned __int128)mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* ---------- cache and generators ---------- */
static seq_t *cache      = NULL;   /* stored a(i) values           */
static idx_t   cache_len = 0;      /* number of values in cache    */
static idx_t   cache_cap = 0;      /* allocated cache capacity     */

/* Beatty sequence generators:  k for sqrt(3), h for sqrt(2) */
static idx_t   k_gen = 0;  /* current k index (1‑based)    */
static seq_t   k_val = 0;  /* floor(k*sqrt(3))             */
static idx_t   h_gen = 0;  /* current h index (1‑based)    */
static seq_t   h_val = 0;  /* floor(h*sqrt(2))             */

/* ---------- ensure cache capacity ---------- */
static void
ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = needed + 16;              /* growth strategy  */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();
    cache     = tmp;
    cache_cap = new_cap;
}

/* ---------- Advance Beatty generators to next values ---------- */
#define NEXT_K   \
    do { k_gen++; \
        k_val = sqrt_floor((seq_t)((unsigned __int128)k_gen * k_gen * 3u)); \
    } while (0)

#define NEXT_H   \
    do { h_gen++; \
        h_val = sqrt_floor((seq_t)((unsigned __int128)h_gen * h_gen * 2u)); \
    } while (0)

/* ---------- Main function ---------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                    /* not used, safety value    */

    /* Initialise cache and generators on the first request   */
    if (cache == NULL) {
        cache_cap = (n > 64) ? n : 64;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            abort();
        cache_len = 0;

        k_gen = 1;            /* first symbol k = 1    */
        k_val = sqrt_floor((seq_t)((unsigned __int128)1 * 1 * 3u));

        h_gen = 1;            /* first symbol h = 1    */
        h_val = sqrt_floor((seq_t)((unsigned __int128)1 * 1 * 2u));
    }

    /* Generate terms until we reach the requested index     */
    while (cache_len < n) {
        /* Bring the two Beatty values into relative order   */
        while (h_val < k_val)
            NEXT_H;           /* advance h to catch up or overjump   */
        while (k_val < h_val)
            NEXT_K;           /* advance k to catch up or overjump   */

        /* If they match, record the h value                 */
        if (h_val == k_val) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = h_gen;
            /* move both generators forward to next possible pair */
            NEXT_K;
            NEXT_H;
        }
    }

    return cache[n - 1];
}
