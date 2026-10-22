/*
 * A360661  Number of factorizations of n into a prime number of factors > 1.
 * https://oeis.org/A360661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of multiplicative factorizations of n into a prime
 * number of factors > 1. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* Cache size – enough for all values used in the tests (up to about 10000). */
#define MAX_CACHE 10000

/* Static cache of computed values and a flag array.  For C++ big‑int types
 * this allocates the objects at program start, which is fine for the
 * relatively small cache size. */
static seq_t cache[MAX_CACHE + 1];
static bool cached[MAX_CACHE + 1];

/* Test whether a given length is prime.  We only need primes up to
 * the maximum possible number of factors for n <= MAX_CACHE, which
 * is log₂(MAX_CACHE) ≈ 13; the list is a tiny constant array. */
static bool is_prime_len(int len)
{
    static const int primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97
    };
    size_t i;
    for (i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        if (primes[i] == len) return true;
        if (primes[i] > len) break;
    }
    return false;
}

/* Recursive helper: count the factorizations of n into factors >= min_factor.
 * `len` is the number of factors already chosen.
 * When n == 1 the current factorization is finished; it counts as 1
 * if `len` is prime, otherwise 0. */
static seq_t count_factorizations(uint64_t n, uint64_t min_factor, int len)
{
    if (n == 1) {
        return is_prime_len(len) ? (seq_t)1 : (seq_t)0;
    }

    seq_t total = 0;
    for (uint64_t d = min_factor; d <= n; ++d) {
        if (n % d == 0) {
            total += count_factorizations(n / d, d, len + 1);
        }
    }
    return total;
}

/* Public interface: return a(n).  Uses a cache for n <= MAX_CACHE. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) is out of the OEIS offset; return 0 safely. */
        return (seq_t)0;
    }

    if (n <= MAX_CACHE) {
        if (!cached[n]) {
            cache[n] = count_factorizations((uint64_t)n, 2, 0);
            cached[n] = true;
        }
        return cache[n];
    }

    /* For values beyond the cache size we compute directly. */
    return count_factorizations((uint64_t)n, 2, 0);
}
