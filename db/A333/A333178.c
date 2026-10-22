/*
 * A333178  a(n) = Sum_{d|n, gcd(d, n/d) = 1} prime(d).
 * https://oeis.org/A333178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1} prime(d)
 * Range:     u8 n <= 41, u16 n <= 3269, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* factor n into distinct prime powers pi^ei */
    idx_t tmp = n;
    idx_t powers[64];
    size_t k = 0;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t power = 1;
            while (tmp % p == 0) {
                tmp /= p;
                power *= p;
            }
            powers[k++] = power;
        }
    }
    if (tmp > 1) {
        powers[k++] = tmp;            /* remaining prime factor */
    }

    seq_t sum = 0;
    size_t subsets = ((size_t)1) << k;    /* 2^k subsets */
    for (size_t mask = 0; mask < subsets; ++mask) {
        idx_t d = 1;
        for (size_t i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i))
                d *= powers[i];
        }
        sum += (seq_t)get_prime(d);
    }
    return sum;
}
