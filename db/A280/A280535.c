/*
 * A280535  Number of partitions of n into two parts with the smaller part
 *          squarefree and the larger part prime.
 * https://oeis.org/A280535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements A280535: a(n) = Σ_{i=1..⌊n/2⌋} μ(i)² · prime(n-i)
    (where μ(i)² is 1 iff i is square‑free, and prime(x) is 1 for prime x).
    The function works for any seq_t and idx_t supplied by the build. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* Static tables holding prime and square‑free info up to the largest n
   for which we have already sieved. */
static idx_t cached_limit = 0;      /* largest index that has been filled (inclusive) */
static bool *prime_cache   = NULL;  /* prime_cache[i] == true  iff i is prime  */
static bool *sqfree_cache  = NULL;  /* sqfree_cache[i] == true  iff i is square‑free */

/* Pre‑compute (and store) all primes and square‑free flags up to at least n.
   Re‑sieves from scratch when the cache must grow. */
static void
ensure_cache(idx_t n)
{
    if (n <= cached_limit)
        return;

    size_t new_limit = (size_t)(n + 1);   /* indices 0 .. n inclusive */

    prime_cache  = (bool *)realloc(prime_cache,  new_limit * sizeof(bool));
    sqfree_cache = (bool *)realloc(sqfree_cache, new_limit * sizeof(bool));
    if (!prime_cache || !sqfree_cache) {
        abort();   /* memory allocation failed */
    }

    /* Initialise all entries to true; then correct the obvious exceptions. */
    for (size_t i = 0; i < new_limit; ++i) {
        prime_cache[i]  = true;
        sqfree_cache[i] = true;
    }
    if (new_limit > 0) prime_cache[0]  = false;
    if (new_limit > 1) prime_cache[1]  = false;
    if (new_limit > 0) sqfree_cache[0] = false;   /* 0 is not square‑free */

    /* Sieve of Eratosthenes to populate primes, and simultaneously flag
       multiples of a prime square as not square‑free. */
    for (size_t i = 2; i * i < new_limit; ++i) {
        if (prime_cache[i]) {
            for (size_t j = i * i; j < new_limit; j += i)
                prime_cache[j] = false;

            size_t p2 = i * i;
            for (size_t j = p2; j < new_limit; j += p2)
                sqfree_cache[j] = false;
        }
    }

    cached_limit = new_limit - 1;   /* inclusive */
}

/* Return the n‑th term of A280535 (offset 1).  For n < 1 the value is
   irrelevant; the function returns 0. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Ensure prime and square‑free tables cover the range needed. */
    ensure_cache(n);

    seq_t ans   = 0;
    size_t lim  = (size_t)(n >> 1);   /* floor(n/2) */

    for (size_t i = 1; i <= lim; ++i) {
        if (sqfree_cache[i] && prime_cache[(size_t)(n - i)])
            ++ans;
    }

    return ans;
}
