/*
 * A364788  a(0) = 0; thereafter a(n) is the number of times the last digit of
 *          a(n-1) has occurred as last digit in all terms prior to a(n-1).
 * https://oeis.org/A364788
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2495, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0;
   for n >= 1, a(n) is the number of times the last digit of a(n-1)
   has appeared as the last digit of any term a(j) with 0 <= j < n-1. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* counts[d] holds how many times digit d has appeared
       in the terms a(0), a(1), …, a(i-1) during the loop */
    idx_t counts[10] = {0};

    seq_t curr = (seq_t)0;   /* a(0) */
    for (idx_t i = 0; i < n; ++i)
    {
        unsigned d = (unsigned)(curr % (seq_t)10); /* last digit of a(i) */
        seq_t next = (seq_t)counts[d];            /* a(i+1) */
        counts[d] += 1;                           /* include a(i) for future steps */
        curr = next;
    }
    return curr;   /* a(n) */
}
