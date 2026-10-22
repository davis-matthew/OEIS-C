/*
 * A392312  a(n) = Sum_{k=0..floor(3*n/8)} (k+1) * binomial(k,3*n-8*k).
 * https://oeis.org/A392312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(3*n/8)} (k+1) * binomial(k, 3*n-8*k)
 * Range:     u8 n <= 19, u16 n <= 41, u32 n <= 84, u64 n <= 172,
 *            u128 n <= 351, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binomial(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k > n - k)
        k = n - k;
    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* Multiply before dividing; division is exact for binomial */
        result = result * (seq_t)(n - k + i) / (seq_t)i;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = (3 * n) / 8;   /* floor(3*n/8) */
    for (idx_t k = 0; k <= kmax; ++k)
    {
        idx_t rem = 3 * n - 8 * k;
        if (rem > k)
            continue; /* binomial will be zero, skip */
        seq_t term = binomial(k, rem);
        term = term * (seq_t)(k + 1);
        sum += term;
    }
    return sum;
}
