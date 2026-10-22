/*
 * A357185  Numbers k such that the k-th composition in standard order has the
 *          same length as the absolute value of its alternating sum.
 * https://oeis.org/A357185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n-th integer k such that the k-th composition in
   standard order (graded reverse-lexicographic enumeration) has
   length = |alternating sum of its parts|. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* memoised list of qualifying k-values */
    static idx_t *list = NULL;   /* array of k indices (idx_t) */
    static size_t len   = 0;      /* number of stored terms */
    static size_t cap   = 0;      /* current capacity */
    static idx_t last_k = 0;      /* next k to test */

    /* expand list until we have at least n values */
    while (len < n)
    {
        idx_t k   = last_k;          /* current k to examine */
        idx_t pos = 1;               /* bit position, 1-based (LSB==1) */
        idx_t prev = 0;              /* previous set-bit position */
        int   t    = 0;              /* number of parts in the composition */
        int64_t alt = 0;             /* alternating sum of the parts */
        int   sign = 1;              /* sign for the current part (+1 or -1) */

        idx_t tmp = k;
        while (tmp)
        {
            if (tmp & 1ULL)                  /* a '1' bit found */
            {
                idx_t diff = pos - prev;     /* length of current part   */
                alt += (int64_t)sign * (int64_t)diff;
                sign = -sign;
                prev = pos;
                ++t;
            }
            ++pos;
            tmp >>= 1ULL;
        }

        /* For k == 0 the composition is empty: t == 0 and alt == 0 */

        int64_t abs_alt = alt >= 0 ? alt : -alt;
        if ((int64_t)t == abs_alt)            /* qualify? */
        {
            if (len == cap)                   /* grow storage */
            {
                size_t newcap = cap ? cap << 1 : 16;
                idx_t *tmp_ptr = (idx_t *)realloc(list, newcap * sizeof(idx_t));
                if (!tmp_ptr) {
                    /* memory allocation failed – abort gracefully */
                    return (seq_t)0;
                }
                list = tmp_ptr;
                cap  = newcap;
            }
            list[len++] = k;
        }
        ++last_k;
    }

    return (seq_t)list[n - 1];
}
