/*
 * A318450  Denominators of the sequence whose Dirichlet convolution with
 *          itself yields A001511, the 2-adic valuation of 2n.
 * https://oeis.org/A318450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 242, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A318450 – Denominators of the Dirichlet square root of the 2‑adic valuation
 *  f(1) = 1
 *  f(n) = (1/2) * ( v2(2n) - Σ_{d|n, 1<d<n} f(d) * f(n/d) )   (n>1)
 *  a(n) = denominator of f(n)
 *
 *  This implementation memoises all intermediate values.
 */

#include <stddef.h>
#include <stdlib.h>

#if defined(__cplusplus)
#include <cstdint>   /* for uint64_t if used */
#endif

/* ---------------------------------------------------------------------- */
/* Helpers */

/* Count trailing zeros of a u64 number (valid for n > 0). */
static inline idx_t trailing_zeros(idx_t n)
{
    idx_t c = 0;
    while ((n & 1) == 0)
    {
        ++c;
        n >>= 1;
    }
    return c;
}

/* Add term_num / 2^term_exp to the fraction sum_num / 2^sum_exp.
 * The result is kept in lowest terms (common powers of two cancelled). */
static void add_fraction(seq_t *sum_num, idx_t *sum_exp,
                         seq_t term_num, idx_t term_exp)
{
    if (*sum_num == 0)
    {
        *sum_num = term_num;
        *sum_exp = term_exp;
        return;
    }
    idx_t e = (*sum_exp > term_exp) ? *sum_exp : term_exp;
    idx_t shift1 = e - *sum_exp;
    idx_t shift2 = e - term_exp;

    seq_t left  = *sum_num << shift1;
    seq_t right = term_num << shift2;
    *sum_num = left + right;
    *sum_exp = e;

    /* cancel common powers of two */
    while ((*sum_num & 1) == 0 && *sum_exp > 0)
    {
        *sum_num >>= 1;
        (*sum_exp)--;
    }
}

/* ---------------------------------------------------------------------- */
/* Cached data for f(n) : numerator N[n] and exponent of 2 in denominator E[n]. */
static seq_t *N = NULL;
static idx_t *E = NULL;
static idx_t cache_size = 0;

/* Ensure the cache can hold indices up to n. */
static void grow_cache(idx_t n)
{
    if (n < cache_size)
        return;

    idx_t new_size = n + 1;   /* indices 0..n inclusive, 0 unused */
    seq_t *newN;
    idx_t *newE;

#if defined(__cplusplus)
    newN = new seq_t[new_size];
    newE = new idx_t[new_size];
    if (N)
    {
        for (idx_t i = 0; i < cache_size; ++i)
            newN[i] = N[i];
        delete[] N;
    }
    if (E)
    {
        for (idx_t i = 0; i < cache_size; ++i)
            newE[i] = E[i];
        delete[] E;
    }
#else
    newN = (seq_t *)realloc(N, new_size * sizeof(seq_t));
    newE = (idx_t *)realloc(E, new_size * sizeof(idx_t));
#endif

    for (idx_t i = cache_size; i < new_size; ++i)
    {
        newN[i] = 0;
        newE[i] = 0;
    }
    N = newN;
    E = newE;
    cache_size = new_size;
}

/* ---------------------------------------------------------------------- */
/* Compute f(n) and store its numerator N[n] and exponent E[n]. */
static void compute_f(idx_t n)
{
    if (n < 1)
        return;                      /* undefined for 0 */

    grow_cache(n);
    if (N[n] != 0)
        return;                      /* already computed */

    if (n == 1)
    {
        N[1] = 1;
        E[1] = 0;
        return;
    }

    /* v2(2n) = 1 + v2(n) */
    idx_t v = 1 + trailing_zeros(n);
    seq_t v_seq = (seq_t)v;

    seq_t sum_num = 0;
    idx_t sum_exp = 0;

    for (idx_t d = 2; d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;
        idx_t e = n / d;
        compute_f(d);
        compute_f(e);

        seq_t nd = N[d];
        seq_t ne = N[e];
        idx_t ed = E[d];
        idx_t ee = E[e];

        seq_t prod_num = nd * ne;
        idx_t prod_exp = ed + ee;

        if (d == e)
        {
            /* one term f(d) * f(e) */
            add_fraction(&sum_num, &sum_exp, prod_num, prod_exp);
        }
        else
        {
            /* two terms: f(d)*f(e) + f(e)*f(d) = 2 * product */
            if (prod_exp > 0)
            {
                /* 2 * (num/2^exp) = num / 2^(exp-1) */
                add_fraction(&sum_num, &sum_exp, prod_num, prod_exp - 1);
            }
            else
            {
                /* exp == 0, integer: double numerator */
                add_fraction(&sum_num, &sum_exp,
                             prod_num << 1, 0);
            }
        }
    }

    /* f(n) = (v - sum) / 2  ->  num_tmp / 2^(sum_exp+1) */
    seq_t num_tmp = (v_seq << sum_exp) - sum_num;
    idx_t denom_exp = sum_exp + 1;

    /* reduce by common powers of two */
    while ((num_tmp & 1) == 0 && denom_exp > 0)
    {
        num_tmp >>= 1;
        denom_exp--;
    }

    N[n] = num_tmp;
    E[n] = denom_exp;
}

/* ---------------------------------------------------------------------- */
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n = 0, safe value */

    compute_f(n);
    /* a(n) = denominator of f(n) = 2^E[n] */
    return (seq_t)1 << E[n];
}
