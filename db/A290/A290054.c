/*
 * A290054  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of (Sum_{j>=0} x^(j^3))^k.
 * https://oeis.org/A290054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient obtained by reading the square array *
 *            A(n,k) (number of ways to write n as a sum of k nonnegative
 *            cubes) * along its antidiagonals. For an index i the
 *            corresponding antidiagonal m * and position pos are determined
 *            by * * m = smallest integer with (m+1)(m+2)/2 > i * pos =...
 * Range:     u8 n <= 123, u16 n <= 384, u32 n <= 1338, u64 n <= 4778,
 *            u128 n <= 9869, big any n
 * Validated: standard, pass, 107 terms through n = 9739 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

static
void index_to_params(idx_t id, idx_t *n, idx_t *k)
{
    /* find m with (m+1)(m+2)/2 > id */
    idx_t lo = 0, hi = 1;
    while (1) {
        unsigned __int128 cum = (unsigned __int128)(hi + 1) * (hi + 2) / 2;
        if (cum > id) break;
        hi <<= 1;
    }
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cum = (unsigned __int128)(mid + 1) * (mid + 2) / 2;
        if (cum <= id) lo = mid + 1; else hi = mid;
    }
    idx_t m = lo;
    unsigned __int128 before = (unsigned __int128)m * (m + 1) / 2;
    idx_t pos = id - (idx_t)before;          /* 0 .. m */
    *n = pos;                                 /* n = pos               */
    *k = m - pos;                             /* k = m-pos             */
}

/* number of ordered k‑tuples of non‑negative cubes summing to n */
static
seq_t ways_cube_sum(idx_t n, idx_t k)
{
    if (k == 0) return (n == 0 ? (seq_t)1 : (seq_t)0);

    /* build list of cube values <= n */
    size_t cubes_alloc = 8;
    seq_t *cubes = mem_new(seq_t, cubes_alloc);
    if (!cubes) return (seq_t)0;
    size_t cube_cnt = 0;
    for (idx_t j = 0; ; ++j) {
        unsigned __int128 v = (unsigned __int128)j * j * j;
        if (v > (unsigned __int128)n) break;
        if (cube_cnt == cubes_alloc) {
            cubes_alloc <<= 1;
            seq_t *tmp = mem_resize(seq_t, cubes, cubes_alloc);
            if (!tmp) { mem_free(cubes); return (seq_t)0; }
            cubes = tmp;
        }
        cubes[cube_cnt++] = (seq_t)v;
    }

    size_t ns = (size_t)(n + 1);
    seq_t *dp_prev = mem_new(seq_t, ns);
    seq_t *dp_cur  = mem_new(seq_t, ns);
    if (!dp_prev || !dp_cur) {
        mem_free(cubes);
        mem_free(dp_prev);
        mem_free(dp_cur);
        return (seq_t)0;
    }
    dp_prev[0] = (seq_t)1;                /* zero elements sum to zero */

    for (idx_t t = 1; t <= k; ++t) {
        /* reset dp_cur */
        for (size_t i = 0; i < ns; ++i) dp_cur[i] = (seq_t)0;

        for (size_t ci = 0; ci < cube_cnt; ++ci) {
            size_t c = (size_t)cubes[ci];
            for (size_t s = c; s < ns; ++s) {
                dp_cur[s] += dp_prev[s - c];
            }
        }
        seq_t *tmp = dp_prev; dp_prev = dp_cur; dp_cur = tmp;
    }

    seq_t result = dp_prev[(size_t)n];
    mem_free(cubes);
    mem_free(dp_prev);
    mem_free(dp_cur);
    return result;
}

seq_t a(idx_t n)
{
    idx_t nn, kk;
    index_to_params(n, &nn, &kk);
    return ways_cube_sum(nn, kk);
}
