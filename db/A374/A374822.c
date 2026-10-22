/*
 * A374822  Antichain-Chain Quilt Numbers: Square table of the number of ASM
 *          quilts of type A_2(j) x C_k read down antidiagonals, where C_k is
 *          the chain poset or rank k and A_2(j) is the rank 2 poset with a
 *          unique minimal and maximal element and j atoms.
 * https://oeis.org/A374822
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sum T(j,k) read along antidiagonals of the array
 *            defined by T(j,k) = 2^j, k = 1 and, for k > 1, T(j,k) =
 *            Σ_{i=2..k} (k+1-i) * i^j
 * Range:     u8 n <= 23, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the diagonal s (sum j+k) containing the n-th term.
       Cumulative counts: up to diagonal s-1 is (s-2)(s-1)/2.  Find
       the smallest s with ((s‑1)s)/2 ≥ n. */
    idx_t s = 2;
    while (((s - 1) * s) / 2 < n)
        ++s;

    /* offset within this diagonal (0‑based). */
    idx_t prev   = ((s - 2) * (s - 1)) / 2;
    idx_t offset = n - prev - 1;

    /* Coordinates (j,k):
       first term of diagonal s has (j = s‑1, k = 1). */
    idx_t j = s - 1 - offset;
    idx_t k = offset + 1;

    /* Compute T(j,k). */
    if (k == 1)
    {
        /* T(j,1) = 2^j. */
        return big_pow((seq_t)2, (seq_t)j);
    }
    else
    {
        seq_t sum = 0;
        for (idx_t i = 2; i <= k; ++i)
        {
            seq_t pow_val = big_pow((seq_t)i, (seq_t)j);   /* i^j   */
            seq_t coeff   = (seq_t)(k + 1 - i);           /* k+1-i */
            sum += coeff * pow_val;
        }
        return sum;
    }
}
