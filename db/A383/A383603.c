/*
 * A383603  Expansion of 1/( (1-x)^2 * (1-x-9*x^2) )^(1/3).
 * https://oeis.org/A383603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (-9)^k * binomial(-1/3,k) *
 *            binomial(n-k,k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* We use the identity (-9)^k * binomial(-1/3,k) = (3^k * ∏_{i=0}^{k-1}(1+3i)) / k! */
/* The recurrence t_k = (3 * (1+3(k-1)) * t_{k-1}) / k generates this factor exactly. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t t_prev = 1;              /* t_0 = 1 */
    idx_t maxk = n / 2;            /* floor(n/2) */

    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin = binomial(n - k, k);   /* binomial(n-k, k) */
        sum = sum + t_prev * bin;         /* add current term */

        if (k == maxk)                    /* finished */
            break;

        /* compute next t_k: t_{k+1} = t_k * 3 * (1 + 3k) / (k+1) */
        seq_t mult = t_prev * 3 * (1 + 3 * k);
        t_prev = mult / (k + 1);          /* exact division */
    }

    return sum;
}
