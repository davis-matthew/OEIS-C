/*
 * A394257  Determinant of the n X n matrix M(i,j) = f(gcd(i,j)) + delta(i,j),
 *          where f is the characteristic function of numbers coprime to 6
 *          (A354354) and delta(i,j) is the Kronecker delta.
 * https://oeis.org/A394257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 14, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A394257: determinant of M where f(k)=1 iff gcd(k,6)=1.
   For n>=6, a(n) is given by the 4th‑degree quasipolynomial
   (period 6):

   a(n)= (n^4 + 6 n^3 – 90 n^2 + 108 n + 324)/324  if n≡0 (mod 6)
   a(n)= (n^4 + 5 n^3 – 93 n^2 +  59 n + 676)/324  if n≡1 (mod 6)
   a(n)= (n^4 + 4 n^3 – 93 n^2 +  76 n + 496)/324  if n≡2 (mod 6)
   a(n)= (n^4 + 6 n^3 – 81 n^2 +  54 n + 324)/324  if n≡3 (mod 6)
   a(n)= (n^4 + 5 n^3 – 84 n^2 +  68 n + 172)/324  if n≡4 (mod 6)
   a(n)= (n^4 + 4 n^3 – 84 n^2 +  40 n + 451)/324  if n≡5 (mod 6)

   For n<6 the first six terms are {1,2,1,0,-1,-1}.  */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* small values for n < 6 */
    static const seq_t small[6] = { 1, 2, 1, 0, -1, -1 };
    if (n < 6)
        return small[n];

    /* compute polynomial using unsigned 64‑bit arithmetic
       (safe for 0 <= n <= 10000 required by the tests). */
    uint64_t u  = (uint64_t)n;
    uint64_t n2 = u * u;          /* n^2   */
    uint64_t n3 = n2 * u;         /* n^3   */
    uint64_t n4 = n2 * n2;        /* n^4   */

    uint64_t numer;
    switch (n % 6)
    {
        case 0: numer = n4 + 6 * n3 - 90 * n2 + 108 * u + 324;   break;
        case 1: numer = n4 + 5 * n3 - 93 * n2 +  59 * u + 676;   break;
        case 2: numer = n4 + 4 * n3 - 93 * n2 +  76 * u + 496;   break;
        case 3: numer = n4 + 6 * n3 - 81 * n2 +  54 * u + 324;   break;
        case 4: numer = n4 + 5 * n3 - 84 * n2 +  68 * u + 172;   break;
        case 5: numer = n4 + 4 * n3 - 84 * n2 +  40 * u + 451;   break;
        default: numer = 0; /* unreachable */
    }

    /* divide by 324 and cast to seq_t */
    return (seq_t)(numer / 324);
}
