/*
 * A371028  a(n) = Sum_{k=1..n} (n + k - 1)! / (k!*(n - k)!) if n > 0 and a(0)
 *          = 1. Row sums of A370983.
 * https://oeis.org/A371028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n + k - 1)! / (k! * (n - k)!) (a(0) = 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 19 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t term_next(seq_t term, idx_t n, idx_t k)
{
    /* computes term * (n + k) * (n - k) / (k + 1) with 128‑bit intermediate */
    unsigned __int128 prod = (unsigned __int128)term *
                            (unsigned __int128)(n + k) *
                            (unsigned __int128)(n - k);
    prod /= (unsigned __int128)(k + 1);
    return (seq_t)prod;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    seq_t term = (seq_t)n;            /* term for k = 1 */
    sum += term;

    for (idx_t k = 1; k < n; ++k)
    {
        term = term_next(term, n, k);
        sum += term;
    }

    return sum;
}
