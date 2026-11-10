/*
 * A339330  Sums of antidiagonals in A283681.
 * https://oeis.org/A339330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*A283681(n+1-k), where A283681(1)=1,
 *            A283681(2)=2 and, for m>2, A283681(m) = r*A283681(d+1-r) with d
 *            the least integer such that d(d+1)/2 >= m and r = m-d(d-1)/2
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t sz = (size_t)n + 3;
    seq_t *A = mem_new(seq_t, sz);
    A[1] = 1;
    A[2] = 2;
    idx_t d = 2;
    for (idx_t m = 3; m <= n; ++m) {
        while (d * (d + 1) / 2 < m) ++d;
        idx_t triPrev = d * (d - 1) / 2;
        idx_t r = m - triPrev;
        idx_t j = d + 1 - r;
        A[m] = (seq_t)r * A[j];
    }
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        sum += (seq_t)k * A[n + 1 - k];
    }
    mem_free(A);
    return sum;
}
