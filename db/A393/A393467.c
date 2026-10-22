/*
 * A393467  Number of maximum irredundant sets in the n-necklace graph.
 * https://oeis.org/A393467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*n) = 4*2^n + 2*(-2)^n a(2*n+1) = (100*n^2 + 132*n +
 *            41)*2^(n-3) + 9*(2*n+1)*(-2)^(n-3) Simplifications: a(2*m) =
 *            6*2^m if m even a(2*m) = 2^(m+1) if m odd a(2*m+1) = S * 2^(m-3)
 *            if m >= 3 a(2*m+1) = S / 2^(3-m) if m < 3 where S =
 *            (100*m^2+132*m+41) + 9*(2*m+1)*(-1)^(m-3)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 40, u64 n <= 100, u128 n <= 222,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* even index: n = 2*m */
    if ((n & 1ULL) == 0ULL) {
        idx_t m = n >> 1;
        if ((m & 1ULL) == 0ULL) {
            /* m even: a = 6 * 2^m */
            return ((seq_t)6) * pow2(m);
        } else {
            /* m odd : a = 2^(m+1) */
            return pow2(m + 1);
        }
    }

    /* odd index: n = 2*m+1 */
    idx_t m = (n - 1) >> 1;             /* m = (n-1)/2 */

    /* Compute S = (100*m^2 + 132*m + 41) + 9*(2*m+1)*(-1)^(m-3) */
    seq_t term1 = ((seq_t)100) * ((seq_t)m * (seq_t)m);
    term1 += ((seq_t)132) * (seq_t)m;
    term1 += (seq_t)41;

    seq_t add = ((seq_t)9) * (((seq_t)2) * (seq_t)m + (seq_t)1);

    seq_t S;
    if (((m - 3) & 1ULL) == 0ULL) {     /* even exponent => positive sign */
        S = term1 + add;
    } else {                            /* odd exponent => negative sign */
        S = term1 - add;
    }

    if (m < 3ULL) {
        /* division by 2^(3-m)  (denom = 8,4,2) */
        seq_t denom = ((seq_t)1) << (3ULL - m);
        return S / denom;
    } else {
        /* multiplication by 2^(m-3) */
        seq_t factor = pow2(m - 3);
        return S * factor;
    }
}
