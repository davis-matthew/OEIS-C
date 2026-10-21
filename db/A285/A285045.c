/*
 * A285045  Expansion of cosh(7*arctanh(2*sqrt(x))).
 * https://oeis.org/A285045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/15)*(512*n^3 + 64*n^2 + 144*n + 15)*binomial(2*n,n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (This sequence admits the recurrence:                                */
/*  n*(2*n-1)*a(n) + 2*(-8*n^2+16*n-57)*a(n-1) + 16*(2*n-3)*(n-2)*a(n-2) = 0   */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)98;

    /* recurrence:  a(n) = [ 2*(8n^2-16n+57)*a(n-1) - 16*(2n-3)(n-2)*a(n-2) ] / (n*(2n-1))  */
    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)98;  /* a(1) */
    seq_t an = a1;

    /* branch on width of seq_t: use 128‑bit int for small seq_t, real big‑int ops for large seq_t */
    if (sizeof(seq_t) <= 8) {
        /* 64‑bit (or smaller) implementation using __int128 to avoid overflow */
        for (idx_t i = 2; i <= n; ++i) {
            unsigned __int128 ii = i;
            unsigned __int128 coeff1 = 2 * (8*ii*ii - 16*ii + 57); /* 2*(8i^2-16i+57)  */
            unsigned __int128 coeff2 = 16 * (2*ii - 3) * (ii - 2);   /* 16*(2i-3)(i-2)   */

            unsigned __int128 prod1 = coeff1 * (unsigned __int128)a1;
            unsigned __int128 prod2 = coeff2 * (unsigned __int128)a0;

            unsigned __int128 numerator   = prod1 - prod2;                     /* numerator */
            unsigned __int128 denominator = ii * (2*ii - 1);                    /* denominator */

            an = (seq_t)(numerator / denominator); /* fits into seq_t by construction */
            a0 = a1;
            a1 = an;
        }
    } else {
        /* arbitrary‑precision implementation */
        for (idx_t i = 2; i <= n; ++i) {
            seq_t coeff1 = (seq_t)(2 * (8*i*i - 16*i + 57));                /* 2*(8i^2-16i+57) */
            seq_t coeff2 = (seq_t)(16 * (2*i - 3) * (i - 2));               /* 16*(2i-3)(i-2) */

            seq_t prod1 = a1 * coeff1;
            seq_t prod2 = a0 * coeff2;

            seq_t numerator   = prod1 - prod2;   /* prod1 >= prod2 for i >= 2 */
            seq_t denominator = (seq_t)(i * (2*i - 1));

            an = numerator / denominator;
            a0 = a1;
            a1 = an;
        }
    }
    return an;
}
