/*
 * A282084  Number of n-element subsets of [n+8] having an even sum.
 * https://oeis.org/A282084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            floor((n+2)(n+4)(n+6)(n+8)*(n^4+16*n^3+86*n^2+176*n+210)/80640 *
 *            - ((n^2+n) mod 4)*(n^4+18*n^3+113*n^2+288*n+244)/768 * -
 *            ((n^2+3*n) mod 4)*(2*n^3+27*n^2+112*n)/768)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282084: Number of n‑element subsets of [n+8] having an even sum. */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned __int128 un = n;                 /* promote to 128‑bit */
    unsigned __int128 n2 = un * un;           /* n^2 */
    unsigned __int128 n3 = n2 * un;           /* n^3 */
    unsigned __int128 n4 = n2 * n2;           /* n^4 */

    /* (n+2)(n+4)(n+6)(n+8) */
    unsigned __int128 term1 = (un + 2) * (un + 4) * (un + 6) * (un + 8);

    /* Polynomial A: n^4+16*n^3+86*n^2+176*n+210 */
    unsigned __int128 polyA = n4 + 16 * n3 + 86 * n2 + 176 * un + 210;
    unsigned __int128 numer1 = term1 * polyA;          /* numerator for the first term */

    /* Subtract first fractional part */
    unsigned int m1 = (unsigned int)((n2 + un) & 3);   /* (n^2 + n) mod 4 */
    unsigned __int128 polyB = n4 + 18 * n3 + 113 * n2 + 288 * un + 244;
    unsigned __int128 sub1 = (unsigned __int128)m1 * polyB * 105;   /* 105 = 80640/768 */

    /* Subtract second fractional part */
    unsigned int m2 = (unsigned int)((n2 + 3 * un) & 3);   /* (n^2 + 3*n) mod 4 */
    unsigned __int128 polyC = 2 * n3 + 27 * n2 + 112 * un;
    unsigned __int128 sub2 = (unsigned __int128)m2 * polyC * 105;

    /* Combine everything (common denominator 80640) */
    unsigned __int128 num = numer1 >= sub1 ? numer1 - sub1 : 0;
    if (sub2 > num) num = 0;
    else            num -= sub2;

    unsigned __int128 result128 = num / 80640u;  /* floor division */

    return (seq_t)result128;
}
