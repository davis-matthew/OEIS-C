/*
 * A266952  Least prime p such that p-2 and 6n-p and 6n+2-p are also prime, or
 *          0 if no such prime exists.
 * https://oeis.org/A266952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = least prime p such that p-2, 6n-p and 6n+2-p are
   all prime; return 0 if no such prime exists.
   Uses a dynamic sieve up to 6n+2 for each requested n. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global sieve state */
static idx_t sieve_limit = 0;          /* largest index already sieved   */
static uint8_t *sieve_arr   = NULL;    /* 0 = composite, 1 = prime       */
static uint32_t *primes_arr = NULL;    /* list of prime numbers          */
static size_t   primes_cnt  = 0;        /* count of primes in primes_arr  */

/* Build a sieve up to 'limit' (inclusive) and store prime list. */
static void rebuild_sieve(idx_t limit)
{
    if (limit < 2) limit = 2;
    uint8_t *arr = (uint8_t *)calloc((size_t)limit + 1, sizeof(uint8_t));
    if (!arr) return;          /* allocation failed – unlikely in tests */

    /* Mark all as prime initially, then clear 0 and 1. */
    for (idx_t i = 0; i <= limit; ++i) arr[i] = 1;
    arr[0] = arr[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (arr[i])
            for (idx_t j = i * i; j <= limit; j += i)
                arr[j] = 0;

    /* Count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (arr[i]) ++cnt;

    uint32_t *pr = NULL;
    if (cnt > 0) {
        pr = (uint32_t *)malloc(cnt * sizeof(uint32_t));
        if (pr) {
            size_t k = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (arr[i]) pr[k++] = (uint32_t)i;
        }
    }

    /* Replace old sieve/primes */
    free(sieve_arr);
    free(primes_arr);
    sieve_arr   = arr;
    primes_arr  = pr;
    primes_cnt  = cnt;
    sieve_limit = limit;
}

/* Ensure sieve covers numbers up to 6*n+2. */
static void ensure_sieve(idx_t n)
{
    idx_t required = 6 * n + 2;
    if (required <= sieve_limit) return;
    rebuild_sieve(required);
}

seq_t a(idx_t n)
{
    /* Make sure we can check all needed numbers. */
    ensure_sieve(n);

    idx_t limit = 6 * n;     /* 6n   */
    idx_t big   = 6 * n + 2; /* 6n+2 */

    /* Search primes in ascending order for the first suitable one. */
    for (size_t i = 0; i < primes_cnt && primes_arr[i] <= (uint32_t)limit; ++i) {
        idx_t p  = (idx_t)primes_arr[i];
        idx_t p2 = p - 2;
        if (p2 < 2 || !sieve_arr[p2]) continue;            /* p-2 not prime */

        idx_t a2 = limit - p;        /* 6n-p   */
        if (a2 < 2 || !sieve_arr[a2]) continue;            /* 6n-p not prime */

        idx_t a3 = big - p;          /* 6n+2-p */
        if (a3 < 2 || !sieve_arr[a3]) continue;            /* 6n+2-p not prime */

        /* All three numbers are prime – this is the least such p. */
        return (seq_t)p;
    }

    /* No suitable prime found. */
    return (seq_t)0;
}
