/*
 * A296197  Harary index of the n X n bishop graph.
 * https://oeis.org/A296197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 8*n - 36*n^2 + 16*n^3 + 6*n^4 - 3*(-1)^n)/48
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute the numerator first using a signed 64‑bit type to allow
       temporary negative values when n = 1.  Then divide by 48 and cast
       to the sequence type.  All intermediate results fit in 64 bits
       for n up to at least 10 000. */
    long long nn = (long long)n;
    long long num = 3 + 8*nn - 36*nn*nn + 16*nn*nn*nn + 6*nn*nn*nn*nn;
    if ((n & 1ULL) == 0)      /* n even: subtract 3*(-1)^n = 3 */
        num -= 3;
    else                      /* n odd: subtract 3*(-1)^n = -3 → add 3 */
        num += 3;
    return (seq_t)(num / 48);
}
