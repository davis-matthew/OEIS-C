/*
 * A392491  a(n) = Sum_{k=0..floor(n/4)} binomial(k+1,2*n-8*k+1).
 * https://oeis.org/A392491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(k+1, 2*n-8*k+1).
 *            Recurrence: a(n) = 2*a(n-4) - a(n-8) + a(n-9) (n ≥ 9). Initial
 *            values: a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=2, a(5)=0, a(6)=0,
 *            a(7)=0, a(8)=3
 * Range:     u8 n <= 41, u16 n <= 75, u32 n <= 143, u64 n <= 280,
 *            u128 n <= 551, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Base values for n = 0 .. 8 */
static const seq_t base_vals[9] = {
    (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)2,
    (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)3
};

seq_t a(idx_t n)
{
    /* Return the pre‑computed base value for n < 9. */
    if (n < 9U) {
        return base_vals[(size_t)n];
    }

    /* Allocate a temporary array of length n+1.  The array is long
       enough to hold all intermediate values needed for the
       recurrence. */
    seq_t *tmp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* In the unlikely event of a memory allocation failure,
           return zero.  The exact value is irrelevant for the
           sequence definition. */
        return (seq_t)0;
    }

    /* Initialise the first 9 values. */
    for (size_t i = 0; i < 9; ++i) {
        tmp[i] = base_vals[i];
    }

    /* Compute the sequence using the recurrence. */
    for (idx_t i = 9; i <= n; ++i) {
        tmp[i] = 2 * tmp[i - 4] - tmp[i - 8] + tmp[i - 9];
    }

    /* The desired value is the last element. */
    seq_t result = tmp[n];

    /* Free the temporary array. */
    free(tmp);

    return result;
}
