/*
 * A281191  Number of holes in the (American) English name of n (as printed in
 *          lower case).
 * https://oeis.org/A281191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, word, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Convert n to its American English name (lower case)
 * and count the number of letter holes according to OEIS A281191. */

#include <stdint.h>
#include <stddef.h>

static const seq_t unit_holes[10] = {
    0,       /* 0 (unused) */
    2,       /* one   */
    1,       /* two   */
    2,       /* three */
    1,       /* four  */
    1,       /* five  */
    0,       /* six   */
    2,       /* seven */
    3,       /* eight */
    1        /* nine  */
};

static const seq_t teen_holes[10] = {
    1,       /* ten   */
    3,       /* eleven*/
    2,       /* twelve*/
    2,       /* thirteen*/
    3,       /* fourteen*/
    2,       /* fifteen*/
    2,       /* sixteen*/
    4,       /* seventeen*/
    5,       /* eighteen*/
    3        /* nineteen*/
};

static const seq_t tens_holes[10] = {
    0,       /* 0 */
    0,       /* 1 */
    1,       /* 20 */
    0,       /* 30 */
    1,       /* 40 */
    0,       /* 50 */
    0,       /* 60 */
    2,       /* 70 */
    3,       /* 80 */
    1        /* 90 */
};

static const seq_t scaling_holes[] = {
    0,        /* units      */
    3,        /* thousand   */
    1,        /* million    */
    2,        /* billion    */
    1,        /* trillion   */
    4,        /* quadrillion*/
    2,        /* quintillion*/
    2,        /* sextillion */
    3,        /* septillion */
    3,        /* octillion  */
    3,        /* nonillion  */
    3         /* decillion  */
};

static inline seq_t count_holes_below_1000(uint32_t v)
{
    seq_t h = 0;
    if (v >= 100) {
        uint32_t hgt = v / 100;
        h += unit_holes[hgt];
        h += 3;           /* "hundred" contributes 3 holes */
        v %= 100;
    }
    if (v >= 20) {
        uint32_t t = v / 10;
        h += tens_holes[t];
        v %= 10;
    }
    if (v >= 10) {
        h += teen_holes[v - 10];
        v = 0;
    }
    if (v > 0) {          /* 1 .. 9 */
        h += unit_holes[v];
    }
    return h;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 2;         /* "zero" */
    }
    seq_t holes = 0;
    idx_t x = n;
    size_t group = 0;
    const size_t scl = sizeof(scaling_holes) / sizeof(scaling_holes[0]);

    while (x > 0) {
        uint32_t val = (uint32_t)(x % 1000);
        x /= 1000;
        if (val > 0) {
            holes += count_holes_below_1000(val);
            if (group < scl) {
                holes += scaling_holes[group];
            }
        }
        ++group;
    }
    return holes;
}
