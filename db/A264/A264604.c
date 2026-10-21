/*
 * A264604  a(n) = smallest square which is a concatenation of n and m, where
 *          m is a decimal number.
 * https://oeis.org/A264604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal square whose decimal representation is a
 *            concatenation of the decimal digits of n and a decimal number m,
 *            where m may be 0 but not a multiple‑digit number starting with 0
 * Range:     u8 n <= 4, u16 n <= 22, u32 n <= 6965, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10, sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"      /* pow10(idx_t) → 10^k as seq_t */
#include "../utils/sqrt_floor.h" /* sqrt_floor(seq_t) → ⌊√x⌋ */

/* Return the smallest perfect square that starts with the decimal
   representation of n followed by a valid m (m may be 0 but no leading
   zeros).  The sequence is 1‑based: a(1) corresponds to n = 1. */
seq_t a(idx_t n)
{
    /* Satisfy the offset requirement: a(1) is the first term.  For
       n == 0 the sequence is undefined; return 0 as a sentinel. */
    if (n == 0)
        return (seq_t)0;

    /* Iterate over the possible number of digits of m.  For each count
       we form the admissible interval of concatenated numbers and
       test whether it contains a perfect square.  When one is found
       we return it immediately. */
    for (idx_t d = 1;; ++d)
    {
        seq_t lower, upper;

        if (d == 1)
        {
            seq_t pow10d = pow10(1);                       /* 10^1 = 10 */
            lower = (seq_t)n * pow10d;                     /* n·10          */
            upper = lower + pow10d - (seq_t)1;             /* n·10 + 9      */
        }
        else
        {
            seq_t pow10d  = pow10(d);                      /* 10^d           */
            seq_t pow10d1 = pow10(d - 1);                  /* 10^(d-1)       */
            lower = (seq_t)n * pow10d + pow10d1;           /* n·10^d + 10^(d-1) */
            upper = (seq_t)n * pow10d + pow10d - (seq_t)1;/* n·10^d + 10^d - 1 */
        }

        /* Compute the smallest integer k such that k² ≥ lower. */
        seq_t k = sqrt_floor(lower);
        if (k * k < lower)
            k = k + (seq_t)1;

        seq_t square = k * k;
        if (square <= upper)
            return square;

        /* Sufficient break: once 10^d becomes astronomically large
           the multiplication above would overflow on 64‑bit builds,
           but in arbitrary‑precision builds the loop will simply
           continue until a square is found.  Here we guard against
           an infinite loop on a 64‑bit build. */
        if (d > 20)   /* 10^20 > 2^63, safe upper bound for 64‑bit types */
            break;
    }

    /* Theoretically unreachable: every n has a solution.  Return 0 as
       a sentinel to satisfy the compiler. */
    return (seq_t)0;
}
