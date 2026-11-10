/*
 * A334206  Fully additive with a(p) = 1 + a(A014682(p)) when p is prime and
 *          a(n*m) = a(n) + a(m) when m,n > 1.
 * https://oeis.org/A334206
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(p) = 1 + a(A014682(p)) for prime p; a(n*m) = a(n) +
 *            a(m) (fully additive)
 * Range:     u8 n <= 20879, u16 n <= 20879, u32 n <= 20879, u64 n <= 20879,
 *            u128 n <= 20879, big any n
 * Validated: standard, pass, 105 terms through n = 9427 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A334206_collatz(uint64_t n)
{
    if ((n & 1ULL) == 0ULL) return n / 2;
    return (3 * n + 1) / 2;
}

static seq_t A334206_f(uint64_t m)
{
    if (m <= 1) return (seq_t)0;
    uint64_t d = 0;
    if ((m & 1ULL) == 0ULL) {
        if (m > 2) d = 2;
    } else {
        for (uint64_t k = 3; k <= m / k; k += 2) {
            if (m % k == 0) { d = k; break; }
        }
    }
    if (d == 0) {
        /* m is prime */
        return (seq_t)1 + A334206_f(A334206_collatz(m));
    }
    return A334206_f(d) + A334206_f(m / d);
}

seq_t a(idx_t n)
{
    return A334206_f((uint64_t)n);
}
