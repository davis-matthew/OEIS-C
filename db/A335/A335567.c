/*
 * A335567  Number of distinct positive integer pairs (s,t) such that s <= t <
 *          n where neither s nor t divides n.
 * https://oeis.org/A335567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - A000005(n)) * (n - A000005(n) + 1) / 2
 * Range:     u8 n <= 26, u16 n <= 366, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A000005(n) is the number of divisors of n. */

#include <stdint.h>

static seq_t divisor_count(idx_t n)
{
    seq_t cnt = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            ++cnt;                     /* divisor i */
            if (i != n / i) ++cnt;     /* paired divisor n/i */
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t d = divisor_count(n);          /* number of divisors of n */
    seq_t m = (seq_t)n - d;              /* numbers < n that are not divisors */
    return m * (m + 1) / 2;              /* combinations with repetition */
}
