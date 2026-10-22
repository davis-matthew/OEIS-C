/*
 * A375087  Numbers added to cumulative correction term in order for prime
 *          numbers to resemble a recursive sequence.
 * https://oeis.org/A375087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*prime(n+1) - prime(n+2) - prime(n-1)
 * Range:     i8 n <= 82, i16 n <= 82, i32 n <= 82, i64 n <= 82, i128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t sieve_limit = 0;
static unsigned char *sieve_arr = NULL;

/* Rebuild the sieve up to LIMIT (inclusive) and store primes. */
static void rebuild_primes(size_t limit)
{
    /* free old arrays */
    free(sieve_arr);
    free(prime_list);
    sieve_arr = (unsigned char *)malloc(limit + 1);
    if (!sieve_arr) abort();          /* allocation failure */
    memset(sieve_arr, 1, limit + 1);
    sieve_arr[0] = sieve_arr[1] = 0;

    for (size_t i = 2; i * i <= limit; ++i)
        if (sieve_arr[i])
            for (size_t j = i * i; j <= limit; j += i)
                sieve_arr[j] = 0;

    /* count primes */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve_arr[i]) ++count;

    prime_list = (uint64_t *)malloc(count * sizeof(uint64_t));
    if (!prime_list) abort();   /* allocation failure */

    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (sieve_arr[i]) prime_list[idx++] = i;

    prime_count = count;
    sieve_limit = limit;
}

/* Ensure we have at least REQUIRED_INDEX+1 primes (since primes are 1‑based). */
static void ensure_primes(idx_t required_index)
{
    /* required_index is 1‑based index, need primes up to at least required_index + 1 */
    size_t needed = required_index + 1;
    if (prime_count > required_index)
        return; /* already enough */

    /* approximate upper bound for the (needed‑th) prime: p_k < k*(log k + log log k)
       For small k we use a generous constant multiple. */
    size_t limit = needed * 20 + 100;
    if (limit < 100) limit = 100;

    while (true) {
        rebuild_primes(limit);
        if (prime_count > required_index) break;
        limit *= 2; /* increase bound and try again */
    }
}

/* Main function for OEIS A375087 */
seq_t a(idx_t n)
{
    if (n == 1)
        return 0; /* by definition */

    /* We need primes up to index n+2 */
    ensure_primes(n + 2);

    /* primes are stored in prime_list[0] = 2 = prime(1) */
    seq_t p_n_minus1 = (seq_t)prime_list[n - 2]; /* prime(n-1) */
    seq_t p_n_plus1  = (seq_t)prime_list[n];     /* prime(n+1) */
    seq_t p_n_plus2  = (seq_t)prime_list[n + 1]; /* prime(n+2) */

    return 2 * p_n_plus1 - p_n_plus2 - p_n_minus1;
}
