/*
 * A260685  Sequence is defined by the condition that Sum_{d|n} a(d)^(n/d) = 1
 *          if n=1, = 0 if n>1.
 * https://oeis.org/A260685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 47, i32 n <= 95, i64 n <= 175,
 *            i128 n <= 351, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Product-sum sequence A260685:  
   a(1)=1; for n>1, ∑_{d|n} a(d)^(n/d) = 0.  */

/* Includes and utility prototypes */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Static cache for computed values */
static seq_t *cache = NULL;           /* cached values a(n) */
static bool *computed = NULL;         /* flags whether cache[n‑1] is valid */
static size_t cache_cap = 0;          /* current capacity of the cache */

/* Ensure the cache can hold values up to index n (1‑based). */
static void ensure_cap(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = n > 256 ? n * 2 : 256;

    cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    computed = (bool *)realloc(computed, sizeof(bool) * new_cap);
    if (!cache || !computed) exit(1);   /* allocation failure */

    for (size_t i = cache_cap; i < new_cap; ++i) computed[i] = false;
    cache_cap = new_cap;
}

/* Forward declaration for recursion. */
static seq_t compute_a(idx_t n);

/* Recursive helper that enumerates all divisors of target_n and
   accumulates the sum ∑ a(d)^(target_n/d) for d < target_n. */
static void rec_div(idx_t target_n,
                    const uint64_t *pr, const uint64_t *ex, size_t prime_cnt,
                    seq_t *sum, uint64_t cur, size_t idx)
{
    if (idx == prime_cnt) {
        uint64_t d = cur;
        if (d == target_n) return;                     /* skip the divisor n itself */
        seq_t val = compute_a((idx_t)d);
        seq_t power = big_pow(val, (seq_t)(target_n / d));
        *sum += power;
        return;
    }

    uint64_t p = pr[idx];
    uint64_t emax = ex[idx];
    uint64_t val = cur;
    for (uint64_t e = 0; e <= emax; ++e) {
        rec_div(target_n, pr, ex, prime_cnt, sum, val, idx + 1);
        val *= p;
    }
}

/* Recursive implementation of the sequence. */
static seq_t compute_a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    ensure_cap(n);
    if (computed[n - 1]) return cache[n - 1];

    uint64_t pr_arr[64];
    uint64_t ex_arr[64];
    size_t prime_cnt;
    factor_u64(n, pr_arr, ex_arr, &prime_cnt);

    seq_t sum = 0;
    rec_div(n, pr_arr, ex_arr, prime_cnt, &sum, 1, 0);

    seq_t result = -sum;            /* a(n) = -sum of proper contributions */
    cache[n - 1] = result;
    computed[n - 1] = true;
    return result;
}

/* External linkage required by the build system. */
seq_t a(idx_t n)
{
    return compute_a(n);
}
