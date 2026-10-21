/*
 * A280346  Numbers with 79 divisors.
 * https://oeis.org/A280346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^78, where prime(n) is the nth prime number
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 2, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1; n==0 is undefined, return 0 for safety */
        return (seq_t)0;
    uint32_t p = get_prime(n);          /* n-th prime, 32-bit unsigned */
    seq_t base = (seq_t)p;              /* promote to seq_t for arbitrary precision */
    seq_t result = big_pow(base, (seq_t)78); /* raise to the 78th power */
    return result;
}
