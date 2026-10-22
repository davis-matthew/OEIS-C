/*
 * A324141  Define b(n) and c(n) by b(0)=0, b(1)=c(0)=c(1)=1;
 *          b(n)=c(n-1)*c(n-2), c(n) = (b(n-1)+c(n-1))*(b(n-2)+c(n-2));
 *          sequence gives c(n).
 * https://oeis.org/A324141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   b(0)=0, b(1)=1, c(0)=c(1)=1;
   b(n) = c(n-1)*c(n-2);
   c(n) = (b(n-1)+c(n-1))*(b(n-2)+c(n-2));
   a(n) = c(n)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static storage for the computed values. */
    static seq_t *b_arr = NULL;
    static seq_t *c_arr = NULL;
    static size_t cap   = 0;
    static idx_t  max_comp = 0;   /* highest index computed so far */

    /* Initialise on first call.  We need room for indices 0 and 1. */
    if (c_arr == NULL) {
        cap = 2;
        b_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        c_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!b_arr || !c_arr) { /* allocation failure – abort */
            return (seq_t)0;
        }
        /* Base values */
        b_arr[0] = (seq_t)0;
        b_arr[1] = (seq_t)1;
        c_arr[0] = (seq_t)1;
        c_arr[1] = (seq_t)1;
        max_comp = 1;
    }

    /* Return if already computed. */
    if (n <= max_comp) return c_arr[n];

    /* Ensure capacity for indices up to n. */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? (cap << 1) : 2;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp_c = (seq_t *)realloc(c_arr, new_cap * sizeof(seq_t));
        seq_t *tmp_b = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        if (!tmp_c || !tmp_b) { /* allocation failure – abort */
            return (seq_t)0;
        }
        c_arr = tmp_c;
        b_arr = tmp_b;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    for (idx_t i = max_comp + 1; i <= n; ++i) {
        seq_t bi = c_arr[i - 1] * c_arr[i - 2];
        seq_t ci = (b_arr[i - 1] + c_arr[i - 1]) *
                   (b_arr[i - 2] + c_arr[i - 2]);

        b_arr[i] = bi;
        c_arr[i] = ci;
    }

    max_comp = n;
    return c_arr[n];
}
