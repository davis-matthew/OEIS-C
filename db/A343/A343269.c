/*
 * A343269  a(n) is the smallest integer whose orbit length is n under
 *          iteration of the map r -> A061602(r).
 * https://oeis.org/A343269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest m with A303935(m) = n (orbit size of m under r
 *            -> A061602(r)); computed by a sieve of orbit sizes below 10^7,
 *            then, if none is found, a minimal-digit preimage search
 * Range:     u8 n <= 19, u16 n <= 57, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define A343269_B 10000000u

static unsigned char A343269_L[A343269_B];
static bool A343269_ready = false;
static const unsigned A343269_fact[10] = {1u,1u,2u,6u,24u,120u,720u,5040u,40320u,362880u};

static unsigned A343269_f(unsigned x)
{
    unsigned s = 0;
    if (x == 0) return 1u;
    while (x > 0) { s += A343269_fact[x % 10]; x /= 10; }
    return s;
}

static void A343269_init(void)
{
    static const unsigned c1[] = {1,2,145,40585};
    static const unsigned c3[] = {169,363601,1454};
    static const unsigned c2[] = {871,45361,872,45362};
    unsigned i;
    unsigned stack[512];
    if (A343269_ready) return;
    for (i = 0; i < 4; i++) A343269_L[c1[i]] = 1;
    for (i = 0; i < 3; i++) A343269_L[c3[i]] = 3;
    for (i = 0; i < 4; i++) A343269_L[c2[i]] = 2;
    for (i = 0; i < A343269_B; i++) {
        unsigned sp = 0, cur = i;
        if (A343269_L[i]) continue;
        while (A343269_L[cur] == 0) {
            stack[sp++] = cur;
            cur = A343269_f(cur);
        }
        {
            unsigned v = A343269_L[cur];
            while (sp > 0) {
                cur = stack[--sp];
                v++;
                A343269_L[cur] = (unsigned char)v;
            }
        }
    }
    A343269_ready = true;
}

static bool A343269_have;
static seq_t A343269_best;

static void A343269_rec(int d, unsigned r, int left, int *cnt)
{
    if (d == 1) {
        if ((int)r != left) return;
        {
            seq_t val = 0;
            int j, c;
            if (r > 0) {
                val = 1;
                for (j = 1; j < (int)r; j++) val = val * 10;
            }
            for (j = 2; j <= 9; j++)
                for (c = 0; c < cnt[j]; c++) val = val * 10 + j;
            if (!A343269_have || val < A343269_best) {
                A343269_best = val;
                A343269_have = true;
            }
        }
        return;
    }
    {
        unsigned maxc = r / A343269_fact[d];
        unsigned c;
        if (maxc > (unsigned)left) maxc = (unsigned)left;
        for (c = 0; c <= maxc; c++) {
            cnt[d] = (int)c;
            A343269_rec(d - 1, r - c * A343269_fact[d], left - (int)c, cnt);
        }
        cnt[d] = 0;
    }
}

seq_t a(idx_t n)
{
    unsigned i;
    unsigned target;
    A343269_init();
    if (n >= 250) return 0;
    target = (unsigned)n;
    for (i = 0; i < A343269_B; i++)
        if (A343269_L[i] == target) return (seq_t)i;

    /* No m below 10^7: m is the smallest preimage of some x with orbit size n-1. */
    {
        int bestk = 1000;
        for (i = 0; i < A343269_B; i++) {
            if (A343269_L[i] == target - 1) {
                unsigned r = i;
                int k = 0, d;
                for (d = 9; d >= 2; d--) { k += (int)(r / A343269_fact[d]); r %= A343269_fact[d]; }
                k += (int)r;
                if (k < bestk) bestk = k;
            }
        }
        A343269_have = false;
        A343269_best = 0;
        for (i = 0; i < A343269_B; i++) {
            if (A343269_L[i] == target - 1) {
                unsigned r = i;
                int k = 0, d;
                for (d = 9; d >= 2; d--) { k += (int)(r / A343269_fact[d]); r %= A343269_fact[d]; }
                k += (int)r;
                if (k == bestk) {
                    int cnt[10] = {0,0,0,0,0,0,0,0,0,0};
                    A343269_rec(9, i, bestk, cnt);
                }
            }
        }
        return A343269_best;
    }
}
