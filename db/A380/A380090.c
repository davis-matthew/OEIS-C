/*
 * A380090  The sum of the unitary divisors of n that are terms in A207481.
 * https://oeis.org/A380090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative function with a(1)=1, for prime power p^e:
 *            a(p^e) = p^e + 1 if e <= p, else 1. This equals the sum of the
 *            unitary divisors of n that are terms in A207481
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;                  /* a(1) = 1 */
    idx_t tmp = n;
    seq_t result = 1;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e <= p) {
                seq_t term = pow_uint(p, e);
                term += (seq_t)1;
                result *= term;
            }
        }
    }
    if (tmp > 1) {                          /* remaining prime factor */
        idx_t p = tmp;
        idx_t e = 1;                        /* exponent is 1 */
        if (e <= p) {
            seq_t term = pow_uint(p, e);
            term += (seq_t)1;
            result *= term;
        }
    }
    return result;
}
