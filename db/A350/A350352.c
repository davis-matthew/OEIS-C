/*
 * A350352  Products of three or more distinct prime numbers.
 * https://oeis.org/A350352
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9895 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A350352: Products of three or more distinct prime numbers.
   The sequence consists of all square‑free integers whose number of
   distinct prime factors is at least three, written in increasing order. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*---------------------------------------------------------------*/
static void sieve_terms(uint64_t bound, uint64_t **out_terms, size_t *out_len)
{
    uint8_t *is_prime = (uint8_t*)calloc(bound + 1, 1);
    uint8_t *cnt      = (uint8_t*)calloc(bound + 1, 1);
    uint8_t *sqfree   = (uint8_t*)calloc(bound + 1, 1);
    if (!is_prime || !cnt || !sqfree) abort();

    /* init all numbers as square‑free, except 0 and 1 */
    memset(is_prime, 1, bound + 1);
    is_prime[0] = is_prime[1] = 0;
    memset(sqfree, 1, bound + 1);
    sqfree[0] = sqfree[1] = 0;

    /* Eratosthenes sieve to find primes */
    for (uint64_t p = 2; p * p <= bound; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= bound; j += p)
                is_prime[j] = 0;
        }
    }

    /* Count distinct prime factors and mark non‑square‑free numbers */
    for (uint64_t p = 2; p <= bound; ++p) {
        if (!is_prime[p]) continue;
        for (uint64_t j = p; j <= bound; j += p)
            cnt[j]++;

        uint64_t sq = p * p;
        if (sq <= bound) {
            for (uint64_t j = sq; j <= bound; j += sq)
                sqfree[j] = 0;
        }
    }

    /* Collect qualifying terms */
    *out_len = 0;
    *out_terms = (uint64_t*)malloc((bound + 1) * sizeof(uint64_t));
    if (!*out_terms) abort();

    for (uint64_t i = 2; i <= bound; ++i) {
        if (sqfree[i] && cnt[i] >= 3)
            (*out_terms)[(*out_len)++] = i;
    }

    /* Free temporaries */
    free(is_prime);
    free(cnt);
    free(sqfree);
}

static uint64_t *terms   = NULL; /* cached terms, stored as 64‑bit values   */
static size_t  terms_len = 0;    /* number of cached terms                  */
static uint64_t terms_bound = 0; /* upper bound used for the last sieve     */

/*---------------------------------------------------------------*/
/* Sieve up to 'bound' and collect all numbers that are          */
/* square‑free and have at least three distinct prime factors.   */
/* The collected numbers are returned in 'out_terms' (array of   */
/* uint64_t).  The caller must free 'out_terms'.                 */

/*---------------------------------------------------------------*/
/* Ensure that at least 'n_needed' terms are cached.  If not, a    */
/* sieve with a suitable bound is performed.                       */
/*---------------------------------------------------------------*/
static void ensure_terms_up_to(idx_t n_needed)
{
    if (terms_len >= n_needed)
        return; /* already have enough */

    uint64_t bound = n_needed * 5;
    if (bound < 100)
        bound = 100;

    /* Increase bound until enough terms are found */
    while (1) {
        uint64_t *new_terms = NULL;
        size_t new_len = 0;
        sieve_terms(bound, &new_terms, &new_len);

        if (new_len >= n_needed) {
            /* Replace old cache */
            if (terms)
                free(terms);
            terms      = new_terms;
            terms_len  = new_len;
            terms_bound = bound;
            break;
        }

        /* Not enough terms – try a larger bound */
        free(new_terms);
        bound <<= 1; /* double */
    }
}

/*---------------------------------------------------------------*/
/* Public sequence accessor                                       */
/*---------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n==0, return 0 safely */

    ensure_terms_up_to(n);

    /* cast cached 64‑bit value to seq_t – works for all supported types */
    return (seq_t)terms[n - 1];
}
