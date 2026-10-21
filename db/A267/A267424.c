/*
 * A267424  Fibonacci numbers that are not of the form x^2 + y^2 + z^2 where
 *          x, y and z are integers.
 * https://oeis.org/A267424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth Fibonacci number that is NOT representable as a sum of three squares
   (i.e., F_k such that after removing all factors of 4, the remainder ≡ 7 (mod 8)). */

#include <stdlib.h>          /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Static state to allow memoisation across calls. */
    static seq_t *selected   = NULL;   /* array of Fibonacci numbers that satisfy the condition */
    static size_t sel_len    = 0;       /* number of stored terms */
    static size_t sel_cap    = 0;       /* current allocation size */

    static seq_t f_prev = 0;   /* F_{k-1} */
    static seq_t f_curr = 1;   /* F_k (currently at k = 1) */

    /* Generate Fibonacci numbers until we have at least n qualifying terms. */
    while (sel_len < n) {
        /* Test current Fibonacci number f_curr. */
        seq_t t = f_curr;
        while ((t % 4) == 0) t /= 4;
        if ((t % 8) == 7) {
            if (sel_len >= sel_cap) {
                size_t new_cap = sel_cap ? 2 * sel_cap : 4;
                seq_t *tmp = (seq_t *)realloc(selected, new_cap * sizeof(seq_t));
                if (!tmp) abort();            /* allocation failure */
                selected = tmp;
                sel_cap   = new_cap;
            }
            selected[sel_len++] = f_curr;
        }

        /* Advance to the next Fibonacci number. */
        seq_t next = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = next;
    }

    return selected[n - 1];
}
