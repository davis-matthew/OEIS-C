/*
 * A276656  Nearest integer to the inverse function of the factorial where the
 *          exponential of n is the argument.
 * https://oeis.org/A276656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to the real solution of Γ(x+1) = exp(n)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /*  n >= 1  */
    if (n == 0) {
        return (seq_t)0;   /* offset safety, not used by OEIS. */
    }

    double dn = (double)n;

    /* Find a bound high such that lgamma(high+1) >= n. */
    idx_t low = 1;
    idx_t high = 2;
    /* Guard against overflow; high will never exceed n for n >= 1. */
    while (lgamma((double)(high + 1)) < dn) {
        if (high > (UINT64_MAX >> 1)) {
            high = UINT64_MAX;
            break;
        }
        high <<= 1;
    }

    /* Binary search for the largest m with lgamma(m+1) <= n. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        double val = lgamma((double)(mid + 1));
        if (val <= dn) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }
    idx_t m_floor = low - 1;

    /* Compute the two nearest integers. */
    double diff1 = dn - lgamma((double)(m_floor + 1));
    double diff2 = lgamma((double)(m_floor + 2)) - dn;

    if (diff1 <= diff2) {
        return (seq_t)m_floor;
    } else {
        return (seq_t)(m_floor + 1);
    }
}
