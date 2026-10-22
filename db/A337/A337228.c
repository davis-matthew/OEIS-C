/*
 * A337228  Number of ordered pairs of divisors of n, (d1,d2), such that d2 is
 *          prime and d1 <= d2.
 * https://oeis.org/A337228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d1|n, d2|n, d2 prime, d1 ≤ d2} 1 (OEIS A337228)
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Compare function for sorting uint64_t values */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t v1 = *(const uint64_t *)a;
    uint64_t v2 = *(const uint64_t *)b;
    return (v1 > v2) - (v1 < v2);
}

/* Recursive generation of all divisors of n.
   pr[0..cnt-1] are prime factors, ex[0..cnt-1] their exponents. */
static void gen_divisors(const uint64_t *pr, const uint64_t *ex, size_t cnt,
                         uint64_t cur, uint64_t *arr, size_t *sz)
{
    if (cnt == 0) {
        arr[(*sz)++] = cur;
        return;
    }
    uint64_t p = pr[cnt - 1];
    uint64_t m = 1;
    for (size_t e = 0; e <= ex[cnt - 1]; ++e) {
        gen_divisors(pr, ex, cnt - 1, cur * m, arr, sz);
        m *= p;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)          /* a(1) = 0 */
        return (seq_t)0;

    /* Factorise n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t prime_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &prime_cnt);
    if (prime_cnt == 0)   /* should not happen for n>1 */
        return (seq_t)0;

    /* Sort prime factors just in case (pairwise sort of primes & exps) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        for (size_t j = i + 1; j < prime_cnt; ++j) {
            if (primes[i] > primes[j]) {
                uint64_t tmp = primes[i];
                primes[i] = primes[j];
                primes[j] = tmp;
                tmp = exps[i];
                exps[i] = exps[j];
                exps[j] = tmp;
            }
        }
    }

    /* Count total number of divisors: product of (exp+1) */
    size_t total_divs = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
        total_divs *= (size_t)(exps[i] + 1);

    /* Allocate array for divisors */
    uint64_t *divs = (uint64_t *)malloc(total_divs * sizeof(uint64_t));
    if (!divs)
        return (seq_t)0;   /* out‑of‑memory fallback */

    size_t divs_sz = 0;
    gen_divisors(primes, exps, prime_cnt, (uint64_t)1, divs, &divs_sz);

    /* Sort divisors */
    qsort(divs, divs_sz, sizeof(uint64_t), cmp_uint64);

    /* Count pairs for each prime divisor */
    size_t didx = 0;            /* index into divs that are ≤ current prime */
    seq_t result = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        while (didx < divs_sz && divs[didx] <= p)
            ++didx;
        result += (seq_t)didx;
    }

    free(divs);
    return result;
}
