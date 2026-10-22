/*
 * A366558  G.f. A(x) satisfies A(x) = 1 + x + x^4*A(x)^4.
 * https://oeis.org/A366558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A366558 – Non‑nonn sequence
    Formula: a(n) = Sum_{k=0..⌊n/4⌋} binomial(3k+1, n-4k) * binomial(4k, k) / (3k+1)   */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n / 4; ++k)
    {
        idx_t r   = n - 4 * k;          /* n - 4k */
        idx_t N   = 3 * k + 1;          /* 3k+1  */
        if (r > N)                     /* binomial=0 otherwise */
            continue;
        seq_t bin1 = binom(N, r);      /* binomial(3k+1, n-4k) */
        seq_t bin2 = binom(4 * k, k);  /* binomial(4k, k)     */
        seq_t term = bin1 * bin2;      /* multiply first      */
        term = term / N;               /* divide by N         */
        sum += term;                   /* accumulate          */
    }
    return sum;
}
