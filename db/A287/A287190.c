/*
 * A287190  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 253", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0)=1, a(1)=10, a(2)=0;
 * For n>=4 even, a(n)=11;
 * For odd n>=3,   a(n)=10*(10^n-1)/9   (decimal representation of n ones followed by 0).
 */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)10;
    if (n == 2u)
        return (seq_t)0;
    if ((n & 1u) == 0u)   /* even n >= 4 */
        return (seq_t)11;

    /* odd n >= 3 */
    seq_t pow10 = pow_uint(10u, n);               /* 10^n  */
    seq_t val   = (pow10 - (seq_t)1) / (seq_t)9;  /* (10^n-1)/9  */
    return val * (seq_t)10;                       /* 10*(10^n-1)/9 */
}
