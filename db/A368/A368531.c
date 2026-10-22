/*
 * A368531  Numbers whose binary indices are all powers of 3, where a binary
 *          index of n (row n of A048793) is any position of a 1 in its
 *          reversed binary expansion.
 * https://oeis.org/A368531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥0} [ ((n-1) >> k & 1) * 2^(3^k - 1) ] (where the sum
 *            runs over all bits set in n-1; the power of 2 uses the exponent
 *            3^k - 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* offset 1: a(1)=0, but n=0 never used */
        return 0;

    idx_t m = n - 1;     /* binary number whose bits indicate the
                            chosen powers of 3 */
    seq_t result = 0;

    idx_t pow3 = 1;      /* 3^0 */
    while (m)
    {
        if (m & 1)
            result += pow2(pow3 - 1);  /* add 2^(3^k-1) */

        pow3 *= 3;       /* next power of 3 */
        m >>= 1;
    }
    return result;
}
