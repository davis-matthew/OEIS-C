/*
 * A290278  a(n) = Sum_{k=0..n} (A007953(5*k) - A007953(k)).
 * https://oeis.org/A290278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Sum_{k=0..n} (digit_sum(5*k) - digit_sum(k))
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t sum5 = 0;
    seq_t sum1 = 0;

    /* iterate over k from 0 to n inclusive */
    for (idx_t k = 0; ; ++k)
    {
        seq_t km  = (seq_t)k;     /* k as seq_t */
        seq_t f   = km * (seq_t)5;/* 5*k as seq_t */

        sum5 += digit_sum(f);    /* sum of digits of 5*k */
        sum1 += digit_sum(km);   /* sum of digits of k */

        if (k == n) break;       /* finished */
    }

    return sum5 - sum1;          /* difference is non‑negative */
}
