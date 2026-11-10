/*
 * A334245  Let S = A061228, T = A070229, and F = S o T (where o denotes
 *          composition). Then a(n) is the least number in the intersection of
 *          the sequences F^k(n) and F^k(S(n)), k>=0, or the intersection of
 *          the sequences T(F^k(n)) and T(F^k(S(n))), k>=0, or 0 if these
 *          intersections are empty.
 * https://oeis.org/A334245
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   with S=A061228, T=A070229, F(m)=S(T(m)); a(n) = least common
 *            term of F^k(n) and F^k(S(n)), or of T(F^k(n)) and T(F^k(S(n))),
 *            k>=0 (0 if none). Computed by a merge walk over the two
 *            increasing sequences, with a step cap
 * Range:     u8 n <= 70, u16 n <= 6042, u32 n <= 6042, u64 n <= 6042,
 *            u128 n <= 6042, big any n
 * Validated: standard, pass, 85 terms through n = 5649 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A334245_gpf(seq_t x)
{
    seq_t g = 1;
    while (x % 2 == 0) { g = 2; x /= 2; }
    for (seq_t d = 3; d * d <= x; d += 2) {
        while (x % d == 0) { g = d; x /= d; }
    }
    if (x > 1) g = x;
    return g;
}

static seq_t A334245_S(seq_t n)
{
    if (n <= 1) return (seq_t)2;
    if (n % 2 == 0) return n + 2;
    for (seq_t d = 3; d * d <= n; d += 2) {
        if (n % d == 0) return n + d;
    }
    return n + n;
}

static seq_t A334245_T(seq_t n)
{
    if (n <= 1) return (seq_t)2;
    return n + A334245_gpf(n);
}

static seq_t A334245_F(seq_t n)
{
    return A334245_S(A334245_T(n));
}

/* Merge walk; returns least common element or 0 if none found. */
static seq_t A334245_walk(seq_t a0, seq_t b0, int useT)
{
    seq_t a = a0, b = b0;
    seq_t x = useT ? A334245_T(a) : a;
    seq_t y = useT ? A334245_T(b) : b;
    for (long step = 0; step < 100000; step++) {
        if (x == y) return x;
        if (x < y) {
            a = A334245_F(a);
            x = useT ? A334245_T(a) : a;
        } else {
            b = A334245_F(b);
            y = useT ? A334245_T(b) : b;
        }
    }
    return (seq_t)0;
}

seq_t a(idx_t n)
{
    seq_t s = (seq_t)n;
    seq_t sn = A334245_S(s);
    seq_t r1 = A334245_walk(s, sn, 0);
    seq_t r2 = A334245_walk(s, sn, 1);
    if (r1 == 0) return r2;
    if (r2 == 0) return r1;
    return r1 < r2 ? r1 : r2;
}
