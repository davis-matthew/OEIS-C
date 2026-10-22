/*
 * A305228  Number of nX6 0..1 arrays with every element unequal to 0, 1, 2, 3
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   mul_word
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=32, a(2)=293, a(3)=236, a(4)=541, a(5)=2050, a(6)=7062,
 * a(7)=26528, a(8)=96528, a(9)=359365, a(10)=1331372, a(11)=4953295,
 * a(12)=18415910, a(13)=68529992, a(14)=254994255, a(15)=948995613,
 * a(16)=3531814704, a(17)=13144690959, a(18)=48922066241,
 * a(19)=182080588807, a(20)=677678047930, a(21)=2522227710570,
 * a(22)=9387403707474;
 * For n>22:
 *   a(n) = 5*a(n-1) + a(n-2) - 26*a(n-3) + 5*a(n-4) + 45*a(n-5) + 11*a(n-6)
 *          - 33*a(n-7) - 49*a(n-8) + 14*a(n-9) + 29*a(n-10) + 2*a(n-11)
 *          + 21*a(n-12) - 20*a(n-13) - 21*a(n-14) + 17*a(n-15) + 4*a(n-16)
 *          - 4*a(n-17)
 */

#include <stddef.h>
#include "../utils/mul_word.h"

/* Compute a(n).  Assumes idx_t is an unsigned integer type. */
seq_t a(idx_t n)
{
    /* initial 22 terms */
    static const seq_t init[22] = {
        32ULL,
        293ULL,
        236ULL,
        541ULL,
        2050ULL,
        7062ULL,
        26528ULL,
        96528ULL,
        359365ULL,
        1331372ULL,
        4953295ULL,
        18415910ULL,
        68529992ULL,
        254994255ULL,
        948995613ULL,
        3531814704ULL,
        13144690959ULL,
        48922066241ULL,
        182080588807ULL,
        677678047930ULL,
        2522227710570ULL,
        9387403707474ULL
    };

    if (n <= 22u)
        return init[n - 1u];

    /* sliding window of the last 17 terms */
    seq_t vec[17];
    for (size_t i = 0; i < 17; ++i)
        vec[i] = init[i + 5];          /* vec[0] = a(6), vec[16] = a(22) */

    for (idx_t cur = 23; cur <= n; ++cur) {
        seq_t pos = 0, neg = 0;
        /* access a(n-k) as vec[16-k] */
        pos += mul_word(vec[16], 5);         /* 5*a(n-1) */
        pos +=         vec[15] ;            /*   a(n-2) */
        neg += mul_word(vec[14], 26);        /* -26*a(n-3) */
        pos += mul_word(vec[13], 5);         /*   5*a(n-4) */
        pos += mul_word(vec[12], 45);        /*  45*a(n-5) */
        pos += mul_word(vec[11], 11);        /*  11*a(n-6) */
        neg += mul_word(vec[10], 33);        /* -33*a(n-7) */
        neg += mul_word(vec[9], 49);         /* -49*a(n-8) */
        pos += mul_word(vec[8], 14);         /*  14*a(n-9) */
        pos += mul_word(vec[7], 29);         /*  29*a(n-10) */
        pos += mul_word(vec[6], 2);          /*   2*a(n-11) */
        pos += mul_word(vec[5], 21);         /*  21*a(n-12) */
        neg += mul_word(vec[4], 20);         /* -20*a(n-13) */
        neg += mul_word(vec[3], 21);         /* -21*a(n-14) */
        pos += mul_word(vec[2], 17);         /*  17*a(n-15) */
        pos += mul_word(vec[1], 4);          /*   4*a(n-16) */
        neg += mul_word(vec[0], 4);          /*  -4*a(n-17) */

        seq_t nxt = pos - neg;               /* compute a(n) */

        /* shift window left by one, discard a(n-17) */
        for (size_t i = 0; i < 16; ++i)
            vec[i] = vec[i + 1];
        vec[16] = nxt;                       /* a(n) becomes newest */
    }

    return vec[16];
}
