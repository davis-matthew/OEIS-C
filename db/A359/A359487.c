/*
 * A359487  a(n) is the smallest start of a run of 2 or more integers having a
 *          prime factor greater than n.
 * https://oeis.org/A359487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k ≥ n+1 such that k and k+1 both have a
 *            prime factor greater than n. The implementation searches from
 *            n+1 upward. A modest sieve of primes up to 200 000 is built once
 *            and reused
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* --- Prime sieve ---------------------------------------------------- */
static uint32_t *prime_list  = NULL; /* array of primes up to LIMIT  */
static size_t   prime_count  = 0;    /* number of primes */

/* Build the prime list once.  Safe for n ≤ 10 000 because a(n) ≈ 2n. */
static void init_primes(void) {
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    const size_t LIMIT = 200000;           /* large enough for n ≤ 10 000 */
    bool *comp = (bool *)calloc(LIMIT + 1, sizeof(bool));
    if (!comp) abort();                     /* improbable but safe */

    comp[0] = comp[1] = true;
    for (size_t i = 2; (uint64_t)i * i <= LIMIT; ++i) {
        if (!comp[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i)
                comp[j] = true;
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!comp[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) abort();

    /* Fill list */
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (!comp[i]) prime_list[idx++] = (uint32_t)i;

    prime_count = cnt;
    free(comp);
}

/* --- Test whether a number has a prime factor > n ------------------- */
static bool has_prime_factor_gt(idx_t x, idx_t n) {
    if (x <= 1) return false;
    for (size_t pi = 0; pi < prime_count; ++pi) {
        uint32_t p = prime_list[pi];
        if ((uint64_t)p * p > x) break;
        if (x % p == 0) {
            if (p > n) return true;
            while (x % p == 0) x /= p;
        }
    }
    /* Remaining x is prime */
    return (x > n);
}

/* --- Main sequence function ---------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* undefined, but safe guard */
    init_primes();

    idx_t start = n + 1;              /* a(n) ≥ n+1 */
    for (idx_t k = start; ; ++k) {
        if (has_prime_factor_gt(k,     n) &&
            has_prime_factor_gt(k + 1, n))
            return (seq_t)k;
    }
}
