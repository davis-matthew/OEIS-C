/*
 * A299033  a(n) = n! * [x^n] Product_{k>=1} (1 - x^k)^(n/k).
 * https://oeis.org/A299033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * [x^n] Product_{k>=1} (1 - x^k)^(n/k)
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The recurrence derived from the logarithmic derivative of the product is:
 *      a_0 = 1
 *      a_m = -n * Σ_{j=0}^{m-1} a_j * d(m-j) * (m-1)! / j!
 *  where d(k) is the divisor function.  The factor (m-1)!/j! is computed
 *  incrementally to keep intermediate values small.  All operations are
 *  performed on seq_t objects, which may be signed 64‑bit, 128‑bit or
 *  arbitrary‑precision integers; no floating point or external libraries
 *  are used.  Divisor counts are cached statically and recomputed only
 *  when a larger index is requested.
 */

#include <stdlib.h>
#include <stdint.h>

/* forward declarations of the integer types */
typedef idx_t     idx_t;   /* provided by the build */
typedef seq_t     seq_t;   /* provided by the build */

static idx_t *div_count = NULL;   /* divisor counts, indexed 1..N */
static idx_t div_max   = 0;       /* maximum N for which div_count has been filled */

seq_t a(idx_t N)
{
    /* special case n = 0 */
    if (N == 0)
        return (seq_t)1;

    /* compute divisor counts up to N if necessary */
    if (N > div_max) {
        idx_t new_max = N;
        idx_t *tmp = (idx_t *)realloc(div_count, (new_max + 1) * sizeof(idx_t));
        if (!tmp) { /* allocation failed */
            exit(EXIT_FAILURE);
        }
        div_count = tmp;
        for (idx_t i = 0; i <= new_max; ++i)
            div_count[i] = 0;
        for (idx_t i = 1; i <= new_max; ++i) {
            for (idx_t j = i; j <= new_max; j += i)
                div_count[j] += 1;
        }
        div_max = new_max;
    }

    /* allocate array for a[0..N] */
    seq_t *a_vals = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!a_vals) {
        exit(EXIT_FAILURE);
    }

    a_vals[0] = (seq_t)1;

    for (idx_t m = 1; m <= N; ++m) {
        seq_t sum   = (seq_t)0;
        seq_t ratio = (seq_t)1;   /* will hold (m-1)! / j! for current j */

        /* iterate j from m-1 down to 0 */
        for (idx_t j = m; j-- > 0;) {
            if (j < m - 1)
                ratio *= (seq_t)(j + 1);   /* ratio = ratio * (j+1) */

            seq_t term = a_vals[j] * (seq_t)div_count[m - j] * ratio;
            sum += term;
        }

        a_vals[m] = -(seq_t)N * sum;
    }

    seq_t result = a_vals[N];
    free(a_vals);
    return result;
}
