/*
 * A345127  Total sum of the distinct prime factors of s*t, for all positive
 *          integer pairs (s,t) such that s + t = n.
 * https://oeis.org/A345127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/2⌋} (sum of distinct prime factors of k*(n-k))
 * Range:     u8 n <= 31, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Cache of primes for factoring. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_limit = 0;

/* Ensure the global prime list contains all primes ≤ limit. */
static void ensure_primes(size_t limit)
{
    if (prime_list == NULL || limit > prime_limit) {
        /* Free old array if any. */
        if (prime_list != NULL)
            free(prime_list);
        uint64_t *tmp;
        size_t cnt;
        /* build_primes_upto allocates the array and sets cnt. */
        build_primes_upto((idx_t)limit, &tmp, &cnt);
        prime_list  = tmp;
        prime_count = cnt;
        prime_limit = limit;
    }
}

/* Sum of distinct prime factors of x (x ≥ 1). */
static seq_t distinct_prime_sum(uint64_t x)
{
    seq_t sum = 0;
    if (x <= 1)
        return 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x)
            break;
        if (x % p == 0) {
            sum += p;
            while (x % p == 0)
                x /= p;
        }
    }
    if (x > 1)        /* x itself is prime > sqrt(original). */
        sum += x;
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n <= 1)           /* a(1) = 0, and for n=0 (out of range) same. */
        return 0;

    /* The largest product k*(n-k) occurs near k = n/2. */
    idx_t max_factor = n / 2;
    ensure_primes((size_t)(max_factor + 1));

    seq_t total = 0;
    for (idx_t k = 1; k <= n / 2; ++k) {
        uint64_t prod = (uint64_t)k * (uint64_t)(n - k);
        total += distinct_prime_sum(prod);
    }
    return total;
}
