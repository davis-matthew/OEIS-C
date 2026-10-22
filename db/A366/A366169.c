/*
 * A366169  Positive integers k such that the second derivative of the k-th
 *          Bernoulli polynomial B(k,x) contains only integer coefficients.
 * https://oeis.org/A366169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, hard
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   fac_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A366169: Positive integers k such that the second derivative of the
 *  k‑th Bernoulli polynomial B(k,x) contains only integer coefficients.
 *
 *  Computation: For each k (1 ≤ k ≤ 200) test whether
 *      n(n−1)*C(n−2,j)*B_j
 *  has integer coefficients for all 0 ≤ j ≤ k−2, where B_j is the
 *  j‑th Bernoulli number.  The Bernoulli number B_j has denominator
 *  equal to the square‐free product of all primes p with (p−1)|j.
 *  The test is performed by comparing prime exponents in numerator and
 *  denominator; see is_valid_k().
 *
 *  The sequence is finite (last term 190); here we generate it once
 *  and cache the list, returning 0 for indices beyond its length.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/fac_exp.h"

/* --------------------------------------------------------------------- */
/*  Prime sieve up to a given limit.  Memory: O(limit).                  */
/* --------------------------------------------------------------------- */
static void sieve_primes(int limit, int **primes_out, size_t *cnt_out)
{
    bool *is_composite = (bool *)calloc((size_t)limit + 1, sizeof(bool));
    int *pr = (int *)malloc((size_t)limit + 1 * sizeof(int));
    size_t cnt = 0;
    for (int i = 2; i <= limit; ++i) {
        if (!is_composite[i]) {
            pr[cnt++] = i;
            if ((int64_t)i * i <= limit) {
                for (int j = i * i; j <= limit; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    free(is_composite);
    *primes_out = pr;
    *cnt_out    = cnt;
}

/* --------------------------------------------------------------------- */
/*  Legendre exponent of prime p in n!.                                   */

/* --------------------------------------------------------------------- */
/*  Test whether a given k satisfies the divisor condition.             */
/* --------------------------------------------------------------------- */
static bool is_valid_k(int k, int *primes, size_t primes_cnt)
{
    /* k = 1 or 2 are trivially valid */
    if (k <= 2) return true;

    /* Precompute exponents of primes in k and k-1 */
    int e_k[64], e_k1[64];                 /* primes_cnt ≤ 64 for k ≤ 200 */
    for (size_t i = 0; i < primes_cnt; ++i) {
        int p = primes[i];
        int a = 0, y = k;
        while (y % p == 0) { ++a; y /= p; }
        e_k[i] = a;

        a = 0; y = k - 1;
        while (y % p == 0) { ++a; y /= p; }
        e_k1[i] = a;
    }

    /* Examine all j from 0 to k-2 */
    for (int j = 0; j <= k - 2; ++j) {
        if (j == 0) continue;               /* B0 = 1, no denominator */
        if (j % 2 == 1 && j != 1) continue; /* B_j = 0 for odd j > 1 */

        /* For each prime p such that (p-1) | j, B_j has denominator p */
        for (size_t pi = 0; pi < primes_cnt; ++pi) {
            int p = primes[pi];
            if ((j % (p - 1)) != 0) continue; /* p not in denominator of B_j */

            /* Exponent of p in the binomial coefficient C(k-2, j) */
            int exp_bin = fac_exp(k - 2, p)
                        - fac_exp(j, p)
                        - fac_exp(k - 2 - j, p);

            /* Net exponent of p in the numerator minus denominator:
               exp(p in k) + exp(p in k-1) + exp_bin - 1  (denominator exp is 1) */
            int net = e_k[pi] + e_k1[pi] + exp_bin - 1;
            if (net < 0) return false;        /* denominator remains */
        }
    }
    return true;
}

/* --------------------------------------------------------------------- */
/*  Lazy initialization of the cache of valid terms (up to 200).         */
/* --------------------------------------------------------------------- */
static seq_t *seq_cache   = NULL;
static size_t seq_len     = 0;

static void generate_sequence(void)
{
    if (seq_cache != NULL) return;         /* already generated */

    int *primes = NULL;
    size_t primes_cnt;
    sieve_primes(200, &primes, &primes_cnt);

    size_t cap = 64;
    seq_cache = (seq_t *)malloc(cap * sizeof(seq_t));
    seq_len   = 0;

    for (int k = 1; k <= 200; ++k) {
        if (is_valid_k(k, primes, primes_cnt)) {
            if (seq_len == cap) {
                cap <<= 1;
                seq_cache = (seq_t *)realloc(seq_cache,
                                             cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)k;
        }
    }
    free(primes);
}

/* --------------------------------------------------------------------- */
/*  Public function: returns the nth term of A366169. 0 if n is out of
 *  range.                                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined below offset */
    generate_sequence();
    if (n > seq_len) return 0;   /* beyond known terms */
    return seq_cache[n - 1];
}
