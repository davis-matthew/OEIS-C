/*
 * A262718  a(n) = (n+1)^n - 2*(n^n) + (n-1)^n.
 * https://oeis.org/A262718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^n - 2*(n^n) + (n-1)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the trivial small indices explicitly */
    if (n <= 1u) {
        return (seq_t)0;
    }

    /* Convert idx_t to seq_t for arithmetic */
    seq_t np1 = (seq_t)(n + 1u);
    seq_t np  = (seq_t)n;
    seq_t nm1 = (seq_t)(n - 1u);

    /* Compute the three power terms */
    seq_t p1 = big_pow(np1, np);   /* (n+1)^n */
    seq_t p2 = big_pow(np,  np);   /*  n^n   */
    seq_t p3 = big_pow(nm1, np);   /* (n-1)^n */

    /* a(n) = p1 - 2*p2 + p3 */
    seq_t result = p1;
    result -= (seq_t)2 * p2;
    result += p3;

    return result;
}
