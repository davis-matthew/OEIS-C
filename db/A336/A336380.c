/*
 * A336380  Primes p(k) such that gcd(k, prime(k-1)+prime(k+1)) = 1.
 * https://oeis.org/A336380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime p(k) where k is the index such that gcd(k,
 *            prime(k-1)+prime(k+1)) = 1
 * Range:     u8 n <= 10, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n < 1, but protect anyway */

    int limit = (int)(n * 400 + 200);
    if (limit < 200) limit = 200;

    while (1) {
        /* Simple sieve of Eratosthenes */
        bool *isPrime = (bool*)malloc((limit + 1) * sizeof(bool));
        if (!isPrime) return 0; /* allocation failure */
        for (int i = 0; i <= limit; ++i) isPrime[i] = true;
        isPrime[0] = isPrime[1] = false;

        for (int i = 2; (long long)i * i <= limit; ++i) {
            if (isPrime[i]) {
                for (int j = i * i; j <= limit; j += i)
                    isPrime[j] = false;
            }
        }

        /* Count primes to allocate array */
        size_t primes_cnt = 0;
        for (int i = 2; i <= limit; ++i)
            if (isPrime[i]) ++primes_cnt;

        seq_t *primes = (seq_t*)malloc(primes_cnt * sizeof(seq_t));
        if (!primes) {
            free(isPrime);
            return 0;
        }
        size_t idx = 0;
        for (int i = 2, j = 0; i <= limit; ++i)
            if (isPrime[i]) primes[j++] = (seq_t)i;

        free(isPrime); /* primes array still needed */

        /* Find the n-th qualifying prime */
        size_t found = 0;
        for (size_t k = 2; k + 1 < primes_cnt; ++k) { /* k is the index (1-based) */
            seq_t sum = primes[k - 2] + primes[k];          /* prime(k-1) + prime(k+1) */
            seq_t g   = gcd((seq_t)k, sum);
            if (g == 1) {
                ++found;
                if (found == n) {
                    seq_t result = primes[k - 1];          /* prime(k) */
                    free(primes);
                    return result;
                }
            }
        }

        /* Not enough primes found; enlarge limit and retry */
        free(primes);
        limit *= 2; /* double the search space */
    }
}
