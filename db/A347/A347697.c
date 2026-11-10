/*
 * A347697  a(n) = max_{k <= n} A347696(k).
 * https://oeis.org/A347697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{k<=n} A347696(k); A347696(0)=0, A347696(i) = 1 + max
 *            over zero bits s of i (below top bit) of A347696(i - 2^s);
 *            computed by DP with prefix maxima
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Validated: standard, pass, 97 terms through n = 7315 (2026-11-10)
 * Helpers:   ilog2, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ilog2.h"
#include "../utils/mem.h"

static seq_t *A347697_len = NULL;  /* A347696 values */
static seq_t *A347697_pm  = NULL;  /* prefix maxima */
static size_t A347697_cnt = 0;     /* number of computed entries */

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N >= A347697_cnt) {
        size_t newcnt = N + 1;
        seq_t *t1 = mem_resize(seq_t, A347697_len, newcnt);
        if (!t1) exit(1);
        A347697_len = t1;
        seq_t *t2 = mem_resize(seq_t, A347697_pm, newcnt);
        if (!t2) exit(1);
        A347697_pm = t2;
        size_t start = A347697_cnt;
        if (start == 0) {
            A347697_len[0] = (seq_t)0;
            A347697_pm[0] = (seq_t)0;
            start = 1;
        }
        for (size_t i = start; i < newcnt; ++i) {
            unsigned bl = ilog2((idx_t)i);
            seq_t best = (seq_t)0;
            for (unsigned s = 0; s <= bl; ++s) {
                if (((i >> s) & 1u) == 0) {
                    seq_t cand = A347697_len[i - ((size_t)1 << s)] + (seq_t)1;
                    if (cand > best) best = cand;
                }
            }
            A347697_len[i] = best;
            A347697_pm[i] = (best > A347697_pm[i - 1]) ? best : A347697_pm[i - 1];
        }
        A347697_cnt = newcnt;
    }
    return A347697_pm[N];
}
