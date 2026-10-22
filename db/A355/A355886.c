/*
 * A355886  a(n) = n! * Sum_{k=1..n} floor(n/k)/k!.
 * https://oeis.org/A355886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=1..n} floor(n/k)/k!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: iterate k from n down to 1, maintaining product = n!/k! */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t product = 1;   /* product = n! / k! for current k   */
    seq_t sum     = 0;
    idx_t k;
    for (k = n; k >= 1; --k) {
        seq_t term = product * (n / k); /* floor(n/k) is an integer */
        sum += term;
        product *= k;                   /* update product for next k-1 */
    }
    return sum;
}
