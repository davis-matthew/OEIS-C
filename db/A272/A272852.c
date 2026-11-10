/*
 * A272852  Smallest value m such that A272851(m) = n.
 * https://oeis.org/A272852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, base
 * Formula:   a(n) = least m with A272851(m) = n, where A272851(m) counts the
 *            distinct nonzero Fibonacci numbers among the binary substrings
 *            of m. Computed by search: for each bit length L, depth-first
 *            search over bit strings in numeric order, with an Aho-Corasick
 *            automaton on the binary Fibonacci strings to t...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 13 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define AC_MAXN 3000
#define AC_MAXL 40

static int ac_go[AC_MAXN][2];
static uint64_t ac_out[AC_MAXN];
static int ac_nodes;
static int ac_maxout;
static int ac_fibcnt_len[AC_MAXL + 2]; /* number of fibs with bit length <= L */
static bool ac_ready = false;

static int ac_target;
static int ac_len;

static void A272852_build(void) {
    uint64_t fibs[80];
    int nf = 0;
    uint64_t f0 = 1, f1 = 2;
    while (f0 < ((uint64_t)1 << AC_MAXL)) {
        fibs[nf++] = f0;
        uint64_t t = f0 + f1;
        f0 = f1;
        f1 = t;
    }
    ac_nodes = 1;
    for (int i = 0; i < AC_MAXN; i++) { ac_go[i][0] = ac_go[i][1] = -1; ac_out[i] = 0; }
    for (int L = 0; L <= AC_MAXL + 1; L++) ac_fibcnt_len[L] = 0;
    for (int i = 0; i < nf; i++) {
        uint64_t v = fibs[i];
        int bl = 0;
        while ((v >> bl) != 0) bl++;
        for (int L = bl; L <= AC_MAXL + 1; L++) ac_fibcnt_len[L]++;
        int cur = 0;
        for (int p = bl - 1; p >= 0; p--) {
            int b = (int)((v >> p) & 1);
            if (ac_go[cur][b] < 0) ac_go[cur][b] = ac_nodes++;
            cur = ac_go[cur][b];
        }
        ac_out[cur] |= ((uint64_t)1 << i);
    }
    static int fail[AC_MAXN];
    static int queue[AC_MAXN];
    int qh = 0, qt = 0;
    fail[0] = 0;
    for (int b = 0; b < 2; b++) {
        if (ac_go[0][b] < 0) ac_go[0][b] = 0;
        else { fail[ac_go[0][b]] = 0; queue[qt++] = ac_go[0][b]; }
    }
    while (qh < qt) {
        int u = queue[qh++];
        ac_out[u] |= ac_out[fail[u]];
        for (int b = 0; b < 2; b++) {
            int v = ac_go[u][b];
            if (v < 0) ac_go[u][b] = ac_go[fail[u]][b];
            else { fail[v] = ac_go[fail[u]][b]; queue[qt++] = v; }
        }
    }
    ac_maxout = 0;
    for (int i = 0; i < ac_nodes; i++) {
        int c = __builtin_popcountll(ac_out[i]);
        if (c > ac_maxout) ac_maxout = c;
    }
    ac_ready = true;
}

static bool A272852_dfs(int depth, int state, uint64_t mask, int cnt, uint64_t val, uint64_t *res) {
    if (depth == ac_len) {
        if (cnt == ac_target) { *res = val; return true; }
        return false;
    }
    int rem = ac_len - depth;
    for (int b = 0; b < 2; b++) {
        int ns = ac_go[state][b];
        uint64_t nb = ac_out[ns] & ~mask;
        int nc = cnt + __builtin_popcountll(nb);
        if (nc > ac_target) continue;
        if (nc + ac_maxout * (rem - 1) < ac_target) continue;
        if (A272852_dfs(depth + 1, ns, mask | nb, nc, (val << 1) | (uint64_t)b, res)) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (!ac_ready) A272852_build();
    if (n == 0) return 0;
    if (n > 60) return 0;
    ac_target = (int)n;
    for (int L = 1; L <= AC_MAXL; L++) {
        if (ac_fibcnt_len[L] < ac_target) continue;
        ac_len = L;
        int s = ac_go[0][1];
        uint64_t mask = ac_out[s];
        int cnt = __builtin_popcountll(mask);
        if (cnt > ac_target) continue;
        uint64_t res = 0;
        if (A272852_dfs(1, s, mask, cnt, 1, &res)) return (seq_t)res;
    }
    return 0;
}
