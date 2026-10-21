/*
 * A298735  Number of odd squares dividing n.
 * https://oeis.org/A298735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p odd | p^e || n} (floor(e/2)+1) (number of odd
 *            square divisors)
 * Range:     u8 n <= 9481, u16 n <= 9481, u32 n <= 9481, u64 n <= 9481,
 *            u128 n <= 9481, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    /* remove all factors of 2 – they do not contribute odd squares */
    while ((x & 1ULL) == 0ULL)
        x >>= 1;
    if (x == 1ULL)      /* n is a power of 2 */
        return (seq_t)1;

    /* factor the remaining odd part of n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    uint64_t temp = x;

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            primes[cnt] = p;
            exps[cnt] = exp;
            cnt++;
        }
    }
    if (temp > 1) {          /* remaining prime factor > sqrt(original temp) */
        primes[cnt] = temp;
        exps[cnt] = 1;
        cnt++;
    }

    /* number of odd square divisors = product over odd primes of (floor(e/2)+1) */
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t term = (seq_t)(exps[i] / 2 + 1);
        res *= term;
    }
    return res;
}
