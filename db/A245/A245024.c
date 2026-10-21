/*
 * A245024  Even numbers n for which lpf(n-1) < lpf(n-3), where lpf = least
 *          prime factor.
 * https://oeis.org/A245024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = even even integer n such that lpf(n-1) < lpf(n-3) (OEIS
 *            A245024). The sequence is memoised; successive calls cause
 *            on‑demand generation of further terms
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/lpf.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;         /* stored terms */
static size_t cache_cap = 0;        /* allocated capacity of cache    */
static idx_t  cache_len = 0;        /* number of terms already cached */
static idx_t  next_even = 10;       /* smallest even number not yet tested */

/* Return the nth term (1‑based index).  Behaviour for n < 1 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety check – not used */

    /* generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t x   = next_even;             /* candidate even integer   */
        idx_t lpf1 = lpf(x - 1);           /* least prime factor of x-1 */
        idx_t lpf2 = lpf(x - 3);           /* least prime factor of x-3 */

        if (lpf1 < lpf2) {                /* qualifies for the sequence? */
            /* enlarge cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) abort();         /* out of memory */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)x; /* store the term          */
        }
        next_even += 2;                    /* move to next even integer */
    }
    return cache[n - 1];                   /* terms are 1‑indexed */
}
