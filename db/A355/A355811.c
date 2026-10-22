/*
 * A355811  a(n) is the number at the apex of a triangle whose base contains
 *          the distinct powers of 2 summing to n (in ascending order), and
 *          each number in a higher row is the product of the two numbers
 *          directly below it; a(0) = 1.
 * https://oeis.org/A355811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^S, where * S = Σ_{i=0}^{k-1} C(k-1,i) * b_i * k =
 *            number of 1‑bits in n, * b_i = position of the i‑th set bit in n
 *            (0‑based, ascending), * C(k-1,i) = binomial coefficient. * *
 *            This follows from the definition of the triangle: each base *
 *            element 2^{b_i} contributes to the apex with expon...
 * Range:     u8 n <= 13, u16 n <= 26, u32 n <= 30, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)                       /* a(0) = 1 */
        return (seq_t)1;                      /* pow2(0) would also work */

    /* collect positions of set bits in ascending order */
    idx_t bits[64];
    idx_t k = 0;
    idx_t tmp = n;
    idx_t pos = 0;
    while (tmp) {
        if ((tmp & ((idx_t)1)) != 0)
            bits[k++] = pos;
        tmp >>= 1;
        pos++;
    }

    /* compute exponent S = Σ bits[i] * C(k-1,i) */
    idx_t sum = 0;
    idx_t coeff = 1;                         /* C(k-1,0) */
    for (idx_t i = 0; i < k; ++i) {
        sum += bits[i] * coeff;
        if (i + 1 < k)                       /* update C(k-1,i+1) */
            coeff = coeff * (k - 1 - i) / (i + 1);
    }

    return pow2(sum);
}
