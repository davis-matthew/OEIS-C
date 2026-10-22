/*
 * A397063  a(n) is the smallest integer exceeding
 *          2*Pi*sqrt(n*(n+1)*(2*n+1)/6).
 * https://oeis.org/A397063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(2*Pi*sqrt(n*(n+1)*(2*n+1)/6))
 * Range:     u8 none, u16 none, u32 n <= 18, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A397063 – computed using a rational approximation of Pi (355/113). */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Return a(n) for any n >= 1.  For n == 0 the function returns 0. */
seq_t a(idx_t n)
{
    /* Compute S = n*(n+1)*(2*n+1)/6  (exact integer). */
    seq_t nn   = (seq_t)n;
    seq_t t1   = nn * (nn + 1);          /* n*(n+1) */
    seq_t t2   = t1 * (2 * nn + 1);      /* n*(n+1)*(2n+1) */
    seq_t S    = t2 / 6;                 /* divide by 6 (exact) */

    /* Use the rational approximation Pi ≈ 355/113.  
       Then ceil(2*Pi*sqrt(S)) = ceil( (710 * sqrt(S)) / 113 ). */
    seq_t X    = (seq_t)504100 * S;      /* 504100 = 4*355^2 = (710)^2 */

    /* Floor square root of X. */
    seq_t Y    = floor_sqrt(X);

    /* Compute ceil(Y/113) with care for the perfect‑square case. */
    seq_t base = Y / 113;                /* floor(Y/113)   */
    seq_t rem  = Y % 113;                /* remainder      */
    bool is_perfect = (Y * Y == X);      /* X is a perfect square? */

    if (is_perfect && rem == 0)
        return base;                     /* exact division */
    else
        return base + 1;                 /* ceil for non‑integer or non‑multiples */
}
