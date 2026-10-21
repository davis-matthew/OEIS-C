/*
 * A241893  The total number of rectangles appearing in the Thue-Morse
 *          sequence logical matrices (1, 0 version) after n stages.
 * https://oeis.org/A241893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*2^n + 2*4^n - (-1)^n*(2^n+12)-28)/18 , n>0; a(0)=0
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

#if defined(__SIZEOF_INT128__) && !defined(_MSC_VER)
    /* For builds with 64‑bit seq_t we use 128‑bit intermediates to avoid overflow.
       __int128 is available on GCC/Clang for 64‑bit targets.                     */
    if (sizeof(seq_t) <= 8) {
        __int128 pow2n = (__int128)1 << n;      /* 2^n   (fits in 128 bits for n <= 63)  */
        __int128 pow4n = pow2n * pow2n;         /* 4^n   = 2^(2n)  */
        __int128 num = (__int128)3 * pow2n + (__int128)2 * pow4n;
        if ((n & 1) == 0) {
            num -= pow2n + 12;
        } else {
            num += pow2n + 12;
        }
        num -= 28;
        return (seq_t)(num / 18);
    }
#endif

    /* General case – works with arbitrary‑precision seq_t as well as 128‑bit seq_t */
    seq_t pow2n = pow2(n);          /* 2^n  */
    seq_t pow4n = pow2(2 * n);     /* 4^n  */
    seq_t num   = (seq_t)3 * pow2n + (seq_t)2 * pow4n;
    if ((n & 1) == 0) {
        num -= pow2n + (seq_t)12;
    } else {
        num += pow2n + (seq_t)12;
    }
    num -= (seq_t)28;
    return num / (seq_t)18;
}
