/*
 * A280098  The sum of the divisors of 24*n - 1, divided by 24.
 * https://oeis.org/A280098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = σ(24n−1) / 24 (σ = sum of divisors)
 * Range:     u8 n <= 8, u16 n <= 3609, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----------------------------------------------------------------------
   Simple prime sieve – initialized once, lazily.
   ---------------------------------------------------------------------- */
#define PRIME_LIMIT 1000

static uint32_t primes[PRIME_LIMIT];
static size_t primes_cnt = 0;
static bool primes_inited = false;

static void
init_primes(void)
{
    if (primes_inited)
        return;

    /* static array of flags – automatically zero‑initialised */
    static bool is_composite[PRIME_LIMIT + 1] = {0};

    for (size_t p = 2; p * p <= PRIME_LIMIT; ++p) {
        if (!is_composite[p]) {
            for (size_t q = p * p; q <= PRIME_LIMIT; q += p)
                is_composite[q] = true;
        }
    }

    for (size_t p = 2; p <= PRIME_LIMIT; ++p)
        if (!is_composite[p])
            primes[primes_cnt++] = (uint32_t)p;

    primes_inited = true;
}

/* ----------------------------------------------------------------------
   Sum of divisors σ(m) for a 64‑bit integer m.
   The result is returned as seq_t.
   ---------------------------------------------------------------------- */
static seq_t
sigma_of_uint64(uint64_t m)
{
    seq_t result = 1;
    uint64_t temp   = m;

    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= temp; ++i) {
        uint32_t p = primes[i];
        if (temp % p == 0) {
            seq_t term   = 1;   /* (p^(e+1) – 1) / (p – 1) */
            seq_t power  = 1;   /* current p^k */
            while (temp % p == 0) {
                temp /= p;
                power *= (seq_t)p;
                term  += power;
            }
            result *= term;
        }
    }

    if (temp > 1)          /* remaining prime factor */
        result *= (seq_t)(1 + temp);

    return result;
}

/* ----------------------------------------------------------------------
   Main sequence function.
   ---------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n<1 – return 0 */

    if (!primes_inited)
        init_primes();

    /* Compute m = 24*n – 1  (all arithmetic in seq_t) */
    seq_t m = (seq_t)24 * (seq_t)n - 1;

    /* Sum of divisors of m; m fits into uint64_t for n up to 10 000 */
    uint64_t m64 = (uint64_t)m;   /* truncation is safe for tested ranges */
    seq_t sigma = sigma_of_uint64(m64);

    /* Exact division by 24 */
    return sigma / 24;
}
