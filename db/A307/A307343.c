/*
 * A307343  Number of partitions of n into 3 mutually distinct, mutually
 *          nonadjacent prime parts.
 * https://oeis.org/A307343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into 3 mutually distinct,
 *            mutually nonadjacent prime parts. Approach: generate all primes
 *            ≤ n, then count triples (p,q,r) with p<q<r, p+q+r = n, and
 *            indices of p,q,r in the prime list differ by at least 2 (i.e.,
 *            no two primes are consecutive)
 * Range:     u8 n <= 406, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: if n is too small to contain three distinct
       non‑consecutive primes, the result is 0. The algorithm below
       handles it automatically, but this guard avoids an empty array
       allocation for n < 5. */
    if (n < 2) return (seq_t)0;

    /* Sieve of Eratosthenes up to n. */
    idx_t limit = n;
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    if (!is_prime) return (seq_t)0;          /* allocation failure */
    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (idx_t p = 2; p * p <= limit; ++p)
        if (is_prime[p])
            for (idx_t m = p * p; m <= limit; m += p)
                is_prime[m] = false;

    /* Count primes and store prime list. */
    size_t prime_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++prime_count;

    if (prime_count < 3) {                  /* cannot form 3 primes */
        free(is_prime);
        return (seq_t)0;
    }

    uint64_t *prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    int *prime_index = (int *)malloc((limit + 1) * sizeof(int));
    if (!prime_list || !prime_index) {
        free(is_prime);
        free(prime_list);
        free(prime_index);
        return (seq_t)0;
    }

    for (idx_t i = 0; i <= limit; ++i) prime_index[i] = -1;
    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            prime_list[idx] = i;
            prime_index[i] = (int)idx;
            ++idx;
        }
    }

    /* Count valid triples. */
    seq_t result = 0;
    for (size_t a = 0; a + 2 < prime_count; ++a) {
        uint64_t p = prime_list[a];
        if (p > n) break;   /* further primes only larger */
        for (size_t b = a + 2; b + 1 < prime_count; ++b) {
            uint64_t q = prime_list[b];
            if (p + q >= n) break;   /* further q only larger */
            uint64_t r = n - p - q;
            if (r <= q) continue;    /* need r > q for ascending order */
            if (r > limit || !is_prime[r]) continue;  /* not a prime */
            int k = prime_index[r];
            if (k == -1) continue;  /* safety */
            if (k <= (int)b + 1) continue;  /* q and r consecutive primes */
            result++;
        }
    }

    /* Clean up. */
    free(is_prime);
    free(prime_list);
    free(prime_index);
    return result;
}
