/*
 * A371669  G.f. satisfies A(x) = 1 + x * A(x)^3 * (1 + A(x))^2/2.
 * https://oeis.org/A371669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/n * Sum_{k=0..⌊(n-1)/2⌋} 2^(n-2k) * binomial(n,k) *
 *            binomial(4*n-k, n-1-2k) ( n > 0, a(0)=1 )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Only the term 2^(n-2k) can overflow a uint64_t.
       When the final result fits in seq_t and seq_t is a 64‑bit type,
       the exponent never exceeds 63, so we can safely shift
       a 128‑bit value.  For larger seq_t this shift is harmless. */
    idx_t K = (n - 1) / 2;          /* ⌊(n-1)/2⌋ */
    unsigned __int128 sum = 0;     /* 128‑bit accumulator to avoid overflow */

    for (idx_t k = 0; k <= K; ++k)
    {
        seq_t bin1 = binomial(n, k);                       /* C(n,k)   */
        seq_t bin2 = binomial(4 * n - k, n - 1 - 2 * k);   /* C(4n-k, n-1-2k) */

        /* 2^(n-2k) */
        unsigned __int128 pow2 = (unsigned __int128)1 << (n - 2 * k);

        unsigned __int128 term = pow2;
        term *= (unsigned __int128)bin1;   /* multiply by C(n,k) */
        term *= (unsigned __int128)bin2;   /* multiply by C(4n-k, ...) */

        sum += term;
    }

    /* Final division by n is integral by the formula. */
    return (seq_t)(sum / n);
}
