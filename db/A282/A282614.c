/*
 * A282614  Number of inequivalent 3 X 3 matrices with entries in {1,2,3,..,n}
 *          up to vertical and horizontal reflections.
 * https://oeis.org/A282614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5*(n+1)*(n^3-n^2+n+1)/4
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 13, u64 n <= 161, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle n = 0 explicitly */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t n_seq = (seq_t)n;

    /* Compute powers */
    seq_t n2 = n_seq * n_seq;       /* n^2 */
    seq_t n3 = n2 * n_seq;          /* n^3 */
    seq_t n5 = n3 * n2;             /* n^5, since n^5 = n^3 * n^2 */

    /* Compute the polynomial factor used in the formula */
    seq_t poly = n3 - n2 + n_seq + (seq_t)1;   /* n^3 - n^2 + n + 1 */

    /* Prepare the three multiplicative factors */
    seq_t a_fac = n5;
    seq_t b_fac = n_seq + (seq_t)1;
    seq_t c_fac = poly;

    /* Divide the total product by 4 by taking two factors of 2
       from the multiplicative factors. */
    int remaining_divs = 2;
    while (remaining_divs > 0) {
        if (remaining_divs > 0 && (a_fac & (seq_t)1) == 0) {
            a_fac >>= 1;
            remaining_divs--;
        }
        if (remaining_divs > 0 && (b_fac & (seq_t)1) == 0) {
            b_fac >>= 1;
            remaining_divs--;
        }
        if (remaining_divs > 0 && (c_fac & (seq_t)1) == 0) {
            c_fac >>= 1;
            remaining_divs--;
        }
        /* If we still need divisions but none of the three
           factors were even, divide the first factor again. */
        if (remaining_divs > 0) {
            if ((a_fac & (seq_t)1) == 0) {
                a_fac >>= 1;
                remaining_divs--;
            } else if ((b_fac & (seq_t)1) == 0) {
                b_fac >>= 1;
                remaining_divs--;
            } else if ((c_fac & (seq_t)1) == 0) {
                c_fac >>= 1;
                remaining_divs--;
            } else {
                /* This should never happen because the product is
                   always divisible by 4. */
                break;
            }
        }
    }

    /* Final multiplication; the product fits in seq_t for the
       values that fit in the target type. */
    seq_t result = a_fac * b_fac;
    result = result * c_fac;

    return result;
}
