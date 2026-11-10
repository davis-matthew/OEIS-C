/*
 * A334202  a(n) = A064097(n) - A323077(n).
 * https://oeis.org/A334202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A064097(A006530(n)); A064097(1) = 0, A064097(p) = 1 +
 *            A064097(p-1) for prime p, completely additive
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9794 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A334202_gpf(uint64_t m)
{
    uint64_t g = 1;
    while (m % 2 == 0) { g = 2; m /= 2; }
    for (uint64_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) { g = i; m /= i; }
    }
    if (m > 1) g = m;
    return g;
}

static uint64_t A334202_f(uint64_t m);

/* value at prime p */
static uint64_t A334202_g(uint64_t p)
{
    return 1 + A334202_f(p - 1);
}

/* A064097(m), completely additive over prime factorization */
static uint64_t A334202_f(uint64_t m)
{
    uint64_t sum = 0;
    if (m <= 1) return 0;
    if (m % 2 == 0) {
        int e = 0;
        while (m % 2 == 0) { m /= 2; ++e; }
        sum += (uint64_t)e * 1; /* g(2) = 1 */
    }
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            sum += (uint64_t)e * A334202_g(p);
        }
    }
    if (m > 1) sum += A334202_g(m);
    return sum;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;
    return (seq_t)A334202_g(A334202_gpf((uint64_t)n));
}
