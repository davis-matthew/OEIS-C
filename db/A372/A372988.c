/*
 * A372988  a(n) = (2n)!/(n!*a(n-2)), with a(0) = 1, a(1) = 1.
 * https://oeis.org/A372988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)!/(n!*a(n-2)) with a(0) = 1, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/gcd.h"

static seq_t *a_cache = NULL;          /* array holding a(0)..a(max) */
static size_t a_cache_capacity = 0;    /* currently allocated length   */
static size_t a_cache_len = 0;         /* computed entries 0..len-1    */

/* ensure the cache can hold indices 0..n */
static void ensure_a_cache(idx_t n)
{
    size_t need = (size_t)(n + 1);
    if (need <= a_cache_capacity)
        return;

    /* reallocates the cache, keeping existing values */
    seq_t *tmp = (seq_t *)realloc(a_cache, need * sizeof(seq_t));
    if (!tmp) {
        free(a_cache);
        abort();          /* allocation failed */
    }
    /* old data is already copied by realloc */
    a_cache = tmp;
    a_cache_capacity = need;
}

seq_t a(idx_t n)
{
    /* base cases */
    if (n < 2)
        return (seq_t)1;

    ensure_a_cache(n);

    /* compute missing terms gradually */
    for (size_t i = a_cache_len; i <= (size_t)n; ++i) {
        if (i < 2) {                  /* guard for initial indices */
            a_cache[i] = (seq_t)1;
            continue;
        }

        seq_t denom = a_cache[i - 2];
        seq_t prod  = (seq_t)1;

        idx_t start = (idx_t)i + 1;
        idx_t end   = (idx_t)(2 * i);

        for (idx_t k = start; k <= end; ++k) {
            seq_t tk = (seq_t)k;
            seq_t g  = gcd(tk, denom);
            denom  /= g;
            tk      /= g;
            prod    *= tk;
        }
        a_cache[i] = prod;   /* denom should be 1 at this point */
    }

    a_cache_len = (size_t)(n + 1);   /* all indices 0..n are now set */
    return a_cache[(size_t)n];
}
