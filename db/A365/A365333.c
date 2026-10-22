/*
 * A365333  The number of exponentially odd coreful divisors of the largest
 *          square dividing n.
 * https://oeis.org/A365333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} max(1, ⌊e/2⌋) (a(1)=1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return the A365333 value for index n (n ≥ 1). */
seq_t a(idx_t n)
{
    /* n is non‑negative; for n=0 a(1) is used by OEIS convention. */
    if (n <= 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;        /* local copy for factorisation */
    seq_t   result = 1;              /* multiplicative product          */

    /* trial division up to sqrt(m)       */
    uint64_t p = 2;
    while (p <= m / p) {
        if (m % p == 0) {
            uint64_t e = 0;
            do {
                m /= p;
                ++e;
            } while (m % p == 0);
            uint64_t val = e / 2;    /* floor(e/2)              */
            if (val == 0)            /* max(1, floor(e/2))      */
                val = 1;
            result *= (seq_t)val;
        }
        p = (p == 2) ? 3 : p + 2;    /* skip even numbers       */
    }

    /* If any prime factor remains > 1 it has exponent 1. */
    if (m > 1) {
        /* exponent e = 1 -> floor(1/2) = 0 -> max = 1 */
        result *= (seq_t)1;
    }

    return result;
}
