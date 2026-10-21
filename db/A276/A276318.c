/*
 * A276318  Least number k such that sigma(Sum_{j=0..n}{k-j}) =
 *          sigma(Sum_{j=0..n}{k+j}).
 * https://oeis.org/A276318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   least k such that sigma(sum_{j=0..n}(k-j)) =
 *            sigma(sum_{j=0..n}(k+j)) This is equivalent to
 *            sigma((n+1)*(2k-n)/2) = sigma((n+1)*(2k+n)/2)
 * Range:     u8 n <= 1, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sigma_of.h"

/* Compute a(n) for OEIS A276318 */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* k must be at least n (for the left sum to stay non‑negative) */
    idx_t k_start = (n > 0) ? n : 1;

    for (idx_t k = k_start;; ++k)
    {
        seq_t two_k = (seq_t)k + (seq_t)k; /* 2*k       */
        seq_t n_plus = (seq_t)n + 1;       /* n + 1     */

        /* L = (n+1)*(2k-n)/2,  R = (n+1)*(2k+n)/2   */
        seq_t L = n_plus * (two_k - (seq_t)n) / 2;
        seq_t R = n_plus * (two_k + (seq_t)n) / 2;

        /* Sanity check: arguments must fit in 64‑bit for sigma_of */
        if (L > (seq_t)UINT64_MAX || R > (seq_t)UINT64_MAX)
            break; /* cannot handle larger numbers with the available helper */

        if (sigma_of((idx_t)L) == sigma_of((idx_t)R))
            return (seq_t)k;
    }

    /* Should never be reached for valid input ranges */
    return 0;
}
