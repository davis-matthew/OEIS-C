/*
 * A373762  Row sums of A372208.
 * https://oeis.org/A373762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=3..n+2} T(n+2,k), where T(r,k) = T(r-1,k-1) +
 *            (r+k-1)^3*T(r-1,k), T(3,3) = 1, T(r,k) = 0 for k<3 or k>r (row
 *            sums of A372208)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *rs_sums = NULL;
static seq_t *rs_cur = NULL;
static size_t rs_done = 0;
static size_t rs_cap = 0;

static void rs_grow(size_t need)
{
    if (need <= rs_cap) return;
    size_t nc = need * 2 + 8;
    seq_t *ns = mem_new(seq_t, nc + 1);
    seq_t *nr = mem_new(seq_t, nc + 1);
    for (size_t i = 0; i <= rs_cap && rs_sums; i++) {
        ns[i] = rs_sums[i];
        nr[i] = rs_cur[i];
    }
    if (rs_sums) { mem_free(rs_sums); mem_free(rs_cur); }
    rs_sums = ns;
    rs_cur = nr;
    rs_cap = nc;
}

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    rs_grow(N);
    for (size_t m = rs_done + 1; m <= N; m++) {
        if (m == 1) {
            rs_cur[0] = 1;
        } else {
            size_t r = m + 2;
            for (size_t j = m; j-- > 0;) {
                seq_t v = 0;
                if (j >= 1) v += rs_cur[j - 1];
                if (j + 2 <= m) {
                    seq_t b = (seq_t)(r + j + 2);
                    v += b * b * b * rs_cur[j];
                }
                rs_cur[j] = v;
            }
        }
        seq_t s = 0;
        for (size_t j = 0; j < m; j++) s += rs_cur[j];
        rs_sums[m] = s;
        rs_done = m;
    }
    return rs_sums[N];
}
