/*
 * A243616  Primes p of the form prime(n) + digitsum(n).
 * https://oeis.org/A243616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 626, u32 n <= 367, u64 n <= 2535,
 *            u128 n <= 2535, big any n
 * Helpers:   build_primes_upto, digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A243616: primes p such that p = prime(n) + digit_sum(n) */
/* a(1) = 3, a(2) = 5, a(3) = 11, a(4) = 19, a(5) = 53, … */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

/* cached data ----------------------------------------------------------- */

static uint64_t *prime_list   = NULL;   /* primes up to a value limit */
static size_t   prime_count   = 0;      /* number of primes in prime_list */

static seq_t *seq_cache        = NULL;   /* sequence values a(i) */
static size_t seq_len          = 0;      /* number of sequence values cached */
static size_t seq_cap          = 0;      /* cache capacity */

static idx_t next_candidate    = 1;      /* next n to test for prime(n)+sum(n) */

/* --------------------------------------------------------------------- */

static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap >= needed) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap   = new_cap;
}

/* Resize the prime table so that it contains at least idx primes. */
static void ensure_prime_table(idx_t idx)
{
    if (prime_list == NULL) {
        /* initial table – large enough for n up to at least 20 000 */
        const idx_t LIMIT = 250000;          /* value limit, not count */
        build_primes_upto(LIMIT, &prime_list, &prime_count);
    }
    if (idx <= prime_count) return;

    /* Need more primes – increase the value limit roughly twice the last prime. */
    idx_t new_limit = prime_list[prime_count - 1] * 2 + 100;
    free(prime_list);
    build_primes_upto(new_limit, &prime_list, &prime_count);
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for offset 1, harmless */

    /* Generate terms until we have the n‑th one */
    while (seq_len < (size_t)n) {
        /* Ensure we have enough primes to evaluate prime(next_candidate) */
        ensure_prime_table(next_candidate);

        /* Compute candidate p = prime(next_candidate) + digit_sum(next_candidate) */
        seq_t cand = (seq_t)prime_list[next_candidate - 1] + digit_sum((seq_t)next_candidate);

        if (is_prime_big(cand)) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = cand;
        }

        ++next_candidate;   /* move to next n regardless of primality */
    }

    return seq_cache[(size_t)n - 1];
}
