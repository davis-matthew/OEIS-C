/*
 * A277021  Left inverse of A277022.
 * https://oeis.org/A277021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A276085(A005940(1+n)); A276085 is fully additive with
 *            a(prime(k)) = prime(k)#/prime(k), and A005940(1+n) is built from
 *            the binary digits of n. The code sums those contributions
 *            directly
 * Range:     u8 n <= 31, u16 n <= 127, u32 n <= 1023, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 99 terms through n = 7565 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A277021_next_prime(uint64_t p)
{
    uint64_t c = p + 1;
    for (;; ++c) {
        int ok = 1;
        for (uint64_t d = 2; d * d <= c; ++d) {
            if (c % d == 0) { ok = 0; break; }
        }
        if (ok && c >= 2) return c;
    }
}

seq_t a(idx_t n)
{
    idx_t m = n;
    seq_t result = 0;
    seq_t P = 1;          /* product of primes less than prime(i) */
    uint64_t p = 2;       /* prime(i) */
    while (m > 0) {
        if (m & 1) {
            result += P;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            P = P * (seq_t)p;
            p = A277021_next_prime(p);
        }
    }
    return result;
}
