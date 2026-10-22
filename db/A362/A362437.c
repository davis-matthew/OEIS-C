/*
 * A362437  a(n) = n + Scrabble score of n.
 * https://oeis.org/A362437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Scrabble score of n
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  // for consistency

/* Scrabble letter values: A1 B3 C3 D2 E1 F4 G2 H4 I1 J8 K5 L1 M3 N1 O1 P3 Q10 R1 S1 T1 U1 V4 W4 X8 Y4 Z10 */

/* Precomputed Scrabble scores for numbers 0..19 */
static const unsigned char unit_scores[20] = {
    13, /* 0 zero    */
    3,  /* 1 one     */
    6,  /* 2 two     */
    8,  /* 3 three   */
    7,  /* 4 four    */
    10, /* 5 five    */
    10, /* 6 six     */
    8,  /* 7 seven   */
    9,  /* 8 eight   */
    4,  /* 9 nine    */
    3,  /*10 ten     */
    9,  /*11 eleven  */
    12, /*12 twelve  */
    11, /*13 thirteen*/
    11, /*14 fourteen*/
    13, /*15 fifteen */
    14, /*16 sixteen */    /* corrected score */
    12, /*17 seventeen*/
    12, /*18 eighteen*/
    8   /*19 nineteen*/
};

/* Precomputed Scrabble scores for tens 20,30,…,90 */
static const unsigned char tens_scores[8] = {
    12, /* 20 twenty  */
    12, /* 30 thirty  */
    11, /* 40 forty   */
    14, /* 50 fifty   */
    15, /* 60 sixty   */
    13, /* 70 seventy */
    13, /* 80 eighty  */
    9   /* 90 ninety  */
};

/* Scrabble scores for the magnitude suffixes:
   1 = thousand, 2 = million, 3 = billion, 4 = trillion,
   5 = quadrillion, 6 = quintillion, 7 = sextillion,
   8 = septillion, 9 = octillion, 10 = nonillion,
   11 = decillion, 12 = undecillion, 13 = duodecillion,
   14 = tredecillion, 15 = quattuordecillion,
   16 = quindecillion, 17 = sexdecillion,
   18 = septendecillion, 19 = octodecillion,
   20 = novemdecillion */
static const seq_t suffix_scores[22] = {
    0,  /* none          */
    12, /* thousand      */
    9,  /* million       */
    9,  /* billion       */
    8,  /* trillion      */
    21, /* quadrillion   */
    20, /* quintillion   */
    16, /* sextillion    */
    12, /* septillion    */
    11, /* octillion     */
    10, /* nonillion     */
    12, /* decillion     */
    14, /* undecillion   */
    15, /* duodecillion  */
    15, /* tredecillion  */
    30, /* quattuordecillion */
    25, /* quindecillion */
    22, /* sexdecillion  */
    20, /* septendecillion */
    18, /* octodecillion */
    22  /* novemdecillion */
};

/* Compute Scrabble score for numbers 0–999 */
static inline seq_t score_0_to_999(idx_t x)
{
    seq_t s = 0;
    if (x < 20) {
        return unit_scores[x];
    }
    if (x < 100) {
        s = tens_scores[(x / 10) - 2];
        if (x % 10) {
            s += unit_scores[x % 10];
        }
        return s;
    }
    /* x >= 100 */
    idx_t h = x / 100;
    s = unit_scores[h] + 12;          /* "hundred" = 12 */
    idx_t r = x % 100;
    if (r) {
        s += score_0_to_999(r);
    }
    return s;
}

/* Compute Scrabble score for any non‑negative integer n */
static seq_t scrabble_score(idx_t n)
{
    if (n == 0) {
        return unit_scores[0];
    }

    seq_t total = 0;
    idx_t m = n;
    int group = 0;                      /* 0 = units, 1 = thousands, ... */
    while (m > 0) {
        unsigned int g = (unsigned int)(m % 1000);
        if (g > 0) {
            total += score_0_to_999(g);
            if (group > 0 && group < (int)(sizeof(suffix_scores) / sizeof(suffix_scores[0]))) {
                total += suffix_scores[group];
            }
        }
        m /= 1000;
        ++group;
    }
    return total;
}

/* OEIS sequence a(n) = n + Scrabble score of n */
seq_t a(idx_t n)
{
    return (seq_t)n + scrabble_score(n);
}
