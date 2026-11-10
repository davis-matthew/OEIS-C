/*
 * A242576  Prime terms in A214828.
 * https://oeis.org/A242576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = n-th prime term of A214828, where A214828(k) =
 *            A214828(k-1) + A214828(k-2) + A214828(k-3), A214828(0) = 1,
 *            A214828(1) = A214828(2) = 6; primality by small-prime trial
 *            division then Miller-Rabin
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 17 terms through n = 17 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* nonzero when seq_t is arbitrary precision (wider than 128 bits) */
static bool A242576_is_big(void)
{
    static int cached = -1;
    if (cached < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 140; ++i) x += x;
        cached = (x != 0) ? 1 : 0;
    }
    return cached == 1;
}

static seq_t A242576_addmod(seq_t a, seq_t b, seq_t m)
{
    /* a, b < m */
    seq_t t = m - b;
    if (a >= t) return a - t;
    return a + b;
}

static seq_t A242576_mulmod(seq_t a, seq_t b, seq_t m)
{
    if (A242576_is_big()) return (a * b) % m;
    {
        seq_t r = 0;
        a %= m;
        b %= m;
        while (b != 0) {
            if ((b & 1) != 0) r = A242576_addmod(r, a, m);
            a = A242576_addmod(a, a, m);
            b >>= 1;
        }
        return r;
    }
}

static seq_t A242576_powmod(seq_t base, seq_t e, seq_t m)
{
    seq_t r = 1;
    base %= m;
    while (e != 0) {
        if ((e & 1) != 0) r = A242576_mulmod(r, base, m);
        e >>= 1;
        if (e != 0) base = A242576_mulmod(base, base, m);
    }
    return r % m;
}

static bool A242576_test(seq_t c)
{
    static const unsigned sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97};
    static const unsigned bases[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    size_t i;
    seq_t d, cm1;
    unsigned s = 0;
    if (c < 2) return false;
    for (i = 0; i < sizeof(sp) / sizeof(sp[0]); ++i) {
        if (c % sp[i] == 0) return c == (seq_t)sp[i];
    }
    if (c < (seq_t)(97 * 97)) return true;
    cm1 = c - 1;
    d = cm1;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    for (i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        seq_t x = A242576_powmod((seq_t)bases[i], d, c);
        unsigned r;
        bool ok;
        if (x == 1 || x == cm1) continue;
        ok = false;
        for (r = 1; r < s; ++r) {
            x = A242576_mulmod(x, x, c);
            if (x == cm1) { ok = true; break; }
            if (x == 1) break;
        }
        if (!ok) return false;
    }
    return true;
}

static seq_t *A242576_found = NULL;
static size_t A242576_cnt = 0, A242576_cap = 0;

static void A242576_push(seq_t v)
{
    if (A242576_cnt == A242576_cap) {
        size_t nc = A242576_cap ? A242576_cap * 2 : 16;
        A242576_found = mem_resize(seq_t, A242576_found, nc);
        A242576_cap = nc;
    }
    A242576_found[A242576_cnt++] = v;
}

seq_t a(idx_t n)
{
    static seq_t c0 = 1, c1 = 6, c2 = 6;
    static bool exhausted = false;   /* next term would overflow seq_t */

    while (A242576_cnt < (size_t)n && !exhausted) {
        if (A242576_test(c0)) A242576_push(c0);
        {
            seq_t s = c1 + c2;
            seq_t t;
            if (s < c2) { exhausted = true; break; }
            t = s + c0;
            if (t < s) { exhausted = true; break; }
            c0 = c1;
            c1 = c2;
            c2 = t;
        }
    }
    if (exhausted && A242576_cnt < (size_t)n) {
        /* the remaining A214828 terms c1, c2 still fit; check them */
        while (A242576_cnt < (size_t)n && (c0 != c1 || c1 != c2)) {
            c0 = c1;
            c1 = c2;
            if (A242576_test(c0)) A242576_push(c0);
        }
        if (A242576_cnt < (size_t)n) return 0;
    }
    return A242576_found[(size_t)n - 1];
}
