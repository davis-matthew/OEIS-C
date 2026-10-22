/*
 * A386719  a(n) = Sum_{k=0..n} 2^k * binomial(4*n,k) * binomial(4*n-k-1,n-k).
 * https://oeis.org/A386719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 3^k * (-1)^(n-k) * binom(4*n, k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* We compute the sum using an iterative binomial recurrence.
       binom := binom(4*n, k) is updated as
          binom_{k+1} = binom_k * (4*n - k) / (k+1). */

    seq_t binom = 1;           /* C(4*n, 0) */
    seq_t pow3  = 1;           /* 3^0       */
    seq_t pos   = 0;           /* sum of positive terms */
    seq_t neg   = 0;           /* sum of negative terms */

    bool positive = ((n & 1) == 0);   /* (-1)^(n-k) is + when n even, - when n odd */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = binom * pow3;
        if (positive)
            pos += term;
        else
            neg += term;

        if (k == n) /* last term; no further update needed */
            break;

        /* Prepare binomial for next k */
        binom = binom * ( (seq_t)(4 * n - k) ) / (k + 1);
        pow3  *= 3;
        positive = !positive;           /* flip sign for next term */
    }

    return pos - neg;
}
