/*
 * A347267  a(n) is the first term of the n-th 3x+1 sequence that shares
 *          infinitely many 1's with the 3x+1 sequence that starts at 1.
 * https://oeis.org/A347267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer whose Collatz total stopping
 *            time (number of steps to reach 1) is congruent to 0 modulo 3.
 *            The function collects such numbers by testing each candidate in
 *            turn and memoises the results so that calls in any order are
 *            fast
 * Range:     u8 n <= 18, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *vals = NULL;
    static size_t len = 0;    /* number of terms stored */
    static size_t cap = 0;    /* allocated capacity */
    static idx_t last_checked = 0;   /* last integer examined */

    /* extend the cache until it contains at least n terms */
    while (len < (size_t)n) {
        ++last_checked;                      /* next candidate integer */
        seq_t x = (seq_t)last_checked;       /* working copy */
        int step_mod = 0;                    /* total stopping time modulo 3 */

        /* compute total stopping time modulo 3 */
        while (x != (seq_t)1) {
            if ((x & (seq_t)1) != (seq_t)0) {   /* odd */
                x = (seq_t)3 * x + (seq_t)1;
            } else {                            /* even */
                x = x >> 1;
            }
            step_mod = (step_mod + 1) % 3;
        }

        /* if total stopping time is a multiple of 3, keep the number */
        if (step_mod == 0) {
            if (len + 1 > cap) {
                cap = cap ? cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(vals, cap * sizeof(seq_t));
                vals = tmp;                /* realloc(NULL, size) is ok */
            }
            vals[len++] = (seq_t)last_checked;
        }
    }

    return vals[(size_t)(n - 1)];
}
