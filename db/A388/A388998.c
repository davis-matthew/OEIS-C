/*
 * A388998  Number of tetrahedra in the n X n queen graph.
 * https://oeis.org/A388998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(183-15*(-1)^n-375*n+215*n^2-45*n^3+7*n^4)/60
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* For n == 0 the offset is 1, but we return 0 for safety */
    if (n == 0)
        return 0;

    bool even = (n & 1ULL) == 0;

    /* Use 128‑bit unsigned arithmetic to avoid overflow for n up to ~10⁴   */
    __uint128_t nn   = (unsigned __int128)n;
    __uint128_t n2   = nn * nn;          /* n^2  */
    __uint128_t n3   = n2 * nn;           /* n^3  */
    __uint128_t n4   = n3 * nn;           /* n^4  */

    /* Positive part of the bracket:
       C = 168 if n even, 198 if n odd (183 ± 15)      */
    __uint128_t pos = even ? 168 : 198;
    pos += 215 * n2;
    pos += 7 * n4;

    /* Negative part of the bracket:
       375*n + 45*n^3                                     */
    __uint128_t neg = 375 * nn + 45 * n3;

    /* The bracket value f = pos - neg (always non‑negative)    */
    __uint128_t f = pos - neg;

    /* Numerator = n * f, then divide by 60                  */
    __uint128_t numer = nn * f;
    seq_t result = (seq_t)(numer / 60);

    return result;
}
