/*
 * A268297  G.f. satisfies: A(x) = Sum_{n>=1} (1 + x^n)/x^n * A(x)^(2*n),
 *          where A(0)=0 and A'(0)=1.
 * https://oeis.org/A268297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 14, i64 n <= 25, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268297
   Power‑series coefficients of the algebraic series A(x) satisfying
       A^4 + 2A^3 - (1+x)A^2 - (1+x)A + x = 0,
   with A(0)=0, A'(0)=1.
   Recurrence (derived from equating coefficients):
       a(1) = 1,
       a(n) = C4[n] + 2*C3[n] - C2[n] - C2[n-1] - a[n-1]   (n ≥ 2)
   where
       C2[n] = Σ_{i=1}^{n-1} a[i] * a[n-i]
       C3[n] = Σ_{i=1}^{n-2} a[i] * C2[n-i]
       C4[n] = Σ_{i=1}^{n-3} a[i] * C3[n-i]
*/

#include <stdlib.h>   /* malloc, free, calloc, memcpy */
#include <string.h>   /* memcpy */

static seq_t *A      = NULL;   /* a(n)   */
static seq_t *C2     = NULL;   /* C2[n]  */
static seq_t *C3     = NULL;   /* C3[n]  */
static idx_t  computed_max = 0; /* largest n already computed */

/* Ensure the internal tables contain values up to index n (inclusive). */
static void grow_to(idx_t n)
{
    if (n <= computed_max) return;

    size_t new_cap = (size_t)(n + 1);   /* indices 0 .. n */

    /* Allocate new arrays (zero‑initialized). */
    seq_t *newA  = (seq_t *)calloc(new_cap, sizeof(seq_t));
    seq_t *newC2 = (seq_t *)calloc(new_cap, sizeof(seq_t));
    seq_t *newC3 = (seq_t *)calloc(new_cap, sizeof(seq_t));

    /* Copy existing contents. */
    if (A)   memcpy(newA,  A,  (size_t)(computed_max + 1) * sizeof(seq_t));
    if (C2)  memcpy(newC2, C2, (size_t)(computed_max + 1) * sizeof(seq_t));
    if (C3)  memcpy(newC3, C3, (size_t)(computed_max + 1) * sizeof(seq_t));

    /* Free old arrays. */
    free(A);  free(C2);  free(C3);

    /* Use new arrays. */
    A = newA;  C2 = newC2;  C3 = newC3;

    /* Compute values from computed_max + 1 up to n. */
    for (idx_t m = computed_max + 1; m <= n; ++m)
    {
        /* C2[m] = Σ_{i=1}^{m-1} a[i]*a[m-i] */
        seq_t sum2 = 0;
        for (idx_t i = 1; i < m; ++i)
            sum2 += A[i] * A[m - i];
        C2[m] = sum2;

        /* C3[m] = Σ_{i=1}^{m-2} a[i]*C2[m-i] */
        seq_t sum3 = 0;
        for (idx_t i = 1; i + 1 < m; ++i)
            sum3 += A[i] * C2[m - i];
        C3[m] = sum3;

        /* C4[m] = Σ_{i=1}^{m-3} a[i]*C3[m-i] */
        seq_t sum4 = 0;
        for (idx_t i = 1; i + 2 < m; ++i)
            sum4 += A[i] * C3[m - i];

        /* a[m] = C4[m] + 2*C3[m] - C2[m] - C2[m-1] - a[m-1] */
        seq_t res = sum4 + C3[m] + C3[m] - sum2 - C2[m - 1] - A[m - 1];

        /* The delta term for n = 1: res += 1 */
        if (m == 1) res += 1;

        A[m] = res;
    }

    computed_max = n;
}

/* External interface required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* behavior below the offset is irrelevant */
    grow_to(n);
    return A[n];
}
