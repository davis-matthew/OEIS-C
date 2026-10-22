/*
 * A323182  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) is Chebyshev polynomial of the second kind U_{n}(x),
 *          evaluated at x=k.
 * https://oeis.org/A323182
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is read from the square array T(n,k) = U_n(k) where
 *            U_0(k)=1, U_1(k)=2*k, and for m>1: U_m(k) = 2*k*U_{m-1}(k) -
 *            U_{m-2}(k). The sequence is obtained by antidiagonal traversal:
 *            for a given index n, let d be the largest integer with d*(d+1)/2
 *            <= n. Then i = n - d*(d+1)/2 (row index) k = d - i...
 * Range:     i8 n <= 23, i16 n <= 50, i32 n <= 128, i64 n <= 315,
 *            i128 n <= 847, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Compute U_n(k) using the recurrence. */
static seq_t chebyshev_U(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2 * (seq_t)k;

    seq_t prev2 = (seq_t)1;                     /* U_0(k)  */
    seq_t prev1 = (seq_t)2 * (seq_t)k;           /* U_1(k)  */
    seq_t curr   = 0;
    seq_t dk     = (seq_t)k;

    for (idx_t t = 2; t <= n; ++t) {
        curr   = (seq_t)2 * dk * prev1 - prev2;
        prev2  = prev1;
        prev1  = curr;
    }
    return curr;
}

/* Main function requested by the build */
seq_t a(idx_t n)
{
    /* Find diagonal index d: largest d with d*(d+1)/2 <= n */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n) ++d;

    /* Coordinates (i,k) for the antidiagonal element */
    idx_t i = n - d * (d + 1) / 2;
    idx_t k = d - i;

    return chebyshev_U(i, k);
}
