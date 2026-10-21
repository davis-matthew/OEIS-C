/*
 * A292644  Rank of (6-r)*n when all the numbers (6-r)*j and (6+r)*k, where r
 *          = sqrt(2), j>=1, k>=1, are jointly ranked.
 * https://oeis.org/A292644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 158, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(n) = n + floor( ((19 - 6*sqrt(2))/17) * n )
 *
 *   Let γ = (19 - 6*sqrt(2))/17  ≈ 0.61803449.
 *   floor(γ*n) can be computed without floating point as
 *     floor( (19*n - floor(6*n*sqrt(2))) / 17 )
 *     with a correction when the numerator is a multiple of 17.
 *
 *   The term floor(6*n*sqrt(2)) is obtained via integer square root:
 *     floor(6*n*sqrt(2)) = floor( sqrt( 2 * (6*n)^2 ) ).
 */

#include <stddef.h>  /* for size_t */

static idx_t integer_sqrt(idx_t x)
{
    /* Integer square root: floor(sqrt(x)) for non‑negative x. */
    idx_t low = 0, high = x;
    idx_t mid, best = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid * mid <= x) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined, but keep safe */
    /* Compute floor(6*n*sqrt(2)) */
    idx_t mn = 6 * n;                       /* 6 * n (safe in idx_t) */
    idx_t t  = integer_sqrt((idx_t)2 * mn * mn); /* floor( sqrt(2) * (6n) ) */

    /* Numerator of the Beatty floor */
    idx_t k   = 19 * n - t;
    idx_t quo = k / 17;
    if (k % 17 == 0) quo -= 1;              /* correction for exact multiple */

    idx_t rank = n + quo;                   /* final rank */
    return (seq_t)rank;
}
