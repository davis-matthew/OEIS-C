/*
 * A327322  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial p(x,n) constructed as in Comments; these polynomials
 *          form a strong divisibility sequence.
 * https://oeis.org/A327322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r = ceil((sqrt(8*n+1)-1)/2) and k = n - r*(r-1)/2 - 1. Then
 *            a(n) = binom(r,k) * (5^r + (-1)^(r-k) * 5^k) / g(r), where g(r)
 *            = gcd_{j=0}^{r-1} [ binom(r,j) * (5^r + (-1)^(r-j) * 5^j) ]
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/gcd.h"

static seq_t *pow5_cache = NULL;
static idx_t pow5_len   = 0;
static idx_t pow5_cap   = 0;

static void ensure_pow5(idx_t n)
{
    if (pow5_len <= n) {
        idx_t new_cap = pow5_cap ? pow5_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(pow5_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow5_cache = tmp;
        pow5_cap   = new_cap;
        if (pow5_len == 0) {
            pow5_cache[0] = (seq_t)1;
            pow5_len      = 1;
        }
        for (idx_t i = pow5_len; i <= n; ++i)
            pow5_cache[i] = pow5_cache[i-1] * (seq_t)5;
        pow5_len = n + 1;
    }
}

static seq_t *row_gcd_cache = NULL;
static idx_t row_gcd_len   = 0;
static idx_t row_gcd_cap   = 0;

static void compute_row_gcd(idx_t r)
{
    ensure_pow5(r);
    seq_t pow5_r = pow5_cache[r];
    seq_t g      = (seq_t)0;
    seq_t binom  = (seq_t)1;                 /* C(r,0) */
    for (idx_t j = 0; j < r; ++j) {
        seq_t term = ( ( (r - j) & 1 ) == 0 )
            ? pow5_r - pow5_cache[j]          /* even -> subtract */
            : pow5_r + pow5_cache[j];          /* odd  -> add   */
        seq_t I = binom * term;
        g = gcd(g, I);
        if (j + 1 < r)
            binom = binom * (seq_t)(r - j) / (seq_t)(j + 1);
    }
    row_gcd_cache[r] = g;
}

static void ensure_row_gcd(idx_t n)
{
    if (row_gcd_len <= n) {
        idx_t new_cap = row_gcd_cap ? row_gcd_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(row_gcd_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        row_gcd_cache = tmp;
        row_gcd_cap   = new_cap;
        if (row_gcd_len == 0) {
            row_gcd_cache[0] = (seq_t)0;       /* unused */
            row_gcd_len      = 1;
        }
        for (idx_t r = row_gcd_len; r <= n; ++r)
            compute_row_gcd(r);
        row_gcd_len = n + 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for 0, keep safe */

    long double d = sqrtl((long double)8*n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);
    if (r*(r+1)/2 < n) ++r;

    idx_t prev = (r-1)*r/2;
    idx_t k = n - prev - 1;               /* 0‑based coefficient index */

    ensure_pow5(r);
    ensure_row_gcd(r);

    seq_t pow5_r = pow5_cache[r];
    seq_t pow5_k = pow5_cache[k];

    seq_t binom = (seq_t)1;
    for (idx_t i = 0; i < k; ++i)
        binom = binom * (seq_t)(r - i) / (seq_t)(i + 1);

    seq_t term = ( ( (r - k) & 1 ) == 0 )
        ? pow5_r - pow5_k
        : pow5_r + pow5_k;

    seq_t raw = binom * term;
    seq_t g   = row_gcd_cache[r];
    return raw / g;
}
