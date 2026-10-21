/*
 * A298369  a(n) = a(n-1) + a(n-2) + 2*a(floor(n/2)) + 3*a(floor(n/3)) + ... +
 *          n*a(floor(n/n)), where a(0) = 1, a(1) = 1, a(2) = 1.
 * https://oeis.org/A298369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + Σ_{k=2..n} k * a(⌊n/k⌋), where
 *            a(0)=a(1)=a(2)=1
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed with iterative dynamic programming and memoised
   in a static array that grows as needed.  All arithmetic is performed
   directly on seq_t; no temporary casts are required except when
   converting the integer loop index into seq_t.  The algorithm runs in
   O(n²) time and O(n) space, which satisfies the performance
   requirement for n up to about 10 000. */

#include <stddef.h>       /* size_t */
#include <stdlib.h>       /* realloc, abort */

/* Cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;     /* number of entries allocated */
static idx_t  max_computed = 2;  /* highest index whose value is known in cache */

/* Ensure that cache can hold indices up to idx (inclusive). */
static void ensure_capacity(idx_t idx)
{
    /* No growth needed */
    if ((size_t)idx + 1 <= cache_cap) { return; }

    /* Compute new capacity: double previous or just enough for idx */
    size_t new_cap = cache_cap ? cache_cap * 2 : (size_t)idx + 1;
    if (new_cap <= (size_t)idx) new_cap = (size_t)idx + 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();           /* allocation failure */

    /* Initialize newly allocated memory to zero */
    size_t old_cap = cache_cap;
    cache = new_cache;
    cache_cap = new_cap;
    for (size_t i = old_cap; i < new_cap; ++i) cache[i] = 0;

    /* Initialise base cases if the cache was just created or was too small
       to contain them. */
    if (new_cap >= 3) {
        cache[0] = cache[1] = cache[2] = (seq_t)1;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 2) {                 /* a(0) = a(1) = a(2) = 1 */
        return (seq_t)1;
    }

    /* Make sure the cache can hold the requested index */
    ensure_capacity(n);

    /* Compute missing terms up to n */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* a(i) = a(i-1) + a(i-2) initially */
        seq_t sum = cache[i - 1] + cache[i - 2];

        /* Add the summation part: Σ_{k=2..i} k * a(⌊i/k⌋) */
        for (idx_t k = 2; k <= i; ++k) {
            seq_t val = cache[i / k];
            sum += (seq_t)k * val;
        }

        cache[i] = sum;
    }

    max_computed = n;
    return cache[n];
}
