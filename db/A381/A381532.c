/*
 * A381532  Smallest integer k>0 such that prime(n) + k*prime(n+1) is prime.
 * https://oeis.org/A381532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that prime(n)+k*prime(n+1) is prime
 * Range:     u8 n <= 9, u16 n <= 370, u32 n <= 1674, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);
    seq_t q = (seq_t)get_prime(n + 1);

    for (idx_t k = 1; ; ++k) {
        seq_t candidate = p + (seq_t)k * q;
        if (is_prime_big(candidate))
            return (seq_t)k;
    }
}
