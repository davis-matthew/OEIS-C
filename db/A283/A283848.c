/*
 * A283848  Number of n-gonal inositol homologs with 2 kinds of achiral
 *          proligands.
 * https://oeis.org/A283848
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   If n is odd: a(n) = 2^n If n is even: a(n) = (1/(2*n)) * Σ_{d|n,
 *            d even} φ(d) * 4^(n/d) + 5 * 2^(n-2) Where φ is Euler's totient
 *            function
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   phi, pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"   /* seq_t phi(idx_t m)       */
#include "../utils/pow2.h"  /* seq_t pow2(idx_t exp)    */
#include "../utils/pow4.h"  /* seq_t pow4(idx_t exp)    */

seq_t a(idx_t n)
{
    /* odd n: a(n) = 2^n */
    if (n & 1U)
        return pow2(n);

    /* even n: compute the divisor sum over even divisors */
    seq_t sum = 0;
    for (idx_t d = 2; d <= n; d += 2) {
        if (n % d == 0) {
            seq_t phi_d  = phi(d);               /* φ(d) */
            idx_t  exp   = n / d;                /* n/d  */
            seq_t p4     = pow4(exp);            /* 4^(n/d) */
            sum += phi_d * p4;
        }
    }

    /* Divide by 2*n */
    seq_t denom = ((seq_t)2) * ((seq_t)n);
    seq_t term1 = sum / denom;

    /* Add the constant part 5 * 2^(n-2) */
    seq_t term2 = ((seq_t)5) * pow2(n - 2);

    return term1 + term2;
}
