/*
 * A241573  2^p + 3 where p is prime.
 * https://oeis.org/A241573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p + 3 where p is the n-th prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; get_prime(n) yields the n‑th prime p */
    uint32_t p = get_prime(n);
    seq_t val = pow2((idx_t)p);   /* compute 2^p */
    val = val + (seq_t)3;          /* add 3 */
    return val;
}
