/*
 * A276963  a(n) = prime(n+1)^4 - prime(n)^4.
 * https://oeis.org/A276963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n+1)^4 - prime(n)^4
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 90, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n >= 1 (offset 1) */
    seq_t p1 = (seq_t)get_prime(n);      /* prime(n)   */
    seq_t p2 = (seq_t)get_prime(n + 1);  /* prime(n+1) */

    seq_t p1_4 = big_pow(p1, (seq_t)4);
    seq_t p2_4 = big_pow(p2, (seq_t)4);

    return p2_4 - p1_4;
}
