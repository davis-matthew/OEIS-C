/*
 * A380203  With given points 0,1 on the x-axis, a(n) is the number of ways to
 *          construct n with m circles where 2^(m-1)<n<=2^m.
 * https://oeis.org/A380203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A380203: a(n) = number of ways to construct n by a chain of circles.
 *  The chain is defined by centers c[0]=2, c[1], … , c[m-1]=n
 *  where for each i (0≤i<m-1) the next center satisfies
 *  c[i+1] = 2*c[i] – L
 *  with L∈{0,1}∪{c[0], … , c[i-1]}  (and L< c[i] to keep the chain strictly increasing).
 *  The number of circles m is determined from n by 2^(m-1) < n ≤ 2^m
 *  (i.e. m = 1 + ⌊log2(n‑1)⌋ for n>1, a(1)=1).
 *
 *  This implementation evaluates a(n) by depth‑first search of all admissible
 *  chains.  The search depth is at most 63 because 2^(m-1) < n and n fits in a
 *  64‑bit unsigned integer.  For the problem range (n ≤ 10000) the search
 *  terminates well within the time limits.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Depth‑first search counting all chains of length m ending at target n.
 * centers[0…pos-1] are already fixed; prev = centers[pos-1].  This
 * function updates *cnt. */
static void dfs_chain(idx_t pos, idx_t m, idx_t n, idx_t prev,
                      idx_t *centers, seq_t *cnt)
{
    /* pruning: even if we increase by 1 in every remaining step, we
     * cannot reach n → stop */
    if (prev + (m-1 - pos) > n) return;

    /* L=0 and L=1 are always available (if < prev) */
    for (idx_t L = 0; L <= 1; ++L) {
        if (L >= prev) continue;
        /* next center */
        seq_t nxt = (seq_t)2 * (seq_t)prev - (seq_t)L;
        if (nxt > (seq_t)n) continue;
        idx_t next = (idx_t)nxt;

        if (pos == m-1) {           /* we are setting the last element */
            if (next == n) (*cnt)++;   /* a valid chain found */
            continue;
        }

        centers[pos] = next;
        dfs_chain(pos+1, m, n, next, centers, cnt);
    }

    /* Try all previous centers as L */
    for (idx_t j = 0; j + 1 < pos; ++j) {  /* j runs over 0 … pos-2 */
        idx_t L = centers[j];
        if (L >= prev) continue;
        seq_t nxt = (seq_t)2 * (seq_t)prev - (seq_t)L;
        if (nxt > (seq_t)n) continue;
        idx_t next = (idx_t)nxt;

        if (pos == m-1) {
            if (next == n) (*cnt)++;
            continue;
        }

        centers[pos] = next;
        dfs_chain(pos+1, m, n, next, centers, cnt);
    }
}

/* Helper: floor(log2(v)) for v>0 */
static inline idx_t floor_log2_u64(idx_t v)
{
    /* gcc/clang built‑in */
    return 63 - __builtin_clzll(v);
}

seq_t a(idx_t n)
{
    /* Special case: the sequence is defined with offset 1 */
    if (n == 1) return (seq_t)1;

    /* Determine m from 2^(m-1) < n ≤ 2^m */
    idx_t m = floor_log2_u64(n-1) + 1;

    /* m=1 only occurs for n==2 */
    if (m == 1) return (n == 2) ? (seq_t)1 : (seq_t)0;

    /* Prepare for DFS */
    idx_t centers[64] = {2};   /* centers[0] = 2 */
    seq_t cnt = 0;
    dfs_chain(1, m, n, 2, centers, &cnt);
    return cnt;
}
