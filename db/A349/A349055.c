/*
 * A349055  Number of multisets of size n that have an alternating permutation
 *          and cover an initial interval of positive integers.
 * https://oeis.org/A349055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1) - (n+2)*2^(n/2-3) for even n > 0, a(n) = 2^(n-1)
 *            - (n-1)*2^((n-5)/2) for odd n > 0; special cases: a(0)=1,
 *            a(1)=1, a(2)=1
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* precomputed values for n < 6 */
    static const seq_t pre[6] = { 1, 1, 1, 3, 5, 12 };

    if (n < 6ULL)
        return pre[n];

    /* compute 2^(n-1) first */
    seq_t two_power_n_minus1 = pow2(n - 1);

    if ((n & 1) == 0) { /* even n */
        idx_t m = n >> 1;              /* m = n/2 */
        seq_t inner = pow2(m - 3);     /* 2^(m-3)  (m >= 3 here) */
        seq_t sub = (seq_t)(n + 2) * inner;
        return two_power_n_minus1 - sub;
    } else { /* odd n */
        idx_t m = (n - 1) >> 1;        /* m = (n-1)/2 */
        seq_t inner = pow2(m - 2);     /* 2^(m-2)  (m >= 2 here for n >= 5) */
        seq_t sub = (seq_t)(n - 1) * inner;
        return two_power_n_minus1 - sub;
    }
}
