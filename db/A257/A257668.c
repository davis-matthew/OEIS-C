/*
 * A257668  Primes containing a digit 7.
 * https://oeis.org/A257668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 2910, u32 n <= 9717, u64 n <= 9717,
 *            u128 n <= 9717, big any n
 * Helpers:   contains_digit_7, ensure_cache, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns a(n), the n-th prime (n≥1) whose decimal representation
   contains the digit 7. The sequence is A257668 in OEIS. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/ensure_cache.h"
#include "../utils/contains_digit_7.h"

/* Static state to allow memoization and reuse across calls. */
static seq_t *primes_with_7 = NULL;      /* cache of found primes */
static size_t primes_with_7_len   = 0;    /* number of cached terms */
static size_t primes_with_7_cap   = 0;    /* capacity of the cache */
static idx_t tested_prime_index   = 0;    /* index of the next prime to test */

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* offset 1; behaviour for n<1 is undefined */

    /* Allocate or grow cache to hold at least n terms. */
    ensure_cache(n, &primes_with_7, &primes_with_7_cap);

    /* Generate primes until we have n terms that contain digit 7. */
    while (primes_with_7_len < (size_t)n) {
        seq_t p = get_prime(tested_prime_index);

        if (contains_digit_7(p)) {
            primes_with_7[primes_with_7_len++] = p;
        }
        ++tested_prime_index;
    }

    return primes_with_7[n - 1];
}
