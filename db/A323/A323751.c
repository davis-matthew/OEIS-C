/*
 * A323751  a(n) = Product_{k=0..n} (k^n + (n-k)^n).
 * https://oeis.org/A323751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A323751: a(n) = ∏_{k=0..n} (k^n + (n‑k)^n) */
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* 0^0 is taken as 1 by big_pow; the product at n == 0 consists of
       a single term (0^0 + 0^0) = 2. */
    if (n == 0)
        return (seq_t)2;

    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t term_k  = big_pow((seq_t)k, (seq_t)n);          /* k^n   */
        seq_t term_nk = big_pow((seq_t)(n - k), (seq_t)n);    /* (n-k)^n */
        prod = prod * (term_k + term_nk);
    }
    return prod;
}
