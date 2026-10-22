/*
 * A303394  Expansion of Product_{n>=1} ((1 - (4*x)^n)/(1 + (4*x)^n))^(1/4).
 * https://oeis.org/A303394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 15, i64 n <= 25, i128 n <= 25,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = coefficient of x^n in
 *   prod_{m>=1} ((1 - (4x)^m)/(1 + (4x)^m))^{1/4}
 *
 * Derived recurrence:
 *   a(0) = 1
 *   a(n) = (1/n) * sum_{k=1}^n k * b(k) * a(n-k)
 *   with b(k) = -(2^{2k-1} * sigma(odd_part(k))) / odd_part(k)
 *   where sigma(x) = sum of divisors of x
 *
 *   Term_k = - (a(n-k) << e) * sigma(odd_part(k)),
 *   with e = 2*k - 1 + v2(k), v2(k)=number of trailing zeros in k.
 *
 * The implementation uses memoisation: values are computed up to the
 * largest n requested and stored in a static array.
 */

#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"
#include <stdint.h>
#include <stdlib.h>

/* Helper: compute integer exponent 2^e as shift */
static inline seq_t pow2_shift(seq_t val, uint64_t e)
{
    /* shift left by e bits */
    return val << e;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* largest index fully computed */
    static size_t capacity = 0;      /* allocated size of cache */

    /* ------------------------------------------------------------------ */
    /* Allocate or enlarge cache when necessary                           */
    /* ------------------------------------------------------------------ */
    if (cache == NULL) {
        capacity = (size_t)n + 1;
        if (capacity == 0) capacity = 1;      /* handle n==0 */
        cache = seq_alloc_new_array(capacity);
        cache[0] = 1;                         /* a(0) = 1 */
        max_computed = 0;
    } else if ((size_t)(n + 1) > capacity) {
        /* reallocate larger cache */
        seq_t *new_cache = seq_alloc_new_array((size_t)n + 1);
        for (size_t i = 0; i <= (size_t)max_computed; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        capacity = (size_t)n + 1;
    }

    /* ------------------------------------------------------------------ */
    /* If required, compute terms up to n                                  */
    /* ------------------------------------------------------------------ */
    if (max_computed < n) {
        /* Pre‑compute sigma for all integers up to n once */
        uint64_t *sigma = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
        if (!sigma) return 0;  /* allocation failure (unlikely) */
        for (idx_t i = 1; i <= n; ++i) sigma[i] = 0;
        for (idx_t i = 1; i <= n; ++i) {
            for (idx_t j = i; j <= n; j += i)
                sigma[j] += i;          /* sigma[j] = sum of divisors of j */
        }

        /* Compute a values from max_computed+1 up to n */
        for (idx_t cur = max_computed + 1; cur <= n; ++cur) {
            seq_t sum = 0;           /* accumulator for the sum */

            /* Inner sum over k = 1 .. cur */
            for (idx_t k = 1; k <= cur; ++k) {
                /* odd part of k and its sigma */
                uint64_t m = k >> __builtin_ctzll(k);   /* odd part */
                uint64_t sigma_m = sigma[m];

                /* exponent for shifting */
                unsigned int v2 = __builtin_ctzll(k);
                uint64_t e = (uint64_t)2 * k - 1 + v2;

                /* term = - (a[cur-k] << e) * sigma_m */
                seq_t term = cache[cur - k];
                term <<= e;                        /* multiply by 2^e */
                term *= (seq_t)sigma_m;            /* multiply by sigma(m) */
                term = -term;                      /* apply the minus sign */

                sum += term;                       /* accumulate */
            }

            /* Final value division by cur */
            cache[cur] = sum / (seq_t)cur;
        }

        free(sigma);
        max_computed = n;
    }

    /* Return the requested term */
    return cache[n];
}
