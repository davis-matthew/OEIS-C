/*
 * A329278  Irregular table read by rows. The n-th row is the permutation of
 *          {0, 1, 2, ..., 2^n-1} given by T(n,k) = k(k+1)/2 (mod 2^n).
 * https://oeis.org/A329278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   a(n) = (k*(k+1))/2 mod 2^r, where r = floor_log2(n+1) and k = n
 *            - (2^r - 1). This is the row‑by‑row concatenation of T(r,k) =
 *            k*(k+1)/2 (mod 2^r) for the irregular table A329278
 * Range:     u8 n <= 533, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Find row index r: r = floor(log2(n+1)) */
    unsigned r = floor_log2_uint64(n + 1);

    /* Compute 2^r and offset k */
    idx_t pow2 = ((idx_t)1 << r);
    idx_t k = n - (pow2 - 1);        /* 0 <= k < 2^r */

    /* Compute k*(k+1)/2 without overflow of the division */
    seq_t prod;
    if ((k & 1) == 0) {              /* k even */
        prod = ((seq_t)(k >> 1)) * ((seq_t)(k + 1));
    } else {                         /* k odd */
        prod = ((seq_t)k) * ((seq_t)((k + 1) >> 1));
    }

    /* Mask to implement mod 2^r: (1 << r) - 1 */
    seq_t mask = (((seq_t)1 << r) - 1);

    return prod & mask;
}
