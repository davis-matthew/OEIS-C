/*
 * A374865  Obverse convolution A000045**A000032; see Comments.
 * https://oeis.org/A374865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Prod_{k=0}^{n} (F(k) + L(n-k)), F = A000045 (F(0)=0,
 *            F(1)=1), L = A000032 (L(0)=2, L(1)=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t *f = mem_new(seq_t, (size_t)n + 1);
    seq_t *l = mem_new(seq_t, (size_t)n + 1);
    idx_t i;
    seq_t prod = 1;
    f[0] = 0;
    l[0] = 2;
    if (n >= 1) {
        f[1] = 1;
        l[1] = 1;
    }
    for (i = 2; i <= n; i++) {
        f[i] = f[i - 1] + f[i - 2];
        l[i] = l[i - 1] + l[i - 2];
    }
    for (i = 0; i <= n; i++)
        prod *= (f[i] + l[n - i]);
    mem_free(f);
    mem_free(l);
    return prod;
}
