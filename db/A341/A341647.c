/*
 * A341647  a(n) = Sum_{k=0..n} A059432(n,k).
 * https://oeis.org/A341647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} T(n,k), where T(0,0) = 1, T(i,0) = 0 for i >
 *            0, and T(i,j) = T(i,j-1) + Sum_{m=i-j..i-1} T(i-1,m) (A059432)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-10)
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
    prev[0] = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i) {
        cur[0] = (seq_t)0;
        seq_t suffix = (seq_t)0;
        for (size_t j = 1; j <= i; ++j) {
            suffix += prev[i - j];
            cur[j] = cur[j - 1] + suffix;
        }
        seq_t *t = prev;
        prev = cur;
        cur = t;
    }
    seq_t sum = (seq_t)0;
    for (size_t k = 0; k <= (size_t)n; ++k)
        sum += prev[k];
    mem_free(prev);
    mem_free(cur);
    return sum;
}
