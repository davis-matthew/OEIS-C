/*
 * A304082  a(n) = 3^n - A006952(n).
 * https://oeis.org/A304082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n - A006952(n), where sum_{n>=0} A006952(n) x^n =
 *            Product_{k>=1} (1-x^k)/(1-3*x^k)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 38 terms through n = 37 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A304082_coef = NULL;
static idx_t A304082_coef_len = 0;

static void A304082_build(idx_t len) {
    seq_t *c = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) c[i] = 0;
    c[0] = 1;
    for (idx_t k = 1; k < len; k++) {
        for (idx_t i = k; i < len; i++)
            c[i] += c[i - k] * 3;
        for (idx_t i = len - 1; i >= k; i--) {
            c[i] -= c[i - k];
            if (i == k) break;
        }
    }
    if (A304082_coef != NULL) {
        for (idx_t i = 0; i < A304082_coef_len; i++) A304082_coef[i] = 0;
        mem_free(A304082_coef);
    }
    A304082_coef = c;
    A304082_coef_len = len;
}

static seq_t A006952_a(idx_t n) {
    if (n >= A304082_coef_len) {
        idx_t len = A304082_coef_len * 2;
        if (len < n + 1) len = n + 1;
        if (len < 32) len = 32;
        A304082_build(len);
    }
    return A304082_coef[n];
}

seq_t a(idx_t n) {
    seq_t p = 1;
    for (idx_t i = 0; i < n; i++) p *= 3;
    return p - A006952_a(n);
}
