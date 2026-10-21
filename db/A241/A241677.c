/*
 * A241677  2^p + 5 where p is prime.
 * https://oeis.org/A241677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^p + 5 where p is the n-th prime
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
    /* Offsets start at 1; n < 1 is undefined in this sequence. */
    uint32_t p = get_prime(n);     /* nth prime (1‑based) */
    seq_t val = pow2(p);           /* 2^p */
    val += (seq_t)5;               /* +5 */
    return val;
}
