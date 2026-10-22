/*
 * A319692  a(n) = Product_{d|n, d<n} prime(1+A001414(d)), where A001414(d)
 *          gives the sum of prime factors of d, with repetition.
 * https://oeis.org/A319692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d<n} A000040(1+A001414(d))
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 59, u64 n <= 179,
 *            u128 n <= 719, big any n
 * Helpers:   factor_u64, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A001414(d) is the sum of the prime factors of d (with multiplicity). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper declarations – the build will provide the required headers. */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t n,uint64_t *pr,uint64_t *ex,size_t *cnt); */
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k); */

/* ------------------------------------------------------------------
 * sum_prime_factors
 *
 * Compute the sum of the prime factors of x, counting multiplicity.
 * For x <= 1 the sum is 0.
 * ------------------------------------------------------------------ */
static seq_t
sum_prime_factors(idx_t x)
{
    if (x <= 1)
        return (seq_t)0;

    /* The number of distinct prime factors of numbers up to 10 000 is < 10. */
    uint64_t primes[16], exps[16];
    size_t cnt = 0;

    factor_u64((uint64_t)x, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        /* primes[i] and exps[i] fit in 64 bits; their product in seq_t. */
        sum += (seq_t)primes[i] * (seq_t)exps[i];
    }
    return sum;
}

/* ------------------------------------------------------------------
 * a
 *
 * Main entry point for OEIS sequence A319692.
 * ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;               /* a(1) = 1 */

    /* Gather proper divisors of n (excluding n itself). */
    idx_t divisors[256];
    size_t dcnt = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 < n)
                divisors[dcnt++] = d1;
            if (d2 < n && d2 != d1)
                divisors[dcnt++] = d2;
        }
    }

    /* Compute the product of the required primes. */
    seq_t prod = (seq_t)1;
    for (size_t k = 0; k < dcnt; ++k) {
        idx_t d = divisors[k];
        seq_t sum = sum_prime_factors(d);           /* A001414(d) */
        idx_t prime_index = 1 + (idx_t)sum;          /* 1‑based index */
        seq_t prime_val   = (seq_t)get_prime(prime_index);
        prod *= prime_val;
    }

    return prod;
}
