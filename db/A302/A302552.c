/*
 * A302552  Number of segments needed to display the n-th prime number on a
 *          7-segment LCD display.
 * https://oeis.org/A302552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of the 7‑segment display counts of the digits of the
 *            n‑th prime
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static const uint8_t seg[10] = {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

/* Cached list of primes (1‑based index).  The array holds values as
   idx_t, not seq_t, because primes themselves may exceed the size
   of seq_t. */
static idx_t *prime_cache = NULL;
static size_t prime_count = 0;
static idx_t  sieve_limit = 0;

/* Estimate a mild upper bound for the n‑th prime.
   For n < 6 the bound is generous, and for larger n 20*n+100
   comfortably exceeds the n‑th prime. */
static idx_t estimate_limit(idx_t n) {
    if (n < 6) return 15;
    return n * 20 + 100;
}

/* Expand the cached prime list until it contains at least n primes. */
static void ensure_primes(idx_t n) {
    if (prime_count >= n) return;

    if (sieve_limit == 0) sieve_limit = estimate_limit(n);

    while (prime_count < n) {
        /* Allocate the sieve */
        uint8_t *is_prime = (uint8_t*)malloc((size_t)sieve_limit + 1);
        if (!is_prime) exit(EXIT_FAILURE);

        /* Initialise all to true */
        for (size_t i = 0; i <= (size_t)sieve_limit; ++i) is_prime[i] = 1;
        is_prime[0] = is_prime[1] = 0;

        /* Sieve of Eratosthenes */
        for (idx_t i = 2; i * i <= sieve_limit; ++i) {
            if (is_prime[i]) {
                for (idx_t j = i * i; j <= sieve_limit; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Count primes up to sieve_limit */
        size_t new_total = 0;
        for (idx_t i = 2; i <= sieve_limit; ++i)
            if (is_prime[i]) ++new_total;

        /* Reallocate the prime list to the new size */
        idx_t *new_cache = (idx_t*)realloc(prime_cache, new_total * sizeof(idx_t));
        if (!new_cache) exit(EXIT_FAILURE);
        prime_cache = new_cache;

        /* Populate the list */
        size_t idx = 0;
        for (idx_t i = 2; i <= sieve_limit; ++i) {
            if (is_prime[i]) {
                prime_cache[idx++] = i;
            }
        }

        prime_count = new_total;
        free(is_prime);

        if (prime_count >= n) return;

        /* Increase the sieve limit and try again */
        sieve_limit <<= 1;
    }
}

/* Main function: number of segments needed to display the n‑th prime. */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for n < 1, safeguard */

    ensure_primes(n);              /* make sure at least n primes are cached */

    idx_t prime = prime_cache[n - 1];
    seq_t sum = 0;

    while (prime >= 10) {
        sum += seg[(unsigned)(prime % 10)];
        prime /= 10;
    }
    sum += seg[(unsigned)prime];   /* last (or only) digit */

    return sum;
}
