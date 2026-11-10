/*
 * A374876  Obverse convolution (n!)**A000032; see Comments.
 * https://oeis.org/A374876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k! + L(n-k)), L = Lucas numbers A000032
 *            (L(0)=2, L(1)=1, L(n)=L(n-1)+L(n-2))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t i, k;
    seq_t *luc = mem_new(seq_t, (size_t)n + 2);
    seq_t fact = 1, prod = 1;
    luc[0] = 2;
    luc[1] = 1;
    for (i = 2; i <= n; i++)
        luc[i] = luc[i - 1] + luc[i - 2];
    for (k = 0; k <= n; k++) {
        if (k > 0)
            fact *= (seq_t)k;
        prod *= (fact + luc[n - k]);
    }
    mem_free(luc);
    return prod;
}
