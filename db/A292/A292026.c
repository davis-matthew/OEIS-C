/*
 * A292026  Wiener index of the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A292026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 46*a(n-1) - 504*a(n-2) + 1856*a(n-3) - 2048*a(n-4)
 *            Initial values: a(1)=6, a(2)=66, a(3)=1476, a(4)=42984
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n<1, return 0 as safety value */

    /* Handle the first four known terms directly */
    switch (n) {
        case 1: return (seq_t)6;
        case 2: return (seq_t)66;
        case 3: return (seq_t)1476;
        case 4: return (seq_t)42984;
    }

    /* Iteratively compute terms up to n using only the last four values */
    seq_t a1 = (seq_t)6;          /* a(n-4) */
    seq_t a2 = (seq_t)66;         /* a(n-3) */
    seq_t a3 = (seq_t)1476;       /* a(n-2) */
    seq_t a4 = (seq_t)42984;      /* a(n-1) */
    seq_t a_next;

    for (idx_t i = 5; i <= n; ++i) {
        a_next = 46 * a4 - 504 * a3 + 1856 * a2 - 2048 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a_next;
    }

    return a4;
}
