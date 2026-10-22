/*
 * A393696  a(n) is the number of unit squares in the square lattice, divided
 *          by 4, that have 3 vertices strictly inside and 1 vertex strictly
 *          outside the circle x^2 + y^2 = n.
 * https://oeis.org/A393696
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (number of unit squares in the integer lattice with 3
 *            vertices strictly inside and 1 vertex strictly outside the
 *            circle x^2 + y^2 = n) / 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   uint_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/uint_sqrt.h"

seq_t a(idx_t n)
{
    /* Intuition: iterate over all unit squares whose corner coordinates
       lie within a square that surely covers every square that can
       intersect the circle of radius sqrt(n).  The minimal bound for
       the coordinates i,j of the lower‑left corner is
          -(⌊√n⌋+1) ≤ i,j ≤ ⌊√n⌋−1 .
       For each such square we count how many of its four vertices are
       strictly inside the circle (distance squared < n).  Squares with
       exactly 3 inside vertices and no vertex exactly on the circle
       are counted.  Finally divide the raw count by 4 to match the
       definition.  The 2‑D loop uses at most (2⌊√n⌋+1)² iterations,
       which is well below the time limits for n up to about 10⁴. */
    
    idx_t count = 0;               /* number of qualifying squares */
    idx_t radius2 = n;             /* radius squared */
    idx_t r = uint_sqrt(radius2);  /* floor(sqrt(n)) */
    
    long long min = -(long long)r - 1;   /* inclusive lower bound */
    long long max =  (long long)r - 1;   /* inclusive upper bound */
    
    for (long long i = min; i <= max; ++i) {
        /* compute |i| and |i+1| once per row */
        idx_t ax = (i >= 0) ? (idx_t)i : (idx_t)(-i);
        idx_t ax1 = ((i + 1) >= 0) ? (idx_t)(i + 1) : (idx_t)(-(i + 1));
        for (long long j = min; j <= max; ++j) {
            idx_t ay  = (j >= 0) ? (idx_t) j  : (idx_t)(-j);
            idx_t ay1 = ((j + 1) >= 0) ? (idx_t)(j + 1) : (idx_t)(-(j + 1));
            
            idx_t d00 = ax * ax + ay * ay;   /* (i,   j)   */
            idx_t d10 = ax1 * ax1 + ay * ay; /* (i+1, j)   */
            idx_t d01 = ax * ax + ay1 * ay1; /* (i,   j+1) */
            idx_t d11 = ax1 * ax1 + ay1 * ay1; /* (i+1, j+1) */
            
            int inside = 0;
            bool on_circle = false;
            
            /* check each vertex */
            if (d00 == radius2)        on_circle = true;
            else if (d00 < radius2)    ++inside;
            
            if (d10 == radius2)        on_circle = true;
            else if (d10 < radius2)    ++inside;
            
            if (d01 == radius2)        on_circle = true;
            else if (d01 < radius2)    ++inside;
            
            if (d11 == radius2)        on_circle = true;
            else if (d11 < radius2)    ++inside;
            
            if (inside == 3 && !on_circle)
                ++count;
        }
    }
    
    return (seq_t)(count / 4);
}
