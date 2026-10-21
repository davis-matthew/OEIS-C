/*
 * A294698  Number of permutations of [n] avoiding {1423, 1234, 3412}.
 * https://oeis.org/A294698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/720)*(720*(41 - 5*2^(3+n)) + 6*(2812 + 465*2^n)*n +
 *            2*(2627 + 45*2^n)*n^2 + 765*n^3 + 145*n^4 + 3*n^5 + n^6)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 24, u64 n <= 54, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 64‑bit implementation (seq_t of 8 bytes or less).  
       Use 128‑bit temporaries to avoid overflow.         */
    if (sizeof(seq_t) <= 8) {
        __int128 pow2n = (__int128)1 << n;           /* 2^n   */
        __int128 pow2n3 = pow2n << 3;                /* 2^(n+3) */

        __int128 term1 = (__int128)720 * 41;
        __int128 term2 = (__int128)6 *
                         (( (__int128)2812 + (__int128)465 * pow2n) * (__int128)n);
        __int128 term3 = (__int128)2 *
                         (( (__int128)2627 + (__int128)45 * pow2n) * (__int128)n * (__int128)n);
        __int128 n1   = (__int128)n;
        __int128 n2   = n1 * n1;
        __int128 n3   = n2 * n1;
        __int128 n4   = n3 * n1;
        __int128 n5   = n4 * n1;
        __int128 n6   = n5 * n1;
        __int128 term4 = (__int128)765 * n3;
        __int128 term5 = (__int128)145 * n4;
        __int128 term6 = (__int128)3 * n5;
        __int128 term7 = n6;

        __int128 Tpos = term1 + term2 + term3 + term4 + term5 + term6 + term7;
        __int128 Tneg = (__int128)720 * 5 * pow2n3;
        __int128 numerator = Tpos - Tneg;
        return (seq_t)(numerator / 720);
    }

    /* Arbitrary‑precision implementation (seq_t larger than 8 bytes).  
       All calculations are performed with the native seq_t type.  */
    seq_t pow2n  = pow2(n);         /* 2^n   */
    seq_t pow2n3 = pow2(n + 3);     /* 2^(n+3) */

    seq_t ns = n;                 /* convert n to seq_t once */
    seq_t n2 = ns * ns;
    seq_t n3 = n2 * ns;
    seq_t n4 = n3 * ns;
    seq_t n5 = n4 * ns;
    seq_t n6 = n5 * ns;

    seq_t Tpos = 720 * (seq_t)41;
    Tpos += 6 * ((seq_t)2812 + 465 * pow2n) * ns;
    Tpos += 2 * ((seq_t)2627 + 45 * pow2n) * n2;
    Tpos += 765 * n3;
    Tpos += 145 * n4;
    Tpos += 3 * n5;
    Tpos += n6;

    seq_t Tneg = 720 * (seq_t)5 * pow2n3;
    seq_t numerator = Tpos - Tneg;
    return numerator / 720;
}
