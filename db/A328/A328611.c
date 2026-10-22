/*
 * A328611  Irregular triangular array read by rows: row n gives the
 *          coefficients of the second of two factors of even-degree
 *          polynomials described in Comments.
 * https://oeis.org/A328611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabf, sign
 * Formula:   let k be the unique integer with k(k+3)/2 ≥ n, and let j = n –
 *            k(k+2)/2 – 1 (0 ≤ j ≤ k). For k = 1: a(n) = (j==0 ? 0 : –1). For
 *            k ≥ 2: a(n) = Lucas(k-1-j) * binomial(k, j). Here Lucas(–1) = –1
 * Range:     i8 n <= 29, i16 n <= 94, i32 n <= 332, i64 n <= 1239,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* return Lucas number L(n).  For n == -1 return -1. */
static seq_t lucas_value(int n)
{
    if (n < 0)
        return (seq_t)(-1);          /* Lucas(-1) = -1 */
    seq_t L0 = 2;
    if (n == 0)
        return L0;
    seq_t L1 = 1;
    if (n == 1)
        return L1;
    seq_t a = L0, b = L1, c;
    for (int i = 2; i <= n; ++i) {
        c = a + b;
        a = b;
        b = c;
    }
    return b;
}

seq_t a(idx_t n)
{
    /* find the row k that contains the nth term */
    idx_t k = 1;
    idx_t cum = 0;                /* total terms in preceding rows */
    while (cum + (k + 1) < n) {
        cum += (k + 1);
        ++k;
    }

    /* position j within the row (0‑based) */
    idx_t j = n - cum - 1;        /* 0 <= j <= k */

    /* first row is special */
    if (k == 1) {
        return (j == 0) ? (seq_t)0 : (seq_t)(-1);
    }

    /* Lucas index */
    int lucas_index = (int)k - (int)j - 1;
    seq_t lucas = lucas_value(lucas_index);

    /* binomial coefficient C(k, j) */
    seq_t binom = 1;
    idx_t t = k - j;
    for (idx_t i = 1; i <= j; ++i) {
        binom = binom * (t + i) / i;
    }

    return lucas * binom;
}
