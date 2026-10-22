/*
 * A368884  The largest unitary divisor of n that is the square of a
 *          squarefree number (A062503).
 * https://oeis.org/A368884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 288, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A368884
 *
 *  Definition:  a(n) = the largest unitary divisor of n that is the
 *  square of a squarefree number.
 *  Multiplicative:  a(p^e) = p^2  if e = 2, and 1 otherwise.
 *
 *  Implementation:  trial division of n.
 *  The algorithm is fast enough for n up to about 10 000
 *  and uses only standard types.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    uint64_t m = (uint64_t)n;   /* work on a 64‑bit copy        */
    seq_t result = (seq_t)1;    /* accumulate the product       */

    /* Trial division by integer factors */
    for (uint64_t p = 2; p <= m / p; ++p) {
        uint64_t exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        if (exp == 2) {            /* contribute p^2 to the divisor */
            seq_t pp = (seq_t)p;
            pp = pp * pp;
            result = result * pp;
        }
    }

    /* If a prime factor > sqrt(original n) remains, its exponent is 1 */
    /* It does not contribute, so nothing to do */

    return result;
}
