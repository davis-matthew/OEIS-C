/*
 * A325094  Write n as a sum of distinct powers of 2, then take the primes of
 *          those powers of 2 and multiply them together.
 * https://oeis.org/A325094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∏_{k : 2^k is a summand of n} prime(2^k)
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 221, u64 n <= 1789,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t bit = 0;
    idx_t m = n;
    while (m != 0) {
        if (m & 1ULL) {
            idx_t power = ((idx_t)1) << bit;
            seq_t p = (seq_t)get_prime(power);
            result *= p;
        }
        m >>= 1;
        ++bit;
    }
    return result;
}
