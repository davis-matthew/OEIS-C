/*
 * A348182  a(1) = 1; for n >= 2, a(n) = 1 + a(A057022(n)).
 * https://oeis.org/A348182
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = 1 + a(floor(sigma(n)/tau(n))) for n >= 2
 *            (A057022 iterated down to 1, counting terms)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 104 terms through n = 104 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A348182_step(uint64_t n)
{
    uint64_t sigma = 1, tau = 1;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t term = 1, sum = 1, e = 0;
            while (m % p == 0) {
                m /= p;
                term *= p;
                sum += term;
                ++e;
            }
            sigma *= sum;
            tau *= (e + 1);
        }
    }
    if (m > 1) {
        sigma *= (m + 1);
        tau *= 2;
    }
    return sigma / tau;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t count = 1;
    while (m > 1) {
        m = A348182_step(m);
        ++count;
    }
    return (seq_t)count;
}
