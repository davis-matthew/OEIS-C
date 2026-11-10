/*
 * A353420  a(n) = A126760(A003961(n)).
 * https://oeis.org/A353420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A126760(A003961(n)) = 1 + 2*floor(m/6) + [m mod 6 == 5],
 *            where m = A003961(A000265(n))
 * Range:     u8 n <= 62, u16 n <= 6560, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 105 terms through n = 9818 (2026-11-10)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    while (m > 0 && (m & 1) == 0)
        m >>= 1;

    seq_t s = 1; /* A003961(odd part of n) */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            seq_t q = (seq_t)next_prime(p);
            while (m % p == 0) {
                m /= p;
                s *= q;
            }
        }
    }
    if (m > 1)
        s *= (seq_t)next_prime(m);

    seq_t r = s % 6;
    seq_t res = (seq_t)2 * (s / 6) + 1;
    if (r == 5)
        res += 1;
    return res;
}
