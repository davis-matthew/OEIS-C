/*
 * A245036  a(n) = 4*prime(n)^3 - 27*prime(n)^2 = (prime(n)^2)*[4*prime(n) -
 *          27], n >= 4.
 * https://oeis.org/A245036
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*prime(n)^3 - 27*prime(n)^2, n >= 4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 172, u64 n <= 6320,
 *            u128 n <= 6320, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

/* A static cache of the first primes that we have already computed.
   prime_cache[n-1] contains prime(n). */
static uint32_t *prime_cache = NULL;
static size_t prime_cache_len = 0;

/* Make sure prime_cache holds at least n primes.
   This may replace the current cache; the old memory is freed. */
static void ensure_primes(idx_t n)
{
    if (prime_cache_len >= (size_t)n)
        return;                           /* we already have enough */

    uint32_t *new_buf = NULL;
    size_t new_cnt = 0;

    /* `sieve_first_n_primes` fills `new_buf` with the first n primes. */
    sieve_first_n_primes(n, &new_buf, &new_cnt);

    /* Replace the old cache (free(NULL) does nothing). */
    free(prime_cache);
    prime_cache = new_buf;
    prime_cache_len = new_cnt;
}

/* Sequence A245036:  a(n) = 4*prime(n)^3 - 27*prime(n)^2,  n >= 4. */
seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;                    /* behaviour below the offset is irrelevant */

    ensure_primes(n);

    seq_t p  = (seq_t)prime_cache[n - 1];   /* prime(n) stored 0‑based */
    seq_t p2 = p * p;                       /* p^2   */
    seq_t p3 = p2 * p;                      /* p^3   */

    return p3 * 4 - p2 * 27;                /* 4p^3 – 27p^2 */
}
