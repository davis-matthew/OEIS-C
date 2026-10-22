/*
 * A342082  Numbers with an inferior odd divisor > 1.
 * https://oeis.org/A342082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth positive integer with an odd prime divisor <= sqrt(n)
 * Range:     u8 n <= 141, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Returns the smallest odd prime factor of an odd number n >= 3.
   If n itself is prime, n is returned. */
static inline uint64_t smallest_odd_prime_factor(uint64_t n)
{
    if ((n % 3ULL) == 0ULL) return 3ULL;
    for (uint64_t d = 5ULL; d <= n / d; d += 6ULL) {
        if ((n % d) == 0ULL) return d;
        if ((n % (d + 2ULL)) == 0ULL) return d + 2ULL;
    }
    return n;               /* n is itself an odd prime */
}

/* Checks whether x has an inferior odd divisor > 1.  
   Equivalent to: x has an odd prime factor p such that p <= sqrt(x). */
static inline bool has_inferior_odd_divisor(uint64_t x)
{
    uint64_t odd = x;
    while ((odd & 1ULL) == 0ULL && odd > 1ULL)
        odd >>= 1;                     /* strip powers of 2 */

    if (odd <= 1ULL) return false;     /* no odd prime factor */

    uint64_t p = smallest_odd_prime_factor(odd);
    return p <= x / p;                 /* p <= sqrt(x) → p*p <= x */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1; not used for n==0 */

    idx_t count = 0;
    uint64_t candidate = 1;
    while (1) {
        ++candidate;
        if (has_inferior_odd_divisor(candidate)) {
            ++count;
            if (count == n) return (seq_t)candidate;
        }
    }
}
