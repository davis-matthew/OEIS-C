/*
 * A331356  Number of chiral pairs of colorings of the edges of a regular
 *          4-dimensional orthoplex with n available colors.
 * https://oeis.org/A331356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 8, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (48*n^3 - 20*n^6 - 60*n^7 + 8*n^8 + 12*n^9 
 *                 - 3*n^12 + 12*n^13 + 18*n^14 - 12*n^15 - 4*n^18 
 *                 + n^24) / 384
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* undefined for n < 1; return 0 harmlessly */
    if (n < 1) return 0;

#ifdef __cplusplus   /* C++17 build – seq_t can be arbitrary-precision */
    /* use the native integer type (larger than 64‑bit when required) */
    seq_t nseq = (seq_t)n;
    seq_t powarr[25];
    powarr[0] = (seq_t)1;
    for (idx_t i = 1; i <= 24; ++i)
        powarr[i] = powarr[i-1] * nseq;

    seq_t numerator = (seq_t)0;
    numerator += (seq_t)48  * powarr[3];
    numerator -= (seq_t)20  * powarr[6];
    numerator -= (seq_t)60  * powarr[7];
    numerator += (seq_t)8   * powarr[8];
    numerator += (seq_t)12  * powarr[9];
    numerator -= (seq_t)3   * powarr[12];
    numerator += (seq_t)12  * powarr[13];
    numerator += (seq_t)18  * powarr[14];
    numerator -= (seq_t)12  * powarr[15];
    numerator -= (seq_t)4   * powarr[18];
    numerator += powarr[24];

    return numerator / (seq_t)384;
#else   /* C11 build – seq_t is a builtin unsigned integer */
    /* use unsigned __int128 for the intermediary to avoid overflow
       when n <= 8 (the only n that fits in uint64_t for the sequence) */
    __uint128_t n128 = (unsigned __int128)n;
    __uint128_t powarr[25];
    powarr[0] = (unsigned __int128)1;
    for (idx_t i = 1; i <= 24; ++i)
        powarr[i] = powarr[i-1] * n128;

    __int128 num = 0;
    num += (__int128)48  * powarr[3];
    num -= (__int128)20  * powarr[6];
    num -= (__int128)60  * powarr[7];
    num += (__int128)8   * powarr[8];
    num += (__int128)12  * powarr[9];
    num -= (__int128)3   * powarr[12];
    num += (__int128)12  * powarr[13];
    num += (__int128)18  * powarr[14];
    num -= (__int128)12  * powarr[15];
    num -= (__int128)4   * powarr[18];
    num += (__int128)powarr[24];

    /* num is non‑negative for all n >= 1 */
    seq_t result = (seq_t)(num / 384);
    return result;
#endif
}
