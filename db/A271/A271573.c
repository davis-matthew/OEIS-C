/*
 * A271573  Numerator of (0 followed by A005126(n)= 2, 4, 7, ...)/2^n.
 * https://oeis.org/A271573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(n/2^n + (if n>=2 then 1/2 else 0)), i.e. a(n) =
 *            numerator((n + 2^{n-1}) / 2^n) (for n≥2); a(0)=0, a(1)=1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 101,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t val = (seq_t)n + pow2(n - 1);   /* val = n + 2^{n-1}  */

    /* cancel all common powers of 2 to obtain the numerator of the reduced fraction */
    while ((val & (seq_t)1) == 0)
        val >>= 1;

    return val;
}
