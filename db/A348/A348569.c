/*
 * A348569  a(n) = (prime(n) + prime(n+2))^2 + prime(n+1)^2.
 * https://oeis.org/A348569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n) + prime(n+2))^2 + prime(n+1)^2 (prime(1) is the
 *            first prime 2)
 * Range:     u8 n <= 2, u16 n <= 28, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>   /* for log() – acceptable for computing an upper bound */

/* ---- Prime sieve utilities ------------------------------------------ */

static uint64_t *prime_arr   = NULL;   /* array of primes (0‑based)      */
static size_t   prime_cnt   = 0;       /* number of primes stored       */
static size_t   sieve_limit = 0;       /* current sieve size (max number) */

/* Estimate an upper bound on the n-th prime (valid for n >= 6)            */
static size_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;              /* first 6 primes are < 15        */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn)));
    return (size_t)(bound + 3.0);      /* small safety margin            */
}

/* Re‑sieve up to a new limit and rebuild prime array                     */
static void rebuild_sieve(size_t new_limit)
{
    /* Allocate boolean array: 1 = prime candidate                     */
    bool *is_prime = (bool*)malloc(new_limit + 1);
    if (!is_prime) exit(1);
    for (size_t i = 0; i <= new_limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t p = 2; p * p <= new_limit; ++p) {
        if (is_prime[p]) {
            for (size_t mult = p * p; mult <= new_limit; mult += p)
                is_prime[mult] = false;
        }
    }

    /* Count primes and store them                                           */
    size_t cnt = 0;
    for (size_t i = 2; i <= new_limit; ++i) if (is_prime[i]) ++cnt;

    uint64_t *new_arr = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_arr) exit(1);

    size_t idx = 0;
    for (size_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) new_arr[idx++] = (uint64_t)i;
    }

    free(is_prime);
    free(prime_arr);
    prime_arr   = new_arr;
    prime_cnt   = cnt;
    sieve_limit = new_limit;
}

/* Ensure that prime_arr contains at least (needed) primes (1-based index)*/
static void ensure_primes(idx_t needed)
{
    if (prime_cnt >= needed) return;

    /* Compute a new limit, keep expanding until enough primes are found  */
    size_t limit = estimate_limit(needed);
    while (true) {
        rebuild_sieve(limit);
        if (prime_cnt >= needed) break;
        limit *= 2;                  /* double the limit and try again     */
    }
}

/* ---- Sequence function ------------------------------------------------ */

seq_t a(idx_t n)
{
    /* Prime indices are 1‑based: prime(n) -> prime_arr[n-1]            */
    idx_t need = n + 2;                               /* because we need p_{n+2}   */
    ensure_primes(need);

    size_t idx1 = n - 1;                      /* zero‑based index of prime n     */
    size_t idx2 = idx1 + 1;                   /* prime n+1                       */
    size_t idx3 = idx1 + 2;                   /* prime n+2                       */

    seq_t p1 = (seq_t)prime_arr[idx1];
    seq_t p2 = (seq_t)prime_arr[idx2];
    seq_t p3 = (seq_t)prime_arr[idx3];

    seq_t sum = p1 + p3;                      /* (prime(n)+prime(n+2))          */
    return sum * sum + p2 * p2;              /* final formula                   */
}
