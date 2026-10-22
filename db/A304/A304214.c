/*
 * A304214  Smallest k > 0 such that 2^(p-1) (mod p^2) < k*p for p = prime(n).
 * https://oeis.org/A304214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( (2^(p-1) mod p^2) / p ) + 1, where p is the n-th
 *            prime. The value equals 1 iff p is a Wieferich prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/build_primes_upto.h" /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */

/* Helper: supply nth prime on demand.  The function expands the sieve
   only when a larger n is requested.  It keeps the array of primes in
   static storage and frees any previously allocated buffer. */
static uint64_t *sieve_primes = NULL;
static size_t sieve_cnt   = 0;
static idx_t  sieve_limit = 0;

static void ensure_n_primes(idx_t n)
{
    if (n <= (idx_t)sieve_cnt) return;

    /* Choose an initial upper bound and grow until enough primes are found. */
    idx_t limit = sieve_limit ? (sieve_limit * 2) : (n * 20 + 100);

    while (true) {
        uint64_t *tmp = NULL;
        size_t   cnt = 0;
        build_primes_upto(limit, &tmp, &cnt);
        free(sieve_primes);
        sieve_primes = tmp;
        sieve_cnt    = cnt;
        sieve_limit  = limit;

        if (sieve_cnt >= (size_t)n) break;
        limit *= 2;   /* increase the bound and try again */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined for n >= 1 only */
    ensure_n_primes(n);
    uint64_t prime_u64 = sieve_primes[n - 1];   /* prime index is 0‑based */
    seq_t   prime = (seq_t)prime_u64;

    seq_t mod = prime * prime;                     /* p^2  (fits in seq_t) */
    seq_t T   = mod_pow((seq_t)2, (uint64_t)(prime_u64 - 1), mod); /* 2^(p-1) mod p^2 */

    seq_t k = T / prime + 1;                       /* smallest k with T < k*p */
    return k;
}
