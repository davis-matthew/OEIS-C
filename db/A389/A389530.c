/*
 * A389530  Standard composition numbers of compositions whose maximal runs
 *          all belong to {(1), (2,2), (3,3,3), ...}.
 * https://oeis.org/A389530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: return the nth integer k whose binary gaps form a
 * composition in which each maximal run of equal parts has length
 * equal to that part (OEIS A389530).  The function enumerates
 * integers from 0 upward, testing the run property. */

#include <stdint.h>
#include <stdbool.h>

/* check whether integer k satisfies the run‑length condition */
static bool valid_k(seq_t k)
{
    if (k == (seq_t)0) return true;

    seq_t temp = k;
    idx_t pos = 1;          /* 1‑based bit position (LSB = 1) */
    idx_t prev_pos = 0;     /* previous set‑bit position, 0 means start */
    bool first_run = true;
    idx_t prev_gap = 0;     /* length of current run */
    idx_t run_len  = 0;     /* number of consecutive equal gaps */

    while (temp != (seq_t)0)
    {
        if ((temp & (seq_t)1) != (seq_t)0)
        {
            idx_t gap = pos - prev_pos;
            prev_pos = pos;

            if (first_run)
            {
                prev_gap = gap;
                run_len  = 1;
                first_run = false;
            }
            else
            {
                if (gap == prev_gap)
                {
                    run_len++;
                }
                else
                {
                    if (run_len != prev_gap) return false;
                    prev_gap = gap;
                    run_len  = 1;
                }
            }
        }
        temp >>= 1;
        pos++;
    }

    if (!first_run && run_len != prev_gap) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;   /* a(1) = 0 */

    idx_t found = 1;   /* k = 0 already counted */
    seq_t k = 1;

    while (true)
    {
        if (valid_k(k))
        {
            ++found;
            if (found == n) return k;
        }
        ++k;
    }
}
