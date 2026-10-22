/*
 * A334340  Decimal representation of n-th iteration of the one-dimensional
 *          cellular automaton defined by Rule 434, based on the 4-celled von
 *          Neumann neighborhood starting with a single black cell.
 * https://oeis.org/A334340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 8·a(n-1) + 16·a(n-2)
 *            − 128·a(n-3) + a(n-4) − 8·a(n-5) − 16·a(n-6) + 128·a(n-7) with
 *            initial values a(1)=1, a(2)=11, a(3)=81, a(4)=699, a(5)=5441,
 *            a(6)=43723, a(7)=349201
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial seven values (a(1) … a(7)). */
    static const seq_t init[7] = {
        (seq_t)1,
        (seq_t)11,
        (seq_t)81,
        (seq_t)699,
        (seq_t)5441,
        (seq_t)43723,
        (seq_t)349201
    };

    if (n <= 7)
        return init[n-1];

    /* Sliding window of the previous seven terms.
       arr[0] = a(n-7), arr[6] = a(n-1).  */
    seq_t arr[7];
    for (idx_t i = 0; i < 7; ++i)
        arr[i] = init[i];

    /* Compute up to the requested n. */
    for (idx_t i = 8; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = (seq_t)8  * arr[6]  /* a(n-1) */
                 + (seq_t)16 * arr[5]  /* a(n-2) */
                 + arr[3]              /* a(n-4) */
                 + (seq_t)128 * arr[0];/* a(n-7) */

        /* Negative contributions */
        seq_t neg = (seq_t)128 * arr[4]  /* a(n-3) */
                 + (seq_t)8   * arr[2]  /* a(n-5) */
                 + (seq_t)16  * arr[1]; /* a(n-6) */

        seq_t next = pos - neg;   /* next = a(n) */

        /* Shift window: drop a(n-7), move others left. */
        for (idx_t k = 0; k < 6; ++k)
            arr[k] = arr[k+1];
        arr[6] = next;
    }
    return arr[6];
}
