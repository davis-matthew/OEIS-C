/*
 * A259697  Triangle read by rows: T(n,k) = number of rook patterns on n X n
 *          board where bottom rook is in column k.
 * https://oeis.org/A259697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = 1 if k=0; T(n,k) = Sum_{i=k..n} A011971(i-1,k-1) for
 *            k>0, where A011971 is Aitken's array (a(0,0)=1,
 *            a(n,0)=a(n-1,n-1), a(n,k)=a(n,k-1)+a(n-1,k-1)); a(n) is T read
 *            by rows
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 144, u64 n <= 365,
 *            u128 n <= 980, big any n
 * Validated: standard, pass, 71 terms through n = 1258 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;
    if (k == 0)
        return (seq_t)1;

    size_t len = (size_t)r + 1;
    seq_t *prev = mem_new(seq_t, len);
    seq_t *cur = mem_new(seq_t, len);
    seq_t sum = 0;
    size_t c = (size_t)k - 1;

    /* Aitken rows j = 0 .. r-1; sum column k-1 over rows j >= k-1 */
    for (size_t j = 0; j < (size_t)r; ++j) {
        if (j == 0) {
            cur[0] = 1;
        } else {
            cur[0] = prev[j - 1];
            for (size_t i = 1; i <= j; ++i)
                cur[i] = cur[i - 1] + prev[i - 1];
        }
        if (j >= c)
            sum += cur[c];
        seq_t *t = prev; prev = cur; cur = t;
    }
    mem_free(prev);
    mem_free(cur);
    return sum;
}
