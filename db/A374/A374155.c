/*
 * A374155  a(n) is the least prime that is a quadratic residue modulo
 *          prime(n). First column of A373751.
 * https://oeis.org/A374155
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime that is a quadratic residue modulo prime(n)
 * Range:     u8 n <= 54, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   get_prime, mod_pow, sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/sieve_primes.h"
#include "../utils/mod_pow.h"

static idx_t *candidate_primes = NULL;
static size_t candidate_cnt = 0;
static idx_t candidate_limit = 0;

static void ensure_primes_up_to(idx_t limit)
{
    if (candidate_limit >= limit) return;
    idx_t *new_primes = NULL;
    size_t new_cnt = 0;
    sieve_primes(limit, &new_primes, &new_cnt);
    if (candidate_primes) free(candidate_primes);
    candidate_primes = new_primes;
    candidate_cnt = new_cnt;
    candidate_limit = limit;
}

seq_t a(idx_t n)
{
    /* Initialize the candidate prime list once */
    if (candidate_primes == NULL) {
        const idx_t INIT_BOUND = 2000000; /* sufficient for n ≈ 10 000 */
        ensure_primes_up_to(INIT_BOUND);
    }

    idx_t p = get_prime(n);          /* prime(n) */
    if (p == 2) return (seq_t)2;     /* special case */

    uint64_t exp = ((uint64_t)p - 1) >> 1;   /* (p-1)/2 */

    /* Search the list of primes for the first residue */
    for (size_t i = 0; i < candidate_cnt; ++i) {
        idx_t q = candidate_primes[i];
        if (q == p) return (seq_t)q;                      /* q == p (residue 0) */
        if (mod_pow((seq_t)q, exp, (seq_t)p) == (seq_t)1)
            return (seq_t)q;
    }

    /* If not found, extend the prime list and retry */
    idx_t new_limit = candidate_limit * 2;
    if (new_limit < p + 10) new_limit = p + 10;
    ensure_primes_up_to(new_limit);

    for (size_t i = 0; i < candidate_cnt; ++i) {
        idx_t q = candidate_primes[i];
        if (q == p) return (seq_t)q;
        if (mod_pow((seq_t)q, exp, (seq_t)p) == (seq_t)1)
            return (seq_t)q;
    }

    /* Should not reach here for valid n */
    return (seq_t)0;
}
