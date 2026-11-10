/*
 * A371151  Numbers k >= 2 such that A362333(k)-A371148(k)/A371149(k) sets a
 *          new maximum.
 * https://oeis.org/A371151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1) = 2; a(n) = 2^(A080085(n+1)+1)*prime(n+1) for n >= 2, where
 *            A080085(m) = Sum_{k>=1} floor(prime(m)/2^k) (conjectured in the
 *            OEIS entry; matches the listed terms)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-10)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)2;

    uint64_t p = nth_prime(n + 1);
    uint64_t e = 1;                 /* the "+1" in the exponent */
    uint64_t t = p;
    while (t) {
        t >>= 1;
        e += t;                     /* adds floor(p / 2^k) */
    }

    seq_t r = (seq_t)p;
    for (uint64_t i = 0; i < e; ++i) {
        r = r * 2;
        if (r == 0)                 /* wrapped out of a fixed-width type */
            break;
    }
    return r;
}
