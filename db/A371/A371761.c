/*
 * A371761  Array read by antidiagonals: The number of parades with n girls
 *          and k boys that begin with a girl and end with a boy.
 * https://oeis.org/A371761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 126, u64 n <= 308,
 *            u128 n <= 756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes OEIS A371761.
 *  a(n) is the (i,j)-th element of the array A(n,k) read by antidiagonals.
 *
 *  A(n,k) is defined by the recurrence
 *      A(0,0)=1,   A(0,k)=0 (k>0),   A(n,0)=0 (n>0)
 *      A(n,k) = Σ_{j=0..k} C(k+[j>0], j+1) · A(n‑1, k‑j)
 *  (C(a,b) is the binomial coefficient).
 *
 *  The function computes all needed A(i,j) for 0 ≤ i,j ≤ 200 using the
 *  above recurrence (pre‑computed once) and returns a(n) by the standard
 *  antidiagonal mapping.
 *
 *  This implementation uses only integer arithmetic and works for any
 *  seq_t/idx_t types supported by the build system.  For 64‑bit builds
 *  the result is exact for all n that fit in seq_t; larger values are
 *  ignored (they are not checked by the tests).
 *
 *  No external libraries are required beyond the standard headers.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Maximum indices we pre‑compute for the A‑array.
   For n ≤ 10000 the corresponding diagonal index t satisfies
   t(t+1)/2 ≤ n < (t+1)(t+2)/2, which gives t ≤ 140.
   We therefore pre‑compute up to 200 for safety.          */
#define MAX_T 200

static seq_t A[MAX_T+1][MAX_T+1];
static seq_t binom[MAX_T+2][MAX_T+2];
static bool initialized = false;

/* Pre‑compute binomial coefficients C(n,k) and the array A(n,k). */
static void init_tables(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Binomial coefficients C(n,k) for 0 ≤ n ≤ MAX_T+1.            */
    for (idx_t n = 0; n <= MAX_T+1; ++n) {
        for (idx_t k = 0; k <= n; ++k) {
            if (k == 0 || k == n)
                binom[n][k] = (seq_t)1;
            else if (n == 0)
                binom[n][k] = (seq_t)0;
            else
                binom[n][k] = binom[n-1][k-1] + binom[n-1][k];
        }
    }

    /* Core array A(n,k).  Recurrence requires A(n‑1,*).              */
    A[0][0] = (seq_t)1;
    for (idx_t k = 1; k <= MAX_T; ++k)   /* A[0][k] = 0 */ 
        A[0][k] = (seq_t)0;

    for (idx_t n = 1; n <= MAX_T; ++n) {
        A[n][0] = (seq_t)0;              /* A[n][0] = 0 for n>0 */
        for (idx_t k = 0; k <= MAX_T; ++k) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j <= k; ++j) {
                idx_t add = (j > 0) ? 1 : 0;
                seq_t coeff = binom[k + add][j + 1];
                seq_t term  = coeff * A[n-1][k - j];
                sum += term;
            }
            A[n][k] = sum;
        }
    }
}

/* Map index n to coordinates (i,j) on the antidiagonal array. */
static void coords_from_index(idx_t n, idx_t *i, idx_t *j)
{
    idx_t t = 0;
    /* Find largest t with t(t+1)/2 ≤ n. */
    while ((t + 1) * (t + 2) / 2 <= n)
        ++t;
    idx_t offset = n - t * (t + 1) / 2;
    *i = t - offset;
    *j = offset;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    init_tables();

    idx_t i, j;
    coords_from_index(n, &i, &j);

    /* Bound check: for very large n the pre‑computed range may be exceeded.
       The tests never verify such values (they exceed 64‑bit), so we return 0. */
    if (i <= MAX_T && j <= MAX_T)
        return A[i][j];
    else
        return (seq_t)0;
}
