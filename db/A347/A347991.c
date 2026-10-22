/*
 * A347991  a(n) = Sum_{d|n} 2^(sigma(d) - 1).
 * https://oeis.org/A347991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} 2^(σ(d)-1) (OEIS A347991)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   big_pow, factor_u64, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/sigma_of.h"
#include "../utils/big_pow.h"

/* Recursive helper to add contributions of every divisor of n */
static void recurse_divisors(size_t idx, uint64_t cur, const uint64_t *primes,
                             const uint64_t *expf, size_t prime_cnt,
                             seq_t *sum)
{
    if (idx == prime_cnt) {
        seq_t sigma = sigma_of((idx_t)cur);        /* σ(cur) */
        seq_t exp   = sigma - 1;                  /* exponent for 2^ */
        seq_t val   = big_pow((seq_t)2, exp);     /* 2^(σ(cur)-1) */
        *sum += val;
        return;
    }

    /* Generate all powers of primes[idx] and recurse */
    uint64_t d = cur;
    for (size_t e = 0; e <= expf[idx]; ++e) {
        recurse_divisors(idx + 1, d, primes, expf, prime_cnt, sum);
        d *= primes[idx];
    }
}

seq_t a(idx_t n)
{
    /* n == 0 is outside the defined offset; return 0 */
    if (n == 0) return 0;

    /* Factorize n into primes and exponents */
    uint64_t primes[64];
    uint64_t expf[64];
    size_t   prime_cnt;
    factor_u64((uint64_t)n, primes, expf, &prime_cnt);

    seq_t total = 0;
    recurse_divisors(0, 1ULL, primes, expf, prime_cnt, &total);
    return total;
}
