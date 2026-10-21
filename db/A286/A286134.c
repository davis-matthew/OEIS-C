/*
 * A286134  Expansion of q^(-1/2) * eta(q^5) * eta(q^6) * eta(q^7) *
 *          eta(q^210) in powers of q.
 * https://oeis.org/A286134
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Expansion of q^(-1/2) * eta(q^5) * eta(q^6) * eta(q^7) * eta(q^210)
   in powers of q:
   GF: x^9 * Π_{k>0} (1 - x^(5k)) (1 - x^(6k)) (1 - x^(7k)) (1 - x^(210k))
   The function a(n) returns the coefficient of x^n in this series
   (offset 0, so a(0)=0). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* If n < 9 the factor x^9 shifts the series, so coefficient is zero. */
    if (n < 9) return 0;

    static seq_t *prod = NULL;          /* coefficients of the product
                                           without the leading x^9.   */
    static idx_t prod_size = 0;          /* size of prod (indices 0..prod_size-1) */

    idx_t target = n - 9;                /* we need coefficient prod[target] */

    /* Recompute the product up to 'target' if necessary. */
    if (target + 1 > prod_size) {
        /* Allocate new array, initialize to 0. */
        seq_t *new_arr = (seq_t *)malloc((target + 1) * sizeof(seq_t));
        for (idx_t i = 0; i <= target; ++i) new_arr[i] = 0;
        new_arr[0] = 1;                  /* product of empty set is 1 */

        /* Apply factors (1 - x^d) for every d that is a positive multiple
           of 5.  The in-place update must go backwards so we use old values. */
        for (idx_t d = 5; d <= target; d += 5)
            for (idx_t i = target; i >= d; --i)
                new_arr[i] -= new_arr[i - d];

        /* Factors for multiples of 6. */
        for (idx_t d = 6; d <= target; d += 6)
            for (idx_t i = target; i >= d; --i)
                new_arr[i] -= new_arr[i - d];

        /* Factors for multiples of 7. */
        for (idx_t d = 7; d <= target; d += 7)
            for (idx_t i = target; i >= d; --i)
                new_arr[i] -= new_arr[i - d];

        /* Factors for multiples of 210. */
        for (idx_t d = 210; d <= target; d += 210)
            for (idx_t i = target; i >= d; --i)
                new_arr[i] -= new_arr[i - d];

        /* Replace old array (if any). */
        free(prod);
        prod      = new_arr;
        prod_size = target + 1;
    }

    return prod[target];
}
