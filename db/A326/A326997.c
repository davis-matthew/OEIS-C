/*
 * A326997  Antidiagonal sums of A327001.
 * https://oeis.org/A326997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A327001(n-k, k), where A327001(r,k) =
 *            Sum_{j=0..k-1} C(r*k-1, r*j)*A327001(r,j), A327001(r,0)=1 for
 *            r>=1; row 0: 1,1,2,4,8,
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-08)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* Entry (r, k) of the generalized Bell array, r >= 0 */
static seq_t A326997_entry(idx_t r, idx_t k)
{
    if (k == 0) return (seq_t)1;
    if (r == 0) {
        seq_t v = (seq_t)1;
        for (idx_t i = 1; i < k; ++i) v = v * 2;
        return v;
    }
    seq_t *buf = mem_new(seq_t, (size_t)k + 1);
    buf[0] = (seq_t)1;
    for (idx_t m = 1; m <= k; ++m) {
        seq_t sum = (seq_t)0;
        idx_t top = r * m - 1;
        for (idx_t j = 0; j < m; ++j) {
            sum += buf[j] * binomial(top, r * j);
        }
        buf[m] = sum;
    }
    seq_t res = buf[k];
    mem_free(buf);
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        total += A326997_entry(n - k, k);
    }
    return total;
}
