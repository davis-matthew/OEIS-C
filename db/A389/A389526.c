/*
 * A389526  a(n) = Sum_{k >= 1} A389524(n, 2*k - 1).
 * https://oeis.org/A389526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k odd, 1<=k<=n} T(n,k), where T is the triangle
 *            A389524: T(1,1) = 1, T(n,k) = [n >= k]*(1 + Sum_{i=1..k-1}
 *            T(n-i,k-1) - Sum_{i=1..n-1} T(n-i,k))
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
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

#define A389526_MAX_ROWS 600

/* rows[r][k] holds T(r,k) for 1<=k<=r; rows[r][0] holds the sum over odd k. */
static seq_t *A389526_rows[A389526_MAX_ROWS + 1];
static idx_t A389526_count = 0;

static void A389526_ensure(idx_t target)
{
    while (A389526_count < target) {
        idx_t n = A389526_count + 1;
        seq_t *row = mem_new(seq_t, (size_t)(n + 1));
        if (!row) abort();
        seq_t odd = 0;
        for (idx_t k = 1; k <= n; ++k) {
            seq_t v;
            if (n == 1 && k == 1) {
                v = 1;
            } else {
                seq_t s1 = 0, s2 = 0;
                for (idx_t i = 1; i < k; ++i) {
                    idx_t r = n - i;           /* i < k <= n so r >= 1 */
                    if (r >= k - 1 && k >= 2) s1 += A389526_rows[r][k - 1];
                }
                for (idx_t i = 1; i < n; ++i) {
                    idx_t r = n - i;
                    if (r >= k) s2 += A389526_rows[r][k];
                }
                v = (seq_t)1 + s1 - s2;
            }
            row[k] = v;
            if (k & 1) odd += v;
        }
        row[0] = odd;
        A389526_rows[n] = row;
        A389526_count = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n > A389526_MAX_ROWS) return (seq_t)0;
    A389526_ensure(n);
    return A389526_rows[n][0];
}
