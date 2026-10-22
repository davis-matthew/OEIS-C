/*
 * A390091  Numbers whose binary indices have distinct first differences.
 * https://oeis.org/A390091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A390090: nth integer whose binary indices have distinct first differences.
   The binary indices of a number n are the positions (starting at 1) of the
   set bits in its binary representation.  n is in the sequence iff the
   differences between consecutive indices are all distinct. */

#include <stdbool.h>

/* Test whether a candidate satisfies the distinct‑differences condition. */
static int valid_candidate(seq_t value)
{
    /* A number with 0 or 1 set bit always qualifies. */
    seq_t tmp = value;
    if (tmp == (seq_t)0) return 1;
    seq_t tmp1 = tmp;
    tmp1 >>= 1;
    if (tmp1 == (seq_t)0) return 1;

    int last_pos = 0;      /* position of the last seen set bit   */
    int pos      = 1;      /* current bit position (LSB = 1)     */
    int diffs[128];        /* store previous differences          */
    int dcnt = 0;          /* number of differences stored        */

    tmp = value;
    while (tmp != (seq_t)0) {
        if ((tmp & (seq_t)1) != (seq_t)0) {          /* bit is set          */
            if (last_pos != 0) {                    /* compute difference   */
                int diff = pos - last_pos;
                /* check for duplicate differences */
                for (int j = 0; j < dcnt; ++j)
                    if (diff == diffs[j])
                        return 0;
                if (dcnt < 128)
                    diffs[dcnt++] = diff;
                else
                    /* too many differences – treat as invalid for safety */
                    return 0;
            }
            last_pos = pos;
        }
        tmp >>= 1;
        ++pos;
    }
    return 1;
}

/* Return the nth term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;     /* undefined for n<1 */

    idx_t   found = 0;
    seq_t   cand  = (seq_t)0;

    while (true) {
        if (valid_candidate(cand)) {
            ++found;
            if (found == n) return cand;
        }
        cand = cand + (seq_t)1;
    }
}
