/*
 * A270226  a(n) is the number of terms in the n-th block of consecutive
 *          integers of A136119.
 * https://oeis.org/A270226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor(n*sqrt(2)+1/sqrt(2)) - floor((n-1)*sqrt(2)+1/sqrt(2)) + 1
 * Recurrence: a(1) = 1, a(n+1) = a(n) + a(n-1) with a(2)=3, a(3)=2
 * This implementation generates the fixed point of the substitution
 * σ: 2 → 32, 3 → 322.  The infinite word of symbols {2,3} encodes
 * the term a(n) for n ≥ 2 as the (n‑1)‑th symbol of that word.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache   = NULL;   /* word of symbols {2,3} */
static size_t cache_len = 0;    /* number of symbols stored */

static void expand_cache(idx_t min_len)
{
    /* Expand until the cache contains at least min_len symbols. */
    while (cache_len < (size_t)min_len)
    {
        size_t new_len = cache_len ? cache_len * 3 : 2;
        if (cache_len == 0)     /* initial word: 2 → 32 */
        {
            new_len = 2;
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
            if (!cache) return;          /* allocation failed – caller must handle */
            cache[0] = 3;                /* first symbol after one substitution */
            cache[1] = 2;
            cache_len = 2;
            continue;
        }

        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_cache) return;          /* allocation failed – caller must handle */

        size_t i, pos = 0;
        for (i = 0; i < cache_len; ++i)
        {
            if (cache[i] == 2)
            {
                new_cache[pos++] = 3;
                new_cache[pos++] = 2;
            }
            else /* cache[i] == 3 */
            {
                new_cache[pos++] = 3;
                new_cache[pos++] = 2;
                new_cache[pos++] = 2;
            }
        }

        free(cache);
        cache = new_cache;
        cache_len = pos;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 0, but safe */
    if (n == 1) return 1;        /* first block length */

    /* For n ≥ 2, a(n) equals the (n-1)-th symbol of the infinite word. */
    expand_cache(n - 1);                       // ensure at least n-1 symbols
    return cache[(size_t)(n - 2)];             // word is zero‑indexed
}
