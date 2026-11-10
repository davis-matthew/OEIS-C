/*
 * A244002  Prime terms in A214830.
 * https://oeis.org/A244002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search -- a(n) is the n-th prime among the terms of A214830,
 *            b(k) = b(k-1) + b(k-2) + b(k-3), b(0) = 1, b(1) = b(2) = 8;
 *            primality by small-prime trial division followed by Miller-Rabin
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A244002_fixed(void)
{
    /* true in a fixed-width unsigned build, false for arbitrary precision */
    return ((seq_t)0 - (seq_t)1) > (seq_t)0;
}

static seq_t A244002_addmod(seq_t x, seq_t y, seq_t m)
{
    /* x, y < m */
    seq_t t = m - y;
    if (x >= t) return x - t;
    return x + y;
}

static seq_t A244002_mulmod(seq_t x, seq_t y, seq_t m)
{
    if (!A244002_fixed()) return (x * y) % m;
    seq_t r = (seq_t)0;
    x %= m;
    y %= m;
    while (y > (seq_t)0) {
        if ((y & (seq_t)1) != (seq_t)0) r = A244002_addmod(r, x, m);
        x = A244002_addmod(x, x, m);
        y >>= 1;
    }
    return r;
}

static seq_t A244002_powmod(seq_t b, seq_t e, seq_t m)
{
    seq_t r = (seq_t)1;
    b %= m;
    while (e > (seq_t)0) {
        if ((e & (seq_t)1) != (seq_t)0) r = A244002_mulmod(r, b, m);
        b = A244002_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

static bool A244002_is_prime(seq_t v)
{
    static const int sp[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,
                             67,71,73,79,83,89,97,101,103,107,109,113,127,131,
                             137,139,149,151,157,163,167,173,179,181,191,193,197,199};
    const int nsp = (int)(sizeof(sp) / sizeof(sp[0]));
    if (v < (seq_t)2) return false;
    for (int i = 0; i < nsp; ++i) {
        if (v == (seq_t)sp[i]) return true;
        if (v % (seq_t)sp[i] == 0) return false;
    }
    if (v < (seq_t)(199 * 199)) return true;

    seq_t d = v - (seq_t)1;
    int s = 0;
    while ((d & (seq_t)1) == (seq_t)0) { d >>= 1; ++s; }
    seq_t vm1 = v - (seq_t)1;

    static const int bases[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
    const int nb = (int)(sizeof(bases) / sizeof(bases[0]));
    for (int i = 0; i < nb; ++i) {
        seq_t x = A244002_powmod((seq_t)bases[i], d, v);
        if (x == (seq_t)1 || x == vm1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = A244002_mulmod(x, x, v);
            if (x == vm1) { comp = false; break; }
            if (x == (seq_t)1) break;
        }
        if (comp) return false;
    }
    return true;
}

#define A244002_MAXC 4096

seq_t a(idx_t n)
{
    static idx_t idxs[A244002_MAXC];   /* indices k in A214830 that are prime */
    static int cnt = 0;
    static idx_t scanned = 0;          /* all k < scanned have been tested */

    if (n < 1 || n > A244002_MAXC) return (seq_t)0;

    seq_t x = (seq_t)1, y = (seq_t)8, z = (seq_t)8;   /* b(k), b(k+1), b(k+2) */
    idx_t k = 0;

    if ((idx_t)cnt >= n) {
        idx_t target = idxs[n - 1];
        while (k < target) {
            seq_t nx = x + y + z;
            x = y; y = z; z = nx;
            ++k;
        }
        return x;
    }

    for (;;) {
        if (k >= scanned) {
            if (A244002_is_prime(x)) {
                if (cnt < A244002_MAXC) idxs[cnt++] = k;
                scanned = k + 1;
                if ((idx_t)cnt == n) return x;
            } else {
                scanned = k + 1;
            }
        }
        seq_t nx = x + y + z;
        if (A244002_fixed() && nx < z) break;   /* would wrap in fixed width */
        x = y; y = z; z = nx;
        ++k;
    }
    return (seq_t)0;
}
