/*
 * A303223  Sum of the perimeters of the family of rectangles with dimensions
 *          p and q such that |q - p| is prime, n = p + q and p < q.
 * https://oeis.org/A303223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n * (# of d < n such that d is prime and d has the same
 *            parity as n; for n even d=2 only, for n odd count odd primes <
 *            n)
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Equivalent: */
/*   if n < 3                => 0 */
/*   if n is even           => 2*n if n>2 else 0 */
/*   if n is odd            => 2*n * (number of odd primes < n) */

static bool is_prime_trial(uint64_t x) {
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2) {
        if (x % i == 0) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t count = 0;

    if ((n & 1) == 0) {                /* n is even */
        if (n > 2) count = 1;          /* only d = 2 */
    } else {                           /* n is odd */
        for (idx_t d = 3; d < n; d += 2) {
            if (is_prime_trial((uint64_t)d))
                ++count;
        }
    }

    seq_t perimeter = ((seq_t)n) * 2ULL;
    return perimeter * count;
}
