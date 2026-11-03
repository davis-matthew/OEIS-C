/*
 * A290826  The sequence b(n,m) of the m polynomial coefficients of the n-th
 *          order B-spline wavelet scaled by n!2^n, read by rows, with n in
 *          {0,1,2,...} and m in {1,2,3,...,2(n+1)^2}.
 * https://oeis.org/A290826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 19, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 59 (2026-11-01)
 * Helpers:   binomial, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = coefficient of the polynomial expansion of the n‑th
 * order B‑spline wavelet B_n(x), scaled by n!·2^n, read row by row.
 *
 * For a given global index idx the function returns the exact integer
 * coefficient, using the formula:
 *
 *   coeff(row, col) = binomial(n, col) * 2^(n-col) * (-1)^col *
 *                     Σ_{a=0}^{row} (-1)^a binomial(2n+2, a) * a^col
 *
 * where:
 *   n       – block number (order of the spline)
 *   row     – row in the (2n+2)×(n+1) matrix A_n (0‑based)
 *   col     – column in the matrix (0‑based)
 *
 * The function works with any integer type seq_t and idx_t supplied by
 * the build environment, using only integer arithmetic. It relies on
 * the utility functions `pow_uint` (for a^p) and `binomial` (for n choose k).
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_u64_idx.h"

static inline seq_t Cum(idx_t n)
{
    /* cumulative number of terms up to block n (inclusive) */
    seq_t a = (seq_t)(n + 1);
    seq_t b = (seq_t)(n + 2);
    seq_t c = (seq_t)(2 * n + 3);
    return a * b * c / 3;            /* (n+1)(n+2)(2n+3)/3 */
}

static idx_t find_block(idx_t idx)
{
    seq_t idxt = (seq_t)idx;

    idx_t low = 0;
    idx_t high = 1;
    /* find high such that Cum(high) > idx */
    while (true) {
        seq_t cumh = Cum(high);
        if (cumh > idxt)
            break;
        if (high > ((idx_t)-1) >> 1) {          /* avoid overflow */
            high = (idx_t)-1;
            break;
        }
        high <<= 1;
    }

    /* binary search for smallest n with Cum(n) > idx */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t cumm = Cum(mid);
        if (cumm > idxt)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

static seq_t my_pow_uint(uint64_t base, idx_t exp)
{
    /* exponentiation by squaring, returns seq_t */
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1)
            result *= b;
        b *= b;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t idx)
{
    /* 1. locate block (order n) and local position */
    idx_t n = find_block(idx);                 /* 0‑based order */
    idx_t prevCumIdx = (idx_t)((n == 0) ? 0 : Cum(n - 1));
    idx_t localIdx = idx - prevCumIdx;         /* 0‑based position within block */

    idx_t row    = localIdx / (n + 1);          /* 0‑based row (0..2n+1) */
    idx_t col    = localIdx % (n + 1);          /* 0‑based column (0..n) */

    /* 2. compute Σ_{a=0}^{row} (-1)^a C(2n+2, a) * a^col */
    seq_t sumS  = 0;
    seq_t binom = 1;                            /* C(2n+2,0) */
    seq_t N = (seq_t)(2 * n + 2);

    for (idx_t a = 0; a <= row; ++a) {
        seq_t a_pow = my_pow_uint((uint64_t)a, col);  /* a^col */
        seq_t term  = binom * a_pow;
        if (a & 1)
            term = -term;
        sumS += term;

        /* prepare binom for next a */
        if (a < row) {
            seq_t num = binom * (N - a);
            binom = num / (a + 1);
        }
    }

    /* 3. final coefficient */
    seq_t coeff = binomial((idx_t)n, col);                     /* C(n, col) */
    coeff *= ipow_u64_idx(2u, (idx_t)(n - col));      /* 2^(n-col) */
    if (col & 1)                                   /* (-1)^col */
        coeff = -coeff;
    coeff *= sumS;

    return coeff;
}
