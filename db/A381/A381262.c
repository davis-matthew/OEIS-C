/*
 * A381262  Expansion of e.g.f. exp( -LambertW(-2 * sinh(x)) / 2 ).
 * https://oeis.org/A381262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A381262
   Formula:  E.g.f. A(x) satisfies A(x) = exp( sinh(x) * A(x)^2 ).
   Computation is performed by expanding the functional equation as
   a power‐series and using the following recurrence (for m ≥ 1):

   Let  A(x) = Σ a[m] x^m/m! ,  A(0)=1.
   Let  Y2[m]  = Σ_{i=0}^m Binom(m,i) a[i] a[m-i]      (coefficients of A^2)
   Let  Z[m]   = Σ_{odd i}   Binom(m,i) Y2[m-i]         (coefficients of sinh(x)*A^2)

   Then
        a[m] = Σ_{i=0}^{m-1} Binom(m-1,i) a[i] * Z[m-i]
   with a[0] = 1, Y2[0] = 1, Z[0] = 0.
   This recurrence reproduces the sequence   1,1,5,50,749,15132,…
   and yields all further terms exactly.

   Complexity: O(n^2) big‑integer operations, memory O(n).
   Parameters: idx_t is the index type; seq_t is the integer type used by the build.   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* allocate auxiliary arrays */
    size_t sz = (size_t)(n + 1);
    seq_t *a_arr = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *y2    = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *z     = (seq_t *)malloc(sz * sizeof(seq_t));

    /* initial values */
    a_arr[0] = 1;
    y2[0]   = 1;
    z[0]    = 0;

    for (idx_t m = 1; m <= n; ++m)
    {
        /* ---------- compute Z[m] ---------- */
        seq_t sumz = 0;
        seq_t binom = 1;                 /* binom(m,0) */
        for (idx_t i = 1; i <= m; ++i)
        {
            binom = (binom * (seq_t)(m - i + 1)) / (seq_t)i;   /* binom(m,i) */
            if (i & 1)                       /* i odd */
                sumz += y2[m - i] * binom;
        }
        z[m] = sumz;

        /* ---------- compute a[m] ---------- */
        seq_t suma = 0;
        seq_t binom_a = 1;                     /* binom(m-1,0) */
        for (idx_t i = 0; i <= m - 1; ++i)
        {
            seq_t term = a_arr[i];
            term *= binom_a;
            term *= z[m - i];
            suma += term;

            if (i < m - 1)
                binom_a = (binom_a * (seq_t)((m - 1) - i)) / (seq_t)(i + 1);
        }
        a_arr[m] = suma;

        /* ---------- compute Y2[m] ---------- */
        seq_t sumy = 0;
        seq_t binom_y = 1;                     /* binom(m,0) */
        for (idx_t i = 0; i <= m; ++i)
        {
            seq_t term = a_arr[i];
            term *= a_arr[m - i];
            term *= binom_y;
            sumy += term;

            if (i < m)
                binom_y = (binom_y * (seq_t)(m - i)) / (seq_t)(i + 1);
        }
        y2[m] = sumy;
    }

    seq_t result = a_arr[n];
    free(a_arr);
    free(y2);
    free(z);
    return result;
}
