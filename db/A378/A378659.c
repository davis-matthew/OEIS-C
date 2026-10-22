/*
 * A378659  Positions of records for ratio A003961(n)/sigma(n), where A003961
 *          is fully multiplicative with a(p) = nextprime(p) and sigma is the
 *          sum of the divisors function.
 * https://oeis.org/A378659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the nth position where the ratio A003961(n)/σ(n)
 * attains a new record.  The sequence can be described as:
 *  a(1) = 1
 *  a(2) = 3
 *  a(3) = 4
 *  a(4) = 7
 *  a(5) = 8
 *  a(6) = 9
 *  a(7) = 16
 *  a(8) = 27
 *  a(9) = 32
 *  a(10)= 64
 *  a(11)=128
 *  a(12)=243
 *  For n ≥ 13, a(n) = 2 ^ (n – 5).
 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    static const seq_t init[12] = {1, 3, 4, 7, 8, 9, 16, 27, 32, 64, 128, 243};

    if (n <= 12)
        return init[n - 1];
    return pow2(n - 5);
}
