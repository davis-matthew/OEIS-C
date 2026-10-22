/*
 * A386709  Number of length n binary palindromic words which are not all zero
 *          in which 1's occur in blocks of at least 4.
 * https://oeis.org/A386709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + b(n-4), where b(0)=1 and b(k)=b(k-2)+b(k-5) for
 *            k≥5. The sequence counts palindromic binary words of length n
 *            with 1's in blocks of at least 4, excluding the all‑zero word
 * Range:     u8 n <= 27, u16 n <= 53, u32 n <= 105, u64 n <= 210,
 *            u128 n <= 419, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 4) {
        return 0;
    }

    /* Length of array holding b(k) for k=0..n-4. */
    size_t len = (size_t)(n - 4 + 1);
    seq_t *b = (seq_t *)malloc(len * sizeof(seq_t));
    if (!b) {
        return 0;          /* allocation failure – not expected for n≤10000 */
    }

    /* Initial values for b(k). */
    b[0] = 1;
    if (len > 1) {
        b[1] = 0;
    }
    if (len > 2) {
        b[2] = 1;
    }

    /* Compute remaining b(k) using the recurrence. */
    for (size_t k = 3; k < len; ++k) {
        seq_t v1 = b[k - 2];
        seq_t v2 = (k >= 5) ? b[k - 5] : 0;
        b[k] = v1 + v2;
    }

    /* Compute a(n) iteratively. a(1..3)=0. */
    seq_t a_prev = 0;   /* corresponds to a(3) after the loop below. */
    seq_t a_curr = 0;

    for (idx_t i = 1; i <= n; ++i) {
        if (i < 4) {
            a_curr = 0;
        } else {
            a_curr = a_prev + b[i - 4];
        }
        if (i == n) {
            free(b);
            return a_curr;
        }
        a_prev = a_curr;
    }

    free(b);           /* unreachable, but keeps the compiler happy */
    return 0;
}
