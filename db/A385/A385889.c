/*
 * A385889  The number k such that the k-th composition in standard order is
 *          the sequence of lengths of maximal runs of binary indices of n.
 * https://oeis.org/A385889
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 510, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = index of the composition formed by the lengths of maximal
   runs of binary indices of n, in standard (graded reverse‑lexicographic)
   order */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Step 1: find the lengths of maximal runs of set bits (binary indices) */
    idx_t    runs[64];          /* at most 64 runs for a 64‑bit integer */
    idx_t    run_cnt = 0;
    idx_t    run_len = 0;
    bool     prev_set = false;

    idx_t ncopy = n;
    idx_t pos = 1;              /* positions are 1‑based, LSB is position 1 */

    while (ncopy != 0)
    {
        bool bit_set = (ncopy & 1ULL) != 0;

        if (bit_set)
        {
            if (prev_set)
                run_len++;          /* extend current run */
            else
            {
                prev_set = true;
                run_len  = 1;       /* start new run */
            }
        }
        else if (prev_set)
        {
            /* run ended at position pos-1 */
            runs[run_cnt++] = run_len;
            prev_set = false;
            run_len  = 0;
        }

        ncopy >>= 1;
        pos++;
    }
    if (prev_set)
        runs[run_cnt++] = run_len;      /* trailing run */

    /* Step 2: convert the run lengths (as a composition) to its index k.
       The mapping is: reverse the composition, take cumulative sums of
       the reversed parts, and set those bit positions of k. */
    seq_t k = 0;
    idx_t cumulative = 0;

    for (idx_t i = run_cnt; i-- > 0; )
    {
        cumulative += runs[i];
        /* set bit at position cumulative-1 (positions are 1‑based) */
        k |= (seq_t)1 << (cumulative - 1);
    }

    return k;
}
