/*
 * A337531  Number of ways that the divisors of 2n can be written as unordered
 *          sums of two other prime divisors of 2n (not necessarily distinct).
 * https://oeis.org/A337531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d1|(2*n), d2|(2*n), d1,d2 prime} [d1 + d2 | (2*n)]
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* distinct prime factorisation of a 64‑bit integer */
static void factor_distinct_uint64(uint64_t n, uint64_t primes[], size_t *cnt)
{
    size_t i = 0;
    if (n < 2) {
        *cnt = 0;
        return;
    }
    if (n % 2ULL == 0) {
        primes[i++] = 2ULL;
        while (n % 2ULL == 0) n /= 2ULL;
    }
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL) {
        if (n % d == 0) {
            primes[i++] = d;
            while (n % d == 0) n /= d;
        }
    }
    if (n > 1ULL) {
        primes[i++] = n;
    }
    *cnt = i;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* 2*n as seq_t */
    seq_t two_n = (seq_t)n * (seq_t)2;

    /* collect distinct prime divisors of n */
    uint64_t primes[64];
    size_t pcnt;
    factor_distinct_uint64((uint64_t)n, primes, &pcnt);

    /* 2 always divides 2*n; ensure the prime 2 is present */
    bool has2 = false;
    for (size_t i = 0; i < pcnt; ++i) {
        if (primes[i] == 2ULL) { has2 = true; break; }
    }
    if (!has2) {
        primes[pcnt++] = 2ULL;
    }

    /* count unordered pairs of prime divisors whose sum divides 2*n */
    seq_t result = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        for (size_t j = i; j < pcnt; ++j) {
            seq_t sum = (seq_t)primes[i] + (seq_t)primes[j];
            if (sum != 0 && two_n % sum == 0) {
                ++result;
            }
        }
    }
    return result;
}
