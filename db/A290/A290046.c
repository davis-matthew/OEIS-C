/*
 * A290046  a(n) = |Sum_{k=0..n} (-1)^k*(3k)!!|.
 * https://oeis.org/A290046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = |Sum_{k=0..n} (-1)^k * (3k)!!|
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* 3k double factorial table: D[i] = i!!  (i = 0 .. 3*n)   */
    static seq_t *D = NULL;
    static size_t D_len = 0;      /* number of computed entries */
    static size_t D_cap = 0;      /* allocated capacity */

    /* Cache of already computed a(k) to support arbitrary call order */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* allocated capacity of cache */
    static idx_t cache_max = (idx_t)-1; /* largest n for which cache[n] is known */

    /* running sums of positive / negative terms (only used up to the cached maximum) */
    static seq_t pos_sum = 0;
    static seq_t neg_sum = 0;

    /* ------------------------------------------------------------------ */
    /* initialise double factorial table on first call                     */
    if (D == NULL) {
        D_cap = 1;
        D_len = 1;
        D = (seq_t*)malloc(D_cap * sizeof(seq_t));
        if (D == NULL) return 0;            /* out of memory – caller will not handle this case */
        D[0] = (seq_t)1;
    }

    /* extend table to cover up to 3*n */
    size_t need_len = 3 * n + 1;
    if (need_len > D_len) {
        if (need_len > D_cap) {
            size_t new_cap = need_len * 2;
            if (new_cap < need_len) new_cap = need_len;   /* avoid overflow */
            seq_t *tmp = (seq_t*)realloc(D, new_cap * sizeof(seq_t));
            if (tmp == NULL) return 0;                /* out of memory */
            D = tmp;
            D_cap = new_cap;
        }
        /* compute new double factorial entries */
        for (size_t i = D_len; i < need_len; ++i) {
            if (i == 0 || i == 1) {
                D[i] = (seq_t)1;
            } else {
                D[i] = D[i - 2] * (seq_t)i;
            }
        }
        D_len = need_len;
    }

    /* ------------------------------------------------------------------ */
    /* initialise cache on first call                                    */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;
        cache[0] = (seq_t)1;    /* a(0) = 1 */
        cache_max = 0;
        pos_sum = (seq_t)1;     /* term k=0 is positive */
        neg_sum = (seq_t)0;
    }

    /* extend cache array if needed */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;
        cache = tmp;
        cache_len = new_len;
    }

    /* ------------------------------------------------------------------ */
    /* compute any new values that are not already cached                 */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t term = D[3 * (size_t)k];   /* (3k)!!  from the table */

        if ((k & 1ULL) == 0) {          /* even k -> positive sign */
            pos_sum += term;
        } else {
            neg_sum += term;           /* odd k -> negative sign */
        }

        /* absolute value of the running sum */
        seq_t val;
        if (pos_sum >= neg_sum)
            val = pos_sum - neg_sum;
        else
            val = neg_sum - pos_sum;

        cache[k] = val;
    }
    cache_max = n;

    return cache[n];
}
