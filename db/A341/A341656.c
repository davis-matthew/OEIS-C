/*
 * A341656  a(n) is the number of divisors of prime(n)^4 - 1.
 * https://oeis.org/A341656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors of prime(n)^4 – 1
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the identity
   prime^4 – 1 = (prime – 1)(prime + 1)(prime^2 + 1)
   and factorises each 64‑bit component with factor_u64.
   The total divisor count is the product of (exponent + 1).  */

#include <stdint.h>
#include "../utils/get_prime.h"      /* uint32_t get_prime(idx_t k); */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */

static void merge_factor(uint64_t prime, uint64_t exp,
                        uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    size_t i;
    for (i = 0; i < *cnt; ++i) {
        if (primes[i] == prime) {
            exps[i] += exp;
            return;
        }
    }
    /* new prime */
    primes[*cnt] = prime;
    exps[*cnt] = exp;
    (*cnt)++;
}

seq_t a(idx_t n)
{
    /* obtain the n-th prime (1‑based) */
    uint32_t p32 = get_prime(n);
    uint64_t p = (uint64_t)p32;

    /* components of p^4 – 1 */
    uint64_t b1 = p * p + 1;   /* p^2 + 1 */
    uint64_t b2 = p - 1;       /* p – 1 */
    uint64_t b3 = p + 1;       /* p + 1 */

    /* arrays to hold aggregated factorization */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;

    /* factor each component and merge */
    {
        size_t cnt;
        uint64_t pr[64], ex[64];

        factor_u64(b1, pr, ex, &cnt);
        for (size_t j = 0; j < cnt; ++j) {
            merge_factor(pr[j], ex[j], primes, exps, &count);
        }
    }
    {
        size_t cnt;
        uint64_t pr[64], ex[64];

        factor_u64(b2, pr, ex, &cnt);
        for (size_t j = 0; j < cnt; ++j) {
            merge_factor(pr[j], ex[j], primes, exps, &count);
        }
    }
    {
        size_t cnt;
        uint64_t pr[64], ex[64];

        factor_u64(b3, pr, ex, &cnt);
        for (size_t j = 0; j < cnt; ++j) {
            merge_factor(pr[j], ex[j], primes, exps, &count);
        }
    }

    /* compute divisor count as product of (exponent+1) */
    seq_t result = 1;
    for (size_t i = 0; i < count; ++i) {
        result *= (seq_t)(exps[i] + 1);
    }

    return result;
}
