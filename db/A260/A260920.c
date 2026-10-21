/*
 * A260920  Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00010101 00101011 or 01010101.
 * https://oeis.org/A260920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 35, u64 n <= 77, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A260920: Number of (n+2) X (1+2) 0..1 arrays with each 3 X 3 subblock having
   clockwise perimeter pattern 00010101, 00101011 or 01010101.
   Empirical recurrence:
     a(n) = a(n-2) + 3*a(n-4) + 2*a(n-5) + 2*a(n-6) + 3*a(n-7) + a(n-8)  for n > 10
   initial values (n = 1 … 10):
     36, 48, 104, 186, 297, 491, 883, 1489, 2534, 4289
   This implementation memoises the computed terms so any order of calls
   is supported.  O(n) time and O(n) space.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined, return 0 */

    /* static memoisation storage */
    static seq_t *buf = NULL;                /* array of a(0)…a(maxComputed) */
    static idx_t bufferSize = 0;              /* allocated size of buf     */
    static idx_t maxComputed = 0;             /* highest index computed   */

    /* Initialise buffer on first call or enlarge if necessary */
    if (buf == NULL) {
        /* at least store terms up to 10 */
        bufferSize = (n > 10 ? n : 10) + 1;
        buf = (seq_t *)malloc(sizeof(seq_t) * bufferSize);
        if (!buf)
            return 0;                        /* allocation failure */

        /* initial values 1 … 10 */
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)36;   buf[2] = (seq_t)48;
        buf[3] = (seq_t)104;  buf[4] = (seq_t)186;
        buf[5] = (seq_t)297;  buf[6] = (seq_t)491;
        buf[7] = (seq_t)883;  buf[8] = (seq_t)1489;
        buf[9] = (seq_t)2534; buf[10] = (seq_t)4289;

        maxComputed = 10;
    }
    else if (n + 1 > bufferSize) {
        /* grow buffer to accommodate index n */
        idx_t oldSize = bufferSize;
        bufferSize = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, sizeof(seq_t) * bufferSize);
        if (!tmp)
            return 0;                        /* allocation failure */
        buf = tmp;
        /* zero‑initialise new slots */
        for (idx_t i = oldSize; i < bufferSize; ++i)
            buf[i] = (seq_t)0;
        /* keep computed content unchanged */
    }

    /* Compute missing terms up to n */
    if (n > maxComputed) {
        for (idx_t i = maxComputed + 1; i <= n; ++i) {
            if (i <= 10)          /* already initialised */
                continue;
            /* recurrence formula */
            buf[i] = buf[i-2]
                   + (seq_t)3 * buf[i-4]
                   + (seq_t)2 * buf[i-5]
                   + (seq_t)2 * buf[i-6]
                   + (seq_t)3 * buf[i-7]
                   + buf[i-8];
        }
        maxComputed = n;
    }

    return buf[n];
}
