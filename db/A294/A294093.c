/*
 * A294093  Sum of the differences of the larger and smaller parts in the
 *          partitions of 2n into two parts with the larger part squarefree
 *          and smaller part prime.
 * https://oeis.org/A294093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{i=1..n} (n-i) * A010051(i) * A008966(2n-i) where
 *            A010051(i) is 1 if i is prime, otherwise 0, and A008966(m) is 1
 *            if m is squarefree, otherwise 0
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes all primes and squarefree numbers up to 2n
   on demand and re‑uses the cached tables.  Each call runs in
   O(n) time for the current n. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached primality and squarefree information.
   prime_cache[i] == true  ⇔  i is prime
   squarefree_cache[i] == true  ⇔  i is squarefree   */
static bool *prime_cache = NULL;
static bool *squarefree_cache = NULL;
static idx_t cache_limit = 0;   /* maximum value currently cached */

/* Grow caches so that indices up to 'limit' (inclusive) are valid. */
static void ensure_cache(idx_t limit)
{
    if (limit <= cache_limit)
        return;

    /* Allocate new arrays large enough for all indices up to 'limit'. */
    size_t new_size = (size_t)(limit + 1);
    bool *new_prime = (bool *)realloc(prime_cache, new_size * sizeof(bool));
    if (!new_prime)
        exit(EXIT_FAILURE);
    bool *new_squarefree = (bool *)realloc(squarefree_cache, new_size * sizeof(bool));
    if (!new_squarefree)
        exit(EXIT_FAILURE);

    prime_cache = new_prime;
    squarefree_cache = new_squarefree;

    /* Initialise all entries ≥ 2 as prime/squarefree; we will correct
       them below.  No need to touch entries 0 and 1 yet. */
    for (idx_t i = 2; i <= limit; ++i) {
        prime_cache[i] = true;
        squarefree_cache[i] = true;
    }

    /* Sieve of Eratosthenes for primes up to 'limit'. */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!prime_cache[i])
            continue;
        for (idx_t j = i * i; j <= limit; j += i)
            prime_cache[j] = false;
    }

    /* Mark non‑squarefree numbers: multiples of a prime square. */
    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!prime_cache[p])
            continue;
        idx_t sq = p * p;
        for (idx_t m = sq; m <= limit; m += sq)
            squarefree_cache[m] = false;
    }

    /* 0 and 1 are not prime.  1 is squarefree by definition. */
    prime_cache[0] = prime_cache[1] = false;
    squarefree_cache[0] = squarefree_cache[1] = true;

    cache_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Ensure tables cover indices up to 2n. */
    ensure_cache(2 * n);

    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (!prime_cache[i])
            continue;
        idx_t m = 2 * n - i;
        if (!squarefree_cache[m])
            continue;
        sum += (seq_t)(n - i);
    }
    return sum * 2;
}
