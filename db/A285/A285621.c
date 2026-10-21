/*
 * A285621  Fixed point of the morphism 0->11, 1->1001.
 * https://oeis.org/A285621
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

/* Computation: Generate successive iterates of the morphism  
   0 → 11, 1 → 1001 until the first n symbols stabilize.  The first n
   symbols no longer change when the next iterate is produced.  This
   yields the value of the infinite fixed point at position n.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) {               /* invalid offset 0 */
        return (seq_t)0;
    }

    const size_t target = (size_t)n;

    unsigned char *prev = NULL;   /* previous iterate */
    size_t prev_len = 0;
    unsigned char *cur = NULL;    /* current iterate */
    size_t cur_len = 0;

    /* start with the seed word "0" */
    cur_len = 1;
    cur = (unsigned char *)malloc(cur_len);
    if (!cur) return (seq_t)0;   /* allocation failure */
    cur[0] = 0;

    for (;;) {
        /* check stabilization: first n symbols unchanged? */
        if (prev && cur_len >= target) {
            int equal = 1;
            for (size_t i = 0; i < target; ++i) {
                if (prev[i] != cur[i]) { equal = 0; break; }
            }
            if (equal) {
                seq_t result = (seq_t)(prev[target - 1] & 1);
                free(prev);
                free(cur);
                return result;
            }
        }

        /* advance: cur becomes prev, new cur is next iterate */
        free(prev);
        prev = cur;  prev_len = cur_len;
        cur_len = prev_len * 4;          /* maximum expansion factor */
        cur = (unsigned char *)malloc(cur_len);
        if (!cur) { free(prev); return (seq_t)0; } /* allocation failure */

        size_t idx = 0;
        for (size_t i = 0; i < prev_len; ++i) {
            unsigned char ch = prev[i];
            if (ch == 0) {               /* 0 → 11 */
                cur[idx++] = 1;
                cur[idx++] = 1;
            } else {                     /* 1 → 1001 */
                cur[idx++] = 1;
                cur[idx++] = 0;
                cur[idx++] = 0;
                cur[idx++] = 1;
            }
        }
        /* cur_len may be larger than allocated if last expansion
           not perfectly 4×; record actual length */
        cur_len = idx;
    }
}
