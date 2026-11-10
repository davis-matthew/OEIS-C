/*
 * A366287  Numbers k such that A163511(k) is a seventh power.
 * https://oeis.org/A366287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (a(1)=0) such that A163511(k) is a 7th power;
 *            equivalently the set bits of k at 1-indexed positions
 *            b_1<...<b_r satisfy b_i == i (mod 7) for i<r and b_r == r-1 (mod
 *            7). Found by a DFS over bit patterns, grouped by top bit
 * Range:     u8 n <= 3, u16 n <= 17, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t A366287_L;
static idx_t A366287_cnt;
static seq_t *A366287_buf;
static bool A366287_fill;

static void A366287_dfs(idx_t i, idx_t p, seq_t k)
{
    idx_t b;
    idx_t ri = i % 7;
    idx_t rf = (i + 6) % 7;
    for (b = p + 1; b <= A366287_L; b++) {
        idx_t r = b % 7;
        if (b < A366287_L) {
            if (r == ri) {
                seq_t k2 = k | ((seq_t)1 << (b - 1));
                A366287_dfs(i + 1, b, k2);
            }
        } else {
            if (r == rf) {
                if (A366287_fill) {
                    A366287_buf[A366287_cnt] = k | ((seq_t)1 << (b - 1));
                }
                A366287_cnt++;
            }
        }
    }
}

seq_t a(idx_t n)
{
    idx_t rem, L, c, j, m;
    seq_t result;
    seq_t *buf;
    if (n <= 1) return (seq_t)0;
    rem = n - 1; /* rank among nonzero terms, 1-based */
    for (L = 1; L < 100000; L++) {
        A366287_L = L;
        A366287_cnt = 0;
        A366287_fill = false;
        A366287_buf = 0;
        A366287_dfs(1, 0, (seq_t)0);
        c = A366287_cnt;
        if (rem <= c) {
            buf = mem_new(seq_t, c + 1);
            A366287_cnt = 0;
            A366287_fill = true;
            A366287_buf = buf;
            A366287_dfs(1, 0, (seq_t)0);
            /* insertion sort */
            for (j = 1; j < c; j++) {
                seq_t v = buf[j];
                m = j;
                while (m > 0 && buf[m - 1] > v) {
                    buf[m] = buf[m - 1];
                    m--;
                }
                buf[m] = v;
            }
            result = buf[rem - 1];
            mem_free(buf);
            return result;
        }
        rem -= c;
    }
    return (seq_t)0;
}
