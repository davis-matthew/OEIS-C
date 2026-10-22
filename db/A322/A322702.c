/*
 * A322702  a(n) is the product of primes p such that p+1 divides n.
 * https://oeis.org/A322702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p prime, p+1 divides n} p
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 167, u64 n <= 479,
 *            u128 n <= 2519, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n < 1) return 1;
    seq_t res = 1;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;
            if (d > 1) {
                if (is_prime(d - 1)) {
                    seq_t p = (seq_t)(d - 1);
                    res *= p;
                }
            }
            if (d2 != d && d2 > 1) {
                if (is_prime(d2 - 1)) {
                    seq_t p = (seq_t)(d2 - 1);
                    res *= p;
                }
            }
        }
    }
    return res;
}
