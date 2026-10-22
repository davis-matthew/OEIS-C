/*
 * A366923  a(n) = A110936(n)/6 for n >= 3.
 * https://oeis.org/A366923
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A110936(n)/6 where * A110936(n) = product of all primes q
 *            < prime[n] such that (prime[n]-1) % (q-1) == 0
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 41, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    /* prime of index n */
    uint32_t p = get_prime(n);
    uint64_t p_minus1 = (uint64_t)p - 1;

    seq_t prod = 1;

    /* iterate over all earlier primes */
    for (idx_t i = 1; i < n; ++i) {
        uint32_t q = get_prime(i);
        uint64_t q_minus1 = (uint64_t)q - 1;
        if (q_minus1 != 0 && p_minus1 % q_minus1 == 0)
            prod *= (seq_t)q;
    }

    /* divide by 6 (product always divisible by 6) */
    prod /= 6;
    return prod;
}
