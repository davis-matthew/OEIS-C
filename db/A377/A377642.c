/*
 * A377642  a(n) = (1/(n-1)!) * Product_{i=1..n-1} (2^n-2^i).
 * https://oeis.org/A377642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{i=1}^{n-1} (2^n-2^i) / (n-1)!; a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#include "../utils/pow2.h"   /* seq_t pow2(idx_t exp);               */
#include "../utils/gcd.h"    /* seq_t gcd(seq_t a, seq_t b);         */

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1. */
    if (n <= 1)
        return (seq_t)1;                    /* empty product / 0! = 1 */

    seq_t product = (seq_t)1;              /* running numerator product   */
    seq_t denom   = (seq_t)1;              /* accumulated part of (n‑1)! */
    seq_t pow2_n  = pow2(n);               /* 2^n, used for all terms */

    for (idx_t i = 1; i < n; ++i) {
        seq_t i_seq  = (seq_t)i;            /* current i as seq_t */
        seq_t pow2_i = pow2(i);            /* 2^i */
        seq_t term   = pow2_n - pow2_i;    /* (2^n – 2^i) */

        /* cancel common factors between numerator term and i */
        seq_t g = gcd(term, i_seq);
        term    /= g;
        i_seq    /= g;

        /* further cancel between product and remaining part of i */
        g = gcd(product, i_seq);
        product /= g;
        i_seq   /= g;

        /* multiply reduced term into product and remaining i into denom */
        product *= term;
        denom   *= i_seq;

        /* reduce product and denom again to keep numbers small */
        g = gcd(product, denom);
        product /= g;
        denom   /= g;
    }

    /* Ensure any leftover denominator is cancelled (should be 1). */
    if (denom != (seq_t)1)
        product /= denom;

    return product;
}
