/*
 * A262392  a(n) = A007504(n) + A010693(n).
 * https://oeis.org/A262392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} prime(k) + n mod 2 + 2 (a(0)=2)
 * Range:     u8 n <= 13, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *prime_sum = NULL;      /* prime_sum[i] = sum of first i primes (prime_sum[0] = 0) */
static uint32_t *prime_list = NULL;  /* array of primes */
static size_t prime_cap = 0;         /* number of primes stored */

/* Make sure we have at least n primes available */
static void _ensure_primes(idx_t n)
{
    if (prime_cap >= (size_t)n)
        return;                     /* already enough */

    /* Estimate an upper bound for the n-th prime.
       For n >= 6, nth prime < n (log n + log log n).
       We use a simple over‑estimate n*20 + 100 which works for n up to ~10^7. */
    size_t limit = (size_t)(n * 20 + 100);
    if (limit < 15) limit = 15;

    /* Find an upper bound that yields at least n primes, expanding if needed. */
    size_t count = 0;
    while (1) {
        /* Sieve of Eratosthenes up to 'limit'. */
        char *is_composite = (char *)calloc(limit + 1, 1);
        if (!is_composite)
            exit(1); /* allocation failure */

        size_t i;
        for (i = 2; i * i <= limit; ++i) {
            if (!is_composite[i]) {
                for (size_t j = i * i; j <= limit; j += i)
                    is_composite[j] = 1;
            }
        }

        /* Count primes to see if we have enough. */
        count = 0;
        for (i = 2; i <= limit; ++i)
            if (!is_composite[i]) ++count;

        if (count >= (size_t)n) {
            /* Enough primes – now collect them. */
            uint32_t *tmp_primes = (uint32_t *)malloc(count * sizeof(uint32_t));
            if (!tmp_primes) exit(1);

            seq_t *tmp_sums = (seq_t *)malloc((count + 1) * sizeof(seq_t));
            if (!tmp_sums) exit(1);

            size_t idx = 0;
            seq_t sum = 0;
            tmp_sums[0] = 0;
            for (i = 2; i <= limit; ++i) {
                if (!is_composite[i]) {
                    tmp_primes[idx] = (uint32_t)i;
                    sum += (seq_t)i;
                    tmp_sums[idx + 1] = sum;
                    ++idx;
                }
            }

            free(is_composite);
            free(prime_list);
            free(prime_sum);

            prime_list = tmp_primes;
            prime_sum = tmp_sums;
            prime_cap = count;
            break;
        }

        /* Not enough primes – increase limit and try again. */
        free(is_composite);
        limit *= 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;

    _ensure_primes(n);

    seq_t result = prime_sum[(size_t)n];
    result += (seq_t)(n & 1) + (seq_t)2;
    return result;
}
