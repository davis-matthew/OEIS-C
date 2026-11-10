/*
 * A246268  Permutation of natural numbers: a(n) = A246264(A064216(2*n)).
 * https://oeis.org/A246268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A246264(A064216(2*n)); computed as the rank of v =
 *            A064989(4n-1) among all k with A003961(k) == 3 (mod 4), i.e. the
 *            values A064216(2m)
 * Range:     u8 n <= 136, u16 n <= 10001, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 95 terms through n = 9109 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static uint64_t A246268_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (!is_prime((idx_t)q)) --q;
    return q;
}

static unsigned A246268_nextprime_mod4(uint64_t p)
{
    uint64_t q = p + 1;
    while (!is_prime((idx_t)q)) ++q;
    return (unsigned)(q & 3);
}

/* true if A003961(v) == 3 (mod 4) */
static bool A246268_inS(uint64_t v)
{
    unsigned r = 1;
    for (uint64_t p = 2; p <= v / p; p += (p == 2 ? 1 : 2)) {
        if (v % p == 0) {
            unsigned e = 0;
            while (v % p == 0) { v /= p; e++; }
            unsigned m = A246268_nextprime_mod4(p);
            if (m == 3 && (e & 1)) r = (r * 3) & 3;
        }
    }
    if (v > 1) {
        unsigned m = A246268_nextprime_mod4(v);
        if (m == 3) r = (r * 3) & 3;
    }
    return r == 3;
}

/* A064989 of an odd number t */
static uint64_t A246268_A064989(uint64_t t)
{
    uint64_t res = 1;
    for (uint64_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            uint64_t pv = A246268_prevprime(p);
            while (t % p == 0) { t /= p; res *= pv; }
        }
    }
    if (t > 1) res *= A246268_prevprime(t);
    return res;
}

seq_t a(idx_t n)
{
    uint64_t t = 4ULL * (uint64_t)n - 1ULL;
    uint64_t v = A246268_A064989(t);
    uint64_t cnt = 0;
    for (uint64_t k = 2; k <= v; ++k)
        if (A246268_inS(k)) ++cnt;
    return (seq_t)cnt;
}
