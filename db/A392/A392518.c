/*
 * A392518  Numbers k such that sigma(k) = psi(k) + tau(k)^2 + omega(k).
 * https://oeis.org/A392518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 37 * p_n^2, where p_n is the n‑th prime (excluding 37)
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Trial‑division primality test for 64‑bit integers. */
static inline bool is_prime_trial(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (idx_t i = 3; i <= n / i; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static uint64_t *primes   = NULL;
    static size_t  primes_len = 0;
    static size_t  primes_cap = 0;
    static idx_t   next_candidate = 2;

    /* Generate primes until we have at least n of them (skipping 37). */
    while (primes_len < n) {
        if (is_prime_trial(next_candidate) && next_candidate != 37ULL) {
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
                if (!tmp) return 0;          /* memory error: return 0 */
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = (uint64_t)next_candidate;
        }
        next_candidate++;
    }

    seq_t p = (seq_t)primes[n - 1];
    return (seq_t)37 * p * p;
}
