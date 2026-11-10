/*
 * A352972  a(n) = Sum_{j=0..2*n} Sum_{k=0..j} A026536(j, k).
 * https://oeis.org/A352972
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..2n} Sum_{k=0..j} T(j,k), where T is the
 *            irregular triangle A026536: T(i,0)=T(i,2i)=1,
 *            T(i,1)=T(i,2i-1)=floor(i/2), and for 2<=k<=2i-2: even i:
 *            T(i,k)=T(i-1,k-2)+T(i-1,k-1)+T(i-1,k); odd i:
 *            T(i,k)=T(i-1,k-2)+T(i-1,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 485 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t rows = 2 * n;
    idx_t width = 2 * rows + 1;
    seq_t *prev = mem_new(seq_t, width);
    seq_t *cur = mem_new(seq_t, width);
    seq_t total = 0;

    for (idx_t i = 0; i <= rows; ++i) {
        idx_t len = 2 * i + 1;
        for (idx_t j = 0; j < len; ++j) cur[j] = 0;
        cur[0] = 1;
        cur[2 * i] = 1;
        if (i >= 1) {
            seq_t half = (seq_t)(i / 2);
            cur[1] = half;
            cur[2 * i - 1] = half;
        }
        if (i >= 2) {
            if ((i & 1) == 0) {
                for (idx_t j = 2; j + 2 <= 2 * i; ++j)
                    cur[j] = prev[j - 2] + prev[j - 1] + prev[j];
            } else {
                for (idx_t j = 2; j + 2 <= 2 * i; ++j)
                    cur[j] = prev[j - 2] + prev[j];
            }
        }
        for (idx_t k = 0; k <= i; ++k) total += cur[k];
        seq_t *t = prev; prev = cur; cur = t;
    }

    mem_free(prev);
    mem_free(cur);
    return total;
}
