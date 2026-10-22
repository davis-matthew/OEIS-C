/*
 * A347643  Number of partitions of n into at most 2 prime powers (including
 *          1).
 * https://oeis.org/A347643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into at most 2 prime powers (including 1).
   The algorithm builds a sieve of Eratosthenes up to the largest
   requested n, marks all prime powers, and then counts
   one‑part and unordered two‑part partitions. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- *
 *  Static tables maintained across calls.                             *
 * --------------------------------------------------------------------- */
static idx_t   table_max = 0;         /* largest n for which tables are built */
static bool*   prime_power = NULL;    /* prime_power[i] == true  iff i is a prime power (incl. 1) */
static idx_t*  prime_list  = NULL;    /* list of primes up to table_max */
static size_t  prime_cnt   = 0;       /* number of primes */

/* --------------------------------------------------------------------- *
 *  Rebuild tables up to n if necessary.                                *
 * --------------------------------------------------------------------- */
static void ensure_tables(idx_t n)
{
    if (n <= table_max)
        return;

    /* Free old tables */
    if (prime_power) free(prime_power);
    if (prime_list)  free(prime_list);

    /* ---------- 1. Sieve primes up to n -------------------------------- */
    bool *is_prime = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!is_prime) return;                /* allocation failure */

    for (idx_t i = 0; i <= n; ++i)   /* start all as true          */
        is_prime[i] = true;
    if (n >= 0) is_prime[0] = false;
    if (n >= 1) is_prime[1] = false;

    for (idx_t p = 2; p <= n / p; ++p)     /* p*p <= n   */
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                is_prime[m] = false;
        }

    /* Count primes to allocate prime_list */
    size_t cnt = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i]) ++cnt;

    prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!prime_list) { free(is_prime); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= n; ++i)
        if (is_prime[i])
            prime_list[idx++] = i;

    prime_cnt = cnt;
    free(is_prime);

    /* ---------- 2. Mark prime powers (including 1) --------------------- */
    prime_power = (bool*)calloc((size_t)(n + 1), sizeof(bool));
    if (!prime_power) { prime_list = NULL; prime_cnt = 0; return; }

    prime_power[1] = true;               /* definition of prime power */
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        prime_power[p] = true;           /* prime itself */
        idx_t val = p;
        while (val <= n / p) {
            val *= p;
            prime_power[val] = true;
        }
    }

    table_max = n;
}

/* --------------------------------------------------------------------- *
 *  Public function:  a(idx_t n)                                        *
 * --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 1;        /* empty partition */

    ensure_tables(n);
    if (!prime_power) return 0;   /* should not happen */

    seq_t result = 0;

    /* One‑part partitions */
    if (prime_power[n])
        ++result;

    /* Two‑part unordered partitions */
    idx_t half = n / 2;
    for (idx_t x = 1; x <= half; ++x)
        if (prime_power[x] && prime_power[n - x])
            ++result;

    return result;
}
