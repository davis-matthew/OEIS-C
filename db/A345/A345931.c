/*
 * A345931  a(n) = gcd(n, A002034(n)), where A002034(n) gives the smallest
 *          positive integer k such that n divides k!.
 * https://oeis.org/A345931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( n, A002034(n) )
 * Range:     u8 n <= 255, u16 n <= 9935, u32 n <= 9935, u64 n <= 9935,
 *            u128 n <= 9935, big any n
 * Helpers:   _min_k_for_prime, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A002034(n) = the smallest k such that n divides k! */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/_min_k_for_prime.h"

/* Small prime sieve for factorisation.  100000 is enough for n <= 10000. */
#define PRIME_SIEVE_MAX 100000
static uint32_t _primes[PRIME_SIEVE_MAX];
static size_t   _prime_cnt = 0;
static bool     _primes_init = false;

static void _init_primes(void)
{
    if (_primes_init) return;
    bool is_comp[PRIME_SIEVE_MAX + 1] = {0};
    for (uint32_t i = 2; i <= PRIME_SIEVE_MAX; ++i) {
        if (!is_comp[i]) {
            _primes[_prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_MAX) {
                for (uint32_t j = i * i; j <= PRIME_SIEVE_MAX; j += i)
                    is_comp[j] = true;
            }
        }
    }
    _primes_init = true;
}

/* Factorisation of a 64‑bit integer into primes and exponents. */
static void _factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    size_t c = 0;
    for (size_t i = 0; i < _prime_cnt && (uint64_t)_primes[i] * _primes[i] <= n; ++i) {
        uint32_t p = _primes[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[c] = (uint64_t)p;
            ex[c] = e;
            ++c;
        }
    }
    if (n > 1) {           /* remaining prime factor */
        pr[c] = n;
        ex[c] = 1;
        ++c;
    }
    *cnt = c;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1u) return 0;                /* undefined below offset */
    _init_primes();

    uint64_t n64 = (uint64_t)n;
    uint64_t pr[64], ex[64];
    size_t fp_cnt = 0;
    _factor_uint64(n64, pr, ex, &fp_cnt);

    seq_t needed_k = 1;
    for (size_t i = 0; i < fp_cnt; ++i) {
        seq_t k = _min_k_for_prime((uint32_t)pr[i], ex[i]);
        if (k > needed_k) needed_k = k;
    }

    return gcd((seq_t)n64, needed_k);
}
