/*
 * A241098  a(n) = (4^n - 1)^n.
 * https://oeis.org/A241098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (4^n - 1)^n
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;               /* value for n == 0  */

    if (n == 0)
        return result;

    /* ----- compute 4^n -----
     * use binary exponentiation so only integer operations are required.  */
    seq_t pow4 = 1;
    seq_t base4 = (seq_t)4;
    idx_t exp = n;
    while (exp) {
        if (exp & 1ULL)
            pow4 *= base4;
        exp >>= 1ULL;
        if (exp)
            base4 *= base4;
    }

    seq_t val = pow4 - (seq_t)1;     /* (4^n - 1) */

    /* ----- compute val^n  */
    result = 1;
    seq_t m = val;
    exp = n;
    while (exp) {
        if (exp & 1ULL)
            result *= m;
        exp >>= 1ULL;
        if (exp)
            m *= m;
    }

    return result;
}
