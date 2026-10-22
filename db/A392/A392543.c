/*
 * A392543  Expansion of 1 / ((1-x)^2 - x^7).
 * https://oeis.org/A392543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392543
   Expansion of 1 / ((1-x)^2 - x^7).
   Formula: a(n) = 2*a(n-1) - a(n-2) + a(n-7),
            a(0) = 1, a(1) = 2, a(2) = 3, a(3) = 4,
            a(4) = 5, a(5) = 6, a(6) = 7
   The function is memoised to allow non‑sequential calls. */

#include <stdlib.h>

/* Initial values for n = 0..6 */
static const seq_t init_vals[7] = { 1, 2, 3, 4, 5, 6, 7 };

/* Memoisation buffer */
static seq_t *buf = NULL;
static idx_t buf_size = 0;

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Return known initial values directly */
    if (n < 7)
        return init_vals[n];

    /* Extend buffer if needed */
    if (n + 1 > buf_size) {
        idx_t new_size = n + 1;
        seq_t *new_buf = (seq_t *)realloc(buf, new_size * sizeof(seq_t));
        if (!new_buf)
            abort(); /* allocation failure */

        /* Initialise first 7 elements if this is a fresh allocation or
           if the buffer was smaller than 7 before realloc.  */
        if (buf_size == 0 || buf_size < 7) {
            for (idx_t i = 0; i < 7; ++i)
                new_buf[i] = init_vals[i];
        }

        /* Compute values from the first missing index up to the new size */
        idx_t start = (buf_size > 7) ? buf_size : 7;
        for (idx_t i = start; i < new_size; ++i) {
            seq_t t = new_buf[i - 1] * 2;   /* 2*a(i-1) */
            t -= new_buf[i - 2];             /* - a(i-2) */
            t += new_buf[i - 7];             /* + a(i-7) */
            new_buf[i] = t;
        }

        /* Commit the new buffer and size */
        buf = new_buf;
        buf_size = new_size;
        buf_size = new_size;
    }

    return buf[n];
}
