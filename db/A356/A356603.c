/*
 * A356603  Position in A356226 of first appearance of the n-th composition in
 *          standard order (row n of A066099).
 * https://oeis.org/A356603
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1}^{#ones(n)} p_{2i-1}^{d_i} where p_{j} is the j‑th
 *            prime (p_1 = 2), and d_i are the differences between consecutive
 *            1‑bit positions (including 0 as a starting position) of the
 *            binary representation of n, read in reverse order. This
 *            implements the mapping from the k‑th composition in...
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

static const uint32_t odd_primes[32] = {
    2,   5,   11,   17,   23,   31,   41,   47,
   59,  67,  73,  83,  97, 103, 107, 113,
  127, 139, 151, 157, 167, 179, 193, 199,
  211, 223, 227, 229, 233, 239, 241, 251
};

seq_t a(idx_t n)
{
    /* Collect positions of set bits (1‑based) */
    int pos[64] = {0};
    int m = 0;                     /* number of set bits */
    idx_t k = n;
    idx_t bit = 1;
    while (k) {
        if (k & 1)
            pos[m++] = (int)bit;
        k >>= 1;
        bit++;
    }
    if (m == 0)          /* n == 0: empty composition → 1 */
        return (seq_t)1;

    /* Compute first differences (differences from previous position,
       starting from 0). */
    int diff[64];
    diff[0] = pos[0];              /* pos[0] - 0 */
    for (int i = 1; i < m; ++i)
        diff[i] = pos[i] - pos[i-1];

    /* Build product: reverse order of differences, map to odd‑index primes */
    seq_t res = (seq_t)1;
    for (int j = 0; j < m; ++j) {
        int exp = diff[m - 1 - j];          /* reversed */
        seq_t prime = (seq_t)odd_primes[j];
        res *= pow_seq(prime, exp);
    }
    return res;
}
