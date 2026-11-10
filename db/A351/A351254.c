/*
 * A351254  a(n) = A276085(gcd(n, A276086(n))).
 * https://oeis.org/A351254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A276085(gcd(n, A276086(n))); gcd taken via exponents
 *            using primorial-base digits of n, A276085 is fully additive with
 *            a(p) = product of primes < p
 * Range:     u8 n <= 483, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9612 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static const uint64_t pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};
    if (n <= 1) return (seq_t)0;

    uint64_t N = (uint64_t)n;
    uint64_t w = 1;            /* product of primes before index j */
    seq_t pp = 1;              /* same, as seq_t */
    seq_t result = 0;

    for (int j = 0; j < 20; ++j) {
        if (w > N) break;
        uint64_t p = pr[j];
        uint64_t digit = (N / w) % p;
        if (digit > 0) {
            uint64_t t = N;
            uint64_t e = 0;
            while (t % p == 0) { t /= p; ++e; }
            uint64_t m = (e < digit) ? e : digit;
            if (m > 0) result += (seq_t)m * pp;
        }
        if (w > UINT64_MAX / p) break;
        w *= p;
        pp *= (seq_t)p;
    }
    return result;
}
