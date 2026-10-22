/*
 * A362087  a(n) = Sum_{k=0..n} (-1)^k * binomial(-n,k) * binomial(2*k,n-k).
 * https://oeis.org/A362087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A362087: a(n) = Sum_{k=0..n} binomial(n+k-1,k) * binomial(2*k,n-k) */
/* We compute terms in O(n) per n by updating two binomial factors incrementally. */

#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t C   = 1;                      // binomial(n+k-1, n-1) for current k
    seq_t B   = (n == 0 ? 1 : 0);        // binomial(2*k, n-k) for current k
    bool B_ok = (n == 0);                // becomes true once B is properly set

    sum += C * B;                        // k = 0 contribution

    /* The first k where binomial(2*k, n-k) is non‑zero is ceil(n/3). */
    idx_t k_start = (n + 2) / 3;         // integer ceil division

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update C_k = binomial(n+k-1, n-1) */
        C = C * ((seq_t)n + (seq_t)k - 1) / (seq_t)k;

        if (!B_ok)
        {
            if (k == k_start)
            {
                B   = binom((idx_t)(2 * k), (idx_t)(n - k));
                B_ok = true;
            }
            else
            {
                /* B is still zero; no term to add */
                continue;
            }
        }
        else if (k > k_start)
        {
            /* Update B_k from B_{k-1} using a multiplicative recurrence:
             * B_k = B_{k-1} * (2k)(2k-1)(n-k+1) /
             *                ((3k-n)(3k-n-1)(3k-n-2))   */
            seq_t num   = (seq_t)(2 * k) * (seq_t)(2 * k - 1) * (seq_t)(n - k + 1);
            seq_t denom = (seq_t)(3 * k - n)
                        * (seq_t)(3 * k - n - 1)
                        * (seq_t)(3 * k - n - 2);
            B = B * num / denom;
        }

        sum += C * B;
    }

    return sum;
}
