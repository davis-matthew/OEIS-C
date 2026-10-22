/*
 * A390181  Numbers not able to be represented in the form c*p^2+b for p
 *          prime, c >= 1 and 0 <= b < p.
 * https://oeis.org/A390181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer that cannot be written in the
 *            form c·p² + b with p prime, c ≥ 1, and 0 ≤ b < p. The algorithm
 *            scans natural numbers and tests each one for representability.
 *            It uses a precomputed prime list up to 1 000 000, which is
 *            sufficient for the required range (n ≤ 10 000)....
 * Range:     u8 n <= 59, u16 n <= 6928, u32 n <= 9831, u64 n <= 9831,
 *            u128 n <= 9831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define PRIME_LIMIT 1000000

/* Global sieve data */
static bool prime_flag[PRIME_LIMIT + 1] = {0};
static uint32_t primes[78498];               /* number of primes ≤ 1 000 000 */
static size_t prime_cnt = 0;
static bool primes_initialized = false;

/* Build the prime list once */
static void init_prime_sieve(void)
{
    if (primes_initialized) return;
    primes_initialized = true;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!prime_flag[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    prime_flag[j] = true;
            }
        }
    }
}

/* Return true iff m can be expressed as c·p² + b with prime p. */
static bool is_representable(idx_t m)
{
    init_prime_sieve();
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t p2 = p * p;
        if (p2 > m) break;                /* only primes with p² ≤ m matter */
        if (m % p2 < p) return true;      /* representable */
    }
    return false;
}

/* Main OEIS function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset starts at 1; guard against misuse */

    idx_t m = 1;
    idx_t count = 0;

    while (1) {
        if (!is_representable(m)) {
            ++count;
            if (count == n) return (seq_t)m;
        }
        ++m;
    }
}
