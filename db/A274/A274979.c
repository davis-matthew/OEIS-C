/*
 * A274979  Integers of the form m*(m + 7)/8.
 * https://oeis.org/A274979
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*(n-1)*n - 3*(2*n-1)*(-1)^n - 3)/4
 * Range:     u8 n <= 6, u16 n <= 112, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A274979: integers of the form m*(m+7)/8. */

seq_t a(idx_t n)
{
    /* The formula holds for n >= 1. For n == 0 (outside the offset) we return 0. */
    if (n == 0) return 0;

    seq_t nn   = (seq_t)n;            /* cast index to the output type */
    seq_t term1 = (8 * (nn - 1)) * nn;/* 8*(n-1)*n  */
    seq_t term2 = 3 * (2 * nn - 1);   /* 3*(2*n-1)  */
    seq_t num;

    if ((n & 1) == 0) {               /* n even: (-1)^n = 1  */
        num = term1 - term2 - 3;
    } else {                          /* n odd:  (-1)^n = -1 */
        num = term1 + term2 - 3;
    }

    return num / 4;                    /* division is exact */
}
