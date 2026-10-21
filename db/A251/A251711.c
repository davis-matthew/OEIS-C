/*
 * A251711  7-step Fibonacci sequence starting with (0,0,0,0,1,0,0).
 * https://oeis.org/A251711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+7) = a(n)+a(n+1)+a(n+2)+a(n+3)+a(n+4)+a(n+5)+a(n+6) initial
 *            values: a(0)=0, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=0, a(6)=0
 * Range:     u8 n <= 15, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>          /* for size_t */
#include "../utils/seq_alloc.h"   /* seq_alloc(size_t) -> seq_t* */
#include "../utils/seq_realloc.h" /* seq_realloc(seq_t*, size_t, size_t) -> seq_t* */

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* cache of computed values          */
    static size_t cap   = 0;    /* current allocated capacity        */
    static idx_t  len   = 0;    /* number of values stored (len = last index + 1) */

    /* initialise cache with the first seven terms */
    if (buf == NULL) {
        cap = 8;                                      /* at least 8 for growth */
        buf = seq_alloc(cap);
        buf[0] = (seq_t)0;
        buf[1] = (seq_t)0;
        buf[2] = (seq_t)0;
        buf[3] = (seq_t)0;
        buf[4] = (seq_t)1;
        buf[5] = (seq_t)0;
        buf[6] = (seq_t)0;
        len   = 7;                                     /* next free index */
    }

    /* if the value has already been computed, return it */
    if (n < len)
        return buf[n];

    /* grow the buffer if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap << 1;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        buf = seq_realloc(buf, cap, new_cap);
        cap = new_cap;
    }

    /* compute missing terms up to index n */
    for (idx_t i = len; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 1; k <= 7; ++k)
            sum += buf[i - k];
        buf[i] = sum;
    }
    len = n + 1;

    return buf[n];
}
