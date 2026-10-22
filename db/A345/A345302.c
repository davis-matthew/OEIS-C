/*
 * A345302  a(n) = Sum_{p|n, p prime} lcm(p,n/p).
 * https://oeis.org/A345302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{p|n, p prime} lcm(p,n/p)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation factors n by trial division and sums the
   required lcm terms.  It works for any n >= 1 and is efficient
   up to at least n = 10,000. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* Preserve the original value for use in sum terms */
    const seq_t n_val = (seq_t)n;
    uint64_t remaining = (uint64_t)n;
    seq_t sum = (seq_t)0;

    /* Handle factor 2 separately */
    size_t expo = 0;
    while ((remaining & 1ULL) == 0ULL) {
        remaining >>= 1;
        ++expo;
    }
    if (expo) {
        if (expo == 1)
            sum += n_val;
        else
            sum += n_val / (seq_t)2;
    }

    /* Loop over odd factors up to sqrt(remaining) */
    for (uint64_t i = 3; i <= remaining / i; i += 2) {
        if (remaining % i != 0ULL)
            continue;
        expo = 0;
        while (remaining % i == 0ULL) {
            remaining /= i;
            ++expo;
        }
        if (expo == 1)
            sum += n_val;
        else
            sum += n_val / (seq_t)i;
    }

    /* If a prime factor larger than sqrt(n) remains, it has exponent 1 */
    if (remaining > 1ULL)
        sum += n_val;

    return sum;
}
