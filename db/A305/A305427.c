/*
 * A305427  Permutation of natural numbers: a(0) = 1, a(1) = 2, a(2n) =
 *          2*a(n), a(2n+1) = A305421(a(n)).
 * https://oeis.org/A305427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1, a(1) = 2, a(2n) = 2*a(n), a(2n+1) = A305421(a(n))
 *            (GF(2)[X] arithmetic)
 * Range:     u8 n <= 34, u16 n <= 262, u32 n <= 4126, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 97 terms through n = 9051 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define A305427_MAXIRR 20000
static uint64_t A305427_irrVals[A305427_MAXIRR];
static int      A305427_irrDegs[A305427_MAXIRR];
static size_t   A305427_irrSize = 1;
static int      A305427_maxGenDeg = 0;

static int A305427_deg(seq_t x)
{
    int d = -1;
    while (x != 0) { x >>= 1; ++d; }
    return d;
}

static int A305427_deg64(uint64_t x)
{
    int d = -1;
    while (x) { x >>= 1; ++d; }
    return d;
}

static uint64_t A305427_rem64(uint64_t a, uint64_t b)
{
    int db = A305427_deg64(b);
    int da;
    while (a != 0 && (da = A305427_deg64(a)) >= db)
        a ^= b << (da - db);
    return a;
}

static void A305427_gen(int maxDeg)
{
    while (A305427_maxGenDeg < maxDeg) {
        int d = A305427_maxGenDeg + 1;
        uint64_t start = 1ULL << d, end = 1ULL << (d + 1), val;
        int half = d / 2;
        for (val = start; val < end; ++val) {
            int irr = 1;
            size_t i;
            for (i = 1; i < A305427_irrSize; ++i) {
                if (A305427_irrDegs[i] > half) break;
                if (A305427_rem64(val, A305427_irrVals[i]) == 0) { irr = 0; break; }
            }
            if (irr) {
                if (A305427_irrSize >= A305427_MAXIRR) return;
                A305427_irrVals[A305427_irrSize] = val;
                A305427_irrDegs[A305427_irrSize] = d;
                ++A305427_irrSize;
            }
        }
        A305427_maxGenDeg = d;
    }
}

static seq_t A305427_clmul(seq_t a, uint64_t b)
{
    seq_t r = 0;
    while (b) {
        if (b & 1) r ^= a;
        a <<= 1;
        b >>= 1;
    }
    return r;
}

/* A305421: replace each irreducible factor by the next irreducible */
static seq_t A305427_shift(seq_t v)
{
    int d, dr;
    size_t i;
    seq_t r, res = 1;
    if (v <= 1) return 1;
    d = A305427_deg(v);
    A305427_gen(d + 1 > 17 ? 17 : d + 1);
    r = v;
    for (i = 1; i + 1 < A305427_irrSize; ++i) {
        uint64_t p = A305427_irrVals[i];
        int dp = A305427_irrDegs[i];
        if (2 * dp > A305427_deg(r)) break;
        for (;;) {
            seq_t rem = r, q = 0;
            dr = A305427_deg(rem);
            if (dr < dp) break;
            while (rem != 0 && (dr = A305427_deg(rem)) >= dp) {
                rem ^= ((seq_t)p) << (dr - dp);
                q |= ((seq_t)1) << (dr - dp);
            }
            if (rem != 0) break;
            r = q;
            res = A305427_clmul(res, A305427_irrVals[i + 1]);
        }
    }
    if (r != 1) {
        uint64_t rv;
        if (A305427_deg(r) > 17) return 0;
        rv = (uint64_t)r;
        for (i = 1; i + 1 < A305427_irrSize; ++i) {
            if (A305427_irrVals[i] == rv) {
                res = A305427_clmul(res, A305427_irrVals[i + 1]);
                return res;
            }
        }
        return 0;
    }
    return res;
}

seq_t a(idx_t n)
{
    int t, i;
    seq_t v;
    if (n == 0) return (seq_t)1;
    t = 63 - __builtin_clzll((unsigned long long)n);
    v = 2;
    for (i = t - 1; i >= 0; --i) {
        if ((n >> i) & 1) v = A305427_shift(v);
        else v <<= 1;
    }
    return v;
}
