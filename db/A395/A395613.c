/*
 * A395613  a(n) = Sum_{k=0..floor(2*n/3)} binomial(n,k) *
 *          binomial(3*n,n+3*k).
 * https://oeis.org/A395613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊2n/3⌋} binomial(n,k) * binomial(3n,n+3k)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation relies on the utility
 * seq_t binomial(idx_t n, idx_t k) declared in "../utils/binomial.h". */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Pre‑compute 3*n avoiding overflow of 2*n */
    idx_t three_n = n + n + n;          /* 3*n  (fits in idx_t for n <= UINT64_MAX/3) */

    /* floor(2n/3) = n - floor(n/3) – this avoids overflow of 2*n */
    idx_t max_k = n - n / 3;

    seq_t ans = 0;
    for (idx_t k = 0; k <= max_k; ++k)
    {
        seq_t term1 = binomial(n, k);                     /* C(n,k)   */
        seq_t term2 = binomial(three_n, n + 3 * k);       /* C(3n,n+3k) */
        ans += term1 * term2;
    }
    return ans;
}
