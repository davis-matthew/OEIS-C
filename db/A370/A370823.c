/*
 * A370823  a(n) is the numerator of the ratio of winning probabilities
 *          P_A/P_B of winning in a 2-player game with a ratio of odds for A
 *          and B in a single round of 2:1. To win the game it is necessary to
 *          win n rounds in a row.
 * https://oeis.org/A370823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of 2^(n-1)*(3^n-1)/(3^n-2^n) after reduction
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   gcd, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Compute 3^n and 2^n */
    seq_t pow3_n = pow3(n);          /* 3^n */
    seq_t pow2_n = pow2(n);          /* 2^n */

    /* A = 3^n - 1 , B = 2^n - 1 */
    seq_t A = pow3_n - 1;
    seq_t B = pow2_n - 1;

    /* g = gcd(3^n-1, 2^n-1) = gcd(A,B) */
    seq_t g = gcd(A, B);

    /* A/g and 2^(n-1) */
    seq_t A_div = A / g;
    seq_t pow2_nm1 = pow2(n - 1);

    /* Result is 2^(n-1) * (3^n-1) / g */
    return pow2_nm1 * A_div;
}
