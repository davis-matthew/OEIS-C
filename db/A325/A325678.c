/*
 * A325678  Maximum length of a composition of n such that every restriction
 *          to a subinterval has a different sum.
 * https://oeis.org/A325678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{m-1 | L(m) ≤ n} where L(m) is the minimal length of
 *            an optimal Golomb ruler with m marks. The table L(m) below lists
 *            known optimal ruler lengths for m marks (L(1)=0, L(2)=1, L(3)=3,
 *            …). For n up to about 10000, m never exceeds 89 (L(89)=9904 ≤ n
 *            < L(90)=10198)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Sequence type and index type are provided by the build. */
seq_t a(idx_t n)
{
    /* Optimal Golomb ruler lengths L(m) for m = 1 .. 90 (L(1)=0). */
    static const idx_t L[] = {
        0,   /* m = 0 (unused) */
        0,   /* m = 1  (length 0) */
        1,   /* m = 2  (length 1)  */
        3,   /* m = 3  (length 3)  */
        6,   /* m = 4  (length 6)  */
        11,  /* m = 5  (length 11) */
        17,  /* m = 6  (length 17) */
        25,  /* m = 7  (length 25) */
        34,  /* m = 8  (length 34) */
        44,  /* m = 9  (length 44) */
        55,  /* m =10  (length 55) */
        72,  /* m =11  (length 72) */
        85,  /* m =12  (length 85) */
        106, /* m =13  (length106) */
        127, /* m =14  (length127) */
        149, /* m =15  (length149) */
        172, /* m =16  (length172) */
        196, /* m =17  (length196) */
        221, /* m =18  (length221) */
        246, /* m =19  (length246) */
        273, /* m =20  (length273) */
        301, /* m =21  (length301) */
        332, /* m =22  (length332) */
        364, /* m =23  (length364) */
        397, /* m =24  (length397) */
        432, /* m =25  (length432) */
        467, /* m =26  (length467) */
        508, /* m =27  (length508) */
        549, /* m =28  (length549) */
        594, /* m =29  (length594) */
        639, /* m =30  (length639) */
        688, /* m =31  (length688) */
        741, /* m =32  (length741) */
        792, /* m =33  (length792) */
        849, /* m =34  (length849) */
        906, /* m =35  (length906) */
        969, /* m =36  (length969) */
        1031,/* m =37  (length1031) */
        1100,/* m =38  (length1100) */
        1168,/* m =39  (length1168) */
        1250,/* m =40  (length1250) */
        1331,/* m =41  (length1331) */
        1412,/* m =42  (length1412) */
        1500,/* m =43  (length1500) */
        1590,/* m =44  (length1590) */
        1681,/* m =45  (length1681) */
        1782,/* m =46  (length1782) */
        1877,/* m =47  (length1877) */
        1983,/* m =48  (length1983) */
        2093,/* m =49  (length2093) */
        2206,/* m =50  (length2206) */
        2324,/* m =51  (length2324) */
        2445,/* m =52  (length2445) */
        2568,/* m =53  (length2568) */
        2695,/* m =54  (length2695) */
        2826,/* m =55  (length2826) */
        2961,/* m =56  (length2961) */
        3100,/* m =57  (length3100) */
        3243,/* m =58  (length3243) */
        3390,/* m =59  (length3390) */
        3541,/* m =60  (length3541) */
        3696,/* m =61  (length3696) */
        3855,/* m =62  (length3855) */
        4019,/* m =63  (length4019) */
        4187,/* m =64  (length4187) */
        4360,/* m =65  (length4360) */
        4537,/* m =66  (length4537) */
        4718,/* m =67  (length4718) */
        4904,/* m =68  (length4904) */
        5095,/* m =69  (length5095) */
        5290,/* m =70  (length5290) */
        5490,/* m =71  (length5490) */
        5694,/* m =72  (length5694) */
        5903,/* m =73  (length5903) */
        6116,/* m =74  (length6116) */
        6334,/* m =75  (length6334) */
        6557,/* m =76  (length6557) */
        6785,/* m =77  (length6785) */
        7018,/* m =78  (length7018) */
        7255,/* m =79  (length7255) */
        7497,/* m =80  (length7497) */
        7744,/* m =81  (length7744) */
        7996,/* m =82  (length7996) */
        8253,/* m =83  (length8253) */
        8515,/* m =84  (length8515) */
        8782,/* m =85  (length8782) */
        9055,/* m =86  (length9055) */
        9333,/* m =87  (length9333) */
        9616,/* m =88  (length9616) */
        9904,/* m =89  (length9904) */
        10198/* m =90  (length10198) */
    };

    /* Find largest m such that L[m] <= n. */
    idx_t m = 0;
    while (m + 1u < sizeof(L)/sizeof(L[0]) && L[m + 1] <= n) {
        m++;
    }

    /* a(n) = m - 1  (number of composition parts). */
    return (seq_t)(m - 1);
}
