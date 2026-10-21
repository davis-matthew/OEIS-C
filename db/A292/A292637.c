/*
 * A292637  Rank of (3+r)*n when all the numbers (3-r)*j and (3+r)*k, where r
 *          = sqrt(2), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(((11 + 6*sqrt(2))*n)/7)
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from solving (3 - sqrt(2))*j < (3 + sqrt(2))*n for integer j. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /*  √2 rational approximation:  p/q <= √2  */
    const unsigned long long SQRT2_P = 1414213562373095ULL;   /* floor(√2 * 10^15) */
    const unsigned long long SQRT2_Q = 1000000000000000ULL;   /* 10^15 */

    /* Numerator (11 + 6*√2) ≈ (11*Q + 6*P) / Q  */
    const unsigned long long A   = 11ULL * SQRT2_Q + 6ULL * SQRT2_P;
    const unsigned long long DEN = 7ULL * SQRT2_Q;

    /* Compute jmax = floor((A * n) / DEN) using 128‑bit arithmetic */
    unsigned __int128 prod = (unsigned __int128)A * (unsigned __int128)n;
    idx_t jmax = (idx_t)(prod / DEN);

    /* Rank of (3+√2)*n among the merged sequences */
    return (seq_t)n + (seq_t)jmax;
}
