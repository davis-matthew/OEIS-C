/*
 * A343672  a(0) = 1; a(n) = 2 * n * a(n-1) + Sum_{k=0..n-1} binomial(n,k) *
 *          a(k).
 * https://oeis.org/A343672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>0, a(n)=2*n*a(n-1)+Σ_{k=0}^{n-1} binomial(n,k)*a(k) */

#include <stdlib.h>
#include <stdbool.h>

/* dynamic cache for computed values and a flag array indicating initialized entries */
static seq_t *cache = NULL;
static bool *seen  = NULL;
static size_t cap  = 0;

/* ensure cache and seen arrays accommodate indices up to n (inclusive) */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (cap < needed) {
        size_t newcap = (cap == 0 ? 16 : cap << 1);
        if (newcap < needed) newcap = needed;

        seq_t *newc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        bool *news  = (bool  *)realloc(seen , newcap * sizeof(bool ));
        if (!newc || !news) abort();          /* allocation failure */

        cache = newc;
        seen  = news;
        /* initialise newly allocated slots as "not seen" */
        for (size_t i = cap; i < newcap; ++i) seen[i] = false;
        cap = newcap;
    }
}

/* compute a(n) with memoisation */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (seen[n]) return cache[n];

    if (n == 0U) {
        cache[0] = (seq_t)1;
        seen[0]  = true;
        return cache[0];
    }

    /* ensure a(n-1) is ready */
    if (!seen[n-1]) a(n-1);

    seq_t sum = (seq_t)0;
    seq_t bin = (seq_t)1; /* binomial(n,0) */

    for (idx_t k = 0; k < n; ++k) {
        sum += bin * cache[k];
        if (k + 1 < n) {
            bin = bin * (seq_t)(n - k);     /* bin *= (n-k) */
            bin = bin / (seq_t)(k + 1);     /* bin /= (k+1) */
        }
    }

    seq_t term = (seq_t)2 * (seq_t)n * cache[n - 1];
    cache[n]  = term + sum;
    seen[n]   = true;
    return cache[n];
}
