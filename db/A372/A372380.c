/*
 * A372380  The number of divisors of n that are numbers whose number of
 *          divisors is a power of 2.
 * https://oeis.org/A372380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e of floor(log2(e+1))+1.
 *            (This implements the known multiplicative definition of
 *            A372380.)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) = 1 */
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t tmp = n;

    /* Factor 2 separately */
    idx_t e = 0;
    while ((tmp & 1) == 0) {
        tmp >>= 1;
        e++;
    }
    if (e > 0)
        result *= (seq_t)(floor_log2_uint64(e + 1) + 1);

    /* Factor odd primes */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            result *= (seq_t)(floor_log2_uint64(exp + 1) + 1);
        }
    }

    /* If a prime factor > sqrt(n) remains */
    if (tmp > 1)
        result *= (seq_t)(floor_log2_uint64(1 + 1) + 1);  /* exp = 1 */

    return result;
}
