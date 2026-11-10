/*
 * A284529  Positions of 1 in A284527; complement of A284528.
 * https://oeis.org/A284529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in A284527, found by scanning
 *            A284527 and caching positions
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 8957 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t A284527_len0[256];
static seq_t A284527_len1[256];
static idx_t A284527_max_precomp = 0;
static int A284527_precomp_done = 0;

static seq_t A284527_get_symbol(seq_t pos, int sym, idx_t level)
{
    if (level == 0) return sym;
    if (sym == 0) {
        if (pos < A284527_len0[level-1]) return A284527_get_symbol(pos, 0, level-1);
        pos -= A284527_len0[level-1];
        if (pos < A284527_len0[level-1]) return A284527_get_symbol(pos, 0, level-1);
        pos -= A284527_len0[level-1];
        if (pos < A284527_len1[level-1]) return A284527_get_symbol(pos, 1, level-1);
        pos -= A284527_len1[level-1];
        return A284527_get_symbol(pos, 0, level-1);
    } else {
        if (pos < A284527_len1[level-1]) return A284527_get_symbol(pos, 1, level-1);
        pos -= A284527_len1[level-1];
        if (pos < A284527_len1[level-1]) return A284527_get_symbol(pos, 1, level-1);
        pos -= A284527_len1[level-1];
        if (pos < A284527_len0[level-1]) return A284527_get_symbol(pos, 0, level-1);
        pos -= A284527_len0[level-1];
        if (pos < A284527_len0[level-1]) return A284527_get_symbol(pos, 0, level-1);
        pos -= A284527_len0[level-1];
        if (pos < A284527_len1[level-1]) return A284527_get_symbol(pos, 1, level-1);
        pos -= A284527_len1[level-1];
        if (pos < A284527_len0[level-1]) return A284527_get_symbol(pos, 0, level-1);
        pos -= A284527_len0[level-1];
        return A284527_get_symbol(pos, 1, level-1);
    }
}

static seq_t A284527_a(idx_t n)
{
    if (n == 0) return 0;
    if (!A284527_precomp_done) {
        A284527_len0[0] = 1;
        A284527_len1[0] = 1;
        A284527_max_precomp = 0;
        A284527_precomp_done = 1;
    }
    seq_t pos = (seq_t)n - 1;
    idx_t level = 0;
    while (A284527_len1[level] <= pos) {
        if (level == A284527_max_precomp) {
            idx_t next = level + 1;
            A284527_len0[next] = 3*A284527_len0[level] + A284527_len1[level];
            A284527_len1[next] = 3*A284527_len0[level] + 4*A284527_len1[level];
            A284527_max_precomp = next;
        }
        level++;
    }
    return A284527_get_symbol(pos, 1, level);
}

static uint64_t *A284529_pos = 0;
static idx_t A284529_cnt = 0;
static idx_t A284529_cap = 0;
static idx_t A284529_next = 1;

seq_t a(idx_t n)
{
    while (A284529_cnt < n) {
        if (A284529_cnt == A284529_cap) {
            idx_t nc = A284529_cap ? A284529_cap * 2 : 1024;
            uint64_t *p = (uint64_t *)realloc(A284529_pos, (size_t)nc * sizeof(uint64_t));
            if (!p) return 0;
            A284529_pos = p;
            A284529_cap = nc;
        }
        if (A284527_a(A284529_next) == 1) {
            A284529_pos[A284529_cnt++] = (uint64_t)A284529_next;
        }
        A284529_next++;
    }
    return (seq_t)A284529_pos[n - 1];
}
