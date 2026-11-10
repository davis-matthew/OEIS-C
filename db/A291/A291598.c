/*
 * A291598  a(n) = log_2(A281130(n)).
 * https://oeis.org/A291598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = log_2(A281130(n)), where A281130(n+1) =
 *            A281130(n-A281130(n)) if A281130(n-1) < A281130(n), else
 *            2*A281130(n); A281130(1) = A281130(2) = 1. Computed on exponents
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 114 terms through n = 9380 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *A291598_cache = NULL;
static idx_t  A291598_size = 0;

static void A291598_ensure(idx_t n) {
    if (n <= A291598_size) return;
    seq_t *tmp = mem_resize(seq_t, A291598_cache, n + 1);
    if (!tmp) exit(EXIT_FAILURE);
    A291598_cache = tmp;
    for (idx_t i = A291598_size + 1; i <= n; ++i) {
        if (i <= 2) {
            A291598_cache[i] = 0;
        } else {
            seq_t e1 = A291598_cache[i - 1];
            seq_t e2 = A291598_cache[i - 2];
            if (e2 < e1) {
                unsigned sh = (unsigned)(long long)e1;
                idx_t step = (sh < 63) ? ((idx_t)1 << sh) : (idx_t)0;
                if (sh >= 63 || step >= i - 1) {
                    A291598_cache[i] = 0;
                } else {
                    A291598_cache[i] = A291598_cache[(i - 1) - step];
                }
            } else {
                A291598_cache[i] = e1 + 1;
            }
        }
    }
    A291598_size = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    A291598_ensure(n);
    return A291598_cache[n];
}
