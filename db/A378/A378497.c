/*
 * A378497  a(n) is the number whose base-4 digits are 1 followed by the first
 *          n-1 terms of the periodic sequence with initial period 2,0,3.
 * https://oeis.org/A378497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor((7/18)*4^n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; for 0 return 0 as a sane default. */
    if (n == 0)
        return (seq_t)0;

    /*
     * For indices small enough that even 4^n·7 fits inside
     * a native unsigned __int128, perform the calculation using that type.
     * This guarantees exactness for all seq_t sizes up to 8, 16, 32 and 64
     * bits for the values that fit.
     */
    if (n <= 32) {
        unsigned __int128 pow4 = 1;
        for (idx_t i = 0; i < n; ++i)
            pow4 *= (unsigned __int128)4;
        unsigned __int128 num = pow4 * (unsigned __int128)7;
        unsigned __int128 res = num / (unsigned __int128)18;
        return (seq_t)res;
    }

    /* For larger indices we rely on the arbitrary‑precision routine provided
       by the build system.  If seq_t is a small integer type the value will
       naturally overflow, but such values do not fit in the target type and
       are not required to be correct. */
    seq_t pow4 = pow_uint((uint64_t)4, n);
    seq_t num  = pow4 * (seq_t)7;
    seq_t res  = num / (seq_t)18;
    return res;
}
