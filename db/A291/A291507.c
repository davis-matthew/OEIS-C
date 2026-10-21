/*
 * A291507  a(n) = (n!)^9 * Sum_{i=1..n} 1/i^9.
 * https://oeis.org/A291507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!)^9 * Sum_{i=1..n} 1/i^9 Recurrence (for n > 1): a(0)
 *            = 0, a(1) = 1, a(n) = [(n-1)^9 + n^9] * a(n-1) - (n-1)^18 *
 *            a(n-2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifndef SEQ_A291507_H
#define SEQ_A291507_H

seq_t a(idx_t n)
{
    /* handle the offset 0 explicitly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0; /* a(n-2) */
    seq_t prev1 = (seq_t)1; /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t km1 = (seq_t)(i - 1); /* (n-1) as seq_t */
        seq_t k   = (seq_t)i;       /* n as seq_t */

        /* compute (n-1)^9 and n^9 */
        seq_t pow_km1 = (seq_t)1;
        seq_t pow_k   = (seq_t)1;
        for (int j = 0; j < 9; ++j) {
            pow_km1 *= km1;
            pow_k   *= k;
        }

        /* sum of the two 9th powers */
        seq_t sum_pow = pow_km1 + pow_k;

        /* (n-1)^18 is (n-1)^9 squared */
        seq_t pow_km1_sq = pow_km1 * pow_km1;

        /* recurrence: a(n) = sum_pow * a(n-1) - pow_km1_sq * a(n-2) */
        cur = sum_pow * prev1 - pow_km1_sq * prev2;

        /* shift previous values for next iteration */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1; /* a(n) */
}

#endif /* SEQ_A291507_H */
