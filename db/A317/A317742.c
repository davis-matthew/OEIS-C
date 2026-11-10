/*
 * A317742  Let b(1) = b(2) = 1; for n >= 3, b(n) = b(t(n)) + b(n-t(n)) where
 *          t = A287422. a(n) = 2*b(n) - n.
 * https://oeis.org/A317742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   b(1) = b(2) = 1; b(n) = b(t(n)) + b(n-t(n)) for n >= 3, where t
 *            = A287422: t(1) = t(2) = 1, t(n) = n - t(t(n-1)) - t(n-t(n-1));
 *            a(n) = 2*b(n) - n
 * Range:     u8 n <= 4815, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 122 terms through n = 8772 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static uint64_t *A317742_t = NULL;
static seq_t *A317742_b = NULL;
static idx_t A317742_len = 0;
static idx_t A317742_cap = 0;

static void A317742_ensure(idx_t n)
{
    if (n < 2) n = 2;
    if (n <= A317742_len) return;
    if (n > A317742_cap) {
        idx_t new_cap = (n < 1024) ? 1024 : n + n / 2;
        uint64_t *nt = (uint64_t *)malloc((size_t)(new_cap + 1) * sizeof(uint64_t));
        if (!nt) abort();
        seq_t *nb = mem_new(seq_t, (size_t)(new_cap + 1));
        if (!nb) abort();
        for (idx_t i = 0; i <= A317742_len; i++) {
            nt[i] = A317742_t ? A317742_t[i] : 0;
            nb[i] = A317742_b ? A317742_b[i] : (seq_t)0;
        }
        free(A317742_t);
        mem_free(A317742_b);
        A317742_t = nt;
        A317742_b = nb;
        A317742_cap = new_cap;
    }
    if (A317742_len == 0) {
        A317742_t[1] = 1;
        A317742_t[2] = 1;
        A317742_b[1] = (seq_t)1;
        A317742_b[2] = (seq_t)1;
        A317742_len = 2;
    }
    for (idx_t i = A317742_len + 1; i <= n; i++) {
        uint64_t p = A317742_t[i - 1];
        uint64_t ti = (uint64_t)i - A317742_t[p] - A317742_t[i - p];
        A317742_t[i] = ti;
        A317742_b[i] = A317742_b[ti] + A317742_b[i - ti];
    }
    A317742_len = n;
}

seq_t a(idx_t n)
{
    A317742_ensure(n);
    seq_t b = A317742_b[n];
    return b + b - (seq_t)n;
}
