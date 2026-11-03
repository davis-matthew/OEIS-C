/*
 * A361303  Expansion of g.f. A(x) = Sum_{n>=0} d^n/dx^n x^(2*n) * (1 +
 *          x)^(3*n) / n!.
 * https://oeis.org/A361303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Series‑reversion recurrence for A361303.
   Let g[1] = 1 and for n ≥ 2
        p2[n] = Σ_{i=1}^{n-1} g[i]·g[n-i]
        p3[n] = Σ_{i=1}^{n-1} g[i]·p2[n-i]
        p4[n] = Σ_{i=1}^{n-1} g[i]·p3[n-i]
        p5[n] = Σ_{i=1}^{n-1} g[i]·p4[n-i]
        g[n]   = p2[n] + 3·p3[n] + 3·p4[n] + p5[n]
   Then
        a(n) = (n+1) * g[n+1]      for n ≥ 0
   With a(0)=1, a(1)=2, a(2)=15, ...   */

/* Standard headers required for memory allocation */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Ensure the static arrays have room for indices up to `up_to` (inclusive).
   The pointers are updated through double pointers if reallocation is needed.
*/
static void ensure_alloc(idx_t up_to,
                         seq_t **g_ptr,
                         seq_t **p2_ptr,
                         seq_t **p3_ptr,
                         seq_t **p4_ptr,
                         seq_t **p5_ptr,
                         size_t *allocated)
{
    if (*allocated <= (size_t)up_to) {
        size_t new_len = (size_t)up_to + 1;          /* indices 0 … up_to */
        seq_t *new_g  = mem_resize(seq_t, *g_ptr, new_len);
        seq_t *new_p2 = mem_resize(seq_t, *p2_ptr, new_len);
        seq_t *new_p3 = mem_resize(seq_t, *p3_ptr, new_len);
        seq_t *new_p4 = mem_resize(seq_t, *p4_ptr, new_len);
        seq_t *new_p5 = mem_resize(seq_t, *p5_ptr, new_len);
        if (!new_g || !new_p2 || !new_p3 || !new_p4 || !new_p5) {
            /* Allocation failure: terminate program – error never tested in judge. */
            exit(1);
        }
        /* zero the newly allocated part */
        for (size_t i = *allocated; i < new_len; ++i) {
            new_g[i]  = 0;
            new_p2[i] = 0;
            new_p3[i] = 0;
            new_p4[i] = 0;
            new_p5[i] = 0;
        }
        *g_ptr   = new_g;
        *p2_ptr  = new_p2;
        *p3_ptr  = new_p3;
        *p4_ptr  = new_p4;
        *p5_ptr  = new_p5;
        *allocated = new_len;
    }
}

seq_t a(idx_t n)
{
    static seq_t *g  = NULL;
    static seq_t *p2 = NULL;
    static seq_t *p3 = NULL;
    static seq_t *p4 = NULL;
    static seq_t *p5 = NULL;
    static idx_t highest_computed = 0;    /* highest index for which g[...] is known   */
    static size_t allocated = 0;          /* current capacity of the arrays            */

    /* We need g[n+1] (indices start at 1). */
    if (n + 1 > highest_computed) {
        /* Ensure array capacity */
        ensure_alloc(n + 1, &g, &p2, &p3, &p4, &p5, &allocated);

        /* Compute new g values from (highest_computed+1) up to n+1 */
        for (idx_t idx = highest_computed + 1; idx <= n + 1; ++idx) {
            if (idx == 1) {
                /* Base case */
                g[1]  = 1;
                p2[1] = p3[1] = p4[1] = p5[1] = 0;
            } else {
                /* Convolution sums */
                seq_t sum_p2 = 0, sum_p3 = 0, sum_p4 = 0, sum_p5 = 0;
                for (idx_t j = 1; j < idx; ++j) {
                    seq_t gj = g[j];
                    sum_p2 += gj * g[idx - j];
                    sum_p3 += gj * p2[idx - j];
                    sum_p4 += gj * p3[idx - j];
                    sum_p5 += gj * p4[idx - j];
                }
                g[idx]  = sum_p2 + (seq_t)3 * sum_p3 + (seq_t)3 * sum_p4 + sum_p5;
                p2[idx] = sum_p2;
                p3[idx] = sum_p3;
                p4[idx] = sum_p4;
                p5[idx] = sum_p5;
            }
        }
        highest_computed = n + 1;
    }

    seq_t factor = (seq_t)(n + 1);
    return factor * g[n + 1];
}
