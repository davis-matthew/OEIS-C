/*
 * A309880  Sum of the odd parts appearing among the fourth largest parts in
 *          the partitions of n into 5 parts.
 * https://oeis.org/A309880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A309880: Sum of the odd parts appearing among the fourth largest parts
   in the partitions of n into 5 parts.
   Formula (given in OEIS):
   a(n) = ⌊ (2*n^5 + 25*n^4 + 100*n^3 - 80*n^2 + 5040*n + 81125)/128000
           + ((n mod 2)*(n^2+5*n)*41)/12800
           - (((n^2+n+2) mod 4)*n)/128
           + (n mod 5==0 ? n/50 : 0)
           + (-1)^⌊n/4⌋ * n/32
           - (-1)^⌊n/5⌋ * ((2*n^4+4*n^2) mod 5)*n/50
           - ((n mod 8)==4)/2 ⌋               */
/* The implementation evaluates the integer numerator with denominator
   128000, using signed __int128 for all intermediate calculations.         */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to signed intermediate type */
    __int128 nn = (int64_t)n;

    /* Powers of n */
    __int128 n2 = nn * nn;          /* n^2 */
    __int128 n3 = n2 * nn;          /* n^3 */
    __int128 n4 = n3 * nn;          /* n^4 */
    __int128 n5 = n4 * nn;          /* n^5 */

    /* term A: (2*n^5 + 25*n^4 + 100*n^3 - 80*n^2 + 5040*n + 81125) / 128000 */
    __int128 termA = 2 * n5 + 25 * n4 + 100 * n3 - 80 * n2
                     + (__int128)5040 * nn + (__int128)81125;

    /* term B: ((n mod 2)*(n^2+5*n)*41)/12800  →  *10 when moved to denom 128000 */
    __int128 termB = ((nn & 1) != 0)
                     ? (nn & 1) * (n2 + 5 * nn) * 41 * 10
                     : 0;

    /* term C: -(((n^2+n+2) mod 4)*n)/128   →  *1000 when moved to denom 128000 */
    __int128 mod4  = (n2 + nn + 2) & 3;  /* (mod 4) */
    __int128 termC = -mod4 * nn * 1000;

    /* term D: [n mod 5 == 0]*n/50   →  *2560 */
    __int128 termD = (n % 5 == 0) ? nn * 2560 : 0;

    /* term E: (-1)^⌊n/4⌋ * n/32   →  *4000 */
    __int128 signE = ((nn / 4) & 1) ? -1 : 1;  /* (-1)^floor(n/4)  */
    __int128 termE = signE * nn * 4000;

    /* term F: -(-1)^⌊n/5⌋ * ((2*n^4+4*n^2) mod 5)*n/50   →  *2560 */
    __int128 coeffF = (2 * n4 + 4 * n2) % (__int128)5;
    /* sign of the whole term: -(-1)^floor(n/5) = (-1)^(floor(n/5)+1) */
    __int128 signF = ((nn / 5) & 1) ? 1 : -1;   /* +1 if floor(n/5) odd, -1 if even */
    __int128 termF = signF * coeffF * nn * 2560;

    /* term G: -[(n mod 8)==4]/2   →  *64000 (negative) */
    __int128 termG = (n % 8 == 4) ? -64000 : 0;

    /* Sum all terms to form the numerator with common denominator 128000 */
    __int128 numer = termA + termB + termC + termD + termE + termF + termG;

    /* Final division by 128000 (numerator is non‑negative). */
    seq_t result = (seq_t)(numer / (__int128)128000);
    return result;
}
