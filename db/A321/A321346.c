/*
 * A321346  Number of integer partitions of n containing no prime powers > 1.
 * https://oeis.org/A321346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 107, u32 n <= 287, u64 n <= 350,
 *            u128 n <= 350, big any n
 * Helpers:   allocate_seq_arr, free_seq_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A321346: number of integer partitions of n containing no prime
 * powers > 1.  The code builds a sieve up to n to determine which parts
 * (1 and composite non‑prime‑power numbers) are allowed, then uses the
 * standard coin‑change DP to count partitions.  Results are memoised
 * across calls. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/allocate_seq_arr.h"
#include "../utils/free_seq_arr.h"

/* Helper to free an array allocated by allocate_seq_arr */

/* Memoised DP table */
static seq_t *dp_table = NULL;   /* dp_table[i] = a(i) for 0 <= i <= max_n */
static size_t dp_size = 0;       /* dp_table entries allocated (max_n + 1) */

/* Extend the DP table to cover at least n */
static void extend_dp(idx_t n)
{
    size_t new_size = (size_t)n + 1;
    if (new_size <= dp_size)
        return; /* already computed */

    /* ---------- Build sieve up to n ---------- */
    bool *is_prime = (bool *)malloc(new_size * sizeof(bool));
    if (!is_prime)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i < new_size; ++i)
        is_prime[i] = false;
    if (new_size > 2) {
        for (size_t i = 2; i < new_size; ++i)
            is_prime[i] = true;
        for (size_t i = 2; i * i < new_size; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j < new_size; j += i)
                    is_prime[j] = false;
            }
        }
    }

    /* ---------- Determine allowed parts ---------- */
    bool *is_allowed = (bool *)malloc(new_size * sizeof(bool));
    if (!is_allowed) {
        free(is_prime);
        exit(EXIT_FAILURE);
    }
    for (size_t i = 0; i < new_size; ++i)
        is_allowed[i] = false;
    if (new_size > 1)
        is_allowed[1] = true;  /* 1 is always allowed */

    for (size_t p = 2; p < new_size; ++p) {
        if (is_prime[p]) {
            /* Mark all prime powers of p (including p itself) as disallowed */
            uint64_t val = p;
            while (val < new_size) {
                is_allowed[(size_t)val] = false;
                if (val > (uint64_t)(new_size - 1) / p)
                    break;
                val *= p;
            }
        }
    }

    /* For composite numbers that are not prime powers, mark allowed */
    for (size_t i = 2; i < new_size; ++i) {
        if (!is_allowed[i]) { /* currently false because could be prime power */
            /* Determine if i is a prime power: already handled by sieve.
               If not marked false, it is allowed. */
        } else {
            /* If is_allowed[i] is still false after prime-power marking,
               then it is not a prime power.  Mark it true. */
        }
    }
    /* Actually, above loop does nothing.  Instead, initialise allowed
       to true for all composites, then mark false for prime powers.  */
    for (size_t i = 2; i < new_size; ++i)
        is_allowed[i] = true;
    for (size_t p = 2; p < new_size; ++p) {
        if (is_prime[p]) {
            uint64_t val = p;
            while (val < new_size) {
                is_allowed[(size_t)val] = false;
                if (val > (uint64_t)(new_size - 1) / p)
                    break;
                val *= p;
            }
        }
    }

    /* ---------- Compute DP ---------- */
    seq_t *new_dp = allocate_seq_arr(new_size);
    for (size_t i = 0; i < new_size; ++i)
        new_dp[i] = 0;
    new_dp[0] = 1;  /* empty partition */

    for (size_t part = 1; part < new_size; ++part) {
        if (is_allowed[part]) {
            for (size_t j = part; j < new_size; ++j) {
                new_dp[j] += new_dp[j - part];
            }
        }
    }

    /* ---------- Replace old DP ---------- */
    if (dp_table)
        free_seq_arr(dp_table);
    dp_table = new_dp;
    dp_size = new_size;

    free(is_prime);
    free(is_allowed);
}

/* Public function required by harness */
seq_t a(idx_t n)
{
    if ((size_t)n >= dp_size)
        extend_dp(n);
    return dp_table[(size_t)n];
}
