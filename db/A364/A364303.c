/*
 * A364303  Square array read by ascending antidiagonals: T(n,k) = [x^k] (1 -
 *          x)^(2*k) * Legendre_P(n*k, (1 + x)/(1 - x)) for n, k >= 0.
 * https://oeis.org/A364303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   T(n,k) = Σ_{i=0..k} binom(n*k, k-i)^2 * binom((n-2)*k+i-1, i)
 *            a(n) is obtained by reading the array T(n,k) along ascending
 *            antidiagonals of the square array, i.e. flattening by increasing
 *            n+k and, for each diagonal, decreasing n from n+k down to 0
 * Range:     i8 n <= 13, i16 n <= 30, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   T, binom_positive, binom_signed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ---------------------------------------------- */
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include "../utils/T.h"
#include "../utils/binom_positive.h"
#include "../utils/binom_signed.h"

/* Convert a global index n into diagonal d and position pos inside
   that diagonal, then compute the corresponding T(n,k).     */
seq_t a(idx_t n)
{
    /* Determine diagonal d such that n lies in it.
       We need the smallest d with n < (d+1)*(d+2)/2.               */
    long double temp = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t d = (idx_t)((temp - 1.0L) / 2.0L);

    /* Correct possible rounding errors.                     */
    while ( (d + 1) * (d + 2) / 2 <= n ) ++d;
    while ( d * (d + 1) / 2 > n ) --d;

    idx_t pos = n - d * (d + 1) / 2;   /* 0 ≤ pos ≤ d       */
    idx_t k   = pos;                   /* column index      */
    idx_t nr  = d - pos;               /* row    index      */

    return T(nr, k);
}
