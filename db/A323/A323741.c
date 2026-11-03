/*
 * A323741  a(n) = m-p where m = (2n+1)^2 and p is the largest prime < m.
 * https://oeis.org/A323741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)^2 – p, where p is the largest prime < (2n+1)^2.
 *            The function uses the provided prev_prime utility to find p
 * Range:     u8 n <= 7, u16 n <= 296, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 116 terms through n = 952 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    /* m = (2n+1)^2 */
    seq_t two = (seq_t)2;
    seq_t two_n_plus_one = two * (seq_t)n + 1;
    seq_t m = two_n_plus_one * two_n_plus_one;

    /* largest prime < m */
    idx_t m_idx = (idx_t)m;          /* fits for all n in the tests */
    idx_t p = prev_prime(m_idx);

    /* a(n) = m – p */
    return m - (seq_t)p;
}
