/*
 * A355281  Number of pairs of nested Dyck paths from (0,0) to (n,n) such that
 *          the upper path only touches the diagonal at its endpoints.
 * https://oeis.org/A355281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x) = 2 - 1/B(x), B(x) = g.f. of A005700, A005700(n) =
 *            C(n)*C(n+2) - C(n+1)^2. Equivalently a(0)=1, a(n) = sum_{k=1..n}
 *            A005700(k)*a(n-k) with signs: c_0=1, c_n = -sum_{k=1..n}
 *            b_k*c_{n-k}, a(n) = -c_n (n>=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 739 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    size_t m = (size_t)n;
    seq_t *cat = mem_new(seq_t, m + 3);
    seq_t *b = mem_new(seq_t, m + 1);
    seq_t *pos = mem_new(seq_t, m + 1);  /* pos[k] = a(k) for k>=1, accumulated */
    cat[0] = 1;
    for (size_t k = 0; k < m + 2; k++) {
        cat[k + 1] = cat[k] * (seq_t)(4 * k + 2) / (seq_t)(k + 2);
    }
    for (size_t k = 0; k <= m; k++) {
        b[k] = cat[k] * cat[k + 2] - cat[k + 1] * cat[k + 1];
    }
    /* d_n = -c_n = sum_{k=1..n} b_k * c_{n-k}  with c_0=1, c_j=-d_j */
    /* d_n = b_n - sum_{k=1..n-1} b_k * d_{n-k} */
    for (size_t i = 1; i <= m; i++) {
        seq_t s = b[i];
        for (size_t k = 1; k < i; k++) {
            s -= b[k] * pos[i - k];
        }
        pos[i] = s;
    }
    seq_t res = pos[m];
    mem_free(cat);
    mem_free(b);
    mem_free(pos);
    return res;
}
