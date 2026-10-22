/*
 * A301693  Partial sums of A301692.
 * https://oeis.org/A301693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 197, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A301693 – Partial sums of A301692  
   Recurrence: a(0) = 1, a(1) = 5, a(2) = 12, a(3) = 21, a(4) = 33, a(5) = 49, a(6) = 70,
   a(7) = 95, a(8) = 121, a(9) = 150, a(10) = 184, a(11) = 222, a(12) = 263,
   a(13) = 305, and for n ≥ 14
       a(n) = a(n-1) + 2·a(n-5) – 2·a(n-6) – a(n-10) + a(n-11).
*/

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* lookup table for the first 14 terms */
    static const seq_t init[14] = {
        1,  /* a(0)  */
        5,  /* a(1)  */
        12, /* a(2)  */
        21, /* a(3)  */
        33, /* a(4)  */
        49, /* a(5)  */
        70, /* a(6)  */
        95, /* a(7)  */
        121,/* a(8)  */
        150,/* a(9)  */
        184,/* a(10) */
        222,/* a(11) */
        263,/* a(12) */
        305 /* a(13) */
    };

    if (n < 14)
        return init[n];

    /* Circular buffer holding the last 11 values:
       buf[0] = a(i-11), buf[1] = a(i-10), ..., buf[10] = a(i-1) for the current i.  */
    seq_t buf[11];
    /* Initialisation for i = 14:  a(3) .. a(13) go into buf[0] .. buf[10] */
    for (uint64_t k = 0; k < 11; ++k)
        buf[k] = init[3 + k];

    seq_t term = 0;  /* Will hold a(n) after the loop */
    for (idx_t i = 14; i <= n; ++i)
    {
        /* Compute a(i) using the recurrence and the values in buf */
        term = buf[10]  /* a(i-1) */
             + (buf[6] + buf[6])   /* 2·a(i-5) */
             - (buf[5] + buf[5])   /* 2·a(i-6) */
             - buf[1]              /* a(i-10) */
             + buf[0];             /* a(i-11) */

        /* Shift the buffer left by one position and insert the new term */
        for (int k = 0; k < 10; ++k)
            buf[k] = buf[k + 1];
        buf[10] = term;
    }

    return term;  /* term now holds a(n) */
}
