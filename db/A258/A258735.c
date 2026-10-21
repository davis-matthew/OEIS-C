/*
 * A258735  Number of length n+5 0..3 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A258735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+5 0..3 arrays with at most one
 *            downstep in every n consecutive neighbor pairs. The function
 *            enumerates all valid sets of downstep positions, then counts the
 *            number of arrays consistent with each set
 * Range:     u8 none, u16 n <= 5, u32 n <= 71, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* ---------- Helper functions for binomial values ---------- */
static seq_t binom_nk(idx_t n, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return res;
}

/* ---------- Count of non‑decreasing sequences of length m,
   first value f, last value l (f <= l) over the alphabet {0,1,2,3}. ---------- */
static seq_t segment_count(idx_t m, int f, int l)
{
    if (m == 1)
        return (f == l) ? (seq_t)1 : (seq_t)0;

    int diff = l - f;          /* 0 <= diff <= 3 */
    idx_t n    = m - 2 + diff; /* n >= diff */
    return binom_nk(n, diff);
}

/* ---------- Recursively count ways for a fixed set of segments ---------- */
static seq_t segRec(const idx_t *seg, size_t segCnt, size_t idx, int prevLast)
{
    if (idx == segCnt)
        return (seq_t)1;

    seq_t total = 0;
    for (int f = 0; f <= 3; ++f) {
        for (int l = f; l <= 3; ++l) {
            if (idx > 0 && f >= prevLast)
                continue;
            seq_t cnt = segment_count(seg[idx], f, l);
            if (!cnt)
                continue;
            seq_t sub = segRec(seg, segCnt, idx + 1, l);
            total += cnt * sub;
        }
    }
    return total;
}

/* ---------- Count arrays for a fixed set of downstep positions ---------- */
static seq_t count_for_positions(const idx_t *pos, size_t k, idx_t len)
{
    idx_t seg[8];          /* maximum number of segments is 5 for all n */
    size_t segCnt;

    if (k == 0) {
        seg[0] = len;
        segCnt = 1;
    } else {
        seg[0] = pos[0] + 1;
        segCnt = k + 1;
        for (size_t i = 1; i < k; ++i)
            seg[i] = pos[i] - pos[i - 1];
        seg[k] = len - 1 - pos[k - 1];
    }
    return segRec(seg, segCnt, 0, -1);
}

/* ---------- Recursive enumeration of all valid downstep sets ---------- */
static void dfs(idx_t nextStart, idx_t *pos, size_t posCnt,
                idx_t len, idx_t n, seq_t *total)
{
    *total += count_for_positions(pos, posCnt, len);

    for (idx_t p = nextStart; p <= len - 2; ++p) {
        pos[posCnt] = p;
        dfs(p + n, pos, posCnt + 1, len, n, total);
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    idx_t len   = n + 5;
    seq_t total = 0;
    idx_t pos[8]; /* enough space for maximum number of downsteps */

    dfs(0, pos, 0, len, n, &total);
    return total;
}
