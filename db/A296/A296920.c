/*
 * A296920  Rational primes that decompose in the quadratic field
 *          Q(sqrt(-11)).
 * https://oeis.org/A296920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth prime p such that p mod 22 ∈ {1,3,5,9,15}
 * Range:     u8 n <= 24, u16 n <= 2572, u32 n <= 2592, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   good_residue, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "../utils/is_prime_big.h"
#include "../utils/good_residue.h"

/* Cached primes that satisfy the residue condition. */
static uint64_t *primes = NULL;
static size_t primes_cap = 0;   /* allocated capacity in elements */
static size_t primes_len = 0;   /* number of primes stored */

/* Last integer that was tested (for linear search). */
static uint64_t last_checked = 2;  /* starts at 2; next checked will be 3 */

/* Grow the cached array when needed. */
static void grow_cache(size_t need)
{
    if (need <= primes_cap)
        return;
    size_t new_cap = (need > 8) ? (need * 2) : 8;
    uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
    if (!tmp)                      /* allocation failure */
        abort();
    primes = tmp;
    primes_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    assert(n >= 1);  /* OEIS offset is 1 */

    /* Expand the cache until it contains at least n entries. */
    while (primes_len < (size_t)n)
    {
        /* Move to the next candidate. */
        last_checked++;
        if ((last_checked & 1ULL) == 0ULL)   /* skip evens */
            continue;

        /* Test primality. */
        if (!is_prime_big((seq_t)last_checked))
            continue;

        /* Test residue condition. */
        if (!good_residue(last_checked))
            continue;

        /* Cache the prime. */
        if (primes_len == primes_cap)
            grow_cache(primes_len + 1);
        primes[primes_len++] = last_checked;
    }

    /* Return the n‑th term (1‑based). */
    return (seq_t)primes[n-1];
}
