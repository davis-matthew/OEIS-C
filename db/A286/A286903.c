/*
 * A286903  {0->00}-transform of the Sturmian word A080764.
 * https://oeis.org/A286903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*r) - floor((n+2)*r) + 1, where r = (3 -
 *            sqrt(2))/7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9871 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(n*(3 - sqrt(2))/7) exactly using only integer arithmetic.
 * This value is floor(n*r) where r = (3 - sqrt(2))/7. */
static seq_t floor_alpha(idx_t n)
{
    /* Represent n*(3 - sqrt(2)) as x - y*sqrt(2) with x = 3n, y = n. */
    unsigned __int128 x = (unsigned __int128)n * 3;
    unsigned __int128 y = (unsigned __int128)n;

    /* Binary search for the integer k such that
     *   k <= (x - y*sqrt(2))/7 < k+1.
     * Using the equivalence:
     *   y*sqrt(2) <= x - 7k
     *   y*sqrt(2) > x - 7(k+1)
     * Squaring eliminates sqrt(2) (since y > 0). */
    unsigned __int128 low = 0;
    unsigned __int128 high = x / 7 + 1;   /* exclusive upper bound */

    while (low < high) {
        unsigned __int128 mid = (low + high) >> 1;
        unsigned __int128 val = x - 7 * mid;      /* >= 0 for our bounds */
        unsigned __int128 lhs = y * y * 2;        /* 2*y^2  */
        unsigned __int128 rhs = val * val;       /* (x - 7mid)^2 */

        if (lhs <= rhs)   /* mid is <= floor((x - y*sqrt(2))/7) */
            low = mid + 1;
        else
            high = mid;
    }

    unsigned __int128 floor_val = low - 1;
    return (seq_t)floor_val;
}

seq_t a(idx_t n)
{
    /* a(n) = 1 if floor(n*r) == floor((n+2)*r), otherwise 0. */
    seq_t f1 = floor_alpha(n);
    seq_t f2 = floor_alpha(n + 2);
    return (f1 == f2) ? (seq_t)1 : (seq_t)0;
}
