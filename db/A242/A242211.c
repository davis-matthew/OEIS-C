/*
 * A242211  a(1) = 4, a(n) = A222208(a(n-1)).
 * https://oeis.org/A242211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^(2^⌊(n-1)/2⌋) * 3^(2^⌊(n-2)/2⌋) (n ≥ 2), a(1) = 4
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
    {
        return (seq_t)4;
    }
    /* exponents for the inner powers of 2 */
    idx_t k2 = (n - 1) / 2;   /* ⌊(n‑1)/2⌋ */
    idx_t k3 = (n >= 2) ? (n - 2) / 2 : 0;  /* ⌊(n‑2)/2⌋, safe for n = 1 */

    /* compute 2^k2 and 2^k3 (may be large) */
    seq_t exp2 = big_pow((seq_t)2, (seq_t)k2);
    seq_t exp3 = big_pow((seq_t)2, (seq_t)k3);

    /* now compute 2^exp2 and 3^exp3 */
    seq_t pow2 = big_pow((seq_t)2, exp2);
    seq_t pow3 = big_pow((seq_t)3, exp3);

    return pow2 * pow3;
}
