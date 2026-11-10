/*
 * A389573  a(n) = Sum_{k=1..floor((n+1)/2)} A389524(n, n - 2*k + 2).
 * https://oeis.org/A389573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n+1)/2)} A389524(n, n-2*k+2), where
 *            T(1,1)=1, T(n,k) = [n>=k](1 + Sum_{i=1..k-1} T(n-i,k-1) -
 *            Sum_{i=1..n-1} T(n-i,k))
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

#define A389573_MAX_ROWS 400

static seq_t *A389573_rows[A389573_MAX_ROWS + 1];
static idx_t A389573_max_rows = 0;

static void A389573_ensure(idx_t target)
{
    while (A389573_max_rows < target) {
        ++A389573_max_rows;
        idx_t m = A389573_max_rows;
        seq_t *row = mem_new(seq_t, (m + 1));
        if (!row) abort();
        row[0] = 0;
        for (idx_t k = 1; k <= m; ++k) {
            if (m == 1 && k == 1) {
                row[1] = 1;
            } else {
                seq_t sp = 0;
                for (idx_t i = 1; i < k; ++i) {
                    idx_t r = m - i;
                    if (r >= k - 1) sp += A389573_rows[r][k - 1];
                }
                seq_t sc = 0;
                for (idx_t i = 1; i < m; ++i) {
                    idx_t r = m - i;
                    if (r >= k) sc += A389573_rows[r][k];
                }
                row[k] = (seq_t)1 + sp - sc;
            }
        }
        A389573_rows[m] = row;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    A389573_ensure(n);
    seq_t s = 0;
    idx_t K = (n + 1) / 2;
    for (idx_t k = 1; k <= K; ++k) {
        s += A389573_rows[n][n - 2 * k + 2];
    }
    return s;
}
