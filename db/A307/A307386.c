/*
 * A307386  Heinz numbers of integer partitions with Durfee square of length
 *          3.
 * https://oeis.org/A307386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer whose prime factorization
 *            contains at least three prime factors (counting multiplicity)
 *            that are >= 5
 * Range:     u8 n <= 4, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool qualifies(idx_t v)
{
    int cnt = 0;

    /* remove factor 2 */
    while ((v % 2) == 0)
        v /= 2;

    /* remove factor 3 */
    while ((v % 3) == 0)
        v /= 3;

    /* remaining primes are all >=5 */
    for (idx_t p = 5; p <= v / p; p += 2)
    {
        while ((v % p) == 0)
        {
            cnt++;
            v /= p;
        }
    }

    if (v > 1 && v >= 5)
        cnt++;

    return (cnt >= 3);
}

seq_t a(idx_t n)
{
    idx_t found   = 0;
    idx_t candidate = 124;     /* the integer before the first term (125) */

    while (found < n)
    {
        candidate++;               /* test the next integer */
        if (qualifies(candidate))
        {
            found++;
            if (found == n)
                return (seq_t)candidate;
        }
    }

    return (seq_t)0;  /* should never be reached */
}
