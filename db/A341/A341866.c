/*
 * A341866  The cardinality of the smallest (nontrivial, except for prime n)
 *          multiset of positive integers whose product and sum equal n.
 * https://oeis.org/A341866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/p - 1)*(p-1) + 1, where p is the smallest factor of n
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Return the smallest factor of n (1 for n==1). */
static uint64_t smallest_factor(uint64_t n)
{
    if (n < 2)
        return 1;
    for (uint64_t i = 2; i <= n / i; ++i) {
        if (n % i == 0)
            return i;
    }
    return n;   /* n is prime */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;
    uint64_t nn = (uint64_t)n;
    uint64_t p = smallest_factor(nn);

    seq_t part1 = (seq_t)(nn / p) - 1;
    seq_t part2 = (seq_t)(p - 1);
    seq_t result = part1 * part2 + 1;
    return result;
}
