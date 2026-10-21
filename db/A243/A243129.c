/*
 * A243129  a(n) = sigma(d(d(d(n)))), where d(n) is the number of divisors of
 *          n.
 * https://oeis.org/A243129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma( d( d( d(n) ) ) ) where d(n) is the number of
 *            divisors of n and sigma(n) is the sum of all positive divisors
 *            of n
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

static seq_t sigma_of(uint64_t x)
{
    seq_t sum = 1;
    uint64_t m = x;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            seq_t term = 1;
            seq_t power = 1;
            while (m % p == 0) {
                m /= p;
                power *= p;
                term += power;
            }
            sum *= term;
        }
    }
    if (m > 1) {
        sum *= (1 + (seq_t)m);
    }
    return sum;
}

seq_t a(idx_t n)
{
    seq_t t1 = num_divisors((uint64_t)n);
    seq_t t2 = num_divisors((uint64_t)t1);
    seq_t t3 = num_divisors((uint64_t)t2);
    return sigma_of((uint64_t)t3);
}
