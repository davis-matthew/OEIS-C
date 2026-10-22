/*
 * A352871  a(n) is the number of iterations, starting with x = n, which can
 *          be made of x -> x/sumdigits(x) with x remaining an integer, or -1
 *          if x remains an integer through infinitely many iterations.
 * https://oeis.org/A352871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   a(n) = number of iterations of the map x → x / sumdigits(x) that
 *            remain integers (0 when the first division fails), or –1 if the
 *            process never terminates (sumdigits(x)=1)
 * Range:     i8 n <= 78, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Convert the 64‑bit index to the sequence type */
    seq_t x = (seq_t)n;
    seq_t count = 0;

    while (1) {
        seq_t s = digit_sum(x);          /* sum of decimal digits of x */
        if (s == 1)                      /* infinite loop, because x / 1 = x */
            return -1;

        if (x % s != 0)                  /* not divisible → terminate */
            return count;

        x = x / s;                       /* next value in the sequence */
        count++;                         /* one successful iteration */
    }
}
