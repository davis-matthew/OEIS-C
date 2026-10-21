/*
 * A260655  a(n) = 4*36^n*Gamma(n+3/2)/(sqrt(Pi)*(n+2)!).
 * https://oeis.org/A260655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n * Catalan(n+1) (Catalan(n) = binom(2n, n)/(n+1),
 *            Catalan(0)=1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, gcd, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/gcd.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

/* Ensure that cache contains values 0 … n */
static void ensure_cache(idx_t n)
{
    if (cache_len == 0) {
        cache_len = 1;
        cache = alloc_seq_array(1);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        return;
    }

    if (n + 1 <= cache_len)
        return;

    /* Allocate a larger array */
    seq_t *new_cache = alloc_seq_array(n + 1);

    /* Copy existing values */
    for (idx_t i = 0; i < cache_len; ++i)
        new_cache[i] = cache[i];

    /* Compute new terms using the exact recurrence
       a(n) = a(n-1) * 18*(2n+1)/(n+2)     (n >= 1) */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = new_cache[i - 1];
        seq_t den   = (seq_t)(i + 2);
        seq_t num   = (seq_t)18 * (seq_t)(2 * i + 1);

        /* Reduce the fraction to avoid overflow */
        seq_t g = gcd(prev, den);
        prev /= g;
        den  /= g;

        g = gcd(num, den);
        num /= g;
        den /= g;  /* den should now be 1 */

        new_cache[i] = prev * num;   /* exact product fits in seq_t */
    }

    seq_free_array(cache);
    cache = new_cache;
    cache_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
