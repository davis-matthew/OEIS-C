/*
 * A247159  Sum of divisors of even semiprimes.
 * https://oeis.org/A247159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(2*prime(n)); a(n>1)=3*prime(n)+3, a(1)=7
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)7;
    uint32_t p = get_prime(n);
    return (seq_t)3 * (seq_t)p + (seq_t)3;
}
