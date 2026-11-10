/*
 * A247892  Number of nonprimes in n-th row of triangle A077581.
 * https://oeis.org/A247892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A247815(n), the count of nonprimes among the n
 *            smallest positive integers coprime to n (row n of A077581);
 *            computed by direct search
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A247892_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static bool A247892_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t found = 0, cnt = 0;
    for (uint64_t c = 1; found < n; ++c) {
        if (A247892_gcd(c, n) == 1) {
            ++found;
            if (!A247892_isprime(c)) ++cnt;
        }
    }
    return (seq_t)cnt;
}
