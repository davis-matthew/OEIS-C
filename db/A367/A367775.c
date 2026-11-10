/*
 * A367775  a(n) = Sum_{k=0..n} (-1)^(n - k) * A011971(n, k).
 * https://oeis.org/A367775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k)*T(n,k), where T is Aitken's array
 *            A011971: T(0,0)=1; T(n,0)=T(n-1,n-1); T(n,k)=T(n,k-1)+T(n-1,k-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-10)
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
    size_t len = (size_t)n + 1;
    seq_t *prev = mem_new(seq_t, len);
    seq_t *cur = mem_new(seq_t, len);
    prev[0] = (seq_t)1;
    for (size_t r = 1; r <= (size_t)n; ++r) {
        cur[0] = prev[r - 1];
        for (size_t k = 1; k <= r; ++k)
            cur[k] = cur[k - 1] + prev[k - 1];
        seq_t *t = prev; prev = cur; cur = t;
    }
    seq_t pos = 0, neg = 0;
    for (size_t k = 0; k <= (size_t)n; ++k) {
        if (((size_t)n - k) % 2 == 0)
            pos += prev[k];
        else
            neg += prev[k];
    }
    seq_t res = pos - neg;
    mem_free(prev);
    mem_free(cur);
    return res;
}
