/*
 * A264502  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having directed index change -1,1 0,-1 0,1 or 1,0.
 * https://oeis.org/A264502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 5*a(n-2) + 36*a(n-3) - 72*a(n-4) - 243*a(n-6)
 *            with offset 1 and initial terms a(1)=6, a(2)=38, a(3)=211,
 *            a(4)=1224, a(5)=7177, a(6)=41622, a(7)=241432
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static idx_t cap  = 0;
    static bool initialized = false;

    /* allocate and initialise the first 7 terms once */
    if (!initialized) {
        cap = 8;                     /* enough for indices 0..7 */
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        seq[1] = (seq_t)6;
        seq[2] = (seq_t)38;
        seq[3] = (seq_t)211;
        seq[4] = (seq_t)1224;
        seq[5] = (seq_t)7177;
        seq[6] = (seq_t)41622;
        seq[7] = (seq_t)241432;
        initialized = true;
    }

    /* resize storage if the requested index is outside current capacity */
    if (n + 1 > cap) {
        idx_t newcap = cap * 2;
        if (newcap < n + 1)
            newcap = n + 1;
        seq = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* return known values directly */
    if (n <= 7)
        return seq[n];

    /* compute missing terms using the recurrence */
    for (idx_t i = 8; i <= n; ++i) {
        seq[i] = (seq_t)6  * seq[i-1]
               - (seq_t)5  * seq[i-2]
               + (seq_t)36 * seq[i-3]
               - (seq_t)72 * seq[i-4]
               - (seq_t)243* seq[i-6];
    }

    return seq[n];
}
