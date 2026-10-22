/*
 * A305615  Next term is the largest earlier term that would not create a
 *          repetition of an earlier subsequence of length 2, if such a number
 *          exists; otherwise it is the smallest nonnegative number not yet in
 *          the sequence.
 * https://oeis.org/A305615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = [t!=0]*k - [t is odd]*(t-1)/2, with k = floor(sqrt(n)), t
 *            = n - k*k
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>

static uint64_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    uint64_t k = (uint64_t)sqrt((long double)n);
    /* adjust up if necessary                                 */
    while ((k + 1) <= n / (k + 1)) ++k;
    /* adjust down if necessary                                */
    while (k > n / k) --k;
    return k;
}

seq_t a(idx_t n)
{
    uint64_t k = isqrt_u64(n);
    uint64_t t = n - k * k;          /* remainder after subtracting k^2 */

    if (t == 0) return 0;            /* a(n) = 0 when t = 0 */

    seq_t result = (seq_t)k;         /* first term: k if t != 0 */

    if (t & 1)                       /* if t is odd, subtract (t-1)/2 */
    {
        result -= (seq_t)((t - 1) >> 1);
    }
    return result;
}
