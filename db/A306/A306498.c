/*
 * A306498  Smallest number m such that A306494(m) = n.
 * https://oeis.org/A306498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, more
 * Formula:   a(n) = smallest m such that A306494(m) = n, i.e. m*3^k has no
 *            two equal adjacent digits for k<n but does for k=n. Computed by
 *            a search: build m digit by digit from the least significant end,
 *            pruning when m*3^k (k<n) gets equal adjacent nonzero low digits
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 13, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A306498_rep(seq_t v)
{
    int last = -1;
    while (v > 0) {
        int d = (int)(v % 10);
        if (d == last) return true;
        last = d;
        v /= 10;
    }
    return false;
}

static int A306498_n;
static int A306498_D;
static bool A306498_found;
static seq_t A306498_best;

static void A306498_dfs(seq_t s, seq_t p10, int d)
{
    if (d == A306498_D) {
        /* s has exactly D digits (top digit nonzero) */
        seq_t t = s;
        for (int j = 0; j < A306498_n; j++) {
            if (A306498_rep(t)) return;
            t = t * 3;
        }
        if (!A306498_rep(t)) return;
        if (!A306498_found || s < A306498_best) {
            A306498_found = true;
            A306498_best = s;
        }
        return;
    }
    seq_t np = p10 * 10;
    for (int c = 0; c < 10; c++) {
        if (d + 1 == A306498_D && c == 0) continue;
        seq_t s2 = s + (seq_t)c * p10;
        bool ok = true;
        if (d >= 1) {
            seq_t t = s2;
            for (int j = 0; j < A306498_n && ok; j++) {
                seq_t x = t % np;
                int hi = (int)((x / p10) % 10);
                int lo = (int)((x / (p10 / 10)) % 10);
                if (hi == lo && hi != 0) ok = false;
                t = t * 3;
            }
        }
        if (ok) A306498_dfs(s2, np, d + 1);
    }
}

seq_t a(idx_t n)
{
    A306498_n = (int)n;
    for (int D = 1; D <= 40; D++) {
        A306498_D = D;
        A306498_found = false;
        A306498_best = 0;
        A306498_dfs((seq_t)0, (seq_t)1, 0);
        if (A306498_found) return A306498_best;
    }
    return (seq_t)0;
}
