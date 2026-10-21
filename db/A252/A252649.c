/*
 * A252649  Number of positive integers that are less than or equal to n and
 *          have a primitive root.
 * https://oeis.org/A252649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integers <= n that are 1, 2, 4, or of the form
 *            p^k or 2·p^k where p is an odd prime
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n >= 1) count++;   /* 1  */
    if (n >= 2) count++;   /* 2  */
    if (n >= 4) count++;   /* 4  */

    if (n < 3) return count;   /* n = 1 or 2 handled above */

    uint64_t N = (uint64_t)n;
    /* simple sieve of Eratosthenes up to N */
    bool *is_prime = (bool *)malloc((N + 1) * sizeof(bool));
    if (!is_prime) return count;   /* fallback, should not happen for small N */
    for (uint64_t i = 0; i <= N; ++i) is_prime[i] = true;
    if (N >= 0) is_prime[0] = false;
    if (N >= 1) is_prime[1] = false;

    for (uint64_t i = 2; i * i <= N; ++i) {
        if (!is_prime[i]) continue;
        for (uint64_t j = i * i; j <= N; j += i) is_prime[j] = false;
    }

    /* iterate over odd primes */
    for (uint64_t p = 3; p <= N; p += 2) {
        if (!is_prime[p]) continue;

        /* count p^k */
        uint64_t pow = p;
        while (pow <= N) {
            count++;
            if (pow > N / p) break;   /* avoid overflow */
            pow *= p;
        }

        /* count 2·p^k */
        pow = p;
        while (2 * pow <= N) {
            count++;
            if (pow > N / p) break;
            pow *= p;
        }
    }

    free(is_prime);
    return count;
}
