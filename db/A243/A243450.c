/*
 * A243450  Primes of the form n^2 + 15.
 * https://oeis.org/A243450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 35, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243450: Primes of the form n^2 + 15. This function returns the nth such prime by iterating over successive squares and testing each candidate for primality. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static idx_t next_m = 1;           // next integer m to test

    while (primes_len < n) {
        seq_t cand = (seq_t)next_m * (seq_t)next_m + (seq_t)15;
        if (is_prime_big(cand)) {
            /* Ensure storage has enough room */
            ensure_capacity(primes_len + 1, &primes_cap, (void **)&primes, sizeof(seq_t));
            primes[primes_len++] = cand;
        }
        ++next_m;
    }
    return primes[n - 1];
}
