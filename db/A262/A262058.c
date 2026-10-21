/*
 * A262058  Least integer k>1 such that sqrt(k)/log(k) exceeds n.
 * https://oeis.org/A262058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least integer k > 1 such that sqrt(k)/log(k) > n (log
 *            denotes the natural logarithm)
 * Range:     u8 n <= 2, u16 n <= 23, u32 n <= 2954, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h> /* sqrt, log */

seq_t a(idx_t n)
{
    /* Special case: n == 0 is not part of the OEIS offset, but
       handling it keeps the function robust. */
    if (n == 0)
        return 2;

    /* Find a suitable upper bound by exponential search.  The bound
       is guaranteed to satisfy sqrt(k)/log(k) > n. */
    idx_t low  = 2;
    idx_t high = 2;
    while (1) {
        double v = sqrt((double)high) / log((double)high);
        if (v > (double)n)
            break;
        /* Prevent overflow; stop at the maximum 64‑bit value. */
        if (high > UINT64_MAX / 2) {
            high = UINT64_MAX;
            break;
        }
        high *= 2;
    }

    /* Binary search between low and high.  The function f(k) = sqrt(k)/log(k)
       is monotone increasing for k >= 8, which suffices for the ranges
       considered here. */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        double v = sqrt((double)mid) / log((double)mid);
        if (v > (double)n)
            high = mid;
        else
            low = mid + 1;
    }

    return (seq_t)low;
}
