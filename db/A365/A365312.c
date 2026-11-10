/*
 * A365312  Number of strict integer partitions with sum <= n that cannot be
 *          linearly combined using nonnegative coefficients to obtain n.
 * https://oeis.org/A365312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of nonempty strict partitions of sum <= n whose
 *            parts cannot form n as a nonnegative combination; computed by
 *            DFS with an unbounded-knapsack reachability array, pruning once
 *            n is reachable
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static unsigned char *g_buf;
static idx_t g_n;
static seq_t g_cnt;

static void dfs(idx_t minp, idx_t s, idx_t depth) {
    unsigned char *cur = g_buf + depth * (g_n + 1);
    unsigned char *nxt = cur + (g_n + 1);
    for (idx_t q = minp; s + q <= g_n; q++) {
        memcpy(nxt, cur, (size_t)(g_n + 1));
        for (idx_t i = q; i <= g_n; i++) {
            if (nxt[i - q]) nxt[i] = 1;
        }
        if (nxt[g_n]) continue;
        g_cnt++;
        dfs(q + 1, s + q, depth + 1);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t k = 0;
    while ((k + 1) * (k + 2) / 2 <= n) k++;
    idx_t levels = k + 3;
    g_n = n;
    g_buf = (unsigned char *)malloc((size_t)(levels * (n + 1)));
    memset(g_buf, 0, (size_t)(levels * (n + 1)));
    g_buf[0] = 1;
    g_cnt = 0;
    dfs(1, 0, 0);
    seq_t r = g_cnt;
    free(g_buf);
    return r;
}
