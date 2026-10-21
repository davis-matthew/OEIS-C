/*
 * A253221  Number of n X 6 nonnegative integer arrays with upper left 0 and
 *          lower right its king-move distance away minus 2 and every value
 *          within 2 of its king move distance from the upper left and every
 *          value increasing by 0 or 1 with every step right, diagonally se or
 *          down.
 * https://oeis.org/A253221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A253221: a(1)=10, a(2)=49, a(3)=263, a(4)=1249, a(5)=3568,
   a(6)=3568, a(7)=47698, a(8)=213155, a(9)=538444, a(10)=1039060,
   a(11)=1719174, a(12)=2579462, a(13)=3619974.
   For n>13 the recurrence a(n)=3*a(n-1)-3*a(n-2)+a(n-3) holds. */

#include <stddef.h>   /* for size_t; not strictly required but harmless */

seq_t a(idx_t n)
{
    /* 64‑bit literals avoid overflow in the source file even when
       seq_t is smaller (e.g. uint8_t).  The cast to seq_t happens
       only on output or when storing into a seq_t variable. */
    static const uint64_t init[13] = {
        10ULL,       /* a(1) */
        49ULL,       /* a(2) */
        263ULL,      /* a(3) */
        1249ULL,     /* a(4) */
        3568ULL,     /* a(5) */
        3568ULL,     /* a(6) */
        47698ULL,    /* a(7) */
        213155ULL,   /* a(8) */
        538444ULL,   /* a(9) */
        1039060ULL,  /* a(10) */
        1719174ULL,  /* a(11) */
        2579462ULL,  /* a(12) */
        3619974ULL   /* a(13) */
    };

    /* For small n just return the pre‑computed value. */
    if (n <= 13)
        return (seq_t)init[n - 1];

    /* Initialise the three most recent values for the recurrence. */
    seq_t a_nm3 = (seq_t)init[10]; /* a(11) */
    seq_t a_nm2 = (seq_t)init[11]; /* a(12) */
    seq_t a_nm1 = (seq_t)init[12]; /* a(13) */

    seq_t a_n = a_nm1;
    for (idx_t i = 14; i <= n; ++i) {
        a_n = 3 * a_nm1 - 3 * a_nm2 + a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
