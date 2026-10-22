/*
 * A307368  a(n) is the minimal positive integer such that 2*a(n)*prime(n)-1
 *          equals another prime.
 * https://oeis.org/A307368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive integer a such that 2*a*prime(n)-1 is
 *            prime
 * Range:     u8 n <= 13, u16 n <= 2450, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    uint32_t p = get_prime(n);   /* n-th prime */
    seq_t k = 1;
    for (;;) {
        seq_t q = (seq_t)2 * k * (seq_t)p - 1;
        if (isPrimeGeneric(q))
            return k;
        ++k;
    }
}
