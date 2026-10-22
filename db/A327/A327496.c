/*
 * A327496  a(n) = a(n - 1) * 4^r where r = valuation(n, 2) if 4 divides n
 *          else r = (n mod 2) + 1; a(0) = 1. The denominators of A327495.
 * https://oeis.org/A327496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0: a(n)=a(n-1)*4^r, where r = valuation(n,2) if 4
 *            | n else r = (n mod 2)+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;
    if (n == 0)
        return res;

    for (idx_t i = 1; i <= n; ++i) {
        idx_t r;

        /* check whether 4 divides i */
        if ((i & 3ULL) == 0) {
            /* compute v2(i) */
            r = 0;
            idx_t tmp = i;
            while ((tmp & 1ULL) == 0) {
                tmp >>= 1;
                ++r;
            }
        } else {
            /* odd: r = 2; even but not multiple of 4: r = 1 */
            r = (i & 1ULL) ? 2 : 1;
        }

        res = res * pow4(r);
    }

    return res;
}
