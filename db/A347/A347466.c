/*
 * A347466  Number of factorizations of n^2.
 * https://oeis.org/A347466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 2519, u32 n <= 9283, u64 n <= 9283,
 *            u128 n <= 9283, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A347466: Number of factorizations of n^2.
    Formula: a(n) = number of unordered multiplicative partitions of n^2
*/
#include <stdint.h>
#include <stdlib.h>

/* Cache to store already computed values; size is 10001 to handle
   indices up to 10000 as required by the performance constraint. */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Recursively count factorizations of 'rem' into integers >= 'start'.
   The recursion counts the trivial factorization using 'rem' itself
   by adding one before exploring proper divisors. */
static seq_t dfs_factor(uint64_t rem, uint64_t start)
{
    if (rem == 1)                      /* complete factorization */
        return 1;

    seq_t count = 1;                    /* choose 'rem' as a single factor */

    for (uint64_t d = start; d <= rem / d; ++d) {
        if (rem % d == 0) {
            uint64_t sub = rem / d;
            count += dfs_factor(sub, d);
        }
    }
    return count;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined, but avoid crash */

    /* Initialise cache once. */
    if (!cache) {
        cache_size = 10001;             /* indices 0..10000 inclusive */
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
    }

    if (n < cache_size && cache[n] != 0)
        return cache[n];

    if (n == 1)                        /* n^2 = 1 has only the empty factorization */
        return cache[1] = 1;

    /* Compute n^2 using a wider integer type to avoid overflow. */
    uint64_t square = (uint64_t)n * (uint64_t)n;

    /* Count factorizations of square. */
    seq_t result = (seq_t)dfs_factor(square, 2);

    if (n < cache_size)
        cache[n] = result;

    return result;
}
