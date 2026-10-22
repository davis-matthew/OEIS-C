/*
 * A321849  Expansion of e.g.f.: exp(x/(1-6*x)).
 * https://oeis.org/A321849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A321849
 * Recurrence:  a(0) = 1,  a(1) = 1
 *              a(n) = (12*n-11)*a(n-1) – 36*(n-2)*(n-1)*a(n-2)   (n ≥ 2)
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)1;

    /* Iteratively compute the recurrence up to n */
    seq_t a_prev_prev = (seq_t)1;   /* a(0)   */
    seq_t a_prev      = (seq_t)1;   /* a(1)   */

    for (idx_t i = 2U; i <= n; ++i) {
        seq_t factor1 = (seq_t)(12 * i - 11);
        seq_t factor2 = (seq_t)(36 * (i - 2U) * (i - 1U));

        seq_t term1 = factor1 * a_prev;
        seq_t term2 = factor2 * a_prev_prev;

        seq_t a_curr = term1 - term2;

        /* Shift the window for the next iteration */
        a_prev_prev = a_prev;
        a_prev      = a_curr;
    }

    return a_prev;
}
