/*
 * A395769  The twisted Euler phi-function for the Dirichlet character
 *          kronecker(-8,.) mod 8.
 * https://oeis.org/A395769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Euler φ twisted by the Dirichlet character
 *            kronecker(-8,.) mod 8
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative function:  
   a(2^e) = 2^e  
   a(p^e) = p^(e-1)*(p-1)  if p ≡ 1,3 (mod 8)  
   a(p^e) = p^(e-1)*(p+1)  if p ≡ 5,7 (mod 8)  */
seq_t a(idx_t n)
{
    /* all computations are exact in seq_t when the result fits */
    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    if (m == 0) return 0;          /* not used for n>=1, but safe */
    if (m == 1) return 1;

    /* handle powers of 2 */
    int e = 0;
    while ((m & 1ULL) == 0ULL) {
        m >>= 1;
        ++e;
    }
    if (e > 0) {
        /* multiply by 2^e */
        seq_t pow2 = 1;
        for (int i = 0; i < e; ++i) pow2 *= 2;
        result *= pow2;
    }

    /* factor remaining odd part */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            /* compute p^(exp-1) */
            seq_t p_pow = 1;
            for (int i = 0; i < exp - 1; ++i) p_pow *= p;

            uint64_t mod8 = p & 7ULL;
            seq_t factor = (mod8 == 1 || mod8 == 3) ? (p_pow * (p - 1))
                                                    : (p_pow * (p + 1));
            result *= factor;
        }
    }

    if (m > 1) {   /* m itself is a prime factor with exponent 1 */
        uint64_t p = m;
        uint64_t mod8 = p & 7ULL;
        seq_t factor = (mod8 == 1 || mod8 == 3) ? (p - 1) : (p + 1);
        result *= factor;
    }

    return result;
}
