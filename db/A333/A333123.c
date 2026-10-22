/*
 * A333123  Consider the mapping k -> (k - (k/p)), where p is any of k's prime
 *          factors. a(n) is the number of different possible paths from n to
 *          1.
 * https://oeis.org/A333123
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(1) = 1; for n > 1, a(n) = Σ_{p|n, p prime} a(n - n/p)
 * Range:     u8 n <= 153, u16 n <= 1644, u32 n <= 8989, u64 n <= 8989,
 *            u128 n <= 8989, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the path‑count recurrence with memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Memoisation cache: cache[n] holds a(n).  cache[0] is unused. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure the cache can hold indices up to n. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap) return;

    size_t old_cap = cache_cap;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp) abort();          /* out of memory */
    cache = tmp;

    /* initialise newly allocated entries to zero */
    for (size_t i = old_cap; i <= new_cap; ++i)
        cache[i] = 0;

    cache_cap = new_cap;
}

/* Count distinct prime–factor paths from n down to 1. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;              /* undefined part of the sequence */

    ensure_cache(n);
    if (cache[n] != 0 || n == 1) {     /* already computed or base case */
        if (n == 1) cache[1] = 1;
        return cache[n];
    }

    seq_t sum = 0;
    idx_t tmp = n;

    /* trial division to locate distinct prime factors */
    for (idx_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            idx_t m = n - n / p;
            sum += a(m);
            while (tmp % p == 0) tmp /= p;   /* strip all copies of p */
        }
    }

    /* remaining prime factor, if any */
    if (tmp > 1) {
        idx_t m = n - n / tmp;
        sum += a(m);
    }

    cache[n] = sum;
    return sum;
}
