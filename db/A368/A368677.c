/*
 * A368677  Number of numbers k less than n and not dividing n such that n-k
 *          is squarefree.
 * https://oeis.org/A368677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ k | 1 ≤ k < n , n % k ≠ 0 , n-k squarefree }
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is memoised on an array of flags that marks squarefree
 * integers up to the largest n encountered so far.  For small n
 * (≤ 10 000 as in the tests) this uses negligible memory and runs in
 * O(n).  The routine compiles under both C11 and C++17. */
#include <stdlib.h>
#include <stdbool.h>

/* Cache of squarefree flags: cache[i] == 1 iff i is squarefree. */
static unsigned char *squarefree_cache = NULL;
static idx_t squarefree_limit = 0;

/* Rebuild the cache up to 'limit' (inclusive). */
static void recompute_squarefree(idx_t limit)
{
    /* Allocate a new buffer. */
    unsigned char *new_cache =
        (unsigned char *)malloc((size_t)(limit + 1) * sizeof(unsigned char));
    if (!new_cache)
        exit(1);                       /* allocation failure */

    /* Initially assume all integers are squarefree. */
    for (idx_t i = 0; i <= limit; ++i)
        new_cache[i] = 1;

    /* Mark multiples of squares as non‑squarefree. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        idx_t sq = i * i;
        for (idx_t j = sq; j <= limit; j += sq)
            new_cache[j] = 0;
    }

    /* Replace the old cache with the new one. */
    free(squarefree_cache);
    squarefree_cache = new_cache;
    squarefree_limit = limit;
}

/* Ensure the cache contains information up to 'limit'. */
static inline void ensure_squarefree(idx_t limit)
{
    if (squarefree_limit >= limit)
        return;            /* already sufficient */
    recompute_squarefree(limit);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;               /* no k with 1 ≤ k < n */

    /* We need squarefree data up to n-1. */
    ensure_squarefree(n - 1);

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        if ((n % k) != 0 && squarefree_cache[n - k])
            ++count;
    }
    return count;
}
