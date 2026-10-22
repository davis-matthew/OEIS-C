/*
 * A364946  Sixth Lie-Betti number of a path graph on n vertices.
 * https://oeis.org/A364946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1,2,3 a(4) = 4 a(5) = 33 a(n) = (n^6 + 45*n^5 -
 *            125*n^4 - 2865*n^3 + 23524*n^2 - 76740*n + 98640) / 720 for n >=
 *            6
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 114, u64 n <= 4858,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  // for size_t, if needed

seq_t a(idx_t n)
{
    /* Handle the special first terms directly */
    if (n <= 3)
        return (seq_t)0;
    if (n == 4)
        return (seq_t)4;
    if (n == 5)
        return (seq_t)33;

    /* Compute using Horner's rule with a 128‑bit intermediate
       to avoid overflow for the 64‑bit build while staying safe for
       the arbitrary‑precision build as well. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 num = (unsigned __int128)1;          // a6
    num = num * nn + (unsigned __int128)45;               // +45*n^5
    num = num * nn + (unsigned __int128)(-125);           // -125*n^4
    num = num * nn + (unsigned __int128)(-2865);          // -2865*n^3
    num = num * nn + (unsigned __int128)23524;            // +23524*n^2
    num = num * nn + (unsigned __int128)(-76740);         // -76740*n
    num = num * nn + (unsigned __int128)98640;           // +98640

    /* The division by 720 is exact for all integer n */
    unsigned __int128 denom = (unsigned __int128)720;
    seq_t result = (seq_t)(num / denom);
    return result;
}
