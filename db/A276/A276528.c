/*
 * A276528  Least number with same prime signature as sigma(n), the sum of the
 *          divisors of n: a(n) = A046523(A000203(n)).
 * https://oeis.org/A276528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number with same prime signature as sigma(n)
 * Range:     u8 n <= 16, u16 n <= 1068, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Required helper functions */
#include "../utils/factor_u64.h"     /* size_t factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/big_pow.h"       /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

static uint64_t *primes = NULL;
static size_t prime_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure that 'primes' holds all primes <= limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    if (primes)
        free(primes);
    build_primes_upto(limit, &primes, &prime_cnt);
    prime_limit = limit;
}

/* Comparison function for sorting integers in descending order. */
static int cmp_desc(const void *a, const void *b)
{
    int ai = *(const int *)a;
    int bi = *(const int *)b;
    if (ai < bi)
        return 1;
    if (ai > bi)
        return -1;
    return 0;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Initialize prime table on first call.  A little bigger than needed. */
    if (!primes)
        ensure_primes(100000);   /* contains many primes (9592) */

    /* -------- compute sigma(n) -------- */
    uint64_t nn = (uint64_t)n;
    uint64_t pr1[64];
    uint64_t ex1[64];
    size_t cnt1 = 0;
    factor_u64(nn, pr1, ex1, &cnt1);

    seq_t sigma = 1;
    for (size_t i = 0; i < cnt1; ++i) {
        seq_t term_num = big_pow((seq_t)pr1[i], (seq_t)(ex1[i] + 1));
        term_num -= 1;
        seq_t term_den = (seq_t)pr1[i] - 1;
        seq_t term = term_num / term_den;
        sigma *= term;
    }

    /* sigma(1) == 1 : signature empty, result 1 */
    if (sigma == 1)
        return (seq_t)1;

    /* -------- factor sigma to get exponents -------- */
    uint64_t pr2[64];
    uint64_t ex2[64];
    size_t cnt2 = 0;
    factor_u64((uint64_t)sigma, pr2, ex2, &cnt2);

    if (cnt2 == 0)
        return (seq_t)1;

    int exps[64];
    for (size_t i = 0; i < cnt2; ++i)
        exps[i] = (int)ex2[i];

    qsort(exps, cnt2, sizeof(int), cmp_desc);

    /* Ensure we have enough primes for the final product */
    if (prime_cnt < cnt2)
        ensure_primes(cnt2 * 20 + 10);   /* generous upper bound */

    /* -------- compute least number with same signature -------- */
    seq_t result = 1;
    for (size_t i = 0; i < cnt2; ++i) {
        seq_t base = (seq_t)primes[i];
        seq_t pow_val = big_pow(base, (seq_t)exps[i]);
        result *= pow_val;
    }

    return result;
}
