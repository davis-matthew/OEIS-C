/*
 * A380966  a(n) is an upper bound such that there exists an m X m magic
 *          square of n-th powers for all m >= a(n).
 * https://oeis.org/A380966
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*2^n + 20 for 2 <= n <= 4; a(n) =
 *            4*ceil(n*(log(n)+4.20032)) + 20 for n >= 5
 * Range:     u8 n <= 9, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;            /* below offset, value unspecified */
    }

    if (n <= 4) {
        /* 4*2^n + 20, n is small so shift is safe */
        seq_t pow2 = 1;
        pow2 <<= (unsigned)n;          /* pow2 = 2^n */
        return (seq_t)(4 * pow2 + 20);
    }

    /* n >= 5 */
    long double nd = (long double)n;
    long double tmp = nd * (logl(nd) + 4.20032L);
    unsigned long long ceil_val = (unsigned long long)ceill(tmp); /* ceil of tmp */
    return (seq_t)(4 * ceil_val + 20);
}
