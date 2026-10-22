/*
 * A358992  a(n) is the number of digits in the product of the first n odd
 *          numbers not divisible by 5.
 * https://oeis.org/A358992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor( Σ_{i=1..n} log10(o_i) ) + 1, where o_i is the i‑th
 *            odd integer not divisible by 5
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* product = 1, 1 digit */

    long double sum = 0.0L;
    idx_t cur = 1;
    idx_t count = 0;

    while (count < n) {
        if (cur % 5 != 0) {
            sum += log10l((long double)cur);
            ++count;
        }
        cur += 2; /* next odd number */
    }

    /* small epsilon to guard against rounding errors */
    const long double eps = 1e-12L;
    unsigned long long digits = (unsigned long long)(floorl(sum + eps) + 1.0L);

    return (seq_t)digits;
}
