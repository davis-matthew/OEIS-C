/*
 * A372103  The first n primes interpreted as digits in base prime(n+1).
 * https://oeis.org/A372103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{k=0..n-1} prime(n-k)*prime(n+1)^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

static uint32_t *prime_list = NULL;   /* array of first primes[:count] */
static size_t prime_count = 0;        /* number of primes currently stored */

/* Ensure prime_list contains at least (n+1) primes, i.e. prime(n+1) exists. */
static void ensure_primes(idx_t n)
{
    idx_t need = n + 1;          /* we need primes[0..n] */
    if (prime_count >= need)
        return;                  /* already have enough */

    /* free previous array if any */
    if (prime_list)
        free(prime_list);

    /* Allocate the array of the required size.  The called function
       will fill it with the first 'need' primes. */
    size_t cnt;
    sieve_first_n_primes(need, &prime_list, &cnt);
    /* cnt should equal need, but we don't depend on it. */
    prime_count = cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)   /* undefined for 0, but return 0 safely */
        return 0;

    ensure_primes(n);

    seq_t result = 0;
    seq_t power  = 1;
    seq_t base   = (seq_t) prime_list[n];          /* prime(n+1) */

    for (idx_t i = 0; i < n; ++i) {
        seq_t digit = (seq_t) prime_list[n - 1 - i]; /* prime(n-i) */
        result += digit * power;
        power  *= base;
    }
    return result;
}
