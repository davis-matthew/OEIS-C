/*
 * A285701  a(n) = number of iterations x -> A064216(x) needed to reach a
 *          nonprime number when starting from n, a(2) = a(3) = 1.
 * https://oeis.org/A285701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is nonprime, else 1 + a(A064216(n)), with a(2) =
 *            a(3) = 1; A064216(n) = A064989(2n-1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9652 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A285701_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A285701_prevprime(uint64_t p)
{
    for (uint64_t q = p - 1; q >= 2; --q)
        if (A285701_isp(q)) return q;
    return 2;
}

/* A064216(x): replace each p^e in 2x-1 by prevprime(p)^e */
static uint64_t A285701_f(uint64_t x)
{
    uint64_t t = 2ULL * x - 1ULL;
    if (t <= 1ULL) return 1;
    uint64_t res = 1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t prev = A285701_prevprime(p);
            while (t % p == 0) {
                t /= p;
                res *= prev;
            }
        }
    }
    if (t > 1) res *= A285701_prevprime(t);
    return res;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    uint64_t count = 0;
    while (A285701_isp(x)) {
        count++;
        if (x == 2 || x == 3) break; /* fixed points */
        x = A285701_f(x);
    }
    return (seq_t)count;
}
