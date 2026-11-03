/*
 * A375987  Expansion of e.g.f. (1 + 2 * log(1 - x))^(3/2).
 * https://oeis.org/A375987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Sum_{k=0..n} (Product_{j=0..k-1} (2*j-3)) *
 *            |Stirling1(n,k)|
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 10, i64 n <= 17, i128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == (idx_t)0)
        return (seq_t)1;

    /* allocate temporary arrays */
    size_t sz = (size_t)n + 1;
    seq_t *b = mem_new(seq_t, sz);
    if (!b)
        return (seq_t)0;
    b[0] = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t factor = (seq_t)(2 * (long long)(k - 1) - 3);
        b[k] = b[k - 1] * factor;
    }

    seq_t *prev = mem_new(seq_t, sz);
    seq_t *cur  = mem_new(seq_t, sz);
    if (!prev || !cur) {
        mem_free(b);
        if (prev) mem_free(prev);
        if (cur)  mem_free(cur);
        return (seq_t)0;
    }
    prev[0] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        cur[0] = (seq_t)0;
        for (idx_t k = 1; k <= i; ++k) {
            cur[k] = prev[k - 1] + (seq_t)(i - 1) * prev[k];
        }
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    /* Compute the final sum */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= n; ++k) {
        sum += b[k] * prev[k];
    }

    mem_free(b);
    mem_free(prev);
    mem_free(cur);
    return sum;
}
