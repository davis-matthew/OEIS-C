/*
 * A325665  First term of n-th difference sequence of (floor(k*r)), r =
 *          -sqrt(2), k >= 0.
 * https://oeis.org/A325665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = first term of the n‑th difference sequence of floor(k *
 *            (-sqrt(2))) for k >= 0. (i.e. a(n) = Δⁿ f₀ where f_k =
 *            ⌊k*(-√2)⌋, Δf_k = f_{k+1}-f_k)
 * Range:     i8 n <= 12, i16 n <= 17, i32 n <= 37, i64 n <= 73, i128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* not part of the sequence, but handle gracefully */
        return 0;

    /* Allocate array to hold the sequence f_k (k = 0..n) and
       to compute successive differences in place. */
    seq_t *cur = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!cur) return 0;               /* allocation failed */

    /* --- compute f_k = floor(k*(-sqrt(2))) for k=0..n --- */

    cur[0] = 0;                   /* f_0 = 0 */
    seq_t s = 0;                  /* s_k = floor(k*sqrt(2)) */

    for (idx_t k = 0; k < n; ++k) {
        /* compute s_{k+1} from s_k */
        unsigned long long left  = 2ULL * (k + 1) * (k + 1);          /* 2*(k+1)^2   */
        seq_t      temp  = s + 2;                                    /* s_k + 2     */
        unsigned long long right = (unsigned long long)temp *
                                   (unsigned long long)temp;        /* (s_k+2)^2   */

        seq_t s_next = (left < right) ? (s + 1) : temp;
        s = s_next;

        /* f_{k+1} = -(s_{k+1} + 1)   (for k+1 > 0) */
        cur[k + 1] = -(s + 1);
    }

    /* --- compute successive differences in place --- */
    for (idx_t level = 1; level <= n; ++level) {
        for (idx_t j = 0; j + 1 < (n + 1 - (level - 1)); ++j) {
            cur[j] = cur[j + 1] - cur[j];
        }
        if (level == n)
            break;   /* cur[0] is the desired a(n) */
    }

    seq_t result = cur[0];
    free(cur);
    return result;
}
