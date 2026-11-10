/*
 * A290047  Primes in A290046.
 * https://oeis.org/A290047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   search: a(n) = n-th prime among A290046(k) = |Sum_{j=0..k}
 *            (-1)^j*(3j)!!|, k = 0..25 (for k >= 26, 41 | A290046(k), so no
 *            further terms)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Validated: partial, pass, 5 terms through n = 5 (2026-11-08)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static seq_t A290047_dfact(idx_t m)
{
    seq_t r = 1;
    while (m >= 2) { r *= (seq_t)m; m -= 2; }
    return r;
}

static seq_t A290047_mulmod(seq_t x, seq_t y, seq_t m)
{
    seq_t r = 0;
    x %= m;
    while (y > 0) {
        if ((y & 1) != 0) { r += x; if (r >= m) r -= m; }
        x += x; if (x >= m) x -= m;
        y >>= 1;
    }
    return r;
}

static seq_t A290047_powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = 1 % m;
    b %= m;
    while (e > 0) {
        if ((e & 1) != 0) r = A290047_mulmod(r, b, m);
        b = A290047_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A290047_isprime(seq_t v)
{
    if (v < 2) return false;
    if (v <= (seq_t)UINT64_MAX) return is_prime_u64((uint64_t)v);
    static const unsigned bases[24] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
                                       53,59,61,67,71,73,79,83,89};
    for (int i = 0; i < 24; i++)
        if (v % bases[i] == 0) return false;
    seq_t d = v - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; s++; }
    for (int i = 0; i < 24; i++) {
        seq_t x = A290047_powmod((seq_t)bases[i], d, v);
        if (x == 1 || x == v - 1) continue;
        bool ok = false;
        for (int r = 1; r < s; r++) {
            x = A290047_mulmod(x, x, v);
            if (x == v - 1) { ok = true; break; }
        }
        if (!ok) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0;
    idx_t count = 0;
    for (idx_t k = 0; k <= 25; k++) {
        seq_t t = A290047_dfact(3 * k);
        if ((k & 1) == 0) pos += t; else neg += t;
        seq_t v = (pos >= neg) ? pos - neg : neg - pos;
        if (A290047_isprime(v)) {
            count++;
            if (count == n) return v;
        }
    }
    return 0;
}
