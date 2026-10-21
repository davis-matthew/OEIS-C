/*
 * A292663  Rank of n*(e-1) when all the numbers j*(e-1) and k*e, for j>=1,
 *          k>=1, are jointly ranked.
 * https://oeis.org/A292663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - 1 - floor(n/e) (n >= 1)
 * Range:     u8 n <= 156, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes floor(n/e) without floating point by using a high‑precision rational
 * approximation of e.  The constant SCALE is 10^15, and DEN = floor(e*SCALE).
 * The values are handled with unsigned __int128 for intermediate products to
 * avoid overflow.  The function works for any idx_t and seq_t type defined by
 * the build. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* High‑precision approximation of e:  e ≈ 2718281828459045 / 10^15  */
    static const unsigned __int128 SCALE = 1000000000000000ULL;          /* 10^15   */
    static const unsigned __int128 DEN     = 2718281828459045ULL;        /* floor(e*S) */
    static const unsigned __int128 DEN_UP  = DEN + 1ULL;                 /* ceil(e*S)  */

    /* Compute an initial guess for floor(n/e) using the rational approximation. */
    unsigned __int128 num = (unsigned __int128)n * SCALE;
    uint64_t k = (uint64_t)(num / DEN);   /* initial approximation */

    /* Adjust k upwards or downwards to ensure the inequality k ≤ n/e < k+1. */
    while (1)
    {
        /* k * DEN  >  n * SCALE  --> k too large */
        if ((unsigned __int128)k * DEN > num)
        {
            if (k == 0) break;      /* cannot go lower than 0 */
            --k;
            continue;
        }
        /* (k+1) * DEN_UP  ≤  n * SCALE  --> k too small */
        if ((unsigned __int128)(k + 1) * DEN_UP <= num)
        {
            ++k;
            continue;
        }
        break;   /* correct k found */
    }

    /* a(n) = 2*n - 1 - k */
    return (seq_t)(2 * (seq_t)n - 1 - (seq_t)k);
}
