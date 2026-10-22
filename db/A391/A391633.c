/*
 * A391633  Triangle read by rows: T(n,k) = Sum_{j=0..k} (-1)^(k-j) *
 *          binomial(k,j) * (4+j)^n.
 * https://oeis.org/A391633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = Sum_{j=0..c} (-1)^(c-j) * binomial(c,j) * (4+j)^r where r
 *            = floor((sqrt(8*n+1)-1)/2) (row index) c = n - r*(r+1)/2 (column
 *            index)
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute small binomial coefficient nCk for non‑negative n,k with n>=k. */
static seq_t binom_small(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i) / i;
    }
    return res;
}

/* Compute base^exp using exponentiation by squaring. */
static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result *= b;
        b *= b;
        e >>= 1;
    }
    return result;
}

/* OEIS A391633: sequence read by rows of the triangle T(n,k). */
seq_t a(idx_t n)
{
    /* Find row r such that start = r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n) ++r;
    if (r > 0) --r;
    idx_t start = (r * (r + 1)) / 2;
    idx_t c = n - start;  /* column within the row */

    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    seq_t binom   = 1;     /* binom(c,0) */
    for (idx_t j = 0; j <= c; ++j) {
        seq_t power = pow_int((seq_t)(4 + j), r);
        seq_t term  = binom * power;
        /* (-1)^(c-j) : even -> positive, odd -> negative */
        if (((c - j) & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;

        /* Prepare binomial for next j (if needed). */
        if (j < c)
            binom = binom * (c - j) / (j + 1);
    }

    return sum_pos - sum_neg;
}
