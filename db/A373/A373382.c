/*
 * A373382  a(n) = gcd(A329697(n), A331410(n)), where A329697, A331410 give
 *          the number of iterations needed to reach a power of 2, when using
 *          the map n -> n-(n/p), or respectively, n -> n+(n/p), where p is
 *          the largest prime factor of n.
 * https://oeis.org/A373382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A329697(n), A331410(n))
 * Range:     u8 n <= 74, u16 n <= 9602, u32 n <= 9602, u64 n <= 9602,
 *            u128 n <= 9602, big any n
 * Helpers:   gcd, is_power_of_two, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of iterations needed to reach a power of 2 when applying
 * the maps n → n - n/p and n → n + n/p, where p is the largest prime factor
 * of n.  Then returns the greatest common divisor of the two counts.   */
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/largest_prime_factor.h"
#include "../utils/is_power_of_two.h"

/* number of steps for the subtraction map n -> n - n/p  */
static seq_t steps_minus(idx_t n)
{
    seq_t count = 0;
    seq_t m = (seq_t)n;
    while (!is_power_of_two(m))
    {
        uint64_t p = largest_prime_factor((uint64_t)m);
        seq_t delta = m / (seq_t)p; /* m / p  */
        m -= delta;                 /* n - n/p  */
        ++count;
    }
    return count;
}

/* number of steps for the addition map n -> n + n/p  */
static seq_t steps_plus(idx_t n)
{
    seq_t count = 0;
    seq_t m = (seq_t)n;
    while (!is_power_of_two(m))
    {
        uint64_t p = largest_prime_factor((uint64_t)m);
        seq_t delta = m / (seq_t)p; /* m / p  */
        m += delta;                 /* n + n/p  */
        ++count;
    }
    return count;
}

seq_t a(idx_t n)
{
    seq_t a1 = steps_minus(n);
    seq_t a2 = steps_plus(n);
    return gcd(a1, a2);
}
