/*
 * A392252  a(n) = Sum_{k=0..floor(n/2)} binomial(k+2,2) *
 *          binomial(k,2*(n-2*k)).
 * https://oeis.org/A392252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} binomial(k+2,2) * binomial(k,2*(n-2k))
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 57, u64 n <= 124,
 *            u128 n <= 262, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Maximum k is floor(n/2) */
    idx_t max_k = n / 2;
    /* Minimum k for a non‑zero binomial(k,2*(n-2k)) is ceil(2n/5)  */
    idx_t k_start = (2 * n + 4) / 5;   /* (2n + 5 – 1) / 5 */
    if (k_start > max_k) {
        return 0;
    }
    for (idx_t k = k_start; k <= max_k; ++k) {
        seq_t c1 = binomial(k + 2, 2);          /* (k+2 choose 2)  */
        idx_t other = 2 * (n - 2 * k);          /* 2*(n-2k)        */
        seq_t c2 = binomial(k, other);         /* (k choose 2*(n-2k)) */
        sum += c1 * c2;
    }
    return sum;
}
