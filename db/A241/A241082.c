/*
 * A241082  a(n) = n for 1 and prime numbers. For composite numbers, first
 *          prime term in sequence starting with n and each time adding
 *          smallest prime number not a divisor of the preceding term.
 * https://oeis.org/A241082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n for n = 1 or prime n; otherwise a(n) is the first prime
 *            reached by starting at n and repeatedly adding the smallest
 *            prime that does **not** divide the current number
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

static seq_t smallest_prime_not_dividing(seq_t x)
{
    /* static array of primes, generated on demand */
    static uint64_t *primes = NULL;
    static size_t prime_count = 0;
    static size_t prime_cap   = 0;

    if (prime_count == 0) {
        prime_cap = 16;
        primes = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        primes[0] = 2;
        prime_count = 1;
    }

    /* try existing primes first */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        if ((x % (seq_t)p) != 0)
            return (seq_t)p;
    }

    /* generate further primes as needed */
    uint64_t candidate = primes[prime_count - 1] + 1;
    while (1) {
        if (is_prime(candidate)) {
            if (prime_count == prime_cap) {
                prime_cap = prime_cap * 2;
                primes = (uint64_t*)realloc(primes, prime_cap * sizeof(uint64_t));
            }
            primes[prime_count++] = candidate;
            if ((x % (seq_t)candidate) != 0)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (is_prime(n))
        return (seq_t)n;

    seq_t x = (seq_t)n;
    while (1) {
        if (is_prime_big(x))
            return x;
        seq_t p = smallest_prime_not_dividing(x);
        x += p;
    }
}
