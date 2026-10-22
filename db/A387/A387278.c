/*
 * A387278  a(n) = Sum_{k=0..n} 3^(n-k) * binomial(n+1,k+1) *
 *          binomial(2*k+2,k+2).
 * https://oeis.org/A387278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} 5^(n-2k) * C(n+1, n-2k) * C(2k+1, k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses multiplicative recurrence for the binomials.
   The computation is exact for any seq_t implementation provided.   */
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* 5^n  */
    seq_t pow5 = pow_seq((seq_t)5, (int)n);

    /* cumulative sum of terms   */
    seq_t total = (seq_t)0;

    /* max k: floor(n/2)   */
    idx_t max_k = n / 2;

    /* current binomial C = C(n+1, r), start at r = 0   */
    seq_t C = (seq_t)1;
    idx_t r = 0;

    /* current B = C(2k+1, k), start with k = 0   */
    seq_t B = (seq_t)1;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t target_r = 2 * k + 1;

        /* advance C to desired r via C(r+1) = C(r) * (n+1-r)/(r+1)   */
        while (r < target_r)
        {
            C = C * ((n + 1) - r) / (r + 1);
            ++r;
        }

        /* term = 5^(n-2k) * C(n+1, target_r) * C(2k+1, k)   */
        seq_t term = pow5 * C * B;
        total += term;

        /* prepare pow5 for next k   */
        if (k < max_k)
            pow5 /= (seq_t)25;

        /* update B for next k   */
        if (k < max_k)
        {
            idx_t next_k = k + 1;
            /* factor = 2*(2*next_k+1)/(next_k+1)   */
            seq_t numer = (seq_t)2 * ((seq_t)2 * next_k + 1);
            seq_t denom = next_k + 1;
            B = B * numer / denom;
        }
    }

    return total;
}
