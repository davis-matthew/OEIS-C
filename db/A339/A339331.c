/*
 * A339331  Sums of antidiagonals in A283682.
 * https://oeis.org/A339331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{k = 1..n} A283682(k), where A283682(1)=0,
 *            A283682(2)=1, and for m>2 with (i,j) the antidiagonal
 *            coordinates of m, A283682(m) = A283682(i) + A283682(j)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *A283682_cache = NULL;
static size_t A283682_len = 0;

static void A283682_ensure(idx_t n)
{
    if (n <= (idx_t)A283682_len)
        return;

    size_t cap = (size_t)n < 2 ? 2 : (size_t)n;
    seq_t *nc = mem_new(seq_t, cap);
    for (size_t i = 0; i < A283682_len; ++i)
        nc[i] = A283682_cache[i];
    if (A283682_cache)
        mem_free(A283682_cache);
    A283682_cache = nc;

    if (A283682_len == 0) {
        A283682_cache[0] = (seq_t)0;
        A283682_len = 1;
    }
    if (A283682_len == 1) {
        A283682_cache[1] = (seq_t)1;
        A283682_len = 2;
    }

    while (A283682_len < (size_t)n) {
        idx_t k = (idx_t)(A283682_len + 1);
        idx_t d = 1;
        while (d * (d + 1) / 2 < k)
            ++d;
        idx_t pos = k - (d - 1) * d / 2;
        idx_t i = pos;
        idx_t j = d + 1 - pos;
        A283682_cache[A283682_len] = A283682_cache[i - 1] + A283682_cache[j - 1];
        ++A283682_len;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    A283682_ensure(n);
    seq_t s = (seq_t)0;
    for (size_t k = 0; k < (size_t)n; ++k)
        s += A283682_cache[k];
    return (seq_t)2 * s;
}
