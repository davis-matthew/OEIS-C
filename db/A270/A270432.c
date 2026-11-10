/*
 * A270432  a(n) = number of terms A270430 <= n; least monotonic left inverse
 *          of A270430.
 * https://oeis.org/A270432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, for n > 1, a(n) = 1-(A048673(n)-A064216(n) reduced
 *            modulo 2) + a(n-1)
 * Range:     u8 n <= 511, u16 n <= 35000, u32 n <= 35000, u64 n <= 35000,
 *            u128 n <= 35000, big any n
 * Validated: standard, pass, 98 terms through n = 9884 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A270432_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A270432_nextp(uint64_t p)
{
    uint64_t q = p + 1;
    while (!A270432_isp(q)) q++;
    return q;
}

static uint64_t A270432_prevp(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !A270432_isp(q)) q--;
    return q;
}

/* A048673(m) = (A003961(m)+1)/2 */
static uint64_t A270432_A048673(uint64_t m)
{
    uint64_t prod = 1, t = m;
    for (uint64_t p = 2; p <= t / p; p++) {
        if (t % p == 0) {
            uint64_t np = A270432_nextp(p);
            while (t % p == 0) { t /= p; prod *= np; }
        }
    }
    if (t > 1) prod *= A270432_nextp(t);
    return (prod + 1) / 2;
}

/* A064216(m) = A064989(2m-1) */
static uint64_t A270432_A064216(uint64_t m)
{
    uint64_t t = 2 * m - 1, prod = 1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t pp = A270432_prevp(p);
            while (t % p == 0) { t /= p; prod *= pp; }
        }
    }
    if (t > 1) prod *= A270432_prevp(t);
    return prod;
}

seq_t a(idx_t n)
{
    static uint64_t cn = 1, cv = 1;
    if (n < cn) { cn = 1; cv = 1; }
    while (cn < (uint64_t)n) {
        cn++;
        uint64_t d = A270432_A048673(cn) - A270432_A064216(cn);
        cv += 1 - (d & 1);
    }
    return (seq_t)cv;
}
