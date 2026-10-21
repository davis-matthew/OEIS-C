/*
 * A299795  Numbers of the form p*2^(p-1) where p is prime.
 * https://oeis.org/A299795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)*2^(prime(n)-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   get_prime, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    uint32_t p = get_prime(n);
    seq_t pow = pow2((idx_t)p - 1);
    return seq_mul((seq_t)p, pow);
}
