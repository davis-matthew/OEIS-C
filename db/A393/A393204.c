/*
 * A393204  Number of n X n matrices over GF(2) such that the row space is
 *          equal to the column space.
 * https://oeis.org/A393204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = Σ_{k=0..n} A002884(k) * A022166(n,k)
   Using the identities
     A002884(k)   = 2^{k(k-1)/2} · ∏_{j=1}^{k} (2^j-1)
     A022166(n,k) = ∏_{j=1}^{k} (2^{n-j+1}-1) / (2^j-1)
   the summand simplifies to
     s_k = 2^{k(k-1)/2} · ∏_{j=1}^{k} (2^{\,n-j+1}-1)
   with recurrence
     s_0 = 1
     s_k = s_{k-1} · 2^{k-1} · (2^{\,n-k+1}-1)   (k ≥ 1)
   Thus the sequence can be computed in O(n) time. */

#include "../utils/pow2.h"

/* Compute a(n) for n ≥ 0. */
seq_t a(idx_t n)
{
    seq_t answer = 1;     /* term for k = 0 */
    if (n == 0) return answer;

    seq_t term = 1;        /* term for current k (starting at 0) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Multiply by 2^{k-1} */
        term = term * pow2(k - 1);

        /* Multiply by (2^{n-k+1} - 1) */
        seq_t two_pow = pow2(n - k + 1);
        term = term * (two_pow - (seq_t)1);

        /* Add current term to the running sum */
        answer += term;
    }

    return answer;
}
