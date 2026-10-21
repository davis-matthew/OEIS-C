/*
 * A280308  Tribonacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) with a(0)=3,
 *          a(1)=4, a(2)=5.
 * https://oeis.org/A280308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3); a(0)=3, a(1)=4, a(2)=5
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)5;

    /* Iteratively compute the Tribonacci numbers up to n */
    seq_t first  = (seq_t)3;  /* a(0) */
    seq_t second = (seq_t)4;  /* a(1) */
    seq_t third  = (seq_t)5;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = first + second + third;
        first  = second;
        second = third;
        third  = next;
    }
    return third;
}
