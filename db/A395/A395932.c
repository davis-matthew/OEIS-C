/*
 * A395932  Main diagonal of A396981.
 * https://oeis.org/A395932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A(n,n) where A(i,k) = A(i,k-f(k)) + (i+f(k)-1)*A(i-1,k),
 *            f(k)=A002024(k)=floor((sqrt(8k+1)-1)/2), A(0,k)=1, A(i,0)=0^i
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t size = n + 1;
    seq_t *table = mem_new(seq_t, (size_t)size * (size_t)size);
    idx_t *f = (idx_t *)malloc((size_t)size * sizeof(idx_t));
    if (!f) {
        mem_free(table);
        return (seq_t)0;
    }
    /* f[k] = largest m with m(m+1)/2 <= k */
    {
        idx_t m = 0;
        for (idx_t k = 0; k < size; ++k) {
            while ((m + 1) * (m + 2) / 2 <= k)
                ++m;
            f[k] = m;
        }
    }
    for (idx_t k = 0; k < size; ++k)
        table[k] = (seq_t)1;
    for (idx_t i = 1; i < size; ++i) {
        table[i * size] = (seq_t)0;
        for (idx_t k = 1; k < size; ++k) {
            idx_t m = f[k];
            seq_t t1 = table[i * size + (k - m)];
            seq_t t2 = (seq_t)(i + m - 1) * table[(i - 1) * size + k];
            table[i * size + k] = t1 + t2;
        }
    }
    seq_t result = table[n * size + n];
    free(f);
    mem_free(table);
    return result;
}
