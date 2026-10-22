/*
 * A371818  a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n-2*k,n-3*k).
 * https://oeis.org/A371818
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (-1)^k * binomial(2*n-2*k, n-3*k)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 3;
    seq_t sum_even = 0;
    seq_t sum_odd = 0;

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t top   = 2 * n - 2 * k;
        idx_t bot   = n - 3 * k;
        seq_t term  = binomial(top, bot);

        if ((k & 1) == 0) {
            sum_even += term;
        } else {
            sum_odd += term;
        }
    }

    /* The final value is sum_even - sum_odd; the sequence is always non‑negative. */
    return sum_even >= sum_odd ? sum_even - sum_odd : 0;
}
