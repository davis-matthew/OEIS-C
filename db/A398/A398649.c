/*
 * A398649  Numbers that can be obtained by concatenating the decimal
 *          representations of x, y, and x+y for x >= 1 and y >= 0.
 * https://oeis.org/A398649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   cmp_seq_t, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A398649:
   a(n) = the n-th smallest integer that can be obtained by
   concatenating the decimal representations of x, y and (x + y)
   where x ≥ 1 and y ≥ 0.  The terms are sorted and duplicates are removed.
*/

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e);           */
#include "../utils/digit_len.h"      /* int digit_len(seq_t x);        */
#include "../utils/cmp_seq_t.h"      /* int cmp_seq_t(const void *, const void *); */

static seq_t *cache      = NULL;     /* sorted unique terms               */
static size_t cache_len  = 0;        /* number of cached terms             */
static idx_t bound        = 20;      /* current x,y search bound           */

/* Build or extend the cache until it contains at least `target` terms. */
static void ensure_cache(idx_t target)
{
    if (cache_len >= (size_t)target)
        return;                      /* already enough terms                */

    /* Expand the search bound until enough terms are generated. */
    while (1) {
        /* Number of candidate pairs (x, y) with 1 ≤ x ≤ bound, 0 ≤ y ≤ bound. */
        size_t pair_cnt = (size_t)bound * (size_t)(bound + 1);
        seq_t *tmp = (seq_t *)malloc(pair_cnt * sizeof(seq_t));
        if (!tmp)
            abort();                  /* memory error                         */

        /* Generate all candidate terms for the current bound. */
        size_t k = 0;
        for (idx_t x = 1; x <= bound; ++x) {
            seq_t xs = (seq_t)x;
            for (idx_t y = 0; y <= bound; ++y) {
                seq_t ys      = (seq_t)y;
                seq_t sum     = xs + ys;
                int q         = (y == 0) ? 1 : digit_len(ys);
                int r         = digit_len(sum);
                seq_t val     = xs * pow10((idx_t)(q + r)) +
                                ys * pow10((idx_t)r) + sum;
                tmp[k++] = val;
            }
        }

        /* Sort the candidates. */
        qsort(tmp, k, sizeof(seq_t), cmp_seq_t);

        /* Deduplicate in-place. */
        size_t uniq = 0;
        for (size_t i = 0; i < k; ++i) {
            if (i == 0 || tmp[i] != tmp[uniq - 1]) {
                tmp[uniq++] = tmp[i];
            }
        }

        /* Replace old cache with the new unique terms. */
        seq_t *new_cache = (seq_t *)malloc(uniq * sizeof(seq_t));
        if (!new_cache)
            abort();
        memcpy(new_cache, tmp, uniq * sizeof(seq_t));

        free(cache);                 /* old cache is free'd */
        cache     = new_cache;
        cache_len = uniq;

        free(tmp);                    /* temporary array */

        if (cache_len >= (size_t)target)
            break;                    /* enough terms obtained   */

        bound <<= 1;                  /* double the bound         */
    }
}

/* Return the n-th term of A398649 (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined input – return 0 for safety */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
