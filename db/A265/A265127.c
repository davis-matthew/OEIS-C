/*
 * A265127  a(n) = prime(n) * 2^n.
 * https://oeis.org/A265127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) * 2^n
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 25, u64 n <= 55, u128 n <= 118,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    uint32_t p = get_prime(n);      /* nth prime */
    seq_t pow = pow2(n);            /* 2^n */
    return (seq_t)p * pow;          /* product in seq_t type */
}
