/*
 * A252375  a(n) = smallest r such that r^k <= spf(n) and gpf(n) < r^(k+1),
 *          for some k >= 0, where spf and gpf (smallest and greatest prime
 *          factor of n) are given by A020639(n) and A006530(n).
 * https://oeis.org/A252375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest r such that r^k <= spf(n) and gpf(n) < r^(k+1)
 *            for some k >= 0, where spf and gpf are the smallest and greatest
 *            prime factors of n, respectively
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)2;

    uint64_t nn = (uint64_t)n;
    seq_t spf = (seq_t)_smallest_prime_factor(nn);
    seq_t gpf_val = (seq_t)gpf(nn);

    for (seq_t r = 2; ; ++r) {
        seq_t power = 1;
        while (power * r <= spf) {
            power *= r;
        }
        if (gpf_val < power * r) {
            return r;
        }
    }
}
