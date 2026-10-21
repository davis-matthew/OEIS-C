/*
 * A268082  Numbers n such that gcd(binomial(2*n-1,n), n) is equal to 1.
 * https://oeis.org/A268082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 79, u16 n <= 9213, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_not_in_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that gcd(binomial(2*n-1,n), n) == 1 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/prime_not_in_binomial.h"

#define MAX_SIEVE 500000          /* enough for sqrt of candidates up to ~5e5 */

static bool primes_initialized = false;
static uint32_t primes[MAX_SIEVE / 10 + 1];  /* ~41538 primes under 500k */
static size_t prime_cnt = 0;

/* Simple sieve to initialise primes array */
static void init_primes(void)
{
    bool *is_comp = (bool *)malloc((MAX_SIEVE + 1) * sizeof(bool));
    if (!is_comp) exit(1);
    for (uint32_t i = 0; i <= MAX_SIEVE; ++i) is_comp[i] = false;

    for (uint32_t i = 2; i * i <= MAX_SIEVE; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= MAX_SIEVE; j += i)
                is_comp[j] = true;
        }
    }
    for (uint32_t i = 2; i <= MAX_SIEVE; ++i)
        if (!is_comp[i]) {
            if (prime_cnt < sizeof(primes)/sizeof(primes[0]))
                primes[prime_cnt++] = i;
        }

    free(is_comp);
}

/* Test whether m satisfies the gcd condition */
static bool good_number(idx_t m)
{
    if (m <= 1) return true;                /* 1 is always good */

    idx_t tmp = m;
    for (size_t i = 0; i < prime_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i) {
        idx_t p = primes[i];
        if (tmp % p == 0) {
            if (!prime_not_in_binomial(m, p))
                return false;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                          /* remaining prime factor */
        if (!prime_not_in_binomial(m, tmp))
            return false;
    }
    return true;
}

/* External function required by the build */
seq_t a(idx_t n)
{
    if (!primes_initialized) {
        init_primes();
        primes_initialized = true;
    }

    idx_t candidate = 1;
    idx_t found = 0;

    while (true) {
        if (good_number(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
