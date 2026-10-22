/*
 * A341976  Number of partitions of n into 5 distinct primes (counting 1 as a
 *          prime).
 * https://oeis.org/A341976
 *
 * Offset:    18    (a(18) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into 5 distinct primes (1 is
 *            treated as a prime)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/* Helper to generate primes (including 1 as the first prime) up to      */
/* a given limit.  The list of primes is stored in dynamically-allocated */
/* arrays. The routine updates the global tables if a larger limit is  */
/* requested.                                                         */
/* ------------------------------------------------------------------ */
static uint64_t *prime_list = NULL;      /* array of primes including 1 */
static size_t   prime_cnt  = 0;          /* number of stored primes   */
static idx_t    prime_max  = 0;          /* current maximum prime     */

static void ensure_primes(idx_t limit)
{
    if (limit <= prime_max)
        return; /* already have all primes up to limit */

    /* Sieve up to 'limit' */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime) abort();  /* allocation failure */

    for (size_t i = 2; i < sz; ++i)
        is_prime[i] = true;

    for (size_t p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= (size_t)limit; m += p)
                is_prime[m] = false;
        }
    }

    /* Count primes */
    size_t newcnt = 1; /* include 1 as the first prime */
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++newcnt;

    /* Allocate new prime array */
    uint64_t *newlist = (uint64_t*)calloc(newcnt, sizeof(uint64_t));
    if (!newlist) abort();

    /* Store 1 first */
    newlist[0] = 1;
    size_t idx = 1;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i])
            newlist[idx++] = (uint64_t)i;

    /* Replace old data */
    free(prime_list);
    prime_list = newlist;
    prime_cnt  = newcnt;
    prime_max  = limit;

    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Dynamic programming table: dp[sum * 6 + k]  (k = 0..5).  dp holds    */
/* the number of ways to form 'sum' using exactly 'k' distinct primes   */
/* from the list of all primes <= current_max.                        */
/* ------------------------------------------------------------------ */
static seq_t *dp_table   = NULL;   /* flat array */
static size_t dp_rows    = 0;      /* number of rows stored (max_n + 1) */
static idx_t  dp_max_n   = 0;      /* largest n for which dp is valid */

static void recompute_dp(idx_t limit)
{
    /* Allocate new table */
    size_t rows = (size_t)(limit + 1);
    seq_t *newdp = (seq_t*)calloc(rows * 6, sizeof(seq_t));
    if (!newdp) abort();

    /* Base case: one way to form 0 with 0 primes */
    newdp[0 * 6 + 0] = 1;

    /* Iterate over each prime */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = prime_list[pi];
        if (p > (uint64_t)limit) break;
        for (int s = (int)limit; s >= (int)p; --s) {
            size_t idx_s = (size_t)s * 6;
            size_t idx_sp = (size_t)(s - (int)p) * 6;
            for (int k = 5; k >= 1; --k) {
                newdp[idx_s + k] += newdp[idx_sp + (k - 1)];
            }
        }
    }

    /* Replace old table */
    free(dp_table);
    dp_table = newdp;
    dp_rows  = rows;
    dp_max_n = limit;
}

/* ------------------------------------------------------------------ */
/* Ensure that dp_table is up-to-date for the requested maximum 'n'    */
/* ------------------------------------------------------------------ */
static void ensure_dp(idx_t limit)
{
    if (limit <= dp_max_n)
        return; /* already computed */

    recompute_dp(limit);
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* For efficiency, ensure we have at least the needed primes and DP. */
    ensure_primes(n);
    ensure_dp(n);

    /* Retrieve the answer from the table: number of ways to form n
       with exactly 5 distinct primes. */
    size_t idx = (size_t)n * 6 + 5;
    return dp_table[idx];
}
