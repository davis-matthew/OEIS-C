/*
 * A317939  Numerators of sequence whose Dirichlet convolution with itself
 *          yields A080339 = A010051 (characteristic function of primes) +
 *          A063524 (1, 0, 0, 0, ...).
 * https://oeis.org/A317939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 143, i16 n <= 2303, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 115 terms through n = 9832 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317939:
   a(n) = numerator of f(n)
   where f(1)=1 and for n>1:
   f(n) = (1/2)*(A010051(n) - Σ_{d|n,1<d<n} f(d)*f(n/d))
   Initial values: a(1)=1, a(2)=1, a(3)=1, a(4)=-1, a(5)=1, a(6)=-1, ... */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* Add prod_num/2^prod_exp to the running sum sum_num/2^sum_exp. */
static void add_to_sum(seq_t *sum_num, idx_t *sum_exp,
                       seq_t prod_num, idx_t prod_exp)
{
    if (*sum_num == 0) {
        *sum_num = prod_num;
        *sum_exp = prod_exp;
        return;
    }
    idx_t max_exp = (*sum_exp > prod_exp) ? *sum_exp : prod_exp;

    seq_t adj_sum = *sum_num;
    if (*sum_exp < max_exp)
        adj_sum *= ((seq_t)1 << (max_exp - *sum_exp));

    seq_t adj_prod = prod_num;
    if (prod_exp < max_exp)
        adj_prod *= ((seq_t)1 << (max_exp - prod_exp));

    *sum_num = adj_sum + adj_prod;
    *sum_exp = max_exp;
}

/* Reduce num/2^exp by removing common factors of 2; keeps sign. */
static void reduce_by_two(seq_t *num, idx_t *exp)
{
    while ((*num & 1) == 0 && *exp > 0) {
        *num >>= 1;
        (*exp)--;
    }
}

/* External types: seq_t … sequence values, idx_t … unsigned indices */
static seq_t *num_cache = NULL;      /* numerator of f(n) */
static idx_t *exp_cache = NULL;      /* exponent of denominator 2^exp */
static size_t cache_size = 0;        /* number of indices computed (max index+1) */

/* Ensure num_cache and exp_cache have values for all n ≤ nmax. */
static void ensure_cache(idx_t nmax)
{
    if (nmax < cache_size) return;                /* already computed */

    size_t new_size = (size_t)nmax + 1;           /* indices 0 .. nmax */

    if (cache_size == 0) {                       /* first allocation */
        num_cache = mem_new(seq_t, new_size);
        exp_cache = (idx_t*)malloc(new_size * sizeof(idx_t));
        if (!num_cache || !exp_cache) abort();
        /* base case n = 1 */
        num_cache[0] = 0;
        exp_cache[0] = 0;
        num_cache[1] = 1;                       /* f(1)=1 */
        exp_cache[1] = 0;
        cache_size = 2;                         /* indices 0 and 1 filled */
    } else {                                    /* grow arrays */
        num_cache = mem_resize(seq_t, num_cache, new_size);
        exp_cache = (idx_t*)realloc(exp_cache, new_size * sizeof(idx_t));
        if (!num_cache || !exp_cache) abort();
    }

    /* Compute missing values from cache_size up to nmax */
    for (idx_t n = cache_size; n <= nmax; ++n) {
        if (n < 2) continue;                    /* already done */

        if (is_prime((uint64_t)n)) {             /* prime: f(n)=1/2 */
            num_cache[n] = 1;
            exp_cache[n] = 1;
            continue;
        }

        seq_t sum_num = 0;
        idx_t sum_exp = 0;

        /* Enumerate divisor pairs (d, e) with 2 ≤ d ≤ e and d*e == n */
        for (idx_t d = 2; d * d <= n; ++d) {
            if (n % d != 0) continue;
            idx_t e = n / d;
            seq_t prod_num = num_cache[d] * num_cache[e];
            idx_t prod_exp = exp_cache[d] + exp_cache[e];
            add_to_sum(&sum_num, &sum_exp, prod_num, prod_exp);
            if (d != e)                         /* symmetric divisor */
                add_to_sum(&sum_num, &sum_exp, prod_num, prod_exp);
        }

        /* f(n) = -(sum) / 2 */
        seq_t f_num = -sum_num;
        idx_t f_exp = sum_exp + 1;
        reduce_by_two(&f_num, &f_exp);

        num_cache[n] = f_num;
        exp_cache[n] = f_exp;
    }

    cache_size = new_size;                       /* update computed range */
}

/* Public function requested by the specification */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* undefined for n<1 */
    ensure_cache(n);
    return num_cache[n];
}
