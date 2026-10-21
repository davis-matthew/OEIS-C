/*
 * A279797  Number of radicals of n strokes in a traditional Chinese
 *          dictionary (using Kangxi Dictionary, 1716, as the reference).
 * https://oeis.org/A279797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini
 * Formula:   a(n) = number of Kangxi radicals whose stroke count equals n
 * Range:     u8 n <= 17, u16 n <= 17, u32 n <= 17, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Counts of Kangxi radicals by stroke count from 1 to 17.
       Index 0 is unused. */
    static const seq_t counts[] = {
        0,   /* 0 strokes, unused */
        6,   /* 1 stroke */
        23,  /* 2 strokes */
        31,  /* 3 strokes */
        34,  /* 4 strokes */
        23,  /* 5 strokes */
        29,  /* 6 strokes */
        20,  /* 7 strokes */
        9,   /* 8 strokes */
        11,  /* 9 strokes */
        8,   /* 10 strokes */
        6,   /* 11 strokes */
        4,   /* 12 strokes */
        4,   /* 13 strokes */
        2,   /* 14 strokes */
        1,   /* 15 strokes */
        2,   /* 16 strokes */
        1    /* 17 strokes */
    };
    if (n == 0)          /* offset is 1; behavior below offset is irrelevant */
        return 0;
    if (n <= (idx_t)sizeof(counts)/sizeof(counts[0]) - 1)
        return counts[(size_t)n];
    return 0;
}
