/*
 * A367701  Number of degree 3 vertices in the n-Menger sponge graph.
 * https://oeis.org/A367701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (24/85)*20^n + (6/5)*8^n - (432/85)*3^n + 8 Recurrence
 *            (valid for n ≥ 5): a(n) = 32*a(n-1) - 275*a(n-2) + 724*a(n-3) -
 *            480*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1)            /* index is outside the offset; behaviour not used */
        return (seq_t)0;

    /* the first four values are known */
    static const seq_t init[4] = {8, 152, 2744, 49688};
    if (n <= 4)          /* return the stored initial values directly */
        return init[n-1];

    /* compute iteratively using the recurrence */
    seq_t prev4 = init[0];   /* a(n-4) */
    seq_t prev3 = init[1];   /* a(n-3) */
    seq_t prev2 = init[2];   /* a(n-2) */
    seq_t prev1 = init[3];   /* a(n-1) */
    seq_t curr  = 0;

    for (idx_t i = 5; i <= n; ++i) {
        curr = 32*prev1 - 275*prev2 + 724*prev3 - 480*prev4;
        /* shift the window for the next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return curr;
}
