/*
 * A252091  Primes p such that p + 34 is prime.
 * https://oeis.org/A252091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 930, u32 n <= 211, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_candidate, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p+34 is prime */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;
static idx_t prime_start = 2;

/* Ensure that seq_cache contains at least n terms with the required property. */
static void
ensure_cache(idx_t n)
{
    while (seq_cache_len < n) {
        /* Find the next prime number. */
        while (!is_prime(prime_start)) {
            prime_start++;
        }
        seq_t p = (seq_t)prime_start;
        /* Check if p + 34 is also prime. */
        if (is_prime_big(p + 34)) {
            append_candidate(&seq_cache, &seq_cache_len, &seq_cache_cap, p);
        }
        prime_start++;          /* Advance to test the next candidate. */
    }
}

seq_t
a(idx_t n)
{
    if (n == 0) {
        return 0;            /* Undefined for offset 0; return 0 for safety. */
    }
    ensure_cache(n);
    return seq_cache[n - 1];
}
