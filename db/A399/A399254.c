/*
 * A399254  Rectangular array read by descending antidiagonals: r(n, k) =
 *          floor(k*x^n), where x = 2+sqrt(2).
 * https://oeis.org/A399254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, changed
 * Formula:   a(n) = r(i,j) where i,j are obtained from the n‑th entry when
 *            the rectangle r(m,k)=⌊k*(2+√2)^m⌋ is read by antidiagonals in
 *            increasing order of (m+k)
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   The computation uses the recurrence for (2+√2)^m =
     A_m + B_m√2 :
          A_{m+1} = 2*A_m + 2*B_m
          B_{m+1} = A_m  + 2*B_m
     with A_1 = 2, B_1 = 1.
   Then ⌊k*A_m⌋ + ⌊k*B_m√2⌋ gives the required value. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

/* Cached powers of (2+√2) in the form A_m + B_m√2. */
static seq_t *A = NULL;
static seq_t *B = NULL;
static idx_t cap = 0;          /* maximum m for which A,B are computed */

/* Ensure that A[m] and B[m] are available for all m <= n. */
static void
ensure_power(idx_t n)
{
    if (cap >= n)
        return;

    idx_t new_cap = (cap == 0) ? 2 : cap;
    while (new_cap <= n)
        new_cap <<= 1;                     /* double until enough */

    /* Reallocate storage; +1 to make the array 1‑indexed. */
    A = (seq_t *)realloc(A, (new_cap + 1) * sizeof(seq_t));
    B = (seq_t *)realloc(B, (new_cap + 1) * sizeof(seq_t));

    if (cap == 0) {      /* initialise the base case */
        A[1] = 2;
        B[1] = 1;
        cap = 1;
    }

    for (idx_t i = cap + 1; i <= new_cap; ++i) {
        A[i] = (seq_t)2 * A[i - 1] + (seq_t)2 * B[i - 1];
        B[i] = A[i - 1] + (seq_t)2 * B[i - 1];
    }
    cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(0) is unused */
        return 0;

    /* Find diagonal k such that k(k-1)/2 < n <= k(k+1)/2. */
    idx_t k = 1;
    while ((k * (k + 1)) / 2 < n)
        ++k;

    idx_t prev = (k - 1) * k / 2;     /* terms before this diagonal   */
    idx_t pos  = n - prev - 1;        /* 0‑based position in diagonal */
    idx_t i = pos + 1;                /* row index (power of x)      */
    idx_t j = k - pos;                /* column index (multiplier k) */

    ensure_power(i);
    seq_t A_i = A[i];
    seq_t B_i = B[i];

    /* floor(j * (A_i + B_i√2)) = j*A_i + floor(j*B_i√2) */
    seq_t M = (seq_t)j * B_i;                    /* j * B_i          */
    seq_t T = M * M;                              /* M^2              */
    T = T + T;                                    /* 2*M^2            */
    seq_t floor_M_sqrt2 = floor_sqrt(T);          /* floor(M*√2)       */

    return (seq_t)j * A_i + floor_M_sqrt2;        /* final result    */
}
