/*
 * A376956  a(n) = least k such that n^(2k)/(2 k)! < 1.
 * https://oeis.org/A376956
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   least k such that n^(2k)/(2k)! < 1 Computed by comparing
 *            logarithms to avoid large factorials. n = 0 returns 1 as special
 *            case
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0)
        return (seq_t)1;          /* n^0/0! = 1, need next k */

    /* Johnson's trick: n^(2k) < (2k)!  ⇔  2k*ln(n) < ln((2k)!)   */
    double ln_n = log((double)n);

    /* Initial bounds for binary search */
    idx_t lo = 0;
    idx_t hi = 1;
    while (1) {
        double lhs = 2.0 * (double)hi * ln_n;
        double rhs = lgamma(2.0 * (double)hi + 1.0);  /* ln((2k)!) */
        if (lhs < rhs)
            break;
        hi <<= 1;                                     /* double hi until condition satisfied */
    }

    /* Binary search for minimal k */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        double lhs = 2.0 * (double)mid * ln_n;
        double rhs = lgamma(2.0 * (double)mid + 1.0);
        if (lhs < rhs)
            hi = mid;
        else
            lo = mid;
    }
    return (seq_t)hi;
}
