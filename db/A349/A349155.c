/*
 * A349155  Numbers k such that the k-th composition in standard order has sum
 *          equal to negative twice its reverse-alternating sum.
 * https://oeis.org/A349155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A349155
   The function enumerates the indices k such that the k‑th composition in
   standard (graded reverse‑lexicographic) order has sum equal to
   negative twice its reverse‑alternating sum.
   The implementation scans k in ascending order, applying the bijection
   between nonnegative integers and compositions. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *values = NULL;       /* stored terms a(1), a(2), … */
    static size_t cap    = 0;          /* current allocation capacity   */
    static size_t len    = 0;          /* number of terms already stored */
    static idx_t  next_k = 0;          /* next k to test                */

    if (n <= 0) return (seq_t)0; /* undefined, but safe */

    while (len < (size_t)n) {
        idx_t k = next_k;
        bool qual = false;

        if (k == 0) {
            /* For k = 0, sum = 0 and reverse‑alternating sum = 0.          */
            qual = true;
        } else {
            idx_t bits_len = 64 - __builtin_clzll(k);  /* bit length of k */
            idx_t prev  = 0;
            int64_t alt = 0;                         /* alternating sum */
            idx_t temp  = k;
            idx_t i     = 0;

            while (temp) {
                int tz   = __builtin_ctzll(temp);   /* trailing zeros */
                idx_t pos = (idx_t)tz + 1U;         /* positions start at 1 */
                idx_t gap = pos - prev;
                ++i;
                if (i & 1) alt += gap; else alt -= gap;
                prev = pos;
                temp &= temp - 1;                  /* clear lowest set bit */
            }
            if ((int64_t)bits_len == -2 * alt)
                qual = true;
        }

        if (qual) {
            if (len >= cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
                if (!new_ptr) abort();             /* memory error */
                values = new_ptr;
                cap    = new_cap;
            }
            values[len++] = (seq_t)k;
        }
        ++next_k;
    }

    return values[n - 1];
}
