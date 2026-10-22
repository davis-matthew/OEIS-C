/*
 * A386785  a(n) = n^4*sigma_5(n).
 * https://oeis.org/A386785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^4 * sigma_5(n) where sigma_5(n) = sum of 5th powers of
 *            divisors of n. The computation uses a sqrt(·) loop and strictly
 *            integer arithmetic, so it works for any seq_t width that can
 *            hold the result. a(0) is defined to be 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 137,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute sigma_5(n) by iterating over divisors up to sqrt(n). */
    seq_t sigma = 0;
    /* integer square root of n */
    idx_t root = 0;
    {
        idx_t low = 0, high = n;
        while (low <= high) {
            idx_t mid = low + ((high - low) >> 1);
            if (mid * (idx_t)mid <= n) {
                root = mid;
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
    }

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            /* compute d^5 */
            seq_t dd = (seq_t)d;
            seq_t d2 = dd * dd;
            seq_t d3 = d2 * dd;
            seq_t d5 = d3 * d2;   /* d^5 = d^3 * d^2 */
            sigma += d5;

            idx_t co = n / d;
            if (co != d) {
                seq_t cc = (seq_t)co;
                seq_t c2 = cc * cc;
                seq_t c3 = c2 * cc;
                seq_t c5 = c3 * c2;
                sigma += c5;
            }
        }
    }

    /* Compute n^4 */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;

    return n4 * sigma;
}
