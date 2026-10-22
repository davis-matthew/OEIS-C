/*
 * A347447  Number of strict factorizations of n with alternating product > 1.
 * https://oeis.org/A347447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A347447
    Formula: a(n) = number of strict factorizations of n with odd length
    such that the alternating product (y1*y3*…)/(y2*y4*…) > 1.
    The sequence is computed by recursion over all increasing sequences
    of distinct integers > 1 whose product is n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* internal recursion: count factorizations of 'rest'
   with previous factor 'prev', current length 'len',
   numerator and denominator of the alternating product.
   Result is added to '*cnt'. */
static void dfs(uint64_t rest,
                uint64_t prev,
                idx_t len,
                seq_t numer,
                seq_t denom,
                seq_t *cnt)
{
    /* end of factorization: take 'rest' itself as the next factor */
    if (rest > prev) {
        seq_t nnum = numer;
        seq_t nden = denom;
        if ((len & 1) == 0)   /* next factor is odd‑positioned */
            nnum = (seq_t)(nnum * (seq_t)rest);
        else                  /* even position */
            nden = (seq_t)(nden * (seq_t)rest);

        if ( ((len + 1) & 1) && nnum > nden )
            (*cnt)++;
    }

    /* try proper divisors of 'rest' that are larger than 'prev' */
    for (uint64_t d = prev + 1; d * d <= rest; ++d) {
        if (rest % d == 0) {
            seq_t nnum   = numer;
            seq_t nden   = denom;
            if ((len & 1) == 0)
                nnum = (seq_t)(nnum * (seq_t)d);
            else
                nden = (seq_t)(nden * (seq_t)d);

            uint64_t next = rest / d;
            if (next == 1) {
                /* This factorization ends here */
                if ( ((len + 1 + 1) & 1) && nnum > nden )
                    (*cnt)++;
            } else {
                dfs(next, d, len + 1, nnum, nden, cnt);
            }
        }
    }
}

/* cache for memoization */
static seq_t  *cache      = NULL;
static unsigned char *cached = NULL;
static idx_t   cache_size = 0;

static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        size_t new_sz = (size_t)n + 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        unsigned char *new_cached = (unsigned char*)realloc(cached, new_sz);
        if (!new_cache || !new_cached) abort();   /* allocation failure */

        cache   = new_cache;
        cached  = new_cached;
        for (idx_t i = cache_size; i <= n; ++i)
            cached[i] = 0;   /* not computed yet */
        cache_size = new_sz;
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 0;

    ensure_cache(n);
    if (cached[n]) return cache[n];

    seq_t cnt = 0;
    dfs((uint64_t)n, 1, 0, (seq_t)1, (seq_t)1, &cnt);

    cache[n]   = cnt;
    cached[n]  = 1;
    return cnt;
}
