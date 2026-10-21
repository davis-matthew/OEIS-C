/*
 * A246837  Expansion of phi(x) * psi(x) * psi(x^4) in powers of x where
 *          phi(), psi() are Ramanujan theta functions.
 * https://oeis.org/A246837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in φ(q)ψ(q)ψ(q^4) Computation: let T_a
 *            = a(a+1)/2 and let H(m) = #{(a,b) : m = T_a + 4T_b}. Then a(n) =
 *            Σ_{k^2≤n} w_k · H(n−k²) (w_0=1, w_k=2 for k>0)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"   /* returns seq_t* of new array */
#include "../utils/seq_free_array.h"        /* frees an array allocated by seq_alloc_* */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* ---------- triangular numbers up to n ---------- */
    idx_t tri_cap = 16;
    idx_t *tri_nums = (idx_t *)malloc(tri_cap * sizeof(idx_t));
    if (!tri_nums) return 0;           /* allocation failure, return 0 as fallback */
    size_t tri_cnt = 0;
    for (idx_t k = 0;; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > n) break;
        if (tri_cnt == tri_cap) {
            tri_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(tri_nums, tri_cap * sizeof(idx_t));
            if (!tmp) { free(tri_nums); return 0; }
            tri_nums = tmp;
        }
        tri_nums[tri_cnt++] = t;
    }

    /* ---------- H[0..n] = #pairs (a,b) with T_a + 4T_b = r ---------- */
    seq_t *H = seq_alloc_new_array(n + 1);  /* seq_alloc_new_array performs construction */
    for (idx_t i = 0; i <= n; ++i) H[i] = 0;

    for (size_t ti = 0; ti < tri_cnt; ++ti) {
        idx_t t = tri_nums[ti];
        for (size_t si = 0; si < tri_cnt; ++si) {
            idx_t s = tri_nums[si];
            idx_t idx = t + 4 * s;
            if (idx > n) break;           /* inner loop can break early */
            H[idx] += 1;
        }
    }

    /* ---------- compute a(n) via squares ---------- */
    seq_t sum = 0;
    for (idx_t k = 0;; ++k) {
        idx_t sq = k * k;
        if (sq > n) break;
        seq_t weight = (k == 0) ? 1 : 2;
        sum += weight * H[n - sq];
    }

    /* ---------- clean up ---------- */
    free(tri_nums);
    seq_free_array(H);

    return sum;
}
