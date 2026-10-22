/*
 * A309398  a(n) is the nearest integer to log(log(10^n)).
 * https://oeis.org/A309398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to log(log(10^n))
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a double intermediate to compute
   the logarithm. The result is first converted to an
   unsigned long long before being cast to seq_t. This
   avoids a direct double‑to‑seq_t conversion, which
   fails when seq_t is a big integer type.             */
#include <math.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the OEIS offset. */
    if (n < 1)
        return 0;

    /* Constant ln(10) */
    const double ln10 = 2.302585092994046;

    /* Compute log(log(10^n)) = log(n * ln10) */
    double val = log((double)n * ln10);

    /* Round to nearest integer */
    unsigned long long rounded = (unsigned long long)(val + 0.5);

    return (seq_t)rounded;
}
