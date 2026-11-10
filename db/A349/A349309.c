/*
 * A349309  Numbers k such that A254926(k) = A254926(k+1).
 * https://oeis.org/A349309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p (in increasing order) such that p^3-1 is
 *            cubefree, giving p^3-1 (these are the k with A254926(k) =
 *            A254926(k+1) = k; found by prime search plus cubefree test)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 100, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 99 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A349309_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

/* p^3-1 = (p-1)(p^2+p+1) cubefree? */
static bool A349309_cubefree(uint64_t p)
{
    uint64_t x = p - 1, y = p * p + p + 1;
    for (uint64_t q = 2; q * q * q <= y; q += (q == 2 ? 1 : 2)) {
        int e = 0;
        while (x % q == 0) { x /= q; e++; }
        while (y % q == 0) { y /= q; e++; }
        if (e >= 3) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t p = 1;
    while (count < n) {
        ++p;
        if (A349309_isprime(p) && A349309_cubefree(p))
            ++count;
    }
    seq_t P = (seq_t)p;
    return P * P * P - 1;
}
