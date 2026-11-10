/*
 * A346919  Numbers that are both palindromes (A002113) and terms of A072389.
 * https://oeis.org/A346919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search over palindromes (A002113) in increasing order, keeping
 *            each p with p = 4*T(i)*T(j), i.e. p = u*v where u = i(i+1) and v
 *            = j(j+1) are both pronic (A072389 = 4*A085780)
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t A002113_a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    idx_t m = n - 1;
    unsigned d = 1;
    idx_t p = 1;
    for (;;) {
        unsigned h = (d + 1) / 2;
        idx_t pw = 1;
        unsigned i;
        int big = 0;
        for (i = 1; i < h; i++) {
            if (pw > UINT64_MAX / 10) { big = 1; break; }
            pw *= 10;
        }
        if (big || pw > UINT64_MAX / 9) { p = pw; break; }
        idx_t cnt = 9 * pw;
        if (m <= cnt) { p = pw; break; }
        m -= cnt;
        d++;
    }
    idx_t half = p + (m - 1);
    seq_t res = (seq_t)half;
    idx_t t = (d % 2 == 1) ? half / 10 : half;
    while (t > 0) {
        res = res * 10 + (seq_t)(t % 10);
        t /= 10;
    }
    return res;
}

static seq_t A346919_isqrt(seq_t q) {
    if (q < 2) return q;
    seq_t x = q;
    seq_t y = (x + 1) / 2;
    while (y < x) {
        x = y;
        y = (x + q / x) / 2;
    }
    return x;
}

/* p = u*v with u, v pronic numbers k(k+1), k >= 1 */
static int A346919_test(seq_t p) {
    if (p == 0) return 1;
    if (p % 4 != 0) return 0;
    seq_t i;
    for (i = 1;; i++) {
        seq_t u = i * (i + 1);
        if (u > p / u) break;
        if (p % u == 0) {
            seq_t q = p / u;
            seq_t r = A346919_isqrt(q);
            if (r * (r + 1) == q) return 1;
        }
    }
    return 0;
}

static seq_t *A346919_terms = NULL;
static idx_t A346919_len = 0;
static idx_t A346919_cap = 0;
static idx_t A346919_next = 2;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (!A346919_terms) {
        A346919_cap = 256;
        A346919_terms = mem_new(seq_t, A346919_cap);
        A346919_terms[0] = (seq_t)0;
        A346919_len = 1;
    }
    while (A346919_len < n) {
        seq_t pal = A002113_a(A346919_next);
        A346919_next++;
        if (A346919_test(pal)) {
            if (A346919_len == A346919_cap) {
                A346919_cap *= 2;
                A346919_terms = mem_resize(seq_t, A346919_terms, A346919_cap);
            }
            A346919_terms[A346919_len++] = pal;
        }
    }
    return A346919_terms[n - 1];
}
