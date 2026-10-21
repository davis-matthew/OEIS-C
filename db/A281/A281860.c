/*
 * A281860  Curious identities based on the Armstrong number 371 =
 *          A005188(12).
 * https://oeis.org/A281860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 10^n + 100^n + 1000^n) / 3
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The pow10 function is available in the utils library. */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute the three powers of ten required by the formula. */
    seq_t p1 = pow10(n);          /* 10^n          */
    seq_t p2 = pow10(n + n);      /* 10^(2n) = 100^n */
    seq_t p3 = pow10(n + n + n);  /* 10^(3n) = 1000^n */

    /* Sum the terms and divide by 3.  The division is exact. */
    seq_t sum = (seq_t)3 + p1 + p2 + p3;
    return sum / (seq_t)3;
}
