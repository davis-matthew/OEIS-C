/*
 * A347761  Number of compositions (ordered partitions) of n into at most 6
 *          prime parts (counting 1 as a prime).
 * https://oeis.org/A347761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A347761 : Number of compositions (ordered partitions) of n
    into at most 6 prime parts (counting 1 as a prime).  The function
    a(n) returns this number for any n ≥ 0, computed by dynamic
    programming and a sieve for primes up to n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime generation utilities ---------- */
static uint64_t *prime_cache = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Generate all primes ≤ limit (inclusive).  The result is stored
   in *out, and *cnt gives the number of primes.  The array is
   allocated with malloc and must be freed by the caller. */
static void generate_primes_upto(idx_t limit, uint64_t **out, size_t *cnt)
{
    if (limit < 2) {
        *out = NULL;
        *cnt = 0;
        return;
    }

    size_t sz = (size_t)(limit + 1);
    bool *sieve = (bool *)malloc(sz);
    for (size_t i = 0; i < sz; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (size_t i = 2; i * i <= (size_t)limit; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= (size_t)limit; j += i) sieve[j] = false;
        }
    }

    /* Count primes */
    size_t pcnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) if (sieve[i]) ++pcnt;

    uint64_t *pr = (uint64_t *)malloc(pcnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i) if (sieve[i]) pr[idx++] = i;

    free(sieve);
    *out = pr;
    *cnt = pcnt;
}

/* Ensure prime_cache contains all primes up to n.  Rebuilds
   the table if necessary. */
static void ensure_primes(idx_t n)
{
    if (n <= prime_limit) return;      /* already sufficient */

    free(prime_cache);
    generate_primes_upto(n, &prime_cache, &prime_count);
    prime_limit = n;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Composition of zero: only the empty composition. */
    if (n == 0) return (seq_t)1;

    /* Build the list of primes needed for this n. */
    ensure_primes(n);

    size_t len = (size_t)n + 1;           /* array size: 0 … n */
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));

    if (!prev || !curr) {                 /* allocation failure */
        if (prev) free(prev);
        if (curr) free(curr);
        return (seq_t)0;
    }

    /* Base: exactly 0 parts. */
    prev[0] = (seq_t)1;

    seq_t answer = 0;                     /* sum of compositions for k = 0..6 */

    /* At k = 0, only n == 0 contributes; for n > 0 prev[n] == 0. */
    answer += prev[(size_t)n];

    /* Iterate over number of parts k = 1 ... 6. */
    for (int k = 1; k <= 6; ++k) {
        /* Reset current row. */
        for (size_t i = 0; i < len; ++i) curr[i] = (seq_t)0;

        /* Add compositions ending with part 1 (allowed part). */
        for (size_t s = 1; s < len; ++s) {
            curr[s] += prev[s - 1];
        }

        /* Add compositions ending with each prime part. */
        for (size_t pi = 0; pi < prime_count; ++pi) {
            uint64_t p = prime_cache[pi];
            if (p > n) break;
            size_t ps = (size_t)p;
            for (size_t s = ps; s < len; ++s) {
                curr[s] += prev[s - ps];
            }
        }

        /* Add contributions for compositions with exactly k parts. */
        answer += curr[(size_t)n];

        /* Prepare for next iteration: swap prev and curr. */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* Clean up. */
    free(prev);
    free(curr);

    return answer;
}
