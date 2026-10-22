/*
 * A333707  Number of ways to write n as the sum of two distinct positive
 *          integers with the same number of divisors.
 * https://oeis.org/A333707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor((n-1)/2)} [d(i)=d(n-i)]
 * Range:     u8 n <= 1259, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* external types seq_t and idx_t are provided by the build system */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* no pairs possible */
    /* Allocate divisor count array */
    uint64_t *d = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!d) return 0;              /* allocation failed → zero */
    for (idx_t i = 0; i <= n; ++i) d[i] = 0;

    /* Simple sieve to fill divisor counts */
    for (idx_t i = 1; i <= n; ++i)
        for (idx_t j = i; j <= n; j += i)
            ++d[j];

    /* Count pairs with equal divisor counts */
    seq_t cnt = 0;
    for (idx_t i = 1; i <= (n - 1) / 2; ++i)
        if ((seq_t)d[i] == (seq_t)d[n - i])
            ++cnt;

    free(d);
    return cnt;
}
