/*
 * A367808  a(n) = Sum_{k=0..n} A011971(n, k) * 2^(n - k).
 * https://oeis.org/A367808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A011971(n,k)*2^(n-k), where A011971 is
 *            Aitken's array: T(0,0)=1, T(n,0)=T(n-1,n-1),
 *            T(n,k)=T(n,k-1)+T(n-1,k-1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-10)
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
    size_t N = (size_t)n;
    seq_t *prev = mem_new(seq_t, N + 2);
    seq_t *cur = mem_new(seq_t, N + 2);
    prev[0] = (seq_t)1;
    for (size_t r = 1; r <= N; ++r) {
        cur[0] = prev[r - 1];
        for (size_t k = 1; k <= r; ++k)
            cur[k] = cur[k - 1] + prev[k - 1];
        seq_t *t = prev; prev = cur; cur = t;
    }
    seq_t s = 0;
    for (size_t k = 0; k <= N; ++k)
        s = s * 2 + prev[k];
    mem_free(prev);
    mem_free(cur);
    return s;
}
