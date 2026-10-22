/*
 * A335947  T(n, k) = numerator([x^k] b_n(x)), where b_n(x) = Sum_{k=0..n}
 *          binomial(n,k)* Bernoulli(k, 1/2)*x^(n-k). Triangle read by rows,
 *          for n >= 0 and 0 <= k <= n.
 * https://oeis.org/A335947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, tabl
 * Formula:   a(n) = numerator of coefficient of x^k in * b_r(x) = Σ_{j=0}^{r}
 *            binomial(r,j) * B_j(1/2) * x^{r-j} * where r =
 *            floor((sqrt(8n+1)-1)/2) (row number) and * k = n - r*(r+1)/2
 *            (column in that row). * * The coefficient is binomial(r,k) *
 *            (2^{1-(r-k)} - 1)*B_{r-k}, * divided by 2^{r-k-1}. The
 *            numerato...
 * Range:     i8 n <= 15, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

/* ---------- Helpers for rational arithmetic ---------- */

static seq_t gcd_seq(seq_t a, seq_t b)
{
    if (a < (seq_t)0) a = -a;
    if (b < (seq_t)0) b = -b;
    if (a == (seq_t)0) return b;
    if (b == (seq_t)0) return a;
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;                     /* always positive */
}

/* ---------- Bernoulli numbers B_n = num/den ---------- */

static seq_t *ber_num = NULL;   /* numerator of B_n */
static seq_t *ber_den = NULL;   /* denominator of B_n (positive) */
static idx_t bern_max = 0;      /* highest n for which B_n is computed */

static void compute_bernoulli(idx_t max_n)
{
    if (max_n <= bern_max) return;

    /* Allocate or grow arrays */
    size_t new_len = (size_t)(max_n + 1);
    seq_t *new_num = (seq_t *)realloc(ber_num, new_len * sizeof(seq_t));
    seq_t *new_den = (seq_t *)realloc(ber_den, new_len * sizeof(seq_t));
    if (!new_num || !new_den) abort();   /* memory allocation failed */

    ber_num = new_num;
    ber_den = new_den;

    /* Init B_0 = 1 */
    if (bern_max == 0) {
        ber_num[0] = (seq_t)1;
        ber_den[0] = (seq_t)1;
    }

    for (idx_t n = bern_max + 1; n <= max_n; ++n) {
        /* sum_{k=0}^{n-1} binom(n+1,k) * B_k */
        seq_t sum_num = (seq_t)0;
        seq_t sum_den = (seq_t)1;
        for (idx_t k = 0; k < n; ++k) {
            seq_t bin = binomial(n + 1, k);          /* binomial(n+1,k) */
            seq_t term_num = bin * ber_num[k];
            seq_t term_den = ber_den[k];
            /* add term_num/term_den to sum_num/sum_den */
            seq_t new_num = sum_num * term_den + term_num * sum_den;
            seq_t new_den = sum_den * term_den;
            seq_t g = gcd_seq(new_num, new_den);
            sum_num = new_num / g;
            sum_den = new_den / g;
        }
        /* B_n = - sum / (n+1) */
        seq_t num = -sum_num;
        seq_t den = sum_den * (seq_t)(n + 1);
        seq_t g = gcd_seq(num, den);
        ber_num[n] = num / g;
        ber_den[n] = den / g;
    }
    bern_max = max_n;
}

/* ---------- Find row r for flattened index n ---------- */

static inline idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while ((__int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Convert flattened index to (row r, column k) */
    idx_t r = find_row(n);
    idx_t first_idx = (__int128)r * (r + 1) / 2;
    idx_t k = n - first_idx;          /* column = 0..r */
    idx_t m = r - k;                 /* index of Bernoulli number */

    /* binomial(r,k) is needed */
    seq_t bin = binomial(r, k);

    /* Handle the trivial case m == 0 directly */
    if (m == 0) {
        return bin;   /* coefficient is an integer */
    }

    /* Ensure Bernoulli numbers up to m are available */
    compute_bernoulli(m);

    /* Compute factor (2^{1-m} - 1) as (1 - 2^{m-1}) / 2^{m-1} */
    seq_t pow2 = pow_seq((seq_t)2, m - 1);            /* 2^(m-1) */
    seq_t factor_num = (seq_t)1 - pow2;              /* 1 - 2^(m-1) */
    seq_t factor_den = pow2;                         /* 2^(m-1) */

    /* B_m = num/den from precomputed arrays */
    seq_t B_num = ber_num[m];
    seq_t B_den = ber_den[m];

    /* Combine everything: numerator = bin * factor_num * B_num
     *                    denominator = factor_den * B_den
     */
    seq_t num = bin * factor_num * B_num;
    seq_t den = factor_den * B_den;

    seq_t g = gcd_seq(num, den);
    return num / g;
}
