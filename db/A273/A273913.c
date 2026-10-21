/*
 * A273913  Consider the sequence b(k) with initial values b(1) = 1 and b(2) =
 *          n and satisfying b(k) = b(k-1) + Pd(b(k-2)), where Pd(x) is the
 *          product of the digits of x. Then b(k) eventually becomes constant,
 *          and this constant is a(n).
 * https://oeis.org/A273913
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2371, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the eventual constant reached by the sequence
 * b defined by b(1)=1, b(2)=n, b(k)=b(k-1)+Pd(b(k-2)) (k≥3),
 * where Pd(x) is the product of the decimal digits of x.
 * The constant is the first value that repeats forever.
 */

#include <stdbool.h>
#include "../utils/digit_product_big.h"

/* a(n): the constant value reached by the sequence described above */
seq_t a(idx_t n)
{
    /* initial b(1)=1, b(2)=n */
    seq_t b_prev2 = (seq_t)1;      /* b(k-2) */
    seq_t b_prev1 = (seq_t)n;      /* b(k-1) */

    while (true) {
        seq_t pd2 = digit_product_big(b_prev2);     /* Pd(b(k-2)) */

        /* if both the last two terms have a zero digit product,
         * the next addition will be zero and the sequence
         * will stay constant thereafter. */
        if (pd2 == (seq_t)0 &&
            digit_product_big(b_prev1) == (seq_t)0)
            return b_prev1;            /* constant reached */

        /* compute next term b(k) = b(k-1) + Pd(b(k-2)) */
        seq_t b_next = b_prev1 + pd2;

        /* shift window forward */
        b_prev2 = b_prev1;
        b_prev1 = b_next;
    }
}
