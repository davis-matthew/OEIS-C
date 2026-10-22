/*
 * A328558  Squares (A000290) that are not arithmetic numbers (A003601).
 * https://oeis.org/A328558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = square of the n‑th integer k for which the arithmetic
 *            mean of the divisors of k² is not an integer
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define PRIME_LIMIT 100000

static uint32_t primes[PRIME_LIMIT];
static size_t primes_cnt = 0;

static void init_primes(void)
{
    static bool done = false;
    if (done) return;
    uint8_t *sieve = (uint8_t *)malloc(PRIME_LIMIT + 1);
    if (!sieve) exit(1);
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i) sieve[j] = 0;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) primes[primes_cnt++] = (uint32_t)i;
    }
    free(sieve);
    done = true;
}

static bool is_not_arith_square(idx_t k)
{
    seq_t sigma = 1;
    seq_t tau   = 1;
    idx_t temp = k;

    for (size_t pi = 0;
         pi < primes_cnt && (idx_t)primes[pi] * (idx_t)primes[pi] <= temp;
         ++pi) {
        uint32_t p = primes[pi];
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) { temp /= p; ++e; }
            seq_t term = 1;
            for (int i = 0; i < 2 * e + 1; ++i) term *= (seq_t)p;
            term = (term - 1) / (seq_t)(p - 1);
            sigma *= term;
            tau *= (seq_t)(2 * e + 1);
        }
    }

    if (temp > 1) {          // remaining prime factor (e=1)
        uint32_t p = (uint32_t)temp;
        seq_t term = (seq_t)p * (seq_t)p * (seq_t)p;
        term = (term - 1) / (seq_t)(p - 1);
        sigma *= term;
        tau *= (seq_t)3;
    }

    return (sigma % tau) != 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   // guard for unused offset
    init_primes();

    idx_t count = 0;
    for (idx_t k = 2; ; ++k) {
        if (is_not_arith_square(k)) {
            ++count;
            if (count == n) return (seq_t)k * (seq_t)k;
        }
    }
}
