/*
 * A296012  Primes of the form k + k+1 + k+2 +-1 where k, k+1, and k+2 are all
 *          composite numbers.
 * https://oeis.org/A296012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 4070, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, isPrimeGeneric, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A296012 (Primes of the form k+k+1+k+2±1
   where k, k+1, k+2 are all composite).  The implementation
   incrementally searches for suitable k and collects the primes
   that satisfy the condition.  The result is cached so that
   subsequent calls in any order are efficient. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/is_prime.h"          // bool is_prime(idx_t x)
#include "../utils/isPrimeGeneric.h"    // bool isPrimeGeneric(seq_t n)
#include "../utils/ensure_capacity.h"   // void ensure_capacity(size_t, size_t*, void**, size_t)

static seq_t *cache = NULL;      // Array of found primes
static size_t cache_len = 0;     // Current number of primes stored
static size_t cache_cap = 0;     // Current capacity of array
static uint64_t processed_k = 0; // Last k value processed

/* Append a prime to the cache, resizing as necessary */
static void
add_prime(seq_t p)
{
    if (cache_len == cache_cap)
    {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
        ensure_capacity(new_cap, &cache_cap, (void **)&cache, sizeof(seq_t));
    }
    cache[cache_len++] = p;
}

/* Extend the cache until it contains at least n elements */
static void
ensure_cache(idx_t n)
{
    while (cache_len < n)
    {
        /* Find the next k such that k and k+2 are composite */
        do
        {
            ++processed_k;
        } while (is_prime(processed_k) || is_prime(processed_k + 2));

        /* Candidates p1 = 3*k + 2 and p2 = 3*k + 4 */
        seq_t p1 = (seq_t)processed_k * (seq_t)3 + (seq_t)2;
        seq_t p2 = p1 + (seq_t)2;

        if (isPrimeGeneric(p1))
            add_prime(p1);
        if (isPrimeGeneric(p2))
            add_prime(p2);
    }
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          // Undefined below offset; harmless
    ensure_cache(n);
    return cache[n - 1];          // a(1) is the first element
}
