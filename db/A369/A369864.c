/*
 * A369864  a(n) = Sum_{p|n, p prime} n^(mu(n/p)^2).
 * https://oeis.org/A369864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{p|n, p prime} n^(μ(n/p)^2)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Closed form derived from the description:
   Let ω(n) be the number of distinct prime factors of n.
   Let count2 = number of prime factors of n with exponent exactly 2.
   Let large = true if n has any prime factor with exponent ≥3.
   Then:
     if (large)          a(n) = ω(n)
     else if (count2==0) a(n) = n*ω(n)
     else if (count2==1) a(n) = n + ω(n) - 1
     else                a(n) = ω(n)
   with a(1) = 0.
   The implementation performs trial division factorisation for each n. */
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t m = (uint64_t)n;
    uint64_t omega = 0;       /* distinct prime count */
    uint64_t count2 = 0;      /* primes with exponent exactly 2 */
    bool large = false;       /* any exponent >= 3? */

    for (uint64_t i = 2; i <= m / i; ++i) {
        if (m % i == 0) {
            uint64_t exp = 0;
            while (m % i == 0) {
                m /= i;
                ++exp;
            }
            ++omega;
            if (exp == 2)
                ++count2;
            else if (exp > 2)
                large = true;
        }
    }
    if (m > 1)
        ++omega;  /* remaining prime factor with exponent 1 */

    seq_t result;
    if (large) {
        result = (seq_t)omega;
    } else if (count2 == 0) {
        result = (seq_t)n * (seq_t)omega;
    } else if (count2 == 1) {
        result = (seq_t)n + (seq_t)omega - 1;
    } else {
        result = (seq_t)omega;
    }
    return result;
}
