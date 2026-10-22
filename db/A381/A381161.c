/*
 * A381161  a(n) = (10*n)!/((n!)^3*(2*n)!*(5*n)!).
 * https://oeis.org/A381161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n)! / ((n!)^3 * (2*n)! * (5*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 7,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Alternative product of binomials: a(n) = binomial(10*n,5*n) * binomial(5*n,n) * binomial(4*n,n) * binomial(3*n,n) */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    idx_t ten_n   = 10 * n;
    idx_t five_n  = 5 * n;
    idx_t four_n  = 4 * n;
    idx_t three_n = 3 * n;

    seq_t res = binomial(ten_n, five_n);
    res = res * binomial(five_n, n);
    res = res * binomial(four_n, n);
    res = res * binomial(three_n, n);

    return res;
}
