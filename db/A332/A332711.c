/*
 * A332711  Sum of all numbers in bijective base-n numeration with digit sum
 *          n.
 * https://oeis.org/A332711
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((n+1)^n - 2^n) / (n-1) (for n ≥ 2)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, pow128_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Special cases: a(0) = 0, a(1) = 1                               */

#include "../utils/big_pow.h"
#include "../utils/pow128_u128.h"

seq_t a(idx_t n)
{
    /* Handle the small indices explicitly */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;

    /* 64‑bit seq_t: use 128‑bit intermediates to avoid overflow   */
    if (sizeof(seq_t) <= 8)
    {
        unsigned __int128 pow_n_plus = pow128_u128((seq_t)(n + 1u), n);
        unsigned __int128 pow_two     = pow128_u128((seq_t)2u,          n);
        unsigned __int128 numerator   = pow_n_plus - pow_two;
        unsigned __int128 denominator = (unsigned __int128)(n - 1u);
        unsigned __int128 result      = numerator / denominator;
        return (seq_t)result;
    }

    /* Larger seq_t (arbitrary precision or 128‑bit): rely on big_pow */
    seq_t pow_n_plus = big_pow((seq_t)(n + 1u), (seq_t)n);
    seq_t pow_two     = big_pow((seq_t)2u,          (seq_t)n);
    seq_t numerator   = pow_n_plus - pow_two;
    seq_t denominator = (seq_t)(n - 1u);
    return numerator / denominator;
}
