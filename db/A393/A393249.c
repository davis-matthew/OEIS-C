/*
 * A393249  a(n) = Sum_{k=0..floor(n/5)} binomial(2*k,k) * binomial(n-4*k,k).
 * https://oeis.org/A393249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/5)} binomial(2*k,k) * binomial(n-4*k,k)
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 52, u64 n <= 99, u128 n <= 194,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* If n < 0 we ignore as not defined */
    if (n == (idx_t)-1) {
        return 0;
    }

    idx_t max_k = n / 5;          /* floor(n/5) */
    seq_t result = 0;
    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t t1 = binomial(2 * k, k);                /* binomial(2k,k)   */
        seq_t t2 = binomial(n - 4 * k, k);            /* binomial(n-4k,k) */
        result += t1 * t2;
    }
    return result;
}
