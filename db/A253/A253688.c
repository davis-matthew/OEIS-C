/*
 * A253688  The total number of pentagons in a variant of pentagon expansion
 *          (vertex-to-vertex, two consecutive vertices and one isolated
 *          vertex) after n iterations.
 * https://oeis.org/A253688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A253688 a(n) = 1, 4, 10, 21, 39, 64, 94, 129, … For n > 5: a(n)
 *            = (53 – (–1)^n – 38*n + 12*n^2) / 4 The implementation uses a
 *            direct quadratic formula, which is valid for all n ≥ 6. For n ≤
 *            5 the initial values are returned
 * Range:     u8 n <= 5, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 5 (offset 1) */
    static const seq_t init[5] = {1, 4, 10, 21, 39};

    if (n <= 5)
        return init[n - 1];

    seq_t nn = (seq_t)n;          /* convert index to seq_t for arithmetic */
    seq_t numerator;

    if ((n & 1) == 0) {           /* n is even */
        /* 12*n^2 – 38*n + 52  (since 53 – 1 = 52) */
        numerator = (12 * nn * nn) - (38 * nn) + 52;
    } else {                      /* n is odd */
        /* 12*n^2 – 38*n + 54  (since 53 + 1 = 54) */
        numerator = (12 * nn * nn) - (38 * nn) + 54;
    }

    return numerator / 4;         /* division truncates toward zero (always exact) */
}
