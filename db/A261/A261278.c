/*
 * A261278  Expansion of eta(q^3)^8 + 4 * eta(q^6)^8 in powers of q.
 * https://oeis.org/A261278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   OEIS A261278 – coefficients of η(q³)⁸ + 4·η(q⁶)⁸
 * Range:     i8 n <= 25, i16 n <= 558, i32 n <= 2500, i64 n <= 2500,
 *            i128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation evaluates the two infinite products by
   successive convolution with (1 – qᵏ)⁸, where k runs over
   multiples of 3 (for η(q³)⁸) and multiples of 6 (for η(q⁶)⁸).   */

#include <stdlib.h>
#include <stdint.h>

/* Pascal triangle values for (1 – x)⁸ */
static const seq_t C8[9] = {1, 8, 28, 56, 70, 56, 28, 8, 1};

/* Multiply f by (1 – q^k)⁸ in place, updating indices up to N. */
static void mult_by_power8(seq_t *f, idx_t N, idx_t k)
{
    for (idx_t base = k; base <= N; base += k) {
        for (idx_t j = N; j != (idx_t)-1; --j) {          /* j == UINT64_MAX after decrement from 0 */
            seq_t val = f[j];
            if (val == 0) continue;
            for (int i = 1; i <= 8; ++i) {
                idx_t idx = j + i * base;
                if (idx > N) break;
                seq_t coeff = (i & 1) ? -C8[i] : C8[i];
                f[idx] += val * coeff;
            }
        }
    }
}

/* Compute η(q³)⁸ + 4·η(q⁶)⁸ coefficient at qⁿ (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* unused index, safeguard */

    /* arrays indexed [0..n], we need up to n‑1 and n‑2 */
    seq_t *f1 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    seq_t *f2 = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!f1 || !f2) {                /* allocation failure */
        free(f1);
        free(f2);
        return 0;
    }

    f1[0] = 1;                         /* η(q³)⁸ starts with q¹ so shift by 1 later */
    f2[0] = 1;                         /* η(q⁶)⁸ starts with q² */

    /* Build f1 for k = 3, 6, 9, … */
    mult_by_power8(f1, n, 3);

    /* Build f2 for k = 6, 12, 18, … */
    mult_by_power8(f2, n, 6);

    seq_t result = f1[n - 1];
    if (n >= 2)
        result += (seq_t)4 * f2[n - 2];

    free(f1);
    free(f2);
    return result;
}
