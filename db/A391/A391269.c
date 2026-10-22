/*
 * A391269  a(n) is the number of iterations of x-> gpf(2*x+1) starting at n
 *          until the value 3 is reached; a(n)=-1 if 3 is not reached.
 * https://oeis.org/A391269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of iterations of x -> gpf(2*x+1) starting at n
 *            until 3 is reached; a(n) = -1 if 3 is not reached
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: direct simulation using the provided gpf function. */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, any n < 1 is undefined */
        return (seq_t)-1;

    uint64_t cur = (uint64_t)n;
    seq_t   cnt  = 0;

    /* Maximum allowed iterations to avoid infinite loops in case
       the conjecture fails; 10,000 is far more than needed for n ≤ 10⁴. */
    const unsigned long long MAX_ITER = 10000ULL;

    for (unsigned long long i = 0; i < MAX_ITER; ++i)
    {
        if (cur == 3U)          /* reaching the goal */
            return cnt;

        /* compute 2*cur + 1 safely using 128-bit intermediate to avoid wrap‑around */
        unsigned __int128 tmp = (unsigned __int128)2 * cur + 1;
        if (tmp > (unsigned __int128)UINT64_MAX)   /* too large for gpf */
            return (seq_t)-1;

        cur = gpf((uint64_t)tmp);
        ++cnt;
    }

    return (seq_t)-1;   /* not reached within MAX_ITER iterations */
}
