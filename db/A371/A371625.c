/*
 * A371625  The x-coordinate of the point (x,y) where x + y = n, x is an
 *          integer, and x/y is as close as possible to phi (by absolute
 *          difference).
 * https://oeis.org/A371625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   floor_sqrt, lower_better
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A371625
 *  a(n) is the integer x∈[0,n] minimizing |x/(n−x)−φ|,  φ=(1+√5)/2.
 *  The algorithm uses the analytic identities:
 *      φ/(φ+1) = (√5−1)/2
 *  and compares the two candidates floor(n·(√5−1)/2) and
 *      ceil(n·(√5−1)/2)  (i.e. the only two integers that can minimise
 *      the distance).  The comparison is done exactly by integer arithmetic
 *      with 128‑bit intermediates, avoiding floating point.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"    /* floor_sqrt(seq_t x) */
#include "../utils/lower_better.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n≥1; return 0 for n=0 (unused). */
    if (n <= 1)
        return 0;

    /* prod = 5·n²  (fits comfortably in seq_t for the required n). */
    seq_t prod = (seq_t)5 * (seq_t)n * (seq_t)n;
    seq_t root = floor_sqrt(prod);                     /* floor(√(5·n²)) */

    /* x0 = floor( (root − n) / 2 ) = floor( n·(√5−1)/2 ) */
    seq_t x0 = (root > n) ? (root - n) / 2 : 0;
    if (x0 >= n)
        x0 = n - 1;                                    /* safety guard */

    /* x0+1 would be invalid if it equals n (division by zero). */
    if (x0 + 1 == n)
        return x0;

    /* Choose between x0 and x0+1 by exact comparison. */
    return lower_better(x0, (seq_t)n) ? x0 : x0 + 1;
}
