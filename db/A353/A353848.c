/*
 * A353848  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has all equal run-sums.
 * https://oeis.org/A353848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 55 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353848: numbers k such that the k‑th composition in standard
   order (graded reverse‑lexicographic) has all equal run‑sums.
   The code enumerates compositions and returns the n‑th valid k. */
#include <stdint.h>
#include <stdbool.h>

static bool has_equal_run_sums(uint64_t k)
{
    if (k == 0)
        return true;                 /* empty composition */

    uint64_t prev_pos = 0;           /* previous run‑difference position */
    uint64_t curr_part = 0;          /* current run part value */
    uint64_t run_len   = 0;          /* length of current run */
    uint64_t ref_sum   = 0;          /* summed value of the first run */
    bool first_run = true;

    /* iterate over bits of k from least significant to most significant */
    for (uint64_t mask = 1, t = 0; mask; mask <<= 1, ++t)
    {
        if (k & mask)
        {
            uint64_t pos  = t + 1;            /* positions are 1‑based */
            uint64_t part = pos - prev_pos;   /* difference gives a part */

            if (run_len == 0)                /* first part of composition */
            {
                curr_part = part;
                run_len   = 1;
            }
            else if (part == curr_part)      /* same run, extend */
            {
                ++run_len;
            }
            else                             /* run finished, compare sum */
            {
                uint64_t run_sum = curr_part * run_len;
                if (first_run)
                {
                    ref_sum   = run_sum;
                    first_run = false;
                }
                else if (run_sum != ref_sum)
                    return false;
                curr_part = part;
                run_len   = 1;
            }

            prev_pos = pos;
        }
    }

    /* final run (after loop) */
    if (run_len > 0)
    {
        uint64_t run_sum = curr_part * run_len;
        if (first_run)          /* composition had only one run */
            return true;
        if (run_sum != ref_sum)
            return false;
    }

    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        if (has_equal_run_sums(k))
        {
            if (count == n)
                return (seq_t)k;
            ++count;
        }
    }
}
