/*
 * A263646  Coefficients for an expansion of the Schwarzian derivative of a
 *          power series.
 * https://oeis.org/A263646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) = 1 + position of n in the special sequence * * The special
 *            sequence S is defined by: * S[1] = 3 * S[i+1] = S[i] + ((i+2)/2)
 *            + 1 for i ≥ 1 * * For every n ≥ 1: * if n ∈ S then a(n) =
 *            position_of_n_in_S + 1 * else a(n) = 1 * * This implements the
 *            OEIS sequence A263646
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* For n = 1 or 2 the special sequence never reaches them. */
    if (n == 1 || n == 2)
        return (seq_t)1;

    /* Use idx_t for all internal calculations to avoid overflow
       for the special sequence values (which can be as large as n). */
    idx_t idx = 1;          /* current position in S (starting from 1) */
    idx_t val = 3;          /* S[1] */

    /* Advance in the special sequence until it meets or exceeds n. */
    while (val < n) {
        /* step = ((i+2)/2) + 1 where i == current idx */
        idx_t step = ((idx + 2) / (idx_t)2) + (idx_t)1;
        val += step;
        idx++;
    }

    /* If we hit the value n, return the corresponding a(n). */
    if (val == n) {
        /* a(n) = position_of_n_in_S + 1  (idx is current position) */
        return (seq_t)(idx + 1);
    }

    /* Otherwise n is not in S, so a(n) = 1. */
    return (seq_t)1;
}
