/*
 * A343541  For n > 1, a(n) is the largest base b <= prime(n)-1 such that the
 *          digits of prime(n)-1 in base b contain the digit b-1.
 * https://oeis.org/A343541
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 8110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_max_digit_idx, sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A343541
   Formula: a(n) = max{ b ≤ prime(n)-1 | the base‑b representation of prime(n)-1 contains the digit b-1 }.
   Offset: 2   (a(2) is the first term).
   The implementation uses a cached sieve to obtain prime(n), an integer
   square root routine, and a digit test that runs in O(√prime(n)). */

/* Includes required for the provided helpers and standard types */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Helper for sieving the first n primes */
#include "../utils/sieve_first_n_primes.h"
#include "../utils/has_max_digit_idx.h"

/* Integer square root for idx_t (uint64_t) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* Cached array of the first primes */
static uint32_t *primes_cache = NULL;
static size_t primes_count = 0;
static idx_t primes_limit = 0;

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for offset < 2 */

    /* Update cached primes if needed */
    if (n > primes_limit) {
        free(primes_cache);
        sieve_first_n_primes(n, &primes_cache, &primes_count);
        primes_limit = n;
    }

    /* Retrieve prime(n) and compute the value to analyze */
    seq_t prime_seq = (seq_t)primes_cache[n - 1];
    idx_t prime = (idx_t)prime_seq;      /* prime(n) */
    idx_t num = prime - 1;               /* prime(n)-1 */

    /* Bound from the OEIS observation: a(n) ≤ (1+√(4*prime-3))/2 */
    idx_t v = 4ull * prime - 3ull;
    idx_t s = isqrt_u64(v);
    idx_t bound = (1 + s) / 2;
    if (bound < 2) bound = 2;

    /* Search downwards from the bound for the largest suitable base */
    for (idx_t b = bound; b >= 2; --b) {
        if (has_max_digit_idx(num, b))
            return (seq_t)b;
    }

    /* Fallback – base 2 always contains digit 1 = 2-1 */
    return (seq_t)2;
}
