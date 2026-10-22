/*
 * A356122  Number of Lucas divisors of the n-th Lucas number.
 * https://oeis.org/A356122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + (n%3==0) + (n odd ? τ(n)-1 : τ(n>>v2(n)))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where v2(n) = exponent of 2 in n and τ(m) is the number of positive divisors of m. */

#include <stdint.h>

static inline idx_t divisor_count(idx_t m)
{
    uint64_t cnt = 0;
    for (idx_t i = 1; i * i <= m; ++i)
    {
        if (m % i == 0)
            cnt += (i == m / i) ? 1 : 2;
    }
    return (idx_t)cnt;
}

seq_t a(idx_t n)
{
    /* Special case n = 0: Lucas numbers L_0 = 2 and L_1 = 1 are the only Lucas divisors. */
    if (n == 0)
        return 2;

    seq_t result = 1;          /* L_1 always divides L_n. */

    if ((n % 3) == 0)
        result++;              /* L_0 divides L_n iff n is a multiple of 3. */

    /* Count k ≥ 2 such that L_k | L_n. */
    idx_t tmp = n;
    idx_t v2 = 0;              /* exponent of 2 in n */
    while ((tmp & 1) == 0)
    {
        ++v2;
        tmp >>= 1;
    }

    idx_t odd_part = tmp;     /* n >> v2 */

    idx_t divcnt;
    if (v2 == 0)               /* n is odd: exclude divisor 1 */
        divcnt = divisor_count(n) - 1;
    else                       /* n is even: all counted divisors are ≥ 2 */
        divcnt = divisor_count(odd_part);

    result += (seq_t)divcnt;
    return result;
}
