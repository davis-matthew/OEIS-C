/*
 * A305417  Permutation of natural numbers: a(0) = 1, a(2n) = A305421(a(n)),
 *          a(2n+1) = 2*a(n).
 * https://oeis.org/A305417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(0) = 1, a(2n) = A305421(a(n)), a(2n+1) = 2*a(n); computed by
 *            reading the bits of n from the top, tracking a(n) as GF(2)[X]
 *            irreducible exponents (shift indices up for a 0 bit, add one
 *            factor x for a 1 bit), then multiplying out carry-less
 * Range:     u8 n <= 51, u16 n <= 479, u32 n <= 8159, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 99 terms through n = 9909 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define A305417_NIRR 80

static uint64_t A305417_irr[A305417_NIRR + 2];
static int A305417_ready = 0;

static int A305417_deg(uint64_t x)
{
    return x ? (63 - __builtin_clzll(x)) : -1;
}

static uint64_t A305417_mod(uint64_t a, uint64_t b)
{
    int db = A305417_deg(b);
    while (a && A305417_deg(a) >= db)
        a ^= b << (A305417_deg(a) - db);
    return a;
}

static void A305417_init(void)
{
    int cnt = 0;
    uint64_t p;
    if (A305417_ready) return;
    for (p = 2; cnt < A305417_NIRR + 1; ++p) {
        int dp = A305417_deg(p);
        int ok = 1;
        uint64_t d;
        for (d = 2; A305417_deg(d) * 2 <= dp; ++d) {
            if (A305417_mod(p, d) == 0) { ok = 0; break; }
        }
        if (ok) A305417_irr[++cnt] = p;
    }
    A305417_ready = 1;
}

seq_t a(idx_t n)
{
    unsigned exps[A305417_NIRR + 2];
    int i, top;
    seq_t res;
    A305417_init();
    for (i = 0; i < A305417_NIRR + 2; ++i) exps[i] = 0;
    if (n > 0) {
        top = 63 - __builtin_clzll((unsigned long long)n);
        for (i = top; i >= 0; --i) {
            if ((n >> i) & 1) {
                exps[1]++;
            } else {
                int j;
                for (j = A305417_NIRR; j >= 1; --j) exps[j + 1] = exps[j];
                exps[1] = 0;
            }
        }
    }
    res = 1;
    for (i = 1; i <= A305417_NIRR; ++i) {
        unsigned e;
        for (e = 0; e < exps[i]; ++e) {
            seq_t x = (seq_t)A305417_irr[i];
            seq_t r = 0;
            seq_t y = res;
            while (x) {
                if ((x & 1) != 0) r ^= y;
                y <<= 1;
                x >>= 1;
            }
            res = r;
        }
    }
    return res;
}
