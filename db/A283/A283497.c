/*
 * A283497  Remainder when n is divided by A004074(n), or -1 if A004074(n) =
 *          0.
 * https://oeis.org/A283497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n mod A004074(n) if A004074(n) > 0, else -1; A004074(n) =
 *            2*A004001(n) - n, A004001(n) = A004001(A004001(n-1)) +
 *            A004001(n-A004001(n-1)), A004001(1) = A004001(2) = 1
 * Range:     i8 n <= 484, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 104 terms through n = 8641 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t *A283497_h_cache = NULL;
static idx_t A283497_h_len = 0;
static idx_t A283497_h_cap = 0;

static void A283497_ensure_h(idx_t n) {
    if (n < 2) n = 2;
    if (n <= A283497_h_len) return;
    if (n > A283497_h_cap) {
        idx_t new_cap = (n < 1024) ? 1024 : n + n / 2;
        uint64_t *p = (uint64_t *)realloc(A283497_h_cache, (size_t)new_cap * sizeof(uint64_t));
        if (p == NULL) abort();
        A283497_h_cache = p;
        A283497_h_cap = new_cap;
    }
    if (A283497_h_len == 0) {
        A283497_h_cache[0] = 1;
        A283497_h_cache[1] = 1;
        A283497_h_len = 2;
    }
    for (idx_t i = A283497_h_len + 1; i <= n; i++) {
        uint64_t prev = A283497_h_cache[i - 2];
        uint64_t v = A283497_h_cache[prev - 1] + A283497_h_cache[(i - prev) - 1];
        A283497_h_cache[i - 1] = v;
    }
    if (n > A283497_h_len) A283497_h_len = n;
}

seq_t a(idx_t n) {
    A283497_ensure_h(n);
    seq_t h = (seq_t)A283497_h_cache[n - 1];
    seq_t d = h + h - (seq_t)n;
    if (d == 0) return (seq_t)(-1);
    return (seq_t)n % d;
}
