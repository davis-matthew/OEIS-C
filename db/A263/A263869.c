/*
 * A263869  Number of (n+1) X (4+1) 0..1 arrays with each row and column
 *          divisible by 3, read as a binary number with top and left being
 *          the most significant bits, and rows and columns lexicographically
 *          nondecreasing.
 * https://oeis.org/A263869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 142, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS:A263869
   Formula: a(n) = (315*(2889-841*(-1)^n) + (537927-96327*(-1)^n)*n
                 - 21*(-4723+755*(-1)^n)*n^2 - 7*(-1469+45*(-1)^n)*n^3
                 - 105*(3+5*(-1)^n)*n^4 - 7*(-29+9*(-1)^n)*n^5 + 42*n^6 + 2*n^7) / 645120
   (The numerator is computed directly to avoid intermediate overflow by using
    unsigned __int128 where available.) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset 1, undefined input */

    /* use unsigned __int128 for intermediate calculations */
    const unsigned __int128 denom128 = (unsigned __int128)645120;
    const unsigned __int128 n128     = (unsigned __int128)n;
    const unsigned __int128 n2       = n128 * n128;
    const unsigned __int128 n3       = n2   * n128;
    const unsigned __int128 n4       = n3   * n128;
    const unsigned __int128 n5       = n4   * n128;
    const unsigned __int128 n6       = n5   * n128;
    const unsigned __int128 n7       = n6   * n128;

    const idx_t is_odd = n & 1U;    /* 1 for odd n, 0 for even n */

    unsigned __int128 num = 0, term;

    /* term1 = 315*(2889-841*(-1)^n) */
    term  = 315 * (is_odd ? 3730 : 2048);
    num  += term;

    /* term2 = (537927-96327*(-1)^n) * n */
    term  = (is_odd ? 634254 : 441600) * n128;
    num  += term;

    /* term3 = -21*(-4723+755*(-1)^n) * n^2 */
    term  = (is_odd ? 115038 : 83328) * n2;   /* 115038 = 21*5478, 83328 = 21*3968 */
    num  += term;

    /* term4 = -7*(-1469+45*(-1)^n) * n^3 */
    term  = (is_odd ? 10598 : 9968) * n3;   /* 10598 = 7*1514, 9968 = 7*1424 */
    num  += term;

    /* term5 = -105*(3+5*(-1)^n)*n^4 */
    if (is_odd)
        num += (unsigned __int128)210 * n4;   /* +210*n^4 for odd n */
    else
        num -= (unsigned __int128)840 * n4;   /* -840*n^4 for even n */

    /* term6 = -7*(-29+9*(-1)^n)*n^5 */
    term  = (is_odd ? 266 : 140) * n5;       /* 266 = 7*38, 140 = 7*20 */
    num  += term;

    /* term7 = 42*n^6 */
    term  = (unsigned __int128)42 * n6;
    num  += term;

    /* term8 = 2*n^7 */
    term  = (unsigned __int128)2 * n7;
    num  += term;

    return (seq_t)(num / denom128);
}
