/*
 * A270645  The sequence a of 1's and 2's starting with (2,1,1,1) such that
 *          a(n) is the length of the (n+2)nd run of a.
 * https://oeis.org/A270645
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

/* Recurrence: a begins with 2,1,1,1.  
   After the first two runs, the length of the r‑th run (r≥3) equals a(r−2).  
   The sequence is the list of run values (1 or 2) produced by this rule. */
#include <stdlib.h>
#include <stddef.h>

/* External linkage required */
seq_t a(idx_t n)
{
    /* Static storage for the generated sequence */
    static seq_t *buf = NULL;        /* array of a values */
    static size_t buf_len = 0;       /* number of values stored */
    static size_t buf_cap = 0;       /* current capacity */
    static size_t run_no = 0;        /* number of runs already completed */

    /* Initialize with the first two runs: 2 (len = 1), 1 (len = 3) */
    if (buf == NULL) {
        buf_cap = 64;                       /* initial capacity */
        buf = (seq_t *)malloc(buf_cap * sizeof(seq_t));
        if (!buf) return 0;                 /* unlikely on these limits */
        /* Append run 1: value 2, length 1 */
        buf[0] = (seq_t)2;
        buf_len = 1;
        /* Append run 2: value 1, length 3 */
        buf[1] = buf[2] = buf[3] = (seq_t)1;
        buf_len = 4;
        run_no = 2;                         /* two runs completed */
    }

    /* Ensure the buffer is large enough for the requested n */
    size_t target = (size_t)n;
    while (buf_len < target) {
        /* Determine next run value: odd run → 2, even run → 1 */
        seq_t val = (run_no % 2 == 1) ? (seq_t)1 : (seq_t)2;
        /* Length of next run: a[run_no−2] (remember 0‑based index) */
        seq_t len = buf[run_no - 2];
        size_t need = (size_t)len;

        /* Grow buffer if necessary */
        if (buf_len + need > buf_cap) {
            size_t new_cap = buf_cap ? buf_cap * 2 : 8;
            while (new_cap < buf_len + need) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
            if (!tmp) return 0;              /* allocation failure */
            buf = tmp;
            buf_cap = new_cap;
        }

        /* Append the run */
        for (size_t i = 0; i < need; ++i)
            buf[buf_len++] = val;

        ++run_no;   /* one more run completed */
    }

    return buf[n - 1];
}
