/*
 * A244841  4^p - 3^p - 1, where p is prime.
 * https://oeis.org/A244841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^p(n) - 3^p(n) - 1, where p(n) is the nth prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* out-of-range; not used in tests */
    seq_t p = (seq_t)get_prime(n);         /* nth prime (1‑based) */
    seq_t pow4 = big_pow((seq_t)4, p);     /* 4^p */
    seq_t pow3 = big_pow((seq_t)3, p);     /* 3^p */
    return pow4 - pow3 - (seq_t)1;         /* 4^p - 3^p - 1 */
}
