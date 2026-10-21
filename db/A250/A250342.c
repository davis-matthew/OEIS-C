/*
 * A250342  Number of length 6+5 0..n arrays with every six consecutive terms
 *          having the maximum of some three terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A250342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (71/315)*n^9 + (851/140)*n^8 + (573/14)*n^7 + 112*n^6 +
 *            162*n^5 + (2717/20)*n^4 + (5041/126)*n^3 - (601/14)*n^2 -
 *            (3272/105)*n + 1 The polynomial produces integers for every n ≥
 *            1. The terms are evaluated exactly by using a common denominator
 *            1260
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 160, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to an intermediate type that can hold up to n^9 exactly.
       unsigned __int128 is a GCC/Clang extension that is available on
       all compilers used for build testing. */
    unsigned __int128 N = (unsigned __int128)n;

    unsigned __int128 n2 = N * N;
    unsigned __int128 n3 = n2 * N;
    unsigned __int128 n4 = n3 * N;
    unsigned __int128 n5 = n4 * N;
    unsigned __int128 n6 = n5 * N;
    unsigned __int128 n7 = n6 * N;
    unsigned __int128 n8 = n7 * N;
    unsigned __int128 n9 = n8 * N;

    /* Numerator of the fraction with common denominator 1260.  All
       intermediate products fit in unsigned __int128 for n ≤ 10 000. */
    unsigned __int128 num = 0;

    num += (unsigned __int128)284   * n9;   /* 71*4   */
    num += (unsigned __int128)7659  * n8;   /* 851*9  */
    num += (unsigned __int128)51570 * n7;   /* 573*90 */
    num += (unsigned __int128)141120 * n6;  /* 112*1260 */
    num += (unsigned __int128)204120 * n5;  /* 162*1260 */
    num += (unsigned __int128)171171 * n4;  /* 2717*63 */
    num += (unsigned __int128)50410  * n3;  /* 5041*10 */
    num -= (unsigned __int128)54090  * n2;  /* -601*90 */
    num -= (unsigned __int128)39264  * N;   /* -3272*12 */
    num += (unsigned __int128)1260;        /* constant term */

    /* Exact division by 1260.  The result fits in seq_t for all n
       where the sequence value does. */
    seq_t result = (seq_t)(num / 1260);

    return result;
}
