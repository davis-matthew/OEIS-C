/*
 * A374294  a(n) is the smallest positive integer k such that A002325(k) = n.
 * https://oeis.org/A374294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over factorizations n = d_1*d_2*...*d_r (d_i>1,
 *            d_1>=d_2>=...) of prod p_i^(d_i-1), p_i = i-th prime == 1,3 (mod
 *            8) (3,11,17,19,41,...); computed by branch-and-bound search
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static idx_t A374294_pr[80];
static int A374294_np = 0;
static seq_t A374294_best;
static bool A374294_have;

static void A374294_init(void) {
    if (A374294_np) return;
    idx_t c = 3;
    while (A374294_np < 80) {
        idx_t m = c % 8;
        if (m == 1 || m == 3) {
            bool ok = true;
            for (idx_t q = 2; q * q <= c; q++)
                if (c % q == 0) { ok = false; break; }
            if (ok) A374294_pr[A374294_np++] = c;
        }
        c++;
    }
}

static void A374294_dfs(idx_t rem, idx_t maxf, int idx, seq_t cur) {
    if (rem == 1) {
        if (!A374294_have || cur < A374294_best) {
            A374294_best = cur;
            A374294_have = true;
        }
        return;
    }
    if (idx >= A374294_np) return;
    idx_t p = A374294_pr[idx];
    idx_t top = rem < maxf ? rem : maxf;
    for (idx_t d = top; d >= 2; d--) {
        if (rem % d != 0) continue;
        seq_t v = cur;
        bool ok = true;
        for (idx_t e = 0; e + 1 < d; e++) {
            if (A374294_have && v > A374294_best / (seq_t)p) { ok = false; break; }
            v = v * (seq_t)p;
        }
        if (!ok) continue;
        if (A374294_have && v >= A374294_best) continue;
        A374294_dfs(rem / d, d, idx + 1, v);
    }
}

seq_t a(idx_t n) {
    A374294_init();
    A374294_have = false;
    A374294_best = 0;
    A374294_dfs(n, n, 0, (seq_t)1);
    return A374294_best;
}
