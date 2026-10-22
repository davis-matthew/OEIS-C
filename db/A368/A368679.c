/*
 * A368679  Sum of the squarefree values of (n-k) where the numbers k are the
 *          numbers less than n that do not divide n.
 * https://oeis.org/A368679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1}^{n-1, n%k!=0} (n-k) if (n-k) is squarefree
 * Range:     u8 n <= 33, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Autonomous squarefree test for small numbers (fits in uint64_t) */
static inline bool is_squarefree(const seq_t x)
{
    uint64_t m = (uint64_t)x;
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            m /= p;
            if (m % p == 0)
                return false;           /* p^2 divides x */
        }
    }
    return true;                       /* no squared prime factor found */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                      /* a(1) = 0 as per OEIS */

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        if (n % k == 0)                /* k divides n, skip */
            continue;

        seq_t d = (seq_t)(n - k);      /* n-k value to test */
        if (is_squarefree(d))
            sum += d;
    }
    return sum;
}
