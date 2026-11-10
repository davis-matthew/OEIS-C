/*
 * A343268  Numbers that are not the sum of exactly four terms from A020330
 *          (not necessarily distinct).
 * https://oeis.org/A343268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m >= 0 that is not a sum of exactly four
 *            terms of A020330 (3,10,15,36,45,...); computed by a DP sieve up
 *            to 5000
 * Range:     u8 n <= 73, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Validated: standard, pass, 80 terms through n = 112 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define A343268_LIM 5000

static int A343268_cnt = -1;
static int A343268_vals[A343268_LIM + 1];

static void A343268_init(void)
{
    static int terms[2000];
    static unsigned char reach[2][A343268_LIM + 1];
    int nt = 0, k, x, c, s, i, cur, nxt;

    for (k = 1; k < 14; k++) {
        for (x = 1 << (k - 1); x < (1 << k); x++) {
            long v = (long)x * ((1L << k) + 1);
            if (v > A343268_LIM) break;
            terms[nt++] = (int)v;
        }
    }
    for (s = 0; s <= A343268_LIM; s++) reach[0][s] = 0;
    reach[0][0] = 1;
    cur = 0;
    for (c = 0; c < 4; c++) {
        nxt = 1 - cur;
        for (s = 0; s <= A343268_LIM; s++) reach[nxt][s] = 0;
        for (s = 0; s <= A343268_LIM; s++) {
            if (!reach[cur][s]) continue;
            for (i = 0; i < nt; i++) {
                int t = s + terms[i];
                if (t > A343268_LIM) break;
                reach[nxt][t] = 1;
            }
        }
        cur = nxt;
    }
    A343268_cnt = 0;
    for (s = 0; s <= A343268_LIM; s++)
        if (!reach[cur][s]) A343268_vals[A343268_cnt++] = s;
}

seq_t a(idx_t n)
{
    if (A343268_cnt < 0) A343268_init();
    if (n < 1 || n > (idx_t)A343268_cnt) return (seq_t)0;
    return (seq_t)A343268_vals[n - 1];
}
