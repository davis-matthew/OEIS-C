/*
 * A368713  The maximal exponent in the prime factorization of the
 *          nonsquarefree numbers.
 * https://oeis.org/A368713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the maximal exponent in the prime factorization of the
 *            nth nonsquarefree number (OEIS A368713)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0u)   /* undefined for n = 0, give 0 as placeholder */
        return (seq_t)0;

    idx_t found = 0u;          /* how many nonsquarefree numbers found so far */
    idx_t num   = 2u;          /* current integer to test */

    for (;;) {
        uint64_t t   = (uint64_t)num;   /* number to factor (fits in 64‑bit) */
        seq_t    maxexp = 1u;           /* maximal exponent in this number */

        uint64_t p = 2u;
        /* trial division up to sqrt(t) */
        while (p <= t / p) {
            if (t % p == 0u) {
                unsigned e = 0u;
                while (t % p == 0u) {
                    t /= p;
                    ++e;
                }
                if (e > maxexp) maxexp = e;
            }
            ++p;
        }
        /* if any prime factor remains, its exponent is 1 */
        if (t > 1u && maxexp < 1u)
            maxexp = 1u;

        /* if the number is nonsquarefree (maxexp > 1) count it */
        if (maxexp > 1u) {
            ++found;
            if (found == n)
                return maxexp;
        }
        ++num;
    }
}
