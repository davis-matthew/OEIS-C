/*
 * A374010  a(n) = (1 + (n+8)^2 + (n-9)*(-1)^n)/2.
 * https://oeis.org/A374010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + (n+8)^2 + (n-9)*(-1)^n)/2 (offset 1)
 * Range:     u8 n <= 7, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n is guaranteed >= 1. */
    seq_t nn = (seq_t)n;                 /* cast index to seq_t for arithmetic */

    /* Compute (n+8)^2 */
    seq_t tmp = nn + 8;
    seq_t sq  = tmp * tmp;

    /* Start with 1 + (n+8)^2 */
    seq_t numerator = sq + 1;

    bool even = ((n & 1) == 0);

    if (even) {
        /* even n: add (n-9) if n>=9, else subtract (9-n) */
        if (n < 9) {
            seq_t diff = (seq_t)(9 - n); /* diff is small (<=8) */
            numerator = numerator - diff;
        } else {
            seq_t diff = (seq_t)(n - 9);
            numerator = numerator + diff;
        }
    } else {
        /* odd n: add (9-n) if n<9, else subtract (n-9) */
        if (n < 9) {
            seq_t diff = (seq_t)(9 - n);
            numerator = numerator + diff;
        } else {
            seq_t diff = (seq_t)(n - 9);
            numerator = numerator - diff;
        }
    }

    /* Divide by 2 (the result is always an integer) */
    return numerator / 2;
}
