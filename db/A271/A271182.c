/*
 * A271182  a(n) = prime(n)^(2*n) - prime(n)^(n-1).
 * https://oeis.org/A271182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^(2*n) - prime(n)^(n-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* get the n-th prime */
    uint32_t p32 = get_prime(n);
    seq_t p = (seq_t)p32;

    /* exponents */
    idx_t exp1 = n * 2;            /* 2*n */
    idx_t exp2 = n - 1;            /* n-1 */

    /* compute powers */
    seq_t term1 = big_pow(p, (seq_t)exp1);
    seq_t term2 = big_pow(p, (seq_t)exp2);

    /* final value */
    return term1 - term2;
}
