/*
 * A350554  a(1) = 0; for n>0, a(n) is the smallest k in A350552 which is not
 *          yet a term and is such that k + a(n-1) is in A350552.
 * https://oeis.org/A350554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 0; for n>1, a(n) = smallest k in A350552, not already a
 *            term, with k + a(n-1) in A350552 (greedy search; A350552:
 *            popcount(k) = popcount(k'))
 * Range:     u8 n <= 17, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static unsigned A350554_pop(seq_t x) {
    unsigned c = 0;
    while (x > 0) {
        if ((x & 1) != 0) c++;
        x >>= 1;
    }
    return c;
}

static seq_t A350554_deriv(seq_t k) {
    if (k <= 1) return 0;
    seq_t m = k, r = 0;
    for (seq_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            seq_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            r += (k / p) * e;
        }
    }
    if (m > 1) r += k / m;
    return r;
}

static bool A350554_in(seq_t k) {
    if (k == 0) return true;
    return A350554_pop(k) == A350554_pop(A350554_deriv(k));
}

static seq_t *A350554_cache = NULL;
static size_t A350554_sz = 0, A350554_cap = 0;

static void A350554_push(seq_t v) {
    if (A350554_sz == A350554_cap) {
        size_t nc = A350554_cap ? A350554_cap * 2 : 64;
        A350554_cache = mem_resize(seq_t, A350554_cache, nc);
        A350554_cap = nc;
    }
    A350554_cache[A350554_sz++] = v;
}

seq_t a(idx_t n) {
    if (A350554_sz == 0) A350554_push((seq_t)0);
    while ((idx_t)A350554_sz < n) {
        seq_t prev = A350554_cache[A350554_sz - 1];
        seq_t k = 1;
        for (;; k++) {
            bool used = false;
            for (size_t i = 0; i < A350554_sz; i++) {
                if (A350554_cache[i] == k) { used = true; break; }
            }
            if (used) continue;
            if (A350554_in(k) && A350554_in(k + prev)) break;
        }
        A350554_push(k);
    }
    return A350554_cache[n - 1];
}
