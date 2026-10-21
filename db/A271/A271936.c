/*
 * A271936  Commutative powers: numbers of the form a^b = b^a.
 * https://oeis.org/A271936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n-1) for 1 <= n <= 3 a(n) = (n-1)^(n-1) for n >= 4
 *            (same as A000302(n-1) for n<=3 and A000312(n-1) for n>=4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 3)
    {
        return big_pow((seq_t)4, (seq_t)(n - 1));
    }
    else
    {
        return big_pow((seq_t)(n - 1), (seq_t)(n - 1));
    }
}
