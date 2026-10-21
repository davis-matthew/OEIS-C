/*
 * A242058  Even numbers n for which lpf_3(n-1) > lpf_3(n-3), where lpf_3(n) =
 *          lpf(n/3^t) (cf. A020639) such that 3^t (t>=0) is the maximal power
 *          of 3 which divides n.
 * https://oeis.org/A242058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th even integer m such that * lpf_3(m-1) >
 *            lpf_3(m-3), * where lpf_3(x) = lpf(x / 3^t) with t the largest
 *            exponent such that 3^t | x
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf3_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/lpf3_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard for safety */
        return 0;

    uint64_t m = 6;      /* first candidate even number */
    idx_t count = 0;

    while (1) {
        uint64_t v1 = m - 1;   /* m-1 is odd */
        uint64_t v2 = m - 3;   /* m-3 is odd */

        seq_t lpf1 = lpf3_uint64(v1);
        seq_t lpf2 = lpf3_uint64(v2);

        if (lpf1 > lpf2) {      /* condition satisfied */
            ++count;
            if (count == n)
                return (seq_t)m;
        }
        m += 2;                  /* next even number */
    }
}
