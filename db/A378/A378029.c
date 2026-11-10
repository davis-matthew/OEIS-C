/*
 * A378029  Record values in A377059.
 * https://oeis.org/A378029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A377059(A378028(n)); computed by scanning m = 1, 2, ...
 *            and collecting strict records of A377059(m) (A377059(m) =
 *            smallest even multiplicative order < m-1 of the least valid x
 *            coprime to m, 0 for m < 4)
 * Range:     u8 n <= 15, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A378029_mulmod(idx_t a, idx_t b, idx_t m)
{
    return (idx_t)((unsigned __int128)a * b % m);
}

static idx_t A378029_powmod(idx_t b, idx_t e, idx_t m)
{
    idx_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A378029_mulmod(r, b, m);
        b = A378029_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static idx_t A378029_gcd(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Euler phi of m via trial division */
static idx_t A378029_phi(idx_t m)
{
    idx_t r = m;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

/* multiplicative order of x mod m, given phi = phi(m) */
static idx_t A378029_order(idx_t x, idx_t m, idx_t phi)
{
    idx_t t = phi;
    idx_t k = phi;
    for (idx_t q = 2; q * q <= k; ++q) {
        if (k % q == 0) {
            while (k % q == 0) k /= q;
            while (t % q == 0 && A378029_powmod(x, t / q, m) == 1) t /= q;
        }
    }
    if (k > 1) {
        while (t % k == 0 && A378029_powmod(x, t / k, m) == 1) t /= k;
    }
    return t;
}

static idx_t A378029_A377059(idx_t m)
{
    if (m < 4) return 0;
    idx_t limit = m - 1;
    idx_t phi = A378029_phi(m);
    for (idx_t x = 2; x < m; ++x) {
        if (A378029_gcd(x, m) != 1) continue;
        idx_t r = A378029_order(x, m, phi);
        if ((r % 2) == 0 && r < limit) return r;
    }
    return 0;
}

#define A378029_MAXREC 512

seq_t a(idx_t n)
{
    static idx_t recs[A378029_MAXREC];
    static idx_t count = 0;
    static idx_t next_m = 1;

    if (n < 1 || n > A378029_MAXREC) return 0;
    while (count < n) {
        idx_t v = A378029_A377059(next_m);
        if (count == 0 || v > recs[count - 1]) {
            recs[count++] = v;
        }
        ++next_m;
    }
    return (seq_t)recs[n - 1];
}
