/*
 * A248977  Triangle read by rows: T(n,k) is the coefficient A_k in the
 *          transformation of 1 + x + x^2 + ... + x^n to the polynomial
 *          A_k*(x+3k)^k for 0 <= k <= n.
 * https://oeis.org/A248977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 10, i16 n <= 22, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   big_pow, binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A248977
   a(n) = coefficient T(r,k) where n = r(r+1)/2 + k, 0 ≤ k ≤ r.
   The coefficients are obtained by solving  
       1 + x + … + x^r = Σ_{k=0}^r A_k (x + 3k)^k.            */

/* Use helper binom() from utils/binom.h and big_pow() from utils/big_pow.h. */
#include <stdlib.h>
#include "../utils/binom.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r such that n is in the r‑th row. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index k within that row. */
    idx_t k = n - r * (r + 1) / 2;          /* 0 ≤ k ≤ r */

    /* Allocate array for the coefficients T[0..r]. */
    seq_t *T = (seq_t *)malloc((r + 1) * sizeof(seq_t));
    if (!T) {
        /* Allocation failure: return zero as safe fallback. */
        return 0;
    }

    /* The top‑degree coefficient is always 1. */
    T[r] = 1;

    /* Backward recursion for lower coefficients. */
    for (idx_t j = r; j-- > 0; ) {
        seq_t sum = 0;
        for (idx_t m = j + 1; m <= r; ++m) {
            seq_t coeff = binom(m, j);                       /* binom(m,j)          */
            seq_t base  = (seq_t)(3 * m);                    /* 3m                   */
            idx_t expidx = m - j;                            /* m-j                 */
            seq_t power = big_pow(base, (seq_t)expidx);       /* (3m)^(m-j)           */
            seq_t term  = T[m] * coeff * power;              /* A_m * binom * power  */
            sum += term;
        }
        T[j] = 1 - sum;                                      /* solve for A_j        */
    }

    seq_t result = T[k];
    free(T);
    return result;
}
