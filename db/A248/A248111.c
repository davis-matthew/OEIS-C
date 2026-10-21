/*
 * A248111  Least k such that ((k+2)/(k-2))^k - e^4 < 1/n.
 * https://oeis.org/A248111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the least k>2 such that ((k+2)/(k-2))^k - e^4 < 1/n
 * Range:     u8 n <= 223, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Offset is 1, so for n == 0 we do nothing special. */
    if (n == 0) return 0;

    /* Constant e^4. */
    const double e4 = 54.598150033144236;   /* exp(4) */

    double threshold = 1.0 / (double)n;

    /* Upper bound: values grow roughly like 437 *n, use a generous guard. */
    idx_t low  = 3;
    idx_t high = (idx_t)((double)n * 460.0) + 10;
    if (high < low) high = low;

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;

        double ratio = (double)(mid + 2) / (double)(mid - 2);
        double value = pow(ratio, (double)mid);
        double diff  = value - e4;

        if (diff < threshold)
            high = mid;      /* mid satisfies inequality, try smaller k */
        else
            low  = mid + 1;  /* mid too small, need larger k */
    }
    return (seq_t)low;
}
