/*
 * A305423  Permutation of natural numbers: a(n) = (A305421(n)+1)/2.
 * https://oeis.org/A305423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A305421(n)+1)/2, where A305421(n) replaces each
 *            irreducible factor of n in GF(2)[X] with the next irreducible
 *            and multiplies them carry-lessly
 * Range:     u8 n <= 16, u16 n <= 513, u32 n <= 13106, u64 n <= 13106,
 *            u128 n <= 13106, big any n
 * Validated: standard, pass, 98 terms through n = 9582 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A305423_carry_mul(seq_t x, seq_t y)
{
    seq_t r = 0;
    while (y) {
        if (y & 1) r ^= x;
        x <<= 1;
        y >>= 1;
    }
    return r;
}

#define A305423_MAX_IRR 5000
static uint64_t A305423_irrVals[A305423_MAX_IRR];
static int      A305423_irrDegs[A305423_MAX_IRR];
static size_t   A305423_irrSize = 1;

static inline int A305423_deg(uint64_t x)
{
    return x ? (63 - __builtin_clzll(x)) : -1;
}

static inline uint64_t A305423_div(uint64_t x, uint64_t y, uint64_t *rem)
{
    uint64_t q = 0;
    uint64_t r = x;
    int degb = A305423_deg(y);
    while (r != 0 && A305423_deg(r) >= degb) {
        int shift = A305423_deg(r) - degb;
        r ^= y << shift;
        q |= 1ULL << shift;
    }
    if (rem) *rem = r;
    return q;
}

static void A305423_gen(int maxDeg)
{
    static int maxGenDeg = 0;
    while (maxGenDeg < maxDeg) {
        ++maxGenDeg;
        uint64_t start = 1ULL << maxGenDeg;
        uint64_t end   = 1ULL << (maxGenDeg + 1);
        for (uint64_t val = start; val < end; ++val) {
            int degHalf = maxGenDeg / 2;
            size_t i;
            int irr = 1;
            for (i = 1; i < A305423_irrSize; ++i) {
                if (A305423_irrDegs[i] > degHalf) break;
                uint64_t r;
                A305423_div(val, A305423_irrVals[i], &r);
                if (r == 0) { irr = 0; break; }
            }
            if (irr) {
                if (A305423_irrSize >= A305423_MAX_IRR) return;
                A305423_irrVals[A305423_irrSize] = val;
                A305423_irrDegs[A305423_irrSize] = maxGenDeg;
                ++A305423_irrSize;
            }
        }
    }
}

static int A305423_index_of(uint64_t val)
{
    for (size_t i = 1; i < A305423_irrSize; ++i)
        if (A305423_irrVals[i] == val)
            return (int)i;
    return 0;
}

static void A305423_factor(uint64_t f, int *out, size_t *len)
{
    if (f == 1) return;
    int idx = A305423_index_of(f);
    if (idx > 0) {
        out[(*len)++] = idx;
        return;
    }
    int degHalf = A305423_deg(f) / 2;
    for (size_t i = 1; i < A305423_irrSize; ++i) {
        if (A305423_irrDegs[i] > degHalf) break;
        uint64_t r;
        uint64_t q = A305423_div(f, A305423_irrVals[i], &r);
        if (r == 0) {
            A305423_factor(A305423_irrVals[i], out, len);
            A305423_factor(q, out, len);
            return;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    int degN = A305423_deg(nn);
    A305423_gen(degN + 1);

    int factorIdx[64];
    size_t cnt = 0;
    A305423_factor(nn, factorIdx, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t val = (seq_t)A305423_irrVals[factorIdx[i] + 1];
        res = A305423_carry_mul(res, val);
    }
    /* res is odd, so (res+1)/2 == res/2 + 1 */
    return res / 2 + 1;
}
