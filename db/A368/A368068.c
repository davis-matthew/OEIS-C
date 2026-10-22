/*
 * A368068  a(n) = Product_{i=1..n, j=1..n} (2*i^2 + 5*i*j + 2*j^2).
 * https://oeis.org/A368068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n , j=1..n} (i+2*j)*(2*i+j)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

/* Return a(n) for sequence A368068 */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    if (n == 0)
        return result;

    idx_t max_m = 3 * n;
    for (idx_t m = 3; m <= max_m; ++m) {
        /* Count pairs (i,j) with 1 ≤ i,j ≤ n and i + 2j = m */
        idx_t lower;
        if (m <= n) {
            lower = 1;
        } else {
            lower = (m - n + 1) / 2; /* ceil((m-n)/2) */
        }
        idx_t upper = (m - 1) / 2;   /* floor((m-1)/2) */
        if (upper > n)
            upper = n;

        idx_t freq = 0;
        if (upper >= lower)
            freq = upper - lower + 1;

        if (freq == 0)
            continue;

        idx_t exponent = 2 * freq;            /* both (i+2j) and (2i+j) */
        seq_t mul_val = big_pow((seq_t)m, (seq_t)exponent);
        result = result * mul_val;
    }
    return result;
}
