/*
 * A317369  a(0) = 0, a(1) = 1; for n >= 2, a(n) = freq(a(n-s(n)),n) where s =
 *          A000196 and freq(i, j) is the number of times i appears in the
 *          terms a(0) .. a(j-1).
 * https://oeis.org/A317369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(n) = freq(a(n-s(n)), n) with s = A000196
 *            (floor(sqrt(n))); computed by simulation with memoized terms and
 *            counts
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 93 (2026-11-08)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/isqrt_u64.h"

static idx_t *A317369_terms = 0;
static idx_t *A317369_cnt = 0;
static idx_t A317369_cap = 0;
static idx_t A317369_have = 0;

seq_t a(idx_t n)
{
    if (A317369_cap < n + 2) {
        idx_t nc = (n + 2) * 2;
        idx_t *t = (idx_t *)realloc(A317369_terms, (size_t)nc * sizeof(idx_t));
        if (!t) return 0;
        A317369_terms = t;
        idx_t *c = (idx_t *)realloc(A317369_cnt, (size_t)nc * sizeof(idx_t));
        if (!c) return 0;
        A317369_cnt = c;
        memset(A317369_cnt + A317369_cap, 0, (size_t)(nc - A317369_cap) * sizeof(idx_t));
        A317369_cap = nc;
    }
    if (A317369_have == 0) {
        A317369_terms[0] = 0;
        A317369_cnt[0] = 1;
        A317369_terms[1] = 1;
        A317369_cnt[1]++;
        A317369_have = 2;
    }
    for (idx_t k = A317369_have; k <= n; k++) {
        idx_t s = (idx_t)isqrt_u64((uint64_t)k);
        idx_t v = A317369_cnt[A317369_terms[k - s]];
        A317369_terms[k] = v;
        A317369_cnt[v]++;
        A317369_have = k + 1;
    }
    return (seq_t)A317369_terms[n];
}
