/*
 * A353514  a(n) = 1 if A328572(2*n) is of the form 4m+3, and 0 otherwise.
 * https://oeis.org/A353514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if A328572(2n) == 3 (mod 4), else 0; A328572(m) = prod
 *            prime_i^(d_i-1) over nonzero digits d_i of m in primorial base
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 137 terms through n = 9936 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A353514_is_prime(uint64_t x)
{
    if (x < 2) return 0;
    for (uint64_t d = 2; d * d <= x; ++d)
        if (x % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t m = 2 * (uint64_t)n;
    uint64_t prod = 1; /* product mod 4 */
    uint64_t p = 2;
    while (m > 0) {
        uint64_t d = m % p;
        m /= p;
        if (d > 1) {
            uint64_t e = d - 1;
            uint64_t pm = p % 4;
            for (uint64_t i = 0; i < e; ++i)
                prod = (prod * pm) % 4;
        }
        do { ++p; } while (!A353514_is_prime(p));
    }
    return (seq_t)(prod == 3 ? 1 : 0);
}
