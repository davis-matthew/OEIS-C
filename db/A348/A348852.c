/*
 * A348852  Numbers k such that the number of odd nonprimes <= k is equal to
 *          the number of primes <= k.
 * https://oeis.org/A348852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A348852: numbers k where the count of odd non‑primes ≤ k equals the count of primes ≤ k (finite sequence). The values are produced by a simple sieve up to a fixed limit. */
#include <stdbool.h>
#include <stddef.h>

#define LIMIT 2000          /* safe upper bound (the largest term is 118) */

seq_t a(idx_t n)
{
    static seq_t terms[LIMIT];
    static size_t seq_len = 0;
    static bool initialized = false;

    if (!initialized) {
        /* Sieve of Eratosthenes for primes up to LIMIT */
        static bool composite[LIMIT + 1];
        for (idx_t p = 2; p * p <= LIMIT; ++p) {
            if (!composite[p]) {
                for (idx_t m = p * p; m <= LIMIT; m += p)
                    composite[m] = true;
            }
        }

        /* Count primes and matching k values */
        idx_t prime_cnt = 0;        /* primes ≤ k */
        idx_t odd_prime_cnt = 0;    /* odd primes ≤ k (primes > 2) */
        size_t tcnt = 0;

        for (idx_t k = 2; k <= LIMIT; ++k) {
            if (!composite[k]) {
                ++prime_cnt;
                if (k > 2)
                    ++odd_prime_cnt;
            }
            idx_t odd_cnt = (k + 1) / 2;              /* odd numbers ≤ k */
            idx_t odd_nonprime_cnt = odd_cnt - odd_prime_cnt;
            if (prime_cnt == odd_nonprime_cnt)
                terms[tcnt++] = (seq_t)k;
        }
        seq_len = tcnt;
        initialized = true;
    }

    if (n == 0)
        return (seq_t)0;
    idx_t idx = n - 1;
    if (idx < seq_len)
        return terms[(size_t)idx];
    return (seq_t)0;
}
