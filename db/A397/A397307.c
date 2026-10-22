/*
 * A397307  Number of primes of the form n - 2^k, where k >= 1.
 * https://oeis.org/A397307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = number of primes p with n = p + 2^k, k >= 1 (i.e. number
 *            of primes of the form n – 2^k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Local prime tester: trial division up to sqrt(x) */
static bool is_prime(idx_t x)
{
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1) == 0) return false;           /* even numbers >2 are composite */
    for (idx_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;   /* for n = 1,2 no powers of 2 satisfy k > 0 and n-2^k >= 2 */

    seq_t count = (seq_t)0;
    for (idx_t power = 2; power <= n - 2; power <<= 1) {
        idx_t val = n - power;
        if (is_prime(val)) ++count;
    }
    return count;
}
