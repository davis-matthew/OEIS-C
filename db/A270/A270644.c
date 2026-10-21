/*
 * A270644  The sequence a of 1's and 2's starting with (1,2,2,2) such that
 *          a(n) is the length of the (n+2)nd run of a.
 * https://oeis.org/A270644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of A270644 by iteratively constructing the 1/2
 * sequence.  Each run has a length that is the preceding element
 * of the sequence (after the first two runs).  The first two runs
 * are fixed: length 1 (value 1) and length 3 (value 2).  The
 * function is lazy and will generate terms up to the requested
 * index using O(n) time and O(n) space. */ 
#include <stdlib.h>

static seq_t *buf = NULL;          /* buffer for the sequence */
static size_t buf_cap = 0;         /* current capacity of buf */

seq_t a(idx_t n)
{
    /* Ensure buffer has enough space for indices 0..n */
    if (n + 1 > (idx_t)buf_cap) {
        size_t newcap = (size_t)n + 5;      /* a few spare slots */
        seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
        if (!tmp)  /* allocation failed – return 0 (should not happen) */
            return (seq_t)0;
        buf = tmp;
        buf_cap = newcap;
    }

    /* Generate sequence up to index n (1‑based). */
    seq_t val = 1;            /* value of current run (1 or 2) */
    idx_t run_num = 1;        /* current run number (starts at 1) */
    idx_t run_rem = 1;        /* remaining positions in current run */

    for (idx_t idx = 1; idx <= n; ++idx) {
        buf[idx] = val;           /* store a(idx) */
        if (--run_rem == 0) {     /* run finished */
            ++run_num;
            val = (val == 1 ? 2 : 1);   /* toggle run value */

            /* determine length of the next run */
            if (run_num == 1)          /* never used */
                run_rem = 1;
            else if (run_num == 2)     /* fixed 2nd run length */
                run_rem = 3;
            else                       /* use previously computed value */
                run_rem = (idx_t)buf[run_num - 2];
        }
    }

    return buf[n];
}
