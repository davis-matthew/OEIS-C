/*
 * A335915  Fully multiplicative with a(2) = 1, and a(p) =
 *          A000265(p-1)*A000265(p+1) = A000265(p^2 - 1), for odd primes p.
 * https://oeis.org/A335915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, look
 * Formula:   a(n) = Product_{p^e || n} f(p)^e, completely multiplicative,
 *            f(2) = 1, f(p) = A000265(p-1)*A000265(p+1) for odd primes p
 * Range:     u8 n <= 52, u16 n <= 732, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 110 terms through n = 7989 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A335915_odd(idx_t x)
{
    if (x == 0)
        return 0;
    while ((x & 1) == 0)
        x >>= 1;
    return x;
}

static seq_t A335915_fp(idx_t p)
{
    if (p == 2)
        return (seq_t)1;
    return (seq_t)A335915_odd(p - 1) * (seq_t)A335915_odd(p + 1);
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    idx_t m = n;
    if (m <= 1)
        return result;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            seq_t f = A335915_fp(p);
            while (m % p == 0) {
                m /= p;
                result *= f;
            }
        }
    }
    if (m > 1)
        result *= A335915_fp(m);
    return result;
}
