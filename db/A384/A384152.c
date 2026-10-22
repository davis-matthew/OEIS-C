/*
 * A384152  Consecutive states of the linear congruential pseudo-random number
 *          generator used by OMNITAB II when started at 1.
 * https://oeis.org/A384152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 125 * a(n-1) mod 2^13, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   _mult_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_mult_mod.h"

seq_t a(idx_t n)
{
    /* a(1) = 1; for n>=2, a(n) = 125^(n-1) mod 8192 */
    if (n <= 1)
        return (seq_t)1;

    idx_t exp = n - 1;
    seq_t base  = (seq_t)125 & ((seq_t)0x1FFF);
    seq_t result = (seq_t)1;

    while (exp > 0)
    {
        if (exp & 1)
            result = _mult_mod(result, base);
        base = _mult_mod(base, base);
        exp >>= 1;
    }
    return result;
}
