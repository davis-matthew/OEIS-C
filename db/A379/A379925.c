/*
 * A379925  Numbers k for which nonnegative integers x and y exist such that
 *          x^2 + y^2 = k and x + y is a square.
 * https://oeis.org/A379925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A379925: numbers k for which ∃ non‑negative integers x,y 
   such that x² + y² = k and x + y is a perfect square.  
   The code enumerates all pairs (x,y) obtained from squares S², computes
   k = x² + y², keeps all distinct k values in sorted order and returns
   the n‑th smallest (a(1) = 0, a(2) = 1, ...).
   The algorithm is O(S² log S²) for the maximal S used, with S chosen to cover
   at least the required number of terms.  For n ≤ 10 000 S ≤ 200 gives
   far more terms than needed, so the generation is fast.
*/
#include <stdlib.h>
#include <stdbool.h>

static int seq_compare(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    if (a < b) return -1;
    if (a > b) return  1;
    return 0;
}

/*
   `cache` holds the sorted list of distinct k values.
   `cache_len` is the number of values currently stored.
   `max_s` is the largest S (= sqrt(x+y)) that has been processed so far.
   When a request for a larger n arrives, the algorithm increases `max_s`
   (in steps of 200) until enough values are collected.
*/
static seq_t  *cache      = NULL;
static size_t  cache_len  = 0;
static size_t  cache_cap  = 0;
static idx_t   max_s      = 0;   /* last processed S value       */

static void rebuild_cache(idx_t up_to_s)
{
    /* free old data */
    free(cache);
    cache = NULL;
    cache_len = 0;
    cache_cap = 0;
    max_s = up_to_s;

    /* temporary buffer for all candidate k's (might contain duplicates) */
    size_t cand_len = 0, cand_cap = 0;
    seq_t *cand = NULL;

    for (idx_t s = 0; s <= up_to_s; ++s) {
        idx_t s2 = s * s;
        for (idx_t x = 0; x <= s2; ++x) {
            idx_t y = s2 - x;
            seq_t k = (seq_t)x * (seq_t)x + (seq_t)y * (seq_t)y;

            /* store k in temporary array */
            if (cand_len == cand_cap) {
                size_t new_cap = cand_cap ? cand_cap << 1 : 16;
                cand = (seq_t *)realloc(cand, new_cap * sizeof(seq_t));
                cand_cap = new_cap;
            }
            cand[cand_len++] = k;
        }
    }

    /* sort and deduplicate */
    qsort(cand, cand_len, sizeof(seq_t), seq_compare);

    size_t uniq = 0;
    for (size_t i = 0; i < cand_len; ++i) {
        if (uniq == 0 || cand[i] != cand[uniq-1]) {
            cand[uniq++] = cand[i];
        }
    }

    /* keep only the unique sorted values in the public cache */
    cache_len = uniq;
    cache_cap = uniq;
    cache = cand;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* sequence is defined for n≥1 only */

    /* initialise cache on the first call */
    if (cache_len == 0) {
        rebuild_cache(200);  /* start with S up to 200, more than enough for n≈10⁴ */
    }

    /* if request is beyond current cache length, extend the search */
    while (n > cache_len) {
        rebuild_cache(max_s + 200);
    }

    return cache[n - 1];
}
