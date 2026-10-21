/*
 * A273337  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 657", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 127, u32 n <= 127, u64 n <= 127,
 *            u128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A273337: first differences of number of active cells in Rule 657
 * Formula: a(n) = 8*(n+1) for n>=3;
 * Initial values: a(0)=3, a(1)=13, a(2)=31
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)13;
    if (n == 2) return (seq_t)31;
    /* For n >= 3 use the closed‑form 8*(n+1) */
    return (seq_t)8 * ((seq_t)n + (seq_t)1);
}
