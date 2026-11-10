/*
 * A282427  Numbers k such that A282442(k) = ceiling(k/2) + 1.
 * https://oeis.org/A282427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = n-th k such that A282442(k) = ceiling(k/2)+1; found by
 *            direct search (simulating the staircase walk for odd k, since
 *            all terms are odd)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static idx_t A282442_a(idx_t n) {
    bool *used = (bool *)calloc((size_t)(n + 2), sizeof(bool));
    if (!used) return 0;
    idx_t pos = 0;
    for (idx_t k = 1; k <= n + 1; ++k) {
        bool stepped = false;
        if (k & 1U) {
            while (pos + k <= n) { pos += k; stepped = true; }
        } else {
            while (pos >= k) { pos -= k; stepped = true; }
        }
        if (stepped) used[k] = true;
    }
    idx_t ans = 1;
    while (ans <= n + 1 && used[ans]) ++ans;
    free(used);
    return ans;
}

static idx_t *A282427_terms = NULL;
static size_t A282427_cnt = 0, A282427_cap = 0;
static idx_t A282427_next = 1; /* next odd k to test */

seq_t a(idx_t n) {
    while (A282427_cnt < (size_t)n) {
        idx_t k = A282427_next;
        A282427_next += 2;
        if (A282442_a(k) == (k + 1) / 2 + 1) {
            if (A282427_cnt == A282427_cap) {
                size_t nc = A282427_cap ? A282427_cap * 2 : 64;
                idx_t *p = (idx_t *)realloc(A282427_terms, nc * sizeof(idx_t));
                if (!p) return (seq_t)0;
                A282427_terms = p;
                A282427_cap = nc;
            }
            A282427_terms[A282427_cnt++] = k;
        }
    }
    return (seq_t)A282427_terms[n - 1];
}
