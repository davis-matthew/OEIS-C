/*
 * A384987  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where column k is the expansion of e.g.f. B(x)^k, where
 *          B(x) is the e.g.f. of A384984.
 * https://oeis.org/A384987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 25, i32 n <= 52, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A384987 – Square array A(n,k) read by antidiagonals downwards  */
/*  a(n) = b(i, -j) where (i,j) denote the position of the n‑th term  */
/*  Recurrence for b(n,k):                                    */
/*      b(0,k) = 1                                            */
/*      b(n,0) = 0   (n > 0)                                   */
/*      for n > 0, k ≠ 0:                                     */
/*          b(n,k) = (-1)^n * k * Σ_{j=1..n}                  */
/*                    (−6n + 5j + k)^(j‑1) * C(n,j) * b(n‑j,3j) */
/*  The sequence is indexed from n = 0.                         */

#include <stdbool.h>
#include "../utils/binomial.h"

/* maximum row index needed for a(n) with n ≤ 10000 */
/* triangular(m)=m*(m+1)/2  ⇒  m ≈ sqrt(2*n)  */
/* m_max ≈ 150 safely covers 0 ≤ n ≤ 10000 */
#define MAX_ROW   150
/* minimal k that may appear in the recursion is –MAX_ROW */
#define MIN_K     (-MAX_ROW)
/* maximal positive k that may appear: 3*MAX_ROW */
#define MAX_K     (3*MAX_ROW)
/* shift to make all k values non‑negative for array indexing */
#define K_SHIFT   (-MIN_K)

/* memoisation tables for b(n,k)  */
static seq_t  B[MAX_ROW+1][MAX_K - MIN_K + 1];
static bool   BVIS[MAX_ROW+1][MAX_K - MIN_K + 1];

/* ------------------------------------------------------------------ */
/*  Recursive evaluation of b(n,k) with memoisation                  */
/* ------------------------------------------------------------------ */
static seq_t bfn(idx_t n, int k)
{
    /* bounds check for safety; should never be violated for valid n */
    if (n > MAX_ROW) {            /* row too large for our tables */
        return (seq_t)0;
    }
    int kIdx = k + K_SHIFT;
    if (kIdx < 0 || kIdx > MAX_K - MIN_K) {
        return (seq_t)0;
    }
    if (BVIS[n][kIdx]) {
        return B[n][kIdx];
    }

    seq_t result;

    /* base cases */
    if (n == 0) {
        result = (seq_t)1;          /* 0^0 = 1   */
    } else if (k == 0) {
        result = (seq_t)0;          /* for n > 0  */
    } else {
        seq_t sum = (seq_t)0;
        for (idx_t j = 1; j <= n; ++j) {
            /* base = -6*n + 5*j + k */
            seq_t base = (seq_t)(-6 * (int)n) + (seq_t)(5 * (int)j) + (seq_t)k;

            /* base^(j-1) */
            seq_t pow = (seq_t)1;
            for (idx_t e = 0; e < j - 1; ++e) {
                pow *= base;
            }

            /* binomial coefficient C(n,j) */
            seq_t bin = binomial(n, j);

            /* recursive subterm b(n-j, 3*j) */
            seq_t sub = bfn(n - j, (int)(3 * j));

            /* term = bin * pow * sub */
            seq_t term = bin * pow;
            term *= sub;

            sum += term;
        }

        /* apply (-1)^n * k */
        if (n & 1) {        /* n odd => multiply by -1 */
            sum = -sum;
        }
        sum *= (seq_t)k;
        result = sum;
    }

    BVIS[n][kIdx] = true;
    B[n][kIdx] = result;
    return result;
}

/* ------------------------------------------------------------------ */
/*  Public function: a(n)                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* locate the antidiagonal (m) that contains index n */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n) {
        ++m;
    }

    /* position within the diagonal */
    idx_t i = n - m * (m + 1) / 2;   /* row   = i */
    idx_t j = m - i;                 /* column = j */

    /* array entry A(i,j) equals b(i, -j) */
    return bfn(i, -(int)j);
}
