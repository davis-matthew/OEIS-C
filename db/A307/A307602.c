/*
 * A307602  Expansion of Product_{k>=1} (1 + x^k/(1 + x)).
 * https://oeis.org/A307602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} (1 + x^k/(1+x))
 * Range:     i8 n <= 16, i16 n <= 37, i32 n <= 93, i64 n <= 240,
 *            i128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    size_t N = (size_t)n;
    seq_t *old = (seq_t*)calloc(N + 1, sizeof(seq_t));
    seq_t *newA = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!old || !newA) { free(old); free(newA); return (seq_t)0; }

    old[0] = (seq_t)1;                /* a(0) = 1 */

    for (size_t k = 1; k <= N; ++k)
    {
        /* start newA as a copy of old (current coefficients before factor k) */
        for (size_t i = 0; i <= N; ++i) newA[i] = old[i];

        /* alternating prefix sum of old[0 .. N-k] */
        seq_t cum = old[0];
        size_t limit = N - k;          /* maximum i for which n = k+i <= N */

        for (size_t i = 0; i <= limit; ++i)
        {
            size_t npos = k + i;
            seq_t sign = ((i & 1) == 0) ? (seq_t)1 : (seq_t)-1;
            newA[npos] += sign * cum;

            /* update cum for next i (i -> i+1) */
            if (i < limit)
            {
                seq_t val = old[i + 1];
                cum += (( (i + 1) & 1) == 0) ? val : -val;
            }
        }

        /* prepare for next factor: swap old and newA */
        seq_t *tmp  = old;
        old = newA;
        newA = tmp;
    }

    seq_t result = old[N];
    free(old);
    free(newA);
    return result;
}
