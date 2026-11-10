/*
 * A362044  a(n) = largest k such that k < m^2 and rad(k) | m, where rad(k) =
 *          A007947(k) and m = A120944(n).
 * https://oeis.org/A362044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{k < m^2 : rad(k) | m}, m = A120944(n) the n-th
 *            composite squarefree number; computed by a DFS over products of
 *            the primes of m below m^2
 * Range:     u8 n <= 4, u16 n <= 102, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8993 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Factor x; return false if not squarefree or not composite; store primes. */
static bool a362044_comp_sqfree(uint64_t x, uint64_t *pr, int *cnt) {
    int c = 0;
    uint64_t m = x;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return false;
            pr[c++] = p;
        }
    }
    if (m > 1) pr[c++] = m;
    *cnt = c;
    return c >= 2;
}

static void a362044_dfs(const uint64_t *pr, int cnt, int i, seq_t cur,
                        seq_t limit, seq_t *best) {
    if (i == cnt) {
        if (cur > *best) *best = cur;
        return;
    }
    seq_t p = (seq_t)pr[i];
    seq_t maxv = (limit - 1) / p;
    a362044_dfs(pr, cnt, i + 1, cur, limit, best);
    while (cur <= maxv) {
        cur *= p;
        a362044_dfs(pr, cnt, i + 1, cur, limit, best);
    }
}

seq_t a(idx_t n) {
    uint64_t pr[32];
    int cnt = 0;
    uint64_t x = 3;
    idx_t found = 0;
    while (found < n) {
        x++;
        if (a362044_comp_sqfree(x, pr, &cnt)) found++;
    }
    seq_t limit = (seq_t)x * (seq_t)x;
    seq_t best = 1;
    a362044_dfs(pr, cnt, 0, (seq_t)1, limit, &best);
    return best;
}
