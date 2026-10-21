/*
 * A243953  E.g.f.: exp( Sum_{n>=1} A000108(n-1)*x^n/n ), where A000108(n) =
 *          binomial(2*n,n)/(n+1) forms the Catalan numbers.
 * https://oeis.org/A243953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the differential equation
   A(x)^2 - A(x)A'(x) + x(A'(x))^2 = 0.
   Initial values: a(0) = 1, a(1) = 1.
   Recurrence for m ≥ 0:
       a_{m+1} = Σ_{i=0..m}    a_i a_{m-i}   C(m,i)
              + m Σ_{i=0..m-1} a_{i+1} a_{m-i} C(m-1,i)
              - Σ_{i=1..m}    a_i a_{m-i+1} C(m,i)
*/

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/free_seq_array.h"

/* Compute a(n) for the OEIS sequence A243953. */
seq_t a(idx_t n)
{
    /* Special case: n = 0. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Allocate array for a[0..n].  The helper takes care of construction. */
    seq_t *a = seq_alloc_new_array((size_t)(n + 1));
    a[0] = (seq_t)1;                     /* a(0) = 1 */

    /* Compute the sequence up to a[n] using the recurrence. */
    for (idx_t m = 0; m < n; ++m) {
        seq_t term1 = 0;   /* Σ_{i=0..m}   a_i a_{m-i} C(m,i)              */
        seq_t term3 = 0;   /* Σ_{i=0..m-1} a_{i+1} a_{m-i} C(m-1,i)      */
        seq_t sum2  = 0;   /* Σ_{i=1..m}   a_i a_{m-i+1} C(m,i)           */

        seq_t bin_m  = 1;   /* C(m,0)       */
        seq_t bin_m1 = 1;   /* C(m-1,0)     */

        for (idx_t i = 0; i <= m; ++i) {
            /* Term1 contribution */
            term1 += a[i] * a[m - i] * bin_m;

            /* Term2 contribution (for i>=1) */
            if (i >= 1) {
                sum2 += a[i] * a[m - i + 1] * bin_m;
            }

            /* Term3 contribution (for i <= m-1) */
            if (i <= m - 1) {
                term3 += a[i + 1] * a[m - i] * bin_m1;
            }

            /* Update binomial coefficients for next i */
            if (i < m) {
                bin_m = (bin_m * (seq_t)(m - i)) / (seq_t)(i + 1);
            }
            if (i < m - 1) {
                bin_m1 = (bin_m1 * (seq_t)(m - 1 - i)) / (seq_t)(i + 1);
            }
        }

        term3 = term3 * (seq_t)m;          /* multiply by m for the recurrence */
        a[m + 1] = term1 + term3 - sum2;   /* compute a_{m+1} */
    }

    seq_t result = a[n];                    /* answer for n */
    free_seq_array(a);                      /* clean up */
    return result;
}
