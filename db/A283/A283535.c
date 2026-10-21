/*
 * A283535  a(n) = Sum_{d|n} d^(3*d + 1).
 * https://oeis.org/A283535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(3*d+1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Static cache of computed a(n) values */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Helper: generate all divisors of n using its prime factorization */
static void gen_divisors_rec(const uint64_t *primes, const int *exps,
                             size_t idx, uint64_t cur,
                             uint64_t *divs, size_t *cnt) {
    if (idx == 0) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t val = 1;
    for (int i = 0; i <= exps[idx - 1]; ++i) {
        gen_divisors_rec(primes, exps, idx - 1, cur * val, divs, cnt);
        val *= primes[idx - 1];
    }
}

static size_t factorize(uint64_t n, uint64_t *primes, int *exps, size_t *factor_cnt) {
    size_t cnt = 0;
    uint64_t temp = n;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (temp > 1) {
        primes[cnt] = temp;
        exps[cnt] = 1;
        ++cnt;
    }
    *factor_cnt = cnt;
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n = 0, return 0 */

    /* Ensure cache is large enough */
    if ((size_t)n > cache_size) {
        size_t new_size = ((size_t)n > 10) ? (size_t)n + 10 : 10;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* allocation failure – unlikely */
        for (size_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        for (size_t i = cache_size; i < new_size; ++i) new_cache[i] = 0;
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }

    /* Return memoized value if already computed */
    if (cache[n - 1] != 0) return cache[n - 1];

    /* Compute a(n) */
    uint64_t temp = (uint64_t)n;
    uint64_t primes[16];
    int exps[16];
    size_t factor_cnt = 0;
    factorize(temp, primes, exps, &factor_cnt);

    /* Estimate number of divisors: product (e_i + 1) */
    size_t max_divisors = 1;
    for (size_t i = 0; i < factor_cnt; ++i) max_divisors *= (size_t)(exps[i] + 1);
    uint64_t *divs = (uint64_t *)malloc(max_divisors * sizeof(uint64_t));
    size_t div_cnt = 0;
    gen_divisors_rec(primes, exps, factor_cnt, 1, divs, &div_cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t d = divs[i];
        seq_t term = big_pow((seq_t)d, (seq_t)(3 * d + 1));
        sum += term;
    }
    free(divs);
    cache[n - 1] = sum;
    return sum;
}
