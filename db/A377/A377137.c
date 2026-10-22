/*
 * A377137  Array read by rows (blocks). Each row is a permutation of a block
 *          of consecutive numbers; the blocks are disjoint and every positive
 *          number belongs to some block. Row n contains 3n/2 elements if n is
 *          even, and (n+1)/2 elements if n is odd; ; see Comments.
 * https://oeis.org/A377137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 9940, u32 n <= 9940, u64 n <= 9940,
 *            u128 n <= 9940, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generates the terms of A377137 by constructing each row
 * as a permutation of its block of consecutive integers according to
 * the rule:
 *   • For even rows or odd rows with an even number of elements:  
 *     evens in descending order, then odds in ascending order.
 *   • For odd rows with an odd number of elements:  
 *     odds in descending order, then evens in ascending order. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t row = 1;          /* current row (1‑based)             */
    seq_t acc = 0;          /* total elements before this row    */

    /* Find the row that contains the index n.          */
    while (1) {
        seq_t L = (row & 1) ?         /* odd row   */
                 ((seq_t)row + 1) / 2 :
                 ((seq_t)row * 3) / 2;          /* even row  */
        if (acc + L >= (seq_t)n)
            break;
        acc += L;
        ++row;
    }

    seq_t L = (row & 1) ? ((seq_t)row + 1) / 2 :
                      ((seq_t)row * 3) / 2;

    seq_t pos   = (seq_t)n - acc - 1;                     /* 0‑based within row */
    seq_t start = acc + 1;                                /* first integer of block */
    seq_t end   = start + L - 1;                          /* last  integer of block */

    bool evens_first = (row % 2 == 0) || ((row % 2 == 1) && (L % 2 == 0));

    seq_t val;                                                /* result to return */

    if (evens_first) {
        /* Count evens in the block.                    */
        seq_t count_evens = end / 2 - (start - 1) / 2;
        seq_t largest_even = end & 1 ? end - 1 : end;

        if (pos < count_evens) {
            val = largest_even - 2 * pos;                    /* evens descending */
        } else {
            seq_t pos_odd = pos - count_evens;
            seq_t first_odd = start & 1 ? start : start + 1;  /* smallest odd >= start */
            val = first_odd + 2 * pos_odd;                   /* odds ascending   */
        }
    } else { /* odds first */
        seq_t count_evens = end / 2 - (start - 1) / 2;
        seq_t count_odds  = L - count_evens;
        seq_t largest_odd = end & 1 ? end : end - 1;

        if (pos < count_odds) {
            val = largest_odd - 2 * pos;                    /* odds descending  */
        } else {
            seq_t pos_even = pos - count_odds;
            seq_t first_even = start & 1 ? start + 1 : start; /* smallest even >= start */
            val = first_even + 2 * pos_even;                /* evens ascending  */
        }
    }

    return val;
}
