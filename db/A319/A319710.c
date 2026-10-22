/*
 * A319710  a(n) = 1 if n is divisible by the square of its smallest prime
 *          factor, 0 otherwise.
 * https://oeis.org/A319710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is divisible by the square of its smallest prime
 *            factor, 0 otherwise
 * Range:     u8 n <= 8652, u16 n <= 8652, u32 n <= 8652, u64 n <= 8652,
 *            u128 n <= 8652, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/lpf.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    idx_t spf = lpf(n);               /* smallest prime factor of n */
    if (spf == 0) return (seq_t)0;    /* safety check; should not occur */
    if (n % spf != 0) return (seq_t)0;
    n /= spf;
    if (n % spf != 0) return (seq_t)0;
    return (seq_t)1;
}
