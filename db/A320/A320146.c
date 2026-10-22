/*
 * A320146  a(n) = 2*prime(n) modulo (prime(n-1) + prime(n+1)).
 * https://oeis.org/A320146
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*prime(n) modulo (prime(n-1)+prime(n+1))
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ---- local prime table ---- */
static idx_t  *prime_tbl  = NULL;  /* array of primes, 1‑based indexing in usage */
static size_t prime_cnt  = 0;      /* number of primes stored */
static idx_t   max_limit  = 0;     /* current sieve limit */

/* rebuilds the prime table up to a new limit */
static void rebuild_primes(idx_t limit)
{
    /* free old table if any */
    if (prime_tbl) {
        free(prime_tbl);
        prime_tbl = NULL;
        prime_cnt = 0;
    }

    /* simple sieve of Eratosthenes using a byte array */
    bool *is_prime = (bool*)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_prime) return;          /* allocation failure */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (idx_t i = 2; (idx_t)(i * i) <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* allocate table and store primes */
    prime_tbl = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!prime_tbl) {
        free(is_prime);
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            prime_tbl[pos++] = i;

    prime_cnt = cnt;
    max_limit  = limit;

    free(is_prime);
}

/* ensures we have at least (n+1) primes (=> indices 0..n inclusive) */
static void ensure_primes(idx_t n)
{
    if (prime_cnt > n + 1) return;      /* already sufficient */

    /* start with a reasonable limit; grow until enough primes */
    if (max_limit == 0) max_limit = 200;
    while (prime_cnt <= n + 1) {
        rebuild_primes(max_limit);
        if (prime_cnt > n + 1) break;
        /* increase limit (approximate nth prime ~ n log n) */
        max_limit *= 2;
    }
}

/* ---- main sequence function ---- */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                /* undefined region: return 0 */

    ensure_primes(n);                   /* need primes up to index n+1 */

    /* retrieve primes (0‑based indices: prime(1)=2 at index 0) */
    seq_t p     = (seq_t)prime_tbl[n - 1];   /* prime(n)   */
    seq_t pprev = (seq_t)prime_tbl[n - 2];   /* prime(n-1) */
    seq_t pnext = (seq_t)prime_tbl[n];       /* prime(n+1) */

    seq_t numerator   = (seq_t)2 * p;
    seq_t denominator = pprev + pnext;

    return numerator % denominator;
}
