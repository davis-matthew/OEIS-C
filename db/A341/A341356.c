/*
 * A341356  The most significant digit in A097801-base.
 * https://oeis.org/A341356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(n / p) where p is the largest element of the
 *            sequence 1, 2, 6, 30, 210, 1890, 20790, … (products of 2 and
 *            successive odd numbers) that does not exceed n. The returned
 *            value is the most significant digit in the A097801-base
 *            representation of n
 * Range:     u8 n <= 781, u16 n <= 9790, u32 n <= 9790, u64 n <= 9790,
 *            u128 n <= 9790, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t p = 1;                 /* current base value (Ck) */
    idx_t mul = 2;               /* next multiplier: 2, then 3,5,7,9,… */

    /* Find the largest base value <= n */
    while (p <= (seq_t)n) {
        seq_t next = p * (seq_t)mul;
        if (next > (seq_t)n)
            break;
        p = next;
        if (mul == 2)
            mul = 3;
        else
            mul += 2;
    }

    return (seq_t)n / p;
}
