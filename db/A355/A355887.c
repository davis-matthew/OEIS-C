/*
 * A355887  a(n) = Sum_{k=1..n} k^k * floor(n/k).
 * https://oeis.org/A355887
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^k * floor(n/k) * = Sum_{k=1..n} Sum_{d|k}
 *            d^d
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"

/* Cache for computed values */
static seq_t *a_cache = NULL;      /* a_cache[i] holds a(i) */
static idx_t a_cached = 0;         /* largest n computed so far */

/* Smallest prime factor sieve */
static uint64_t *spf = NULL;
static idx_t spf_limit = 0;

/* Ensure spf table is computed up to 'limit' (inclusive) */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;

    size_t sz = (size_t)(limit + 1);
    uint64_t *new_spf = (uint64_t *)malloc(sz * sizeof(uint64_t));
    for (size_t i = 0; i <= limit; ++i) new_spf[i] = i;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (new_spf[i] == i) {
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                if (new_spf[j] == j) new_spf[j] = i;
        }
    }

    free(spf);
    spf = new_spf;
    spf_limit = limit;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0, return 0 */

    if (n > a_cached) {
        /* Build SPF table up to n */
        if (spf == NULL || spf_limit < n) ensure_spf(n);

        /* Resize cache array to accommodate 0..n */
        size_t old_cap = (size_t)(a_cached + 1);
        size_t new_cap = (size_t)(n + 1);
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        for (size_t i = old_cap; i <= n; ++i) a_cache[i] = 0;
        a_cache[0] = 0;   /* unused but keeps indexing simple */

        /* Compute missing terms from a_cached+1 up to n */
        for (idx_t i = a_cached + 1; i <= n; ++i) {
            /* Factorise i using spf */
            idx_t tmp = i;
            idx_t primes_local[16];
            size_t exps_local[16];
            size_t pf_cnt = 0;
            while (tmp > 1) {
                uint64_t p = spf[(size_t)tmp];
                size_t cnt = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++cnt;
                }
                primes_local[pf_cnt] = p;
                exps_local[pf_cnt] = cnt;
                ++pf_cnt;
            }

            /* Enumerate all divisors of i */
            size_t divs[256];
            size_t div_cnt = 1;
            divs[0] = 1;
            for (size_t j = 0; j < pf_cnt; ++j) {
                uint64_t p = primes_local[j];
                size_t e  = exps_local[j];
                size_t prev_cnt = div_cnt;
                uint64_t p_pow = 1;
                for (size_t k = 1; k <= e; ++k) {
                    p_pow *= p;
                    for (size_t t = 0; t < prev_cnt; ++t) {
                        div_cnt++;
                        divs[div_cnt - 1] = divs[t] * p_pow;
                    }
                }
            }

            /* Sum d^d over all divisors d of i */
            seq_t sum_d = 0;
            for (size_t z = 0; z < div_cnt; ++z) {
                seq_t d = (seq_t)divs[z];
                seq_t pow_val = big_pow(d, d);
                sum_d += pow_val;
            }

            a_cache[i] = a_cache[i - 1] + sum_d;
        }

        a_cached = n;
    }

    return a_cache[n];
}
