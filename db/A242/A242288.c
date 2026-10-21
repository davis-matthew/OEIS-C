/*
 * A242288  Greedy residue sequence of tetrahedral numbers 4, 10, 20, 35, ...
 * https://oeis.org/A242288
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 72, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Greedy residue of tetrahedral numbers.
   a(n) is obtained by subtracting, from the tetrahedral number
   T_n = n*(n+1)*(n+2)/6, the largest earlier tetrahedral number
   repeatedly until no such earlier number is ≤ the current remainder.
   The function works for any n ≥ 2; for n < 2 it returns 0. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Undefined for n < 2 – return 0 as a safe placeholder. */
    if (n < 2)
        return (seq_t)0;

    /* Target tetrahedral number T_n. */
    seq_t rem = ((seq_t)n * (n + 1) * (n + 2)) / 6;

    /* Greedy subtraction using earlier tetrahedral numbers. */
    for (idx_t j = n - 1; j > 0; --j) {
        seq_t coin = ((seq_t)j * (j + 1) * (j + 2)) / 6;
        if (coin <= rem)
            rem -= coin;
    }

    return rem;
}
