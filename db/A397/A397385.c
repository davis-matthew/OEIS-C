/*
 * A397385  a(n) = Sum_{k=0..n} binomial(n+3*k,4*k) * binomial(n+4*k,k-1).
 * https://oeis.org/A397385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+3*k,4*k) * binomial(n+4*k,k-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note that the term for k=0 is zero because binomial(n, -1) is defined as 0. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* explicit for clarity, though loop handles it */
        return 0;

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t a1 = n + 3 * k;   /* n + 3k */
        idx_t b1 = 4 * k;       /* 4k  */
        seq_t bin1 = binomial(a1, b1);

        idx_t a2 = n + 4 * k;   /* n + 4k */
        idx_t b2 = k - 1;       /* k-1 */
        seq_t bin2 = binomial(a2, b2);

        total += bin1 * bin2;
    }
    return total;
}
