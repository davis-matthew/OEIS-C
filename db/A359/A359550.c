/*
 * A359550  Characteristic function of A048103; From n >= 1, a(n) is
 *          multiplicative with a(p^e) = 1 if p > e, otherwise 0. a(0) = 0 by
 *          convention.
 * https://oeis.org/A359550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult
 * Formula:   Characteristic function of A048103; a(0)=0; for n>0, a(n)=1 iff
 *            for every prime p dividing n the exponent e of p satisfies p >
 *            e, otherwise 0
 * Range:     u8 n <= 9461, u16 n <= 9461, u32 n <= 9461, u64 n <= 9461,
 *            u128 n <= 9461, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(0) is defined to be 0 by convention. */
    if (n == 0) return (seq_t)0;
    /* n=1 has no prime factors, so the condition is vacuously true. */
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    bool ok = true;

    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p <= e) ok = false;
        }
    }

    /* If any prime factor remains after the loop, it has exponent 1. */
    if (m > 1) {
        /* m is prime.  Its exponent is 1. */
        if (m <= 1) ok = false;  /* never happens, kept for clarity */
    }

    return ok ? (seq_t)1 : (seq_t)0;
}
