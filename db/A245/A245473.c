/*
 * A245473  Nearest integer to 2^log(n).
 * https://oeis.org/A245473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to 2^log(n) (log = natural logarithm, n
 *            >= 1)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes 2^log(n) using double arithmetic and rounds to the nearest integer.
   It converts the result to a long long and then to seq_t, which is compatible with
   both fixed‑width and arbitrary‑precision integer types. */
/* If n == 0, the function returns 0 as an unused default. */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* offset is 1, n<1 is not used */

    /* Compute 2^ln(n) as pow(2.0, log((double)n)) using double precision. */
    double val = pow(2.0, log((double)n));

    /* Round to nearest integer using +0.5 trick and convert to long long. */
    long long x = (long long)(val + 0.5);

    /* Return the value as seq_t; seq_t supports construction from long long. */
    return (seq_t)x;
}
