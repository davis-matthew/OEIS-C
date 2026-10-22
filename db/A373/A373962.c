/*
 * A373962  Number of compositions of 6*n-2 into parts 5 and 6.
 * https://oeis.org/A373962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor((n-2)/5)} binomial(n+k, n-2-5k)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 29, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(1) = 0, and a(0) is undefined – return 0 for safety */
    if (n < 2) {
        return (seq_t)0;
    }

    idx_t max_k = (n - 2) / 5;   /* largest k with n-2-5k >= 0 */
    seq_t sum = (seq_t)0;

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t upper = n + k;
        idx_t lower = n - 2 - 5 * k;   /* guaranteed non‑negative */
        sum += binomial(upper, lower);
    }

    return sum;
}
