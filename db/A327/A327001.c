/*
 * A327001  Generalized Bell numbers, square array read by ascending
 *          antidiagonals, A(n, k) for n, k >= 0.
 * https://oeis.org/A327001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 38, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A327001: Generalized Bell numbers
   A(n,k) = Sum_{j=0..k-1} C(n*k-1, n*j) * A(n,j)   for k>0,  A(n,0)=1
   a(m) is read by antidiagonals: a(m) = A(n_row, k_col) where
   n_row = s - (m - T(s)),  k_col = m - T(s),  with T(s)=s*(s+1)/2
   The first two terms of row n=0 are special: A(0,0)=A(0,1)=1, for k>=2
   A(0,k)=2^{k-1}.                                             */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Triangular number T(x)=x*(x+1)/2, using idx_t to avoid overflow. */
static inline idx_t triangular(idx_t x)
{
    return (x * (x + 1)) >> 1;
}

/* Compute A(n,k) for n>=1, k>=0.  The result is stored locally,
   using a fixed buffer whose size covers all k that may arise for
   n up to about 10,000 (k<=140).  Uses only seq_t arithmetic. */
static seq_t compute_A(idx_t n, idx_t k)
{
    /* For k==0 the result is always 1. */
    if (k == 0ULL) return (seq_t)1;

    /* Buffer to hold intermediate values A(n,0..k).  200>140 needed. */
    static seq_t buf[200];
    buf[0] = (seq_t)1;          /* A(n,0)=1 */

    for (idx_t m = 1ULL; m <= k; ++m) {
        seq_t sum = (seq_t)0;
        idx_t a = n * m - 1ULL;   /* n*k-1 */
        for (idx_t j = 0ULL; j < m; ++j) {
            idx_t b = n * j;          /* n*j */
            seq_t bin = binomial(a, b);
            sum += buf[j] * bin;
        }
        buf[m] = sum;
    }
    return buf[k];
}

/* Main sequence entry.  Handles all n >= 0 and any call order. */
seq_t a(idx_t m)
{
    /* Locate the antidiagonal (s) containing index m. */
    idx_t s = 0;
    while (triangular(s + 1ULL) <= m) ++s;

    idx_t offset = m - triangular(s);       /* position on diagonal */
    idx_t n_row  = s - offset;               /* row index n */
    idx_t k_col  = offset;                   /* column index k */

    if (n_row == 0ULL) {
        /* Special values for n=0: 1,1,2,4,8,...  (k>=2 -> 2^{k-1}) */
        if (k_col <= 1ULL) return (seq_t)1;
        return big_pow((seq_t)2, (seq_t)(k_col - 1ULL));
    }

    /* For n>=1 use the recurrence. */
    return compute_A(n_row, k_col);
}
