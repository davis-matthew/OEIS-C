/*
 * A330608  T(n, k) = P(n-k, k) where P(n, x) = Sum_{k=0..n} A053121(n,
 *          k)*x^k. Triangle read by rows, for 0 <= k <= n.
 * https://oeis.org/A330608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = P(n-k,k), P(m,x) = Sum_{j=0..m} A053121(m,j)*x^j,
 *            A053121(r,j) = A053121(r-1,j-1) + A053121(r-1,j+1), A053121(0,0)
 *            = 1
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t c = n - r * (r + 1) / 2;
    idx_t m = r - c;

    seq_t *prev = mem_new(seq_t, m + 3);
    seq_t *curr = mem_new(seq_t, m + 3);
    prev[0] = 1;
    for (idx_t i = 1; i <= m; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            seq_t left = (j == 0) ? (seq_t)0 : prev[j - 1];
            seq_t right = prev[j + 1];
            curr[j] = left + right;
        }
        curr[i + 1] = 0;
        seq_t *t = prev; prev = curr; curr = t;
    }

    seq_t x = (seq_t)c;
    seq_t res = 0;
    for (idx_t j = m + 1; j-- > 0; )
        res = res * x + prev[j];

    mem_free(prev);
    mem_free(curr);
    return res;
}
