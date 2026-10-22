/*
 * A336619  a(n) = n!/d where d is the maximum divisor of n! with equal prime
 *          exponents.
 * https://oeis.org/A336619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / d where d is the maximum divisor of n! with equal
 *            prime exponents
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 18, u64 n <= 27, u128 n <= 44,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

typedef struct {
    uint64_t p;
    uint64_t e;
} prime_exp_t;

static int cmp_exp_desc(const void *A, const void *B)
{
    const prime_exp_t *a = (const prime_exp_t *)A;
    const prime_exp_t *b = (const prime_exp_t *)B;
    if (a->e < b->e) return 1;
    if (a->e > b->e) return -1;
    return 0;
}

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n <= 1) return (seq_t)1;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);
    if (prime_cnt == 0) {
        free(primes);
        return (seq_t)1;
    }

    /* compute exponents of each prime in n! */
    prime_exp_t *pexp = (prime_exp_t *)malloc(prime_cnt * sizeof(prime_exp_t));
    uint64_t max_e = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = 0;
        uint64_t m = n;
        while (m >= p) {
            m /= p;
            e += m;
        }
        pexp[i].p = p;
        pexp[i].e = e;
        if (e > max_e) max_e = e;
    }

    /* sort by exponent descending */
    qsort(pexp, prime_cnt, sizeof(prime_exp_t), cmp_exp_desc);

    /* find best k maximizing k * sum_{p with e>=k} log p */
    double sum_log = 0.0;
    double best_log = -1.0e308; /* effectively -infinity */
    uint64_t best_k = 0;
    size_t idx = 0;
    for (uint64_t k = max_e; k > 0; --k) {
        while (idx < prime_cnt && pexp[idx].e >= k) {
            sum_log += log((double)pexp[idx].p);
            ++idx;
        }
        double current = (double)k * sum_log;
        if (current > best_log) {
            best_log = current;
            best_k = k;
        }
    }

    /* compute the answer as product of p^(e - best_k if e>=best_k) */
    seq_t result = 1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t e = pexp[i].e;
        uint64_t exp = e;
        if (e >= best_k) exp -= best_k;
        if (exp > 0) {
            seq_t term = seq_pow((seq_t)pexp[i].p, (idx_t)exp);
            result = seq_mul(result, term);
        }
    }

    free(primes);
    free(pexp);
    return result;
}
