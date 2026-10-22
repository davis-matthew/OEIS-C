/*
 * A381194  Number of equal-length matchings of 2n uniformly spaced points on
 *          a circle.
 * https://oeis.org/A381194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{i=1..n-1} [2*n/GCD(2*n,i) mod 2 == 0] *
 *            2^GCD(2*n,i)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* m = 2*n */
    idx_t m = n + n;
    seq_t m_seq = (seq_t)m;
    seq_t sum = 1;                     /* initial term */

    for (idx_t i = 1; i < n; ++i) {
        seq_t g = gcd(m_seq, (seq_t)i);            /* gcd(2*n, i) */
        seq_t m_div_g = m_seq / g;
        if ((m_div_g & 1) == 0) {                  /* (2*n/g) is even */
            seq_t term = pow2((idx_t)g);           /* 2^g */
            sum += term;
        }
    }
    return sum;
}
