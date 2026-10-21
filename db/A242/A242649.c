/*
 * A242649  Dirichlet g.f.: Product_{n>=2} 1/(1-1/n^s)^d(n), where d(n) =
 *          A000005(n) is the number of divisors of n.
 * https://oeis.org/A242649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of 1/n^s in ∏_{m≥2} (1 - 1/m^s)^(-d(m))
 *            (d(m)=A000005(m))
 * Range:     u8 n <= 71, u16 n <= 5759, u32 n <= 9604, u64 n <= 9604,
 *            u128 n <= 9604, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* forward declarations */
static seq_t rec_func(idx_t n, idx_t min_base);
static void ensure_precomputed(idx_t N);
static seq_t binom_small(seq_t D, idx_t e);

/* global storage for divisor data */
static idx_t *dCount = NULL;          /* number of divisors of i (i>=1)          */
static idx_t **divisors_arr = NULL;   /* array of divisors for each i           */
static idx_t precomputedMax = 0;      /* maximum index already pre‑computed     */

/* ------------------------------------------------------------------ */
/* binomial coefficient  C(D + e - 1, e)  where D and e are small   */
static seq_t binom_small(seq_t D, idx_t e)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= e; ++i) {
        result = result * (D + (seq_t)(i - 1)) / (seq_t)i;
    }
    return result;
}

/* ------------------------------------------------------------------ */
/* recursive enumeration of factorizations of n with bases >= min_base */
static seq_t rec_func(idx_t n, idx_t min_base)
{
    if (n == 1)          /* empty factorization, weight 1 */
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    idx_t *divs = divisors_arr[n];
    idx_t dcnt = dCount[n];

    for (idx_t idx = 0; idx < dcnt; ++idx) {
        idx_t k = divs[idx];
        if (k < min_base || k == 1)     /* skip forbidden bases */
            continue;

        /* largest exponent e such that k^e divides n */
        idx_t temp = n;
        idx_t emax = 0;
        while (temp % k == 0) {
            temp /= k;
            ++emax;
        }

        idx_t power = 1;               /* holds k^e */
        for (idx_t e = 1; e <= emax; ++e) {
            power *= k;
            seq_t weight = binom_small((seq_t)dCount[k], e);
            seq_t rest   = rec_func(n / power, k + (idx_t)1);
            sum += weight * rest;
        }
    }
    return sum;
}

/* ------------------------------------------------------------------ */
/* precompute divisor lists up to N (inclusive) */
static void ensure_precomputed(idx_t N)
{
    if (precomputedMax >= N)
        return;

    /* resize arrays to hold data up to N */
    dCount = (idx_t *)realloc(dCount, (N + 1) * sizeof(idx_t));
    divisors_arr = (idx_t **)realloc(divisors_arr, (N + 1) * sizeof(idx_t *));

    /* first count divisors */
    for (idx_t i = 1; i <= N; ++i)
        dCount[i] = 0;
    for (idx_t i = 1; i <= N; ++i)
        for (idx_t j = i; j <= N; j += i)
            dCount[j]++;

    /* allocate space for divisors arrays */
    for (idx_t i = 1; i <= N; ++i)
        divisors_arr[i] = (idx_t *)malloc(dCount[i] * sizeof(idx_t));

    /* temporary indices for filling */
    idx_t *fill_idx = (idx_t *)calloc((N + 1), sizeof(idx_t));
    for (idx_t i = 1; i <= N; ++i)
        for (idx_t j = i; j <= N; j += i) {
            idx_t pos = fill_idx[j]++;
            divisors_arr[j][pos] = i;
        }
    free(fill_idx);

    precomputedMax = N;
}

/* ------------------------------------------------------------------ */
/* public sequence function a(n) */
seq_t a(idx_t n)
{
    ensure_precomputed(n);          /* make divisor data up to n available */
    return rec_func(n, (idx_t)2);   /* start with minimum base 2          */
}
