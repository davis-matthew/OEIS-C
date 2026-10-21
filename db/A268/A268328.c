/*
 * A268328  Integers of the form (prime(m) + prime(m+1))/10 for some m.
 * https://oeis.org/A268328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer of the form (prime(m)+prime(m+1))/10
 *            where m ranges over positive integers and the sum is divisible
 *            by 10
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, seq_alloc, seq_alloc_new_array,
 *            seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Cached primes (uint64_t) up to current sieve limit */
static uint64_t *g_primes = NULL;
static size_t   g_primes_cnt = 0;
static idx_t    g_limit = 0;
static idx_t    g_last_prime_needed = 0;   /* index of the last prime used in a pair */

/* Cached sequence values */
static seq_t   *g_seq = NULL;
static size_t   g_seq_len = 0;
static size_t   g_seq_cap = 0;

/* Expand the sieve and recompute primes when more primes are required. */
static void ensure_primes(void)
{
    idx_t new_limit = g_limit ? g_limit * 2 : 1000;
    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;

    build_primes_upto(new_limit, &new_primes, &new_cnt);

    /* free the old prime array */
    free(g_primes);
    g_primes = new_primes;
    g_primes_cnt = new_cnt;
    g_limit = new_limit;
}

/* Grow the sequence array when necessary. */
static void ensure_seq_cap(void)
{
    if (g_seq_len >= g_seq_cap)
    {
        size_t new_cap = g_seq_cap ? g_seq_cap * 2 : 16;
        seq_t *new_seq = seq_alloc_new_array(new_cap);
        if (g_seq)
        {
            for (size_t i = 0; i < g_seq_len; ++i)
                new_seq[i] = g_seq[i];
            seq_free_array(g_seq);
        }
        g_seq = new_seq;
        g_seq_cap = new_cap;
    }
}

/* Generate sequence values until the desired length n is reached. */
static void ensure_sequence(idx_t n)
{
    while (g_seq_len < n)
    {
        /* If we have exhausted the current prime list, enlarge it. */
        if (g_last_prime_needed + 1 >= g_primes_cnt)
            ensure_primes();

        /* Process the next pair of consecutive primes. */
        uint64_t sum = g_primes[g_last_prime_needed] + g_primes[g_last_prime_needed + 1];
        if ((sum % 10) == 0)
        {
            seq_t val = sum / 10;
            ensure_seq_cap();
            g_seq[g_seq_len++] = val;
        }
        ++g_last_prime_needed;
    }
}

/* External interface required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; value for n=0 is unused */
    ensure_sequence(n);
    return g_seq[n - 1];
}
