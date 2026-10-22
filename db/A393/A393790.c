/*
 * A393790  Expansion of e.g.f. (exp(exp(2*x) - 1) + 1) / 2.
 * https://oeis.org/A393790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 2^(n-1) * Bell(n) for n > 0 where Bell(n) is
 *            the nth Bell number
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array for Bell numbers up to n.
       The array is POD, so malloc is acceptable in both C and C++. */
    seq_t *Bell = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!Bell)
        return (seq_t)0;   /* allocation failed – return zero */

    Bell[0] = (seq_t)1;   /* Bell(0) = 1 */

    /* Compute Bell[1] .. Bell[n] using the recurrence
       B(k+1) = Σ_{i=0..k} C(k,i) * B(i). */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t sum = 0;
        seq_t binom = 1;          /* C(k-1,0) */

        for (idx_t i = 0; i < k; ++i) {
            sum += binom * Bell[i];

            /* Update binomial: C(k-1,i+1) = C(k-1,i) * (k-1-i)/(i+1) */
            if (i < k - 1)
                binom = (binom * (seq_t)(k - 1 - i)) / (seq_t)(i + 1);
        }

        Bell[k] = sum;
    }

    /* Compute 2^(n-1) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow2 *= (seq_t)2;

    seq_t result = pow2 * Bell[n];

    free(Bell);
    return result;
}
