/*
 * A356163  a(n) = 1 if sum of prime factors of n (taken with multiplicity) is
 *          even, otherwise 0.
 * https://oeis.org/A356163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sum of prime factors of n (taken with multiplicity)
 *            is even, otherwise 0. The function uses only integer arithmetic
 *            and works for any n >= 1 in any order of calls
 * Range:     u8 n <= 9512, u16 n <= 9512, u32 n <= 9512, u64 n <= 9512,
 *            u128 n <= 9512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* External integer types; seq_t is the output type.  The header that
   defines seq_t, idx_t and any other required utilities is provided by
   the build system. */
seq_t a(idx_t n)
{
    /* For n == 1 the sum of prime factors is 0 (even), so return 1. */
    if (n == 1)
        return (seq_t)1;

    /* Work on a copy of n so that we can factor it. */
    uint64_t t = (uint64_t)n;
    bool parity = false;            /* false -> sum of odd prime factors is even */

    /* Remove all factors of 2; they do not affect the parity of the sum
       because 2 is even. */
    while ((t & 1ULL) == 0)
        t >>= 1;                    /* t /= 2 */

    /* Trial division by odd numbers up to sqrt(t).  The condition
       i <= t / i prevents overflow. */
    for (uint64_t i = 3; i <= t / i; i += 2) {
        while (t % i == 0) {
            parity = !parity;       /* each odd prime factor toggles parity */
            t /= i;
        }
    }

    /* If the remaining t is a prime > sqrt(original), it is odd and
       contributes one more to the sum (unless it was 2, but 2 would
       have been removed already). */
    if (t > 1)
        parity = !parity;

    /* Return 1 if sum is even (parity == false), otherwise 0. */
    return parity ? (seq_t)0 : (seq_t)1;
}
