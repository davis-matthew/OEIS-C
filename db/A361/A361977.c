/*
 * A361977  a(n) is the largest prime p such that 2^p - 1 <= 10^n.
 * https://oeis.org/A361977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest prime p such that 2^p - 1 <= 10^n * The value p
 *            is given by p = floor(n * log_2(10)). * The sequence is obtained
 *            by sieving primes up to a safe limit * (40000 is enough for n ≤
 *            10000) and picking the largest prime ≤ p
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* limit for precomputed primes (covers n up to ≈10000) */
#define PRIME_LIMIT 40000

/* global sieve data (initialized on first use) */
static bool primes_initialized = false;
static size_t primes_cnt = 0;
static uint32_t primes_arr[PRIME_LIMIT + 1];

/* initialize the prime sieve */
static void init_primes(void) {
    bool sieve[PRIME_LIMIT + 1];
    for (size_t i = 0; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;
    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;
        }
    }
    size_t idx = 0;
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes_arr[idx++] = i;
    primes_cnt = idx;
    primes_initialized = true;
}

/* main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;

    if (!primes_initialized)
        init_primes();

    /* high‑precision log2(10) */
    const long double LOG2_10 = 3.321928094887362347870319429489400748L;
    long double val = (long double)n * LOG2_10;
    idx_t p = (idx_t)val; /* floor is implicit */

    /* binary search for largest prime ≤ p */
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if ((idx_t)primes_arr[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is first index > p; answer is lo-1 */
    if (lo == 0) return (seq_t)0; /* should not happen for n≥1 */
    return (seq_t)primes_arr[lo - 1];
}
