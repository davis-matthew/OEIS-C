/*
 * A378816  Expansion of 2*(x - 1)^3/(3*x^3 - 5*x^2 + x + 1 + sqrt(-(x -
 *          1)^3*(x + 1)^2*(3*x + 1))).
 * https://oeis.org/A378816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n+1} binomial(n,k-1)*(-1)^k*Sum_{m=0..k-1}
 *            binomial(2*m+2,m)
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 12, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Valid for all n >= 0; a(0) = -1) */

#include <stdint.h>

#define MAX_N 20001   /* enough for the expected range of n (up to 10000) */

static seq_t S[MAX_N];      /* cumulative sums S[j] = sum_{m=0..j} C(2m+2,m) */
static idx_t S_size = 0;    /* number of valid entries in S (>= 1 after init) */
static seq_t last_c = 0;    /* C(2m+2,m) for the last computed m */

/* Ensure that S[0..n] are computed.  The array is statically sized. */
static void ensure_S(idx_t n)
{
    if (S_size > 0 && n < S_size)
        return;                   /* already computed */

    /* First time initialization */
    if (S_size == 0) {
        last_c = 1;   /* C(2*0+2,0) = C(2,0) = 1 */
        S[0] = 1;     /* S[0] = 1 */
        S_size = 1;
    }

    /* Compute missing entries up to n */
    for (idx_t m = S_size; m <= n; ++m) {
        /* Compute C(2m+2, m) from previous value */
        seq_t num = (seq_t)(2*(seq_t)m + 2) * (seq_t)(2*(seq_t)m + 1);
        seq_t den = (seq_t)m * (seq_t)(m + 2);
        seq_t c = last_c * num / den;
        last_c = c;
        S[m] = S[m-1] + c;
    }
    S_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_S(n);                     /* make sure S[0..n] exist */
    seq_t sum = 0;
    seq_t binom = 1;                  /* C(n,0) */
    seq_t sign = -1;                  /* (-1)^1 */

    for (idx_t k = 1; k <= n + 1; ++k) {
        idx_t j = k - 1;               /* binomial index */
        seq_t term = binom * S[j];
        if (sign == -1)
            term = -term;
        sum += term;

        /* Update binomial for next j+1, but avoid division by zero when j==n */
        if (j < n) {
            binom = binom * (n - j) / (j + 1);
        }
        sign = -sign;                  /* flip sign for next k */
    }
    return sum;
}
