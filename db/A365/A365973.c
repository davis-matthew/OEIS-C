/*
 * A365973  Expansion of e.g.f. exp( Sum_{k>=0} x^(4*k+3) / (4*k+3) ).
 * https://oeis.org/A365973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 1; a(1) = 0; a(2) = 0;
 * for n >= 3:
 *   a(n) = (n-1)! * Σ_{k=0..⌊(n-3)/4⌋} a(n-4k-3) / (n-4k-3)!
 *
 * This implementation uses the recurrence directly.  For each n we
 * iterate over the admissible k-values.  A running product `m`
 * represents the ratio (n-1)!/(n-4k-3)! and is updated incrementally,
 * so each step performs only a few multiplications.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the first three indices explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1 || n == 2) return (seq_t)0;

    /* allocate an array to store all values up to n */
    size_t len = (size_t)n + 1;
    seq_t *arr = (seq_t *)calloc(len, sizeof(seq_t));
    if (!arr) return (seq_t)0;                 /* out‑of‑memory fallback */

    /* initialise the base values */
    arr[0] = (seq_t)1;
    arr[1] = (seq_t)0;
    arr[2] = (seq_t)0;

    /* compute the sequence up to n */
    for (idx_t i = 3; i <= n; ++i)
    {
        /* number of terms in the sum for this i */
        idx_t max_k = (i - 3) / 4;
        seq_t sum = (seq_t)0;

        /* initial multiplier: (i-1)!/(i-3)! = (i-2)*(i-1) */
        seq_t m = (seq_t)(i - 2) * (seq_t)(i - 1);

        for (idx_t k = 0; k <= max_k; ++k)
        {
            idx_t t = i - 4 * k - 3;          /* the index of a(t) */
            sum += arr[(size_t)t] * m;

            if (k < max_k)
            {
                /* update m to (i-1)!/(t-4)! by multiplying by
                   (t-3)*(t-2)*(t-1)*t  */
                seq_t a1 = (seq_t)(t - 3);
                seq_t a2 = (seq_t)(t - 2);
                seq_t a3 = (seq_t)(t - 1);
                seq_t a4 = (seq_t)(t);

                m = ((m * a1) * a2) * a3 * a4;
            }
        }

        arr[(size_t)i] = sum;
    }

    seq_t result = arr[(size_t)n];
    free(arr);
    return result;
}
