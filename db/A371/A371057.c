/*
 * A371057  Ternary numbers consisting of a run of 2's, then a run of 0's,
 *          then a run of 1's.
 * https://oeis.org/A371057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th ternary number consisting of a run of 2, a run
 *            of 0, and a run of 1 (each run has positive length). The digits
 *            are interpreted as decimal digits. The terms are listed in
 *            ascending numeric order
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Find the length L of the term.  For a given L the number of
       triples (a,b,c) with a,b,c≥1 and a+b+c=L equals
       C(L‑1,2) = (L‑1)(L‑2)/2.  Subtract these counts until the
       index falls inside the current length. */
    idx_t L = 3;
    while (1) {
        idx_t cnt = (L - 1) * (L - 2) / 2;   /* number of terms of length L */
        if (n <= cnt)
            break;
        n -= cnt;
        ++L;
    }

    /* Zero‑based position within this length block */
    idx_t offset = n - 1;

    /* Determine the run length of 2's (a).  For each a the number of
       possible b's is L‑a‑1. */
    idx_t a_len = 1;
    while (1) {
        idx_t group_size = L - a_len - 1;  /* count of b's for this a */
        if (offset < group_size)
            break;
        offset -= group_size;
        ++a_len;
    }

    /* For fixed a, larger b yields a smaller number, so enumerate
       b in decreasing order.  offset runs from 0 to group_size‑1. */
    idx_t b_len = (L - a_len - 1) - offset;
    idx_t c_len = L - a_len - b_len;  /* the remaining zeros for 1's */

    /* Build the decimal integer from the digit runs. */
    seq_t value = 0;
    for (idx_t i = 0; i < a_len; ++i)
        value = value * 10 + 2;
    for (idx_t i = 0; i < b_len; ++i)
        value = value * 10 + 0;
    for (idx_t i = 0; i < c_len; ++i)
        value = value * 10 + 1;

    return value;
}
