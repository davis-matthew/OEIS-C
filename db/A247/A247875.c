/*
 * A247875  Numbers that are even or whose binary expansions contain one or
 *          more pairs of adjacent zeros when odd.
 * https://oeis.org/A247875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_adjacent_zeros
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: even numbers and odd numbers with at least one pair of adjacent zeros in binary representation */
#include <stdbool.h>
#include "../utils/has_adjacent_zeros.h"

/* Returns the nth term (1‑based) of the sequence.
 * The sequence contains all even integers and all odd integers whose
 * binary representation contains at least one occurrence of the substring "00". */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for n=0; return 0 for safety. */
        return (seq_t)0;
    }

    idx_t count = 0;      /* how many sequence elements have been found */
    seq_t x = (seq_t)0;   /* candidate integer to test */

    for (;;) {
        /* Check if x is in the sequence: even, or odd with adjacent zeros */
        if ((x & (seq_t)1) == (seq_t)0 || has_adjacent_zeros(x)) {
            ++count;
            if (count == n) {
                return x;
            }
        }
        ++x;   /* next integer to test */
    }
}
