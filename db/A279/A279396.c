/*
 * A279396  Triangle read by rows T(n, m) = sigma^*_(n-m)(m), n >= 1, m = 1,
 *          2, ..., n, with sigma^*_(k)(n) given in a comment in A279395.
 * https://oeis.org/A279396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,m) where r is the smallest integer with r(r+1)/2 >=
 *            n, m = n - r(r-1)/2, and T(r,m) = Σ_{d|m} (-1)^(m-d) * d^(r-m)
 *            (indices start at 1)
 * Range:     u8 n <= 31, u16 n <= 69, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find the row number r (1‑based) that contains index n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Count of terms in all complete rows before row r. */
    idx_t prev = ((r - 1) * r) / 2;

    /* Column index m (1‑based) within this row. */
    idx_t m = n - prev;

    /* Exponent k = r - m (the power of each divisor). */
    idx_t k = r - m;

    /* Separate sums for positive and negative contributions to avoid
       unsigned underflow during intermediate subtraction. */
    seq_t pos = 0;
    seq_t neg = 0;

    /* Enumerate all positive divisors d of m. */
    for (idx_t d = 1; d * d <= m; ++d)
    {
        if (m % d == 0)
        {
            seq_t val = seq_pow((seq_t)d, k); /* d^k */

            /* Sign of this term: (+) if (m - d) is even, otherwise (-). */
            if (((m - d) & 1) == 0)
                pos += val;
            else
                neg += val;

            /* Handle the paired divisor e = m/d when it's distinct. */
            idx_t e = m / d;
            if (e != d)
            {
                seq_t val2 = seq_pow((seq_t)e, k); /* e^k */

                if (((m - e) & 1) == 0)
                    pos += val2;
                else
                    neg += val2;
            }
        }
    }

    /* Final result: pos - neg.  For this sequence pos >= neg always holds. */
    seq_t result = pos;
    result -= neg;
    return result;
}
