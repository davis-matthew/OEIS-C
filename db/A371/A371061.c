/*
 * A371061  a(1)=1, a(2)=2; for n > 2, a(n) is the sum of the largest proper
 *          divisor of each of the previous two terms, except that the term
 *          itself is used if it has no proper divisors > 1.
 * https://oeis.org/A371061
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A371061: a(n) = A117818(a(n‑1)) + A117818(a(n‑2));
   a(1)=1, a(2)=2; for n>2, a(n) is the sum of the largest proper
   divisors of the previous two terms (using the term itself if it has
   no proper divisors >1).  For n ≥ 39 the sequence is periodic with
   period 18 (a(n+18)=a(n)). */
#include <stdlib.h>
#include "../utils/lpd.h"

/* cache for computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_size = 0;

/* ensure cache can hold 'needed' terms (1‑based count) */
static void ensure_cache(idx_t needed)
{
    if ((size_t)needed <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    while (new_cap < (size_t)needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined case – return 0 */

    /* periodic parameters */
    const idx_t PERIOD_BASE = 39;          /* first term of the period */
    const idx_t PERIOD_LEN  = 18;
    const idx_t PERIOD_END  = PERIOD_BASE + PERIOD_LEN - 1; /* 56 */

    /* handle indices within the non‑periodic range */
    if (n <= PERIOD_END) {
        if (cache_size < n) {
            ensure_cache(n);
            for (idx_t i = cache_size + 1; i <= n; ++i) {
                seq_t value;
                if (i == 1)
                    value = 1;
                else if (i == 2)
                    value = 2;
                else {
                    seq_t p1 = cache[(size_t)(i - 2)];   /* a(i‑1) */
                    seq_t p2 = cache[(size_t)(i - 3)];   /* a(i‑2) */
                    value = lpd(p1) + lpd(p2);
                }
                cache[(size_t)(i - 1)] = value;
            }
            cache_size = n;
        }
        return cache[(size_t)(n - 1)];
    }

    /* n > PERIOD_END: use the period.  Ensure we have all period terms. */
    if (cache_size < PERIOD_END)
        a(PERIOD_END);   /* fill the cache up to PERIOD_END */

    idx_t offset = (n - PERIOD_BASE) % PERIOD_LEN;
    idx_t idx = PERIOD_BASE + offset;   /* 1‑based index of the term */
    return cache[(size_t)(idx - 1)];
}
