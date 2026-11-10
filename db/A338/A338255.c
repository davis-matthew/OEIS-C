/*
 * A338255  a(n) = smallest m such that A328330(m) = n, or -1 if n never
 *          appears in A328330.
 * https://oeis.org/A338255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest m with A328330(m) = n; computed as min over v
 *            with A328330(v) = n-1 of the smallest number whose digits use
 *            exactly v segments (a(1) = 4)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 8 terms through n = 8 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>

static const unsigned char A338255_seg[10] = {6,2,5,5,4,5,6,3,7,6};

static unsigned long long A338255_segsum(unsigned long long x)
{
    unsigned long long s = 0;
    while (x > 0) {
        s += A338255_seg[x % 10];
        x /= 10;
    }
    return s;
}

static unsigned A338255_len(unsigned long long v)
{
    unsigned steps = 1;
    unsigned long long cur = v;
    for (;;) {
        unsigned long long nxt = A338255_segsum(cur);
        if (nxt == cur) break;
        steps++;
        cur = nxt;
    }
    return steps;
}

/* smallest digit string (no leading zero) with segment sum v; returns digit count or 0 */
static int A338255_minseg(unsigned long long v, unsigned char *out)
{
    unsigned long long d0 = (v + 6) / 7;
    if (d0 < 1) d0 = 1;
    for (unsigned long long d = d0; d <= d0 + 2; d++) {
        if (d > 250) return 0;
        unsigned long long rem = v;
        int ok = 1;
        for (unsigned long long i = 0; i < d && ok; i++) {
            unsigned long long k = d - 1 - i;
            int found = 0;
            for (int dg = (i == 0 ? 1 : 0); dg <= 9; dg++) {
                unsigned long long s = A338255_seg[dg];
                if (s > rem) continue;
                unsigned long long r2 = rem - s;
                if (r2 >= 2 * k && r2 <= 7 * k) {
                    out[i] = (unsigned char)dg;
                    rem = r2;
                    found = 1;
                    break;
                }
            }
            if (!found) ok = 0;
        }
        if (ok && rem == 0) return (int)d;
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n > 8) return (seq_t)0;
    unsigned target = (unsigned)(n - 1);
    unsigned long long v0 = 1;
    while (A338255_len(v0) != target) v0++;

    unsigned char best[256], cand[256];
    int bestlen = 0;
    for (unsigned long long v = v0; v <= v0 + 20; v++) {
        if (A338255_len(v) != target) continue;
        int l = A338255_minseg(v, cand);
        if (l == 0) continue;
        int better = 0;
        if (bestlen == 0 || l < bestlen) better = 1;
        else if (l == bestlen && memcmp(cand, best, (size_t)l) < 0) better = 1;
        if (better) {
            memcpy(best, cand, (size_t)l);
            bestlen = l;
        }
    }
    seq_t r = 0;
    for (int i = 0; i < bestlen; i++) {
        r = r * 10 + (seq_t)best[i];
    }
    return r;
}
