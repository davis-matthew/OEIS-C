/*
 * A376803  Expansion of 1/((1 - x)^3 - 9*x^2)^(1/3).
 * https://oeis.org/A376803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} (-9)^k * binomial(-1/3,k) *
 *            binomial(n+k,n-2k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

//          = Sum_{k=0..⌊n/2⌋} (3^k * P_k / k!) * binomial(n+k,n-2k)
//          where P_k = ∏_{j=0}^{k-1} (3j+1)
//          The factor 3^k * P_k / k! can be computed recursively
//          as fact_k = fact_{k-1} * 3 * (3k-2) / k, with fact_0 = 1.
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Sum over k = 0..floor(n/2) */
    seq_t sum     = 0;   /* running sum of terms */
    seq_t fact    = 1;   /* fact_k for current k */
    idx_t max_k   = n / 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        /* binomial coefficient binomial(n+k, n-2k) */
        idx_t a    = n + k;
        idx_t b    = n - 2 * k;
        seq_t binom = binomial(a, b);

        /* term = fact_k * binomial(...) */
        seq_t term = fact * binom;
        sum += term;

        /* Update fact for next k */
        if (k < max_k) {
            /* fact_{k+1} = fact_k * 3 * (3(k+1)-2) / (k+1) */
            idx_t mult = 3 * (3 * (k + 1) - 2);
            fact = fact * (seq_t)mult / (seq_t)(k + 1);
        }
    }
    return sum;
}
