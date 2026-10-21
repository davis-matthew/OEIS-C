/*
 * A250148  Number of length n+6 0..2 arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 17, u64 n <= 42, u128 n <= 91,
 *            big any n
 * Helpers:   encode6, next_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over the last six digits.
   For each step we keep counts of sequences of the current length
   that end with a particular 6‑tuple.  A new digit is admissible iff
   the 7‑tuple formed with the current 6‑tuple satisfies
   "max of some two equals min of the other five".  The arrays
   ok[6‑tuple][new‑digit] are precomputed once.  The sequence
   a(n) is the total number of admissible arrays of length n+6. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/encode6.h"
#include "../utils/next_state.h"

/* precomputed table: for every 6‑digit state (0..3^6-1)
   and every new digit v in {0,1,2} whether the resulting
   7‑window satisfies the property. */
static bool ok[729][3];
static bool ok_inited = false;

/* check whether a 7‑tuple satisfies the condition */
static bool window_ok(const int *w)
{
    for (int i = 0; i < 7; ++i) {
        for (int j = i + 1; j < 7; ++j) {
            int m = w[i] > w[j] ? w[i] : w[j];
            int min_other = 3;                 /* larger than any symbol */
            for (int k = 0; k < 7; ++k) {
                if (k == i || k == j) continue;
                if (w[k] < min_other) min_other = w[k];
            }
            if (m == min_other) return true;
        }
    }
    return false;
}

/* initialise ok table once */
static void init_ok(void)
{
    if (ok_inited) return;
    for (int s = 0; s < 729; ++s) {
        int d[6];
        int tmp = s;
        for (int i = 5; i >= 0; --i) { d[i] = tmp % 3; tmp /= 3; }
        for (int v = 0; v <= 2; ++v) {
            int w[7];
            for (int i = 0; i < 6; ++i) w[i] = d[i];
            w[6] = v;
            ok[s][v] = window_ok(w);
        }
    }
    ok_inited = true;
}

/* main function requested by the build system */
seq_t a(idx_t n)
{
    init_ok();

    idx_t N = n + 6;          /* target sequence length */

    /* DP tables allocated on the stack; seq_t is default‑constructible
       (integer or arbitrary–precision class). */
    seq_t cnt[729];
    seq_t nxt[729];
    for (int i = 0; i < 729; ++i) { cnt[i] = 0; nxt[i] = 0; }

    /* Base case: sequences of length 7 (one window) */
    for (int s7 = 0; s7 < 2187; ++s7) {          /* 3^7 */
        int w[7];
        int tmp = s7;
        for (int k = 6; k >= 0; --k) { w[k] = tmp % 3; tmp /= 3; }
        int state0 = encode6(w);
        int last   = w[6];
        if (!ok[state0][last]) continue;        /* window invalid */
        int suf = next_state(state0, last);
        cnt[suf] += (seq_t)1;
    }

    /* Extend to longer lengths */
    if (N > 7) {
        for (idx_t pos = 8; pos <= N; ++pos) {
            for (int i = 0; i < 729; ++i) nxt[i] = 0;
            for (int s = 0; s < 729; ++s) {
                seq_t cur = cnt[s];
                if (cur == 0) continue;
                for (int v = 0; v <= 2; ++v) {
                    if (!ok[s][v]) continue;
                    int ns = next_state(s, v);
                    nxt[ns] += cur;
                }
            }
            /* copy nxt into cnt for the next iteration */
            for (int i = 0; i < 729; ++i) cnt[i] = nxt[i];
        }
    }

    /* Sum all counts to get the total number of valid arrays */
    seq_t total = 0;
    for (int i = 0; i < 729; ++i) total += cnt[i];
    return total;
}
