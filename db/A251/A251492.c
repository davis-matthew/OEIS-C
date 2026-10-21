/*
 * A251492  Number of (n+1) X (1+1) 0..3 arrays with every 2 X 2 subblock
 *          summing to 4 5 6 7 or 8.
 * https://oeis.org/A251492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 13*a(n-1) - 5*a(n-2) - 52*a(n-3) + 48*a(n-4) for n >= 5
 *            Initial values: a(1)=186, a(2)=2270, a(3)=27844, a(4)=341718
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by a linear recurrence; computing it by iterating
   backward through the recurrence avoids the need for dynamic memory and
   works both for built‑in integer builds and for arbitrary‑precision
   builds.  The function works for any order of calls. */

#include <stdint.h>  // for idx_t

seq_t a(idx_t n)
{
    if (n == 0) {
        /* a(0) is unused in the OEIS offset; returning 0 keeps the
           index alignment intact. */
        return (seq_t)0;
    }

    /* initial terms */
    const seq_t a1_init = (seq_t)186;
    const seq_t a2_init = (seq_t)2270;
    const seq_t a3_init = (seq_t)27844;
    const seq_t a4_init = (seq_t)341718;

    if (n == 1) return a1_init;
    if (n == 2) return a2_init;
    if (n == 3) return a3_init;
    if (n == 4) return a4_init;

    /* iterate the recurrence up to the requested n */
    seq_t a1 = a1_init;
    seq_t a2 = a2_init;
    seq_t a3 = a3_init;
    seq_t a4 = a4_init;

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next =   (seq_t)13 * a4
                     - (seq_t)5  * a3
                     - (seq_t)52 * a2
                     + (seq_t)48 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
