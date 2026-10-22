/*
 * A373836  a(n) = 1 if bigomega(n) and A276085(n) are both multiples of 3,
 *          otherwise 0, where bigomega (A001222) and A276085 are fully
 *          additive with a(p) = 1 and a(p) = p#/p respectively.
 * https://oeis.org/A373836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if bigomega(n) and A276085(n) are both multiples of 3,
 *            otherwise 0
 * Range:     u8 n <= 9560, u16 n <= 9560, u32 n <= 9560, u64 n <= 9560,
 *            u128 n <= 9560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* below offset, undefined */

    uint64_t m = (uint64_t)n;
    uint64_t e2 = 0, e3 = 0;

    /* exponent of 2 */
    while ((m & 1ULL) == 0) {
        e2++;
        m >>= 1;
    }

    /* exponent of 3 */
    while (m % 3ULL == 0) {
        e3++;
        m /= 3ULL;
    }

    /* remaining prime exponents */
    uint64_t rest_omega = 0;
    /* trial division for the rest of the factorization */
    for (uint64_t p = 5; p <= m / p; p += 2) {
        while (m % p == 0) {
            rest_omega++;
            m /= p;
        }
    }
    if (m > 1) {
        /* one prime factor remains with exponent 1 */
        rest_omega++;
    }

    /* modulo 3 checks */
    uint64_t big_mod   = (e2 + e3 + rest_omega) % 3;
    uint64_t a276_mod  = (e2 + 2 * e3) % 3;

    return (seq_t)((big_mod == 0) && (a276_mod == 0));
}
