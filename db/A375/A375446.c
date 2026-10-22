/*
 * A375446  Triangle read by rows: T(n, k) = n! * 3^k * hypergeom([-k], [-n],
 *          -1/3).
 * https://oeis.org/A375446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle read by rows of T(n,k) where T(n,k) =
 *            Σ_{j=0}^{k} (-1)^j * C(k,j) * 3^{k-j} * (n-j)!
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   triangle_start
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=5, a(5)=13, … */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/triangle_start.h"

static inline idx_t row_of(idx_t n)
{
    double d = sqrt((double)8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) * 0.5);
    /* correct possible rounding errors */
    while (triangle_start(r + 1) <= n) ++r;
    while (triangle_start(r) > n) --r;
    return r;
}

#define MAXR 500      /* enough for all n up to ~1.25e5          */
static seq_t fact[MAXR + 1];
static seq_t pow3[MAXR + 1];
static seq_t choose[MAXR + 1][MAXR + 1];
static bool initialized = false;

static void init_arrays(void)
{
    if (initialized) return;
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAXR; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    pow3[0] = (seq_t)1;
    for (idx_t i = 1; i <= MAXR; ++i)
        pow3[i] = pow3[i - 1] * (seq_t)3;

    /* Pascal's triangle for binomial coefficients */
    for (idx_t n = 0; n <= MAXR; ++n) {
        choose[n][0] = choose[n][n] = (seq_t)1;
        for (idx_t k = 1; k < n; ++k)
            choose[n][k] = choose[n - 1][k - 1] + choose[n - 1][k];
    }
    initialized = true;
}

seq_t a(idx_t n)
{
    init_arrays();

    idx_t r = row_of(n);                     /* row index      */
    idx_t c = n - triangle_start(r);         /* column index   */

    seq_t pos = (seq_t)0;
    seq_t neg = (seq_t)0;

    for (idx_t j = 0; j <= c; ++j) {
        seq_t term = fact[r - j];            /* (n-j)! part    */
        term *= choose[c][j];                /* binomial part  */
        term *= pow3[c - j];                /* 3^{k-j} part   */
        if (j & 1U)
            neg += term;                     /* subtract odd j  */
        else
            pos += term;                     /* add even j      */
    }
    /* All intermediate subtractions kept positive; the final result is pos - neg */
    return pos - neg;
}
