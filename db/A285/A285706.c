/*
 * A285706  a(n) = number of iterations x -> A064216(x) needed to reach a
 *          nonprime number when starting from prime(n), a(1) = a(2) = 1.
 * https://oeis.org/A285706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations x -> A064216(x) = A064989(2x-1)
 *            starting at prime(n) until the value is nonprime; a(1) = a(2) =
 *            1 (computed by direct iteration)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 136 terms through n = 9070 (2026-11-08)
 * Helpers:   nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/nth_prime.h"

static bool a285706_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x < 4) return true;
    if ((x & 1) == 0) return false;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t a285706_prevp(uint64_t p)
{
    uint64_t q = p - 1;
    while (q > 2 && !a285706_isp(q)) --q;
    return q;
}

/* A064216(x) = A064989(2x-1) */
static uint64_t a285706_f(uint64_t x)
{
    uint64_t t = 2 * x - 1;
    uint64_t res = 1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t pv = a285706_prevp(p);
            while (t % p == 0) {
                t /= p;
                res *= pv;
            }
        }
    }
    if (t > 1) res *= a285706_prevp(t);
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)1;
    uint64_t cur = nth_prime(n);
    seq_t steps = 0;
    do {
        cur = a285706_f(cur);
        ++steps;
    } while (a285706_isp(cur));
    return steps;
}
