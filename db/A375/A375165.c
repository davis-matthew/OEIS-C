/*
 * A375165  Expansion of 1/((1 - x)^6 - 2*x^6).
 * https://oeis.org/A375165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/6⌋} 2^k * binomial(n+5, 6*k+5)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 31 (2026-11-01)
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the utilities pow2 and binomial from the utils directory. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;                /* accumulator for the sum */
    idx_t  kk, kmax;
    idx_t  nplus5 = n + 5;        /* common upper index for binomial */

    /* floor division: number of terms in the sum */
    kmax = n / 6;
    for (kk = 0; kk <= kmax; ++kk) {
        idx_t  m = 6 * kk + 5;                     /* lower index for binomial */
        seq_t  bin = binomial(nplus5, m);          /* binomial coefficient */
        seq_t  pow = pow2(kk);                     /* 2^k */
        sum += bin * pow;                          /* add current term */
    }
    return sum;
}
