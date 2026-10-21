/*
 * A256532  Product of n and the sum of remainders of n mod k, for k = 1, 2,
 *          3, ..., n.
 * https://oeis.org/A256532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * sum_{k=1..n} (n mod k)
 * Range:     u8 n <= 12, u16 n <= 72, u32 n <= 2892, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation using the identity sum_{k=1}^n (n mod k) = n*n - Σ_k=1^n k*⌊n/k⌋,
   where the sum Σ is evaluated efficiently by grouping equal values of ⌊n/k⌋. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases */
    if (n == 0) return 0;

    seq_t n_seq = (seq_t)n;
    seq_t sum_floor = 0;            /* Σ k*⌊n/k⌋ */
    idx_t i = 1;

    /* Iterate over blocks where ⌊n/k⌋ is constant */
    while (i <= n)
    {
        idx_t v = n / i;                /* current value of ⌊n/k⌋ */
        idx_t last = n / v;             /* last k with same ⌊n/k⌋ */

        /* Sum of consecutive integers from i to last:
           sum_k = (i + last) * (last - i + 1) / 2
        */
        seq_t l = (seq_t)i;
        seq_t r = (seq_t)last;
        seq_t cnt = (seq_t)(last - i + 1);
        seq_t sum_k = (l + r) * cnt / 2;

        sum_floor += (seq_t)v * sum_k;

        i = last + 1;
    }

    /* sum_{k=1}^n (n mod k) = n*n - sum_floor */
    seq_t sum_rema = n_seq * n_seq - sum_floor;

    /* a(n) = n * sum_{k=1}^n (n mod k) */
    return n_seq * sum_rema;
}
