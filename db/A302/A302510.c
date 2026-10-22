/*
 * A302510  Number of n X 3 0..1 arrays with every element equal to 0, 1, 4 or
 *          5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-4), for n>7 Initial terms: a(1)=3, a(2)=3,
 *            a(3)=11, a(4)=9, a(5)=14, a(6)=21, a(7)=28
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 65, u64 n <= 134,
 *            u128 n <= 210, big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

static seq_t *buf = NULL;      /* array of computed values (0‑unused) */
static size_t sz = 0;          /* current allocated size of buf         */
static idx_t  maxCom = 0;      /* highest index for which buf[i] is valid */

seq_t a(idx_t n)
{
    /* the sequence is defined only for n >= 1 */
    if (n == 0)
        return 0;

    /* initialise the cache on the first call */
    if (buf == NULL) {
        buf = alloc_seq_array(8);   /* indices 0..7 (0 unused) */
        sz  = 8;
        maxCom = 7;
        buf[1] = 3;
        buf[2] = 3;
        buf[3] = 11;
        buf[4] = 9;
        buf[5] = 14;
        buf[6] = 21;
        buf[7] = 28;
    }

    /* if requested index already computed, return it */
    if (n <= maxCom)
        return buf[(size_t)n];

    /* ensure the buffer is large enough */
    size_t needed = (size_t)n + 1;
    if (needed > sz)
        grow_seq_array(&buf, &sz, needed);

    /* compute new values using the recurrence */
    for (idx_t i = maxCom + 1; i <= n; ++i) {
        size_t idx = (size_t)i;
        buf[idx] = buf[idx - 1] + buf[idx - 4];
    }

    maxCom = n;
    return buf[(size_t)n];
}
