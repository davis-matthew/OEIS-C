/*
 * A321944  Starting from n, repeatedly compute the sum of the prime divisors
 *          until a fixed point or 0 is reached; a(n) is the number of terms,
 *          including n.
 * https://oeis.org/A321944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4284, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses iterative prime factorization to compute a(n) from OEIS A321944. */
#include <stdint.h>
#include <stdbool.h>

/* Sum of distinct prime divisors of m. */
static seq_t sum_distinct_prime_divisors(uint64_t m)
{
    if (m <= 1) return 0;
    seq_t sum = 0;
    uint64_t x = m;
    for (uint64_t p = 2; p * p <= x; ++p)
    {
        if (x % p == 0)
        {
            sum += (seq_t)p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) sum += (seq_t)x;
    return sum;
}

/* Required function: a(n) = number of terms until fixed point or 0. */
seq_t a(idx_t n)
{
    seq_t count = 1;               /* start with the original term */
    uint64_t current = (uint64_t)n;
    while (1)
    {
        seq_t next = sum_distinct_prime_divisors(current);
        if (next == 0)            /* reached zero */
        {
            ++count;              /* count the zero term */
            break;
        }
        if (next == current)      /* fixed point */
            break;
        current = (uint64_t)next;
        ++count;
    }
    return count;
}
