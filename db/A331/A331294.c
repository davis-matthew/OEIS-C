/*
 * A331294  Positions of records in A331293.
 * https://oeis.org/A331294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n-4)^2 for n>=9; a(1)=1, a(2)=8, a(3)=16, a(4)=25,
 *            a(5)=27, a(6)=49, a(7)=75, a(8)=81
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n <= 8) {
        static const seq_t first8[8] = {1, 8, 16, 25, 27, 49, 75, 81};
        return first8[n-1];
    }
    uint32_t p32 = get_prime(n-4);   /* n-4 >= 5 for n >= 9 */
    seq_t p = (seq_t)p32;
    return p * p;
}
