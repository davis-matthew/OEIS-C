/*
 * A368997  a(n) = 1 if A342001(n) is even, otherwise 0.
 * https://oeis.org/A368997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - A342001(n) mod 2; A342001(n) = Sum_{p^v || n}
 *            v*rad(n)/p. Parity: if n is even, A342001(n) == v_2(n) (mod 2);
 *            if n is odd, A342001(n) == bigomega(n) (mod 2)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9618 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    idx_t t = n;
    unsigned par = 0;

    if ((t & 1) == 0) {
        /* even n: only the exponent of 2 matters */
        unsigned e = 0;
        while ((t & 1) == 0) {
            t >>= 1;
            ++e;
        }
        par = e & 1;
    } else {
        /* odd n: parity of the total number of prime factors with multiplicity */
        unsigned cnt = 0;
        for (idx_t p = 3; p <= t / p; p += 2) {
            while (t % p == 0) {
                t /= p;
                ++cnt;
            }
        }
        if (t > 1)
            ++cnt;
        par = cnt & 1;
    }

    return (seq_t)(1 - par);
}
