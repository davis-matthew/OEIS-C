/*
 * A345071  Sums of two consecutive odd-indexed primes.
 * https://oeis.org/A345071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(2*n-1) + prime(2*n+1)
 * Range:     u8 n <= 15, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Prime list and its length, created once on first use */
static uint64_t *prime_list  = NULL;
static size_t   prime_count  = 0;
static int      primes_ready = 0;

/* Build a list of the first 20001 or more primes using a
 * sieve of Eratosthenes.  LIMIT is chosen large enough
 * that the 20001st prime (226729) is included. */
static void init_prime_sieve(void)
{
    if (primes_ready)
        return;  /* already done */

    const uint64_t LIMIT = 300000ULL;   /* > 226729 */

    /* boolean sieve: 1 = prime, 0 = composite */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(LIMIT + 1));
    if (!is_prime)
        abort();

    for (size_t i = 0; i <= LIMIT; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to allocate the list */
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++prime_count;

    prime_list = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!prime_list)
        abort();

    /* Fill the list */
    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            prime_list[pos++] = i;

    free(is_prime);
    primes_ready = 1;
}

/* ------------------------------------------------------------------ */
/* Required public function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* behaviour below offset is irrelevant */

    init_prime_sieve();

    /* 1‑based indices of the primes we need */
    idx_t low_index  = 2 * n - 1;   /* prime(2*n-1) */
    idx_t high_index = 2 * n + 1;   /* prime(2*n+1) */

    /* Convert to 0‑based positions in prime_list */
    size_t pos_low  = (size_t)low_index  - 1;
    size_t pos_high = (size_t)high_index - 1;

    if (pos_high >= prime_count)
        abort();   /* n too large for the pre‑computed limit */

    return (seq_t)prime_list[pos_low] + (seq_t)prime_list[pos_high];
}
