/*
 * A330492  a(n) = sum of second differences of the sorted divisors of n.
 * https://oeis.org/A330492
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n/p - 1) * (p - 1) where p is the least prime factor of
 *            n
 * Range:     u8 n <= 287, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Find the least prime factor of n (n >= 2).  Naïve O(sqrt(n)) algorithm,
   sufficient for n ≤ 10000 and fast enough for the required time limit. */
static uint64_t _least_prime_factor(uint64_t n)
{
    if (n % 2ULL == 0ULL) return 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL)
        if (n % i == 0ULL) return i;
    return n;            /* n itself is prime */
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* offset is 2, below that is unused */
    uint64_t p = _least_prime_factor((uint64_t)n);
    seq_t sp = (seq_t)p;                 /* least prime factor as seq_t   */
    seq_t div = (seq_t)n / sp;           /* integer division n/p           */
    return (div - 1) * (sp - 1);
}
