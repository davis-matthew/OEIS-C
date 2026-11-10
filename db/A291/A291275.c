/*
 * A291275  Primes p such that p does not divide any term of the Apéry-like
 *          sequence A005258.
 * https://oeis.org/A291275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p such that p divides no Apery number
 *            A005258(k). Method: by the Lucas property of A005258, p divides
 *            some term iff p divides some a(m) with 0 <= m <= p-1. Compute
 *            a(m) mod p by the recurrence (m+1)^2 a(m+1) = (11m^2+11m+3) a(m)
 *            + m^2 a(m-1), a(0)=1, a(1)=3, and scan the primes...
 * Range:     u8 n <= 19, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static uint64_t a291275_pw(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

static bool a291275_isprime(uint64_t x)
{
    uint64_t d;
    if (x < 2) return false;
    for (d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

/* true if p divides no Apery number */
static bool a291275_good(uint64_t p)
{
    uint64_t prev, cur, m;
    prev = 1 % p;
    cur = 3 % p;
    if (prev == 0 || cur == 0) return false;
    for (m = 1; m + 1 < p; m++) {
        uint64_t m2 = m * m % p;
        uint64_t c = (11 * m2 + 11 * m + 3) % p;
        uint64_t num = (c * cur + m2 * prev) % p;
        uint64_t d = (m + 1) % p;
        uint64_t inv = a291275_pw(d * d % p, p - 2, p);
        uint64_t nxt = num * inv % p;
        prev = cur;
        cur = nxt;
        if (cur == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t p = 1;
    idx_t cnt = 0;
    while (cnt < n) {
        p++;
        if (a291275_isprime(p) && a291275_good(p))
            cnt++;
    }
    return (seq_t)p;
}
