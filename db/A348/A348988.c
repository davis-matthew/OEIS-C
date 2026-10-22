/*
 * A348988  Numerator of A332994(n) / sigma(n).
 * https://oeis.org/A348988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A332994(n) / gcd(sigma(n), A332994(n)) where A332994(n) =
 *            sum of the chain { n, ⌊n/p⌋, ⌊⌊n/p⌋/p'\⌋, ... } with p, p', ...
 *            being the successive largest prime factors of each term, until 1
 *            is reached
 * Range:     u8 n <= 191, u16 n <= 9530, u32 n <= 9530, u64 n <= 9530,
 *            u128 n <= 9530, big any n
 * Helpers:   gcd, largest_prime_divisor, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/largest_prime_divisor.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1, but guard anyway */

    /* Compute A332994(n) : sum of the divisor chain. */
    seq_t sum = 0;
    seq_t cur = (seq_t)n;
    uint64_t m   = n;

    while (m > 1) {
        sum += cur;                       /* add current term to the sum */
        uint64_t p = largest_prime_divisor(m);
        m   /= p;                         /* remove one instance of its largest prime divisor */
        cur  /= p;                         /* same for the value stored in cur */
    }
    /* add the final term '1' of the chain */
    sum += (seq_t)1;

    /* Compute sigma(n) and reduce the fraction. */
    seq_t sigma = sigma_of(n);           /* sum of all positive divisors of n */
    seq_t g = gcd(sum, sigma);
    return sum / g;
}
