/*
 * A374850  Obverse convolution A000032**A000032; see Comments.
 * https://oeis.org/A374850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (L(k) + L(n-k)), where L = A000032
 *            (L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 14 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t *L = mem_new(seq_t, (size_t)n + 1);
    idx_t i, k;
    seq_t prod = 1;
    L[0] = 2;
    if (n >= 1)
        L[1] = 1;
    for (i = 2; i <= n; i++)
        L[i] = L[i - 1] + L[i - 2];
    for (k = 0; k <= n; k++)
        prod *= (L[k] + L[n - k]);
    mem_free(L);
    return prod;
}
