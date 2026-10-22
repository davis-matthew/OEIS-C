/*
 * A337963  Number of unoriented colorings of the 30 edges of a regular
 *          dodecahedron or icosahedron using n or fewer colors.
 * https://oeis.org/A337963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^30 + 15*n^17 + 15*n^16 + n^15 + 20*n^10 + 24*n^6 +
 *            20*n^5 + 24*n^3) / 120
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The build supplies the type definitions for seq_t and idx_t. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Convert the index to the sequence type once. */
    seq_t N = (seq_t)n;

    /* For small integral built‑in types (≤ 64 bit) use the 128‑bit
       intermediate to avoid overflow.  For arbitrary‑precision seq_t
       (size > 8) we compute directly with seq_t arithmetic. */
    if (sizeof(seq_t) <= 8)
    {
        /* 128‑bit intermediate. */
        unsigned __int128 n128 = (unsigned __int128)N;
        unsigned __int128 p3  = n128 * n128 * n128;        /* n^3  */
        unsigned __int128 p5  = p3 * n128 * n128;          /* n^5  */
        unsigned __int128 p6  = p3 * p3;                   /* n^6  */
        unsigned __int128 p10 = p5 * p5;                   /* n^10 */
        unsigned __int128 p15 = p10 * p5;                  /* n^15 */
        unsigned __int128 p16 = p15 * n128;                /* n^16 */
        unsigned __int128 p17 = p16 * n128;                /* n^17 */
        unsigned __int128 p30 = p15 * p15;                 /* n^30 */

        unsigned __int128 sum = p30
                               + (unsigned __int128)15 * p17
                               + (unsigned __int128)15 * p16
                               + p15
                               + (unsigned __int128)20 * p10
                               + (unsigned __int128)24 * p6
                               + (unsigned __int128)20 * p5
                               + (unsigned __int128)24 * p3;

        return (seq_t)(sum / (unsigned __int128)120);
    }
    else
    {
        /* 64+ bit or arbitrary‑precision seq_t: use seq_t arithmetic. */
        seq_t p3 = 1;
        for (int i = 0; i < 3; ++i) p3 *= N;      /* n^3 */

        seq_t p5 = p3;
        for (int i = 0; i < 2; ++i) p5 *= N;      /* n^5 */

        seq_t p6 = p3 * p3;                       /* n^6 */

        seq_t p10 = p5 * p5;                      /* n^10 */

        seq_t p15 = p10 * p5;                     /* n^15 */

        seq_t p16 = p15 * N;                      /* n^16 */
        seq_t p17 = p16 * N;                      /* n^17 */

        seq_t p30 = p15 * p15;                    /* n^30 */

        seq_t sum = p30
                    + (seq_t)15 * p17
                    + (seq_t)15 * p16
                    + p15
                    + (seq_t)20 * p10
                    + (seq_t)24 * p6
                    + (seq_t)20 * p5
                    + (seq_t)24 * p3;

        return sum / 120;
    }
}
