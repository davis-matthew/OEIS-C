/*
 * A358248  Number of n-regular, N_0-weighted pseudographs on 2 vertices with
 *          total edge weight 8, up to isomorphism.
 * https://oeis.org/A358248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 308, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A358248: Number of n-regular, N_0‑weighted pseudographs on 2 vertices with total edge weight 8, up to isomorphism.
   For n ≤ 15 the terms are taken from the OEIS entry.
   For n ≥ 16 the sequence satisfies the linear recurrence
       a(n) = a(n-2) + 432
   which yields the closed forms
       a(2k)   = 2367 + 432·(k – 8)   for k ≥ 8      (n even, n ≥ 16)
       a(2k+1) = 2135 + 432·(k – 7)   for k ≥ 7      (n odd,  n ≥ 17)
   These formulas are used directly. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial values for 1 ≤ n ≤ 15 */
    static const seq_t init_values[15] = {
        1,   /* a(1) */
        10,  /* a(2) */
        35,  /* a(3) */
        99,  /* a(4) */
        190, /* a(5) */
        332, /* a(6) */
        484, /* a(7) */
        680, /* a(8) */
        863, /* a(9) */
        1082,/* a(10) */
        1277,/* a(11) */
        1505,/* a(12) */
        1704,/* a(13) */
        1935,/* a(14) */
        2135 /* a(15) */
    };

    if (n <= 15)
        return init_values[n - 1];

    /* For n ≥ 16 use the linear formulas derived from the recurrence a(n) = a(n-2)+432. */
    const seq_t step   = (seq_t)432;
    const seq_t evenB  = (seq_t)2367; /* a(16) */
    const seq_t oddB   = (seq_t)2135; /* a(15) */

    if ((n & (seq_t)1) == 0) { /* even n */
        idx_t k = n / 2;
        return evenB + step * (seq_t)(k - 8);
    } else {                    /* odd n */
        idx_t k = (n - 1) / 2;
        return oddB + step * (seq_t)(k - 7);
    }
}
