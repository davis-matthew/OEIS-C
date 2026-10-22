/*
 * A366107  a(n) = Sum_{i=0..floor(q(n)/3)} binomial(n-3*(i+1), q(n)-3*i) with
 *          q(n) = ceiling((n-3)/2).
 * https://oeis.org/A366107
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..floor(q(n)/3)} binomial(n-3*(i+1), q(n)-3*i)
 *            with q(n) = ceil((n-3)/2). Offset 3. n >= 3. See OEIS A366107
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>                       /* for compilers that need it */
#include "../utils/binomial.h"             /* provides: seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0; /* undefined for n < 3 – return 0 as a safe placeholder */

    /* q(n) = ceil((n-3)/2) == floor((n-2)/2) using integer arithmetic */
    idx_t q = (n - 2) / 2;
    idx_t i_max = q / 3;

    seq_t sum = 0;
    for (idx_t i = 0; i <= i_max; ++i)
    {
        idx_t m = n - 3 * (i + 1);          /* n - 3*(i+1) */
        idx_t k = q - 3 * i;                /* q - 3*i */

        /* ignore terms where k > m (binomial undefined for negative arguments) */
        if (k <= m)
            sum += binomial(m, k);
    }
    return sum;
}
