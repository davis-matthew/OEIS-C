/*
 * A370602  a(n) = n! * Sum_{d|n} 1/((d-1)! * (n/d)^(d-1)).
 * https://oeis.org/A370602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{d|n} 1/((d-1)! * (n/d)^(d-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"   /* gives seq_t big_pow(seq_t base, seq_t exp) */

/* Static cache of factorials up to the maximum n requested. */
static seq_t *fact_cache = NULL;
static idx_t fact_cache_size = 0;

/* Ensure fact_cache has factorials up to n (inclusive). */
static void ensure_factorials(idx_t n)
{
    if (fact_cache_size <= n) {
        idx_t old_size = fact_cache_size;
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));
        if (!tmp) exit(1);                      /* abort on allocation failure */
        fact_cache = tmp;
        if (old_size == 0) {
            fact_cache[0] = 1;                  /* 0! = 1 */
            old_size = 1;
        }
        for (idx_t i = old_size; i < new_size; ++i) {
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
        fact_cache_size = new_size;
    }
}

/* Compute A370602 at index n (n >= 1). */
seq_t a(idx_t n)
{
    /* 0! and all needed factorials must be ready. */
    ensure_factorials(n);

    seq_t result = 0;
    idx_t sq = (idx_t)(n * n);   /* will be used only as bound check */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* first divisor d = i */
            idx_t d = i;
            seq_t num = fact_cache[n] / fact_cache[d - 1];
            seq_t pow_term = big_pow((seq_t)(n / d), (seq_t)(d - 1));
            result += num / pow_term;

            /* second divisor d = n / i, distinct from i when i*i != n */
            idx_t d2 = n / i;
            if (d2 != d) {
                seq_t num2 = fact_cache[n] / fact_cache[d2 - 1];
                seq_t pow_term2 = big_pow((seq_t)(n / d2), (seq_t)(d2 - 1));
                result += num2 / pow_term2;
            }
        }
    }
    return result;
}
