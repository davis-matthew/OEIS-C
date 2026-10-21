/*
 * A277592  Numbers k such that k/10^m == 5 mod 10, where 10^m is the greatest
 *          power of 10 that divides n.
 * https://oeis.org/A277592
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence of integers with last non‑zero decimal digit 5, i.e.
 * numbers k such that k / 10^m == 5 (mod 10), where 10^m is the
 * greatest power of 10 dividing k.  The terms are listed in increasing
 * order; this routine returns the n‑th such term (n ≥ 1). */
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Local cache of already computed terms. */
    static seq_t *cache      = NULL;   /* pointer to the cached array */
    static size_t cache_len  = 0;      /* how many terms we have stored */
    static size_t cache_cap  = 0;      /* current capacity of the array */
    /* The next integer to test for the property. */
    static seq_t cur = 1;

    /* If n is 0 or less, the behaviour is undefined; return 0
       so that callers that make invalid requests do not crash. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t val = cur;
        /* Find the next integer whose last non‑zero digit is 5. */
        for (;;) {
            seq_t t = val;
            while (t % 10 == 0)
                t /= 10;
            if (t % 10 == 5)
                break;
            ++val;
        }

        /* Store the term in the cache, expanding the array if
           necessary.  The array grows in powers of two for
           efficiency. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
            seq_t *tmp;
            if (cache_cap == 0)
                tmp = seq_alloc(new_cap);
            else
                tmp = seq_realloc(cache, cache_cap, new_cap);
            if (!tmp) /* allocation failure → abort */
                abort();
            cache      = tmp;
            cache_cap  = new_cap;
        }

        cache[cache_len++] = val;
        /* Advance the scanning position for the next search. */
        cur = val + 1;
    }

    return cache[(size_t)n - 1];
}
