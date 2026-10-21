/*
 * A253419  Number of (n+2)X(3+2) nonnegative integer arrays with all values
 *          the knight distance from the upper left minus as much as 1, with
 *          successive minimum path knight move differences either 0 or +1,
 *          and any unreachable value zero.
 * https://oeis.org/A253419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n < 1. For n > 8 the sequence satisfies a quartic
 *            polynomial whose form depends on n (mod 4): a(n) = (3008/3)n⁴ -
 *            (17836/3)n³ + (1507261/48)n² - (2120783/12)n + 386938 (n mod 4 =
 *            0) a(n) = (3008/3)n⁴ - (14828/3)n³ + (1281949/48)n² -
 *            (3938669/24)n + (5831375/16)(n mod 4 = 1) a(n) = (3...
 * Range:     u8 none, u16 n <= 4, u32 n <= 46, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* offset is 1 – behaviour for n < 1 is irrelevant */

    /* first eight terms from the OEIS entry */
    static const seq_t init[9] = {
        0,          /* dummy for index 0 */
        257, 1087, 9985, 44729, 215037, 321383, 1399041, 2045480
    };
    if (n <= 8) return init[n];

    /* work with 128‑bit arithmetic to avoid overflow */
    unsigned __int128 N = n;
    unsigned __int128 term, result;

    switch (n % 4) {
        case 0:  /* n ≡ 0 (mod 4) */
            term = 3008 * 16 * N * N * N * N;          /* 3008*16*n⁴ */
            term += (-17836 * 16 * N * N * N);         /* -17836*16*n³ */
            term += 1507261 * N * N;                  /* +1507261*n² */
            term += (-2120783 * 4 * N);               /* -2120783*4*n */
            term += 386938 * 48;                     /* +386938*48 */
            result = term / 48;
            break;

        case 1:  /* n ≡ 1 (mod 4) */
            term = 3008 * 16 * N * N * N * N;          /* 3008*16*n⁴ */
            term += (-14828 * 16 * N * N * N);         /* -14828*16*n³ */
            term += 1281949 * N * N;                  /* +1281949*n² */
            term += (-3938669 * 2 * N);               /* -3938669*2*n */
            term += 5831375 * 3;                      /* +5831375*3 */
            result = term / 48;
            break;

        case 2:  /* n ≡ 2 (mod 4) */
            term = 3008 * 16 * N * N * N * N;          /* 3008*16*n⁴ */
            term += (-26860 * 16 * N * N * N);         /* -26860*16*n³ */
            term += 2607325 * N * N;                  /* +2607325*n² */
            term += (-3154427 * 4 * N);               /* -3154427*4*n */
            term += 2342671 * 12;                     /* +2342671*12 */
            result = term / 48;
            break;

        case 3:  /* n ≡ 3 (mod 4) */
            term = 3008 * 16 * N * N * N * N;          /* 3008*16*n⁴ */
            term += (-5804 * 16 * N * N * N);          /* -5804*16*n³ */
            term += (-251267 * N * N);                /* -251267*n² */
            term += (-694613 * 2 * N);                /* -694613*2*n */
            term += 1836387 * 3;                      /* +1836387*3 */
            result = term / 48;
            break;
    }   
    return (seq_t)result;
}
