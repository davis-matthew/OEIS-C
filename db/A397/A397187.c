/*
 * A397187  a(n) = ((n + 1)^prime(n) - n^prime(n) - 1)/prime(n).
 * https://oeis.org/A397187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n + 1)^prime(n) - n^prime(n) - 1) / prime(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
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
    if (n == 0) return 0;                     /* Offset starts at 1 */
    uint32_t p = get_prime(n);                 /* nth prime, k = n */
    seq_t base1 = (seq_t)(n + 1);              /* (n + 1) */
    seq_t base2 = (seq_t)n;                   /* n       */
    seq_t exp   = (seq_t)p;                   /* prime(n) */

    seq_t pow1 = big_pow(base1, exp);          /* (n + 1)^prime(n) */
    seq_t pow2 = big_pow(base2, exp);          /* n^prime(n)       */

    seq_t numer = pow1;
    numer -= pow2;
    numer -= 1;                                /* numerator = (n+1)^p - n^p - 1 */

    return numer / (seq_t)p;                   /* exact division by p */
}
