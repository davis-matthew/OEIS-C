/*
 * A241867  Number of compositions of n such that the smallest part has
 *          multiplicity seven.
 * https://oeis.org/A241867
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 36, u64 n <= 73, u128 n <= 155,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A241867 – Number of compositions of n such that the smallest part has multiplicity seven.
    a(n) = sum_{s>=1} sum_{r>=0} C(r+7,7) * C(n-7s-r(s+1)+r-1, r-1),
    where the inner sum over r runs over all r such that n-7s - r(s+1) >= 0.
    For r = 0 this reduces to the single composition n = 7s.  */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 7) return 0;
    seq_t res = 0;
    for (idx_t s = 1; 7 * s <= n; ++s) {
        idx_t n1 = n - 7 * s;
        /* r = 0 case: only possible when n1 == 0 */
        if (n1 == 0) {
            res += 1;
            continue;
        }
        idx_t max_r = n1 / (s + 1);
        for (idx_t r = 1; r <= max_r; ++r) {
            idx_t M = n1 - r * (s + 1);
            seq_t term1 = binomial(r + 7, 7);                 /* C(r+7,7)   */
            seq_t term2 = binomial(M + r - 1, r - 1); /* C(M+r-1,r-1) */
            res += term1 * term2;
        }
    }
    return res;
}
