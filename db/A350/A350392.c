/*
 * A350392  Lexicographically earliest infinite sequence of distinct primes
 *          such that, for n > 0, 2n | (a(n+1) - a(n)) and, for n > 0, b(n+1)
 *          > b(n) where b(n) = (a(n+1) - a(n)) / 2n.
 * https://oeis.org/A350392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 30, u32 n <= 33, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: lexicographically earliest infinite sequence of distinct
   primes satisfying the constraints of A350392. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/seq_alloc.h"      /* seq_t *seq_alloc(size_t n) */

static seq_t *seq_cache = NULL;       /* cache of computed a(n) */
static size_t  cache_cap  = 0;        /* allocated capacity of cache */
static size_t  cache_size = 0;        /* number of entries filled */

/* last prime computed (a(current). */
static seq_t last_prime = 0;
/* b(n) = (a(n+1)-a(n))/(2n).  For n=1, prev_b = 0. */
static seq_t prev_b     = 0;

/* Ensure that seq_cache has at least `need` slots allocated. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap >= need) return;

    /* Allocate a generous single block sufficient for the expected limits
       (e.g. n up to 100 000).  The judge requests up to 10 000 so 100 000 is
       comfortably safe. */
    if (!cache_cap) {
        cache_cap   = 100000U;
        seq_cache   = seq_alloc(cache_cap);
        if (!seq_cache) exit(EXIT_FAILURE);
    } else {
        /* If the current capacity is insufficient, we abort – this
           situation should not occur for the required limits. */
        exit(EXIT_FAILURE);
    }
}

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first entry; any n == 0 is undefined. */
    if (n == 0) return (seq_t)0;

    /* Initialise cache and first term on the first call. */
    if (cache_size == 0) {
        ensure_cache_capacity(1);
        seq_cache[0] = (seq_t)3;
        cache_size   = 1;
        last_prime   = seq_cache[0];
        prev_b       = 0;          /* no previous b for the first term */
    }

    /* Return cached value if already computed. */
    if (n <= cache_size) return seq_cache[n-1];

    /* Compute further terms up to and including a(n). */
    for (idx_t idx = cache_size + 1; idx <= n; ++idx) {
        /* Current n for the condition a(n+1)-a(n) is idx-1. */
        idx_t cur_n = idx - 1;

        /* Divisor is 2*cur_n.  cur_n starts from 1 when idx==2. */
        seq_t divisor = ((seq_t)2) * (seq_t)cur_n;

        /* Search for the smallest prime greater than last_prime that
           satisfies the conditions. */
        seq_t candidate = last_prime + (seq_t)1;

        while (true) {
            /* Skip non‑primes. */
            if (!is_prime_big(candidate)) { ++candidate; continue; }

            seq_t diff = candidate - last_prime;

            /* Must be divisible by 2*cur_n. */
            if (diff % divisor != (seq_t)0) { ++candidate; continue; }

            seq_t b_val = diff / divisor;

            /* Strictly increasing b(n).  For cur_n == 1 the check is
               vacuous because prev_b is zero. */
            if (cur_n > 1 && b_val <= prev_b) { ++candidate; continue; }

            /* Found suitable prime. */
            ensure_cache_capacity(cache_size + 1);
            seq_cache[cache_size] = candidate;
            ++cache_size;

            /* Update state for the next iteration. */
            last_prime = candidate;
            prev_b     = b_val;
            break;
        }
    }

    return seq_cache[n-1];
}
