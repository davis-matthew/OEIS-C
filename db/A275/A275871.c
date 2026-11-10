/*
 * A275871  Row sums and second diagonal of A046934.
 * https://oeis.org/A275871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} T(n,k) where T is A046934: T(0,0)=1,
 *            T(1,0)=0, T(n,0)=T(n-1,n-1) for n>=2, T(n,k)=T(n,k-1)+T(n-1,k-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t len = (size_t)n + 1;
    seq_t *prev = mem_new(seq_t, len);
    seq_t *cur = mem_new(seq_t, len);
    prev[0] = 1;
    for (size_t r = 1; r <= (size_t)n; ++r) {
        cur[0] = (r == 1) ? (seq_t)0 : prev[r - 1];
        for (size_t k = 1; k <= r; ++k)
            cur[k] = cur[k - 1] + prev[k - 1];
        seq_t *t = prev; prev = cur; cur = t;
    }
    seq_t sum = 0;
    for (size_t k = 0; k <= (size_t)n; ++k)
        sum += prev[k];
    mem_free(prev);
    mem_free(cur);
    return sum;
}
