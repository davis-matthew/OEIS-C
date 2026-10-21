/*
 * A299784  Maximum size of a main class for diagonal Latin squares of order n
 *          with the first row in ascending order.
 * https://oeis.org/A299784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A299784
   Formula: a(n) = 1                      for n = 1
              0                      for n = 2, 3
              2                      for n = 4
              4                      for n = 5
              96                     for n = 6
              2^⌊n/2⌋ · (⌊n/2⌋)! · 4                          for n ≥ 7
   where ⌊n/2⌋ is the integer part of n/2. */
   
#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* small fixed values for n ≤ 6 */
    if (n <= 6) {
        /* Index 0 is unused; entries 1…6 correspond to the table values */
        static const seq_t special[7] = {
            0,          /* dummy */
            1,          /* a(1) */
            0,          /* a(2) */
            0,          /* a(3) */
            2,          /* a(4) */
            4,          /* a(5) */
            96          /* a(6) */
        };
        return special[(idx_t) n];
    }

    /* For n ≥ 7 */
    idx_t m = n >> 1;               /* floor(n/2) */

    /* Compute m! */
    seq_t res = factorial(m);

    /* Multiply by 2^m and by 4 */
    res *= pow2(m);
    res *= (seq_t) 4;

    return res;
}
