/*
 * A350816  Number of minimum dominating sets in the 2 X n king graph.
 * https://oeis.org/A350816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(3*n) = 2^n a(3*n+1) = (n^2 + 5*n + 2) * 2^n a(3*n+2) = (n + 2)
 *            * 2^(n+1) (valid for n >= 0) Sequence offset: a(1) = 2
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 69, u64 n <= 159,
 *            u128 n <= 345, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is 1-based, so first term is a(1) */
    idx_t q = n % 3;          /* remainder of division by 3 */
    idx_t m = n / 3;          /* floor(n / 3) */

    if (q == 0) {             /* n ≡ 0 (mod 3)  ->  a(n) = 2^(n/3) */
        return pow2(m);
    } else if (q == 1) {      /* n ≡ 1 (mod 3)  ->  a(n) = (m^2 + 5*m + 2) * 2^m */
        seq_t coeff = (seq_t)m * m + (seq_t)5 * m + 2;
        return coeff * pow2(m);
    } else {                  /* q == 2,  n ≡ 2 (mod 3)
                                 a(n) = (m + 2) * 2^(m+1) */
        seq_t coeff = (seq_t)(m + 2);
        return coeff * pow2(m + 1);
    }
}
