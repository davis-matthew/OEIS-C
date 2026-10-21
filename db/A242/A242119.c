/*
 * A242119  Primes modulo 18.
 * https://oeis.org/A242119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) mod 18
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static int is_prime_candidate(uint64_t cand, const uint64_t *pr, size_t len)
{
    for (size_t i = 0; i < len; ++i) {
        uint64_t p = pr[i];
        if (p * p > cand) break;
        if (cand % p == 0) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t *primes = NULL;
    static size_t primes_count = 0;
    static size_t primes_cap   = 0;

    if (primes_cap == 0) {
        primes_cap = 16;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
    }

    while (primes_count < n) {
        uint64_t candidate;
        if (primes_count == 0)
            candidate = 2;
        else
            candidate = primes[primes_count - 1] + 1;

        if (candidate != 2 && (candidate & 1) == 0)
            candidate++;

        while (!is_prime_candidate(candidate, primes, primes_count)) {
            candidate += 2;           /* skip even numbers */
        }

        if (primes_count == primes_cap) {
            primes_cap *= 2;
            uint64_t *tmp = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            if (!tmp) exit(EXIT_FAILURE);
            primes = tmp;
        }

        primes[primes_count++] = candidate;
    }

    return (seq_t)(primes[n - 1] % 18ull);
}
