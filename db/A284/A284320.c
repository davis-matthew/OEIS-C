/*
 * A284320  Expansion of Product_{k>=0} (1 - x^(5*k+3)) in powers of x.
 * https://oeis.org/A284320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in Π_{k≥0} (1 – x^(5k+3)). Computed by
 *            a direct subset–sum DP. a(0)=1. (Not using the recurrence from
 *            the OEIS entry.) Only O(n²/10) operations are needed for n≤10000
 * Range:     i8 n <= 151, i16 n <= 410, i32 n <= 1250, i64 n <= 4165,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cache for computed coefficients */
static seq_t *coeff = NULL;   /* array of coefficients */
static size_t cap = 0;        /* allocated capacity */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Recompute the table if the requested index is beyond the
       current cached range.  The table is recomputed from scratch
       for the new maximum, which keeps the algorithm simple and
       correct regardless of call order. */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        /* Free old cache if it exists */
        if (coeff) {
            free(coeff);
            coeff = NULL;
        }
        /* Allocate and zero-initialize */
        coeff = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!coeff) return (seq_t)0;       /* allocation failure → return 0 */
        memset(coeff, 0, new_cap * sizeof(seq_t));

        /* Recurrence: a[0] = 1 */
        coeff[0] = (seq_t)1;

        /* Subset–sum DP over parts 5k+3 */
        for (idx_t part = 3; part <= n; part += 5) {
            for (idx_t j = n; j >= part; --j) {
                coeff[j] = coeff[j] - coeff[j - part];
            }
        }

        cap = new_cap;
    }

    return coeff[(size_t)n];
}
