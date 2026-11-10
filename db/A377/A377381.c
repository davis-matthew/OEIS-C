/*
 * A377381  a(n) is the number of divisors of n that are interprime numbers
 *          (A024675).
 * https://oeis.org/A377381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d = (p+q)/2 for
 *            consecutive odd primes p<q (A024675); computed by divisor
 *            enumeration and primality testing
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 87 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A377381_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (uint64_t i = 5; i * i <= x; i += 6)
        if (x % i == 0 || x % (i + 2) == 0) return false;
    return true;
}

static bool A377381_interprime(uint64_t d)
{
    if (d < 4) return false;
    if (A377381_isp(d)) return false;
    for (uint64_t k = 1; k < d - 2; k++) {
        bool lo = A377381_isp(d - k);
        bool hi = A377381_isp(d + k);
        if (lo || hi) return lo && hi && (d - k >= 3);
    }
    return false;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    uint64_t cnt = 0;
    for (uint64_t i = 1; i * i <= m; i++) {
        if (m % i != 0) continue;
        uint64_t j = m / i;
        if (A377381_interprime(i)) cnt++;
        if (j != i && A377381_interprime(j)) cnt++;
    }
    return (seq_t)cnt;
}
