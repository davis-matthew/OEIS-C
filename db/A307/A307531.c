/*
 * A307531  a(n) is the greatest sum i + j + k + l where i^2 + j^2 + k^2 + l^2
 *          = n and 0 <= i <= j <= k <= l.
 * https://oeis.org/A307531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max{i+j+k+l | i^2+j^2+k^2+l^2 = n, 0 ≤ i ≤ j ≤ k ≤ l}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function enumerates all quadruples (i,j,k,l) with the given constraints
 * and keeps the largest sum of the components.  The integer square root
 * helper is implemented with an efficient binary–search algorithm that
 * avoids overflow. */

#include <stdbool.h>

/* Integer square root for unsigned 64‑bit values (floor of sqrt(x)). */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    idx_t bit = ((idx_t)1 << 62);              /* highest power of two for 64 bits */
    while (bit) {
        idx_t next = r | bit;
        if (next <= x / next)                 /* avoid overflow: next^2 ≤ x  <=>  next ≤ x/next */
            r = next;
        bit >>= 1;
    }
    return r;
}

/* Computes a(n) for the OEIS sequence A307531. */
seq_t a(idx_t n)
{
    seq_t best = 0;                           /* best sum found so far */
    idx_t i_max = isqrt_u64(n);

    for (idx_t i = 0; i <= i_max; ++i) {
        idx_t i2 = i * i;
        idx_t rem_i = n - i2;
        idx_t j_max = isqrt_u64(rem_i);

        for (idx_t j = i; j <= j_max; ++j) {
            idx_t j2 = j * j;
            idx_t rem_j = rem_i - j2;
            idx_t k_max = isqrt_u64(rem_j);

            for (idx_t k = j; k <= k_max; ++k) {
                idx_t k2 = k * k;
                if (rem_j < k2)                /* not enough remaining to reach l ≥ k */
                    continue;
                idx_t rem_k = rem_j - k2;      /* remaining square for l^2 */
                idx_t l = isqrt_u64(rem_k);
                if ((idx_t)l * l == rem_k && l >= k) {
                    seq_t sum = (seq_t)i + j + k + l;
                    if (sum > best)
                        best = sum;
                }
            }
        }
    }
    return best;
}
