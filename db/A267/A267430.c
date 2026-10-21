/*
 * A267430  Squares whose digit sum is not a prime.
 * https://oeis.org/A267430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th square whose digit sum is NOT prime (OEIS
 *            A267430)
 * Range:     u8 n <= 13, u16 n <= 182, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* a helper to test primality of small integers (used for digit sums) */
static bool is_prime_small(unsigned int x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (unsigned int i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* cache of previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure the cache has at least need elements, growing as needed */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap < need) new_cap *= 2U;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/* compute digit sum of a seq_t value */
static unsigned int digit_sum_seq_t(seq_t x)
{
    unsigned int sum = 0;
    while (x > 0) {
        sum += (unsigned int)(x % 10);
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used, offset is 1 */

    /* grow cache until it contains at least n elements */
    if ((size_t)n > cache_len) {
        ensure_cache_cap((size_t)n);

        /* start from the next integer after the last computed value */
        idx_t start = 0;
        if (cache_len > 0) {
            /* to obtain the integer whose square corresponds to the last cached term,
               we don't need to recover the exact integer; we can simply start from
               the integer whose square is greater than the last cached square and
               increment.  For simplicity, start from 0 each time we extend the
               cache.  This incurs extra work but is acceptable for n <= 10000. */
            start = 0;
        }

        /* generate terms until we have n of them */
        size_t found = 0;
        for (idx_t i = 0; ; ++i) {
            seq_t sq = (seq_t)i * (seq_t)i;
            unsigned int sum = digit_sum_seq_t(sq);
            if (!is_prime_small(sum)) { /* digit sum is not prime */
                if (found < cache_len) {
                    /* we may be recomputing terms already cached; skip them */
                    ++found;
                    continue;
                }
                cache[cache_len++] = sq;
                ++found;
                if ((size_t)found >= n) break;
            }
        }
    }

    /* zero-based index in cache */
    return cache[(size_t)n - 1];
}
