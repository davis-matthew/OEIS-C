/*
 * A331775  a(n) = Sum_{-n<i<n, -n<j<n, gcd{i,j}=3} (n-|i|)*(n-|j|)/4.
 * https://oeis.org/A331775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{-n<i<n, -n<j<n, gcd(i,j)=3} (n-|i|)*(n-|j|)/4
 *            (including i or j equal to zero as long as gcd(i,j)=3) Sum is
 *            taken over signed integer indices i and j
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    seq_t nseq  = (seq_t)n;

    /* Highest multiple of 3 that is < n (for |i|,|j|). */
    idx_t max3 = (n - 1) / 3;

    for (idx_t a3 = 0; a3 <= max3; ++a3) {
        idx_t a         = a3 * 3;                 /* |i|  */
        seq_t mult_a = (a == 0) ? (seq_t)1 : (seq_t)2; /* sign multiplicity for i */

        for (idx_t b3 = 0; b3 <= max3; ++b3) {
            idx_t b         = b3 * 3;                 /* |j|  */
            if (a == 0 && b == 0) continue;         /* gcd(0,0) = 0 */

            /* gcd of |i| and |j| must be exactly 3. */
            if (gcd((seq_t)a, (seq_t)b) != (seq_t)3) continue;

            seq_t mult_b = (b == 0) ? (seq_t)1 : (seq_t)2; /* sign multiplicity for j */

            seq_t term = (nseq - (seq_t)a) * (nseq - (seq_t)b)
                        * mult_a * mult_b;
            total += term;
        }
    }

    return total / 4;   /* final division by 4 as specified */
}
