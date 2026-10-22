/*
 * A377057  Numbers k such that there is at least one prime-power between
 *          prime(k)+1 and prime(k+1)-1.
 * https://oeis.org/A377057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   build_primes_upto, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A377057: Numbers k such that the interval between prime(k)
   and prime(k+1) contains at least one prime‑power (prime power with
   exponent ≥ 2).  The implementation generates primes up to a growing
   limit, uses a smallest‑prime‑factor sieve for fast prime‑power
   detection, and memoises the resulting sequence. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/build_spf.h"

static bool initialized = false;
static uint64_t *primes = NULL;      /* primes[i] = i-th prime (0‑based) */
static size_t    prime_cnt = 0;      /* number of primes stored      */
static idx_t    *spf = NULL;         /* smallest prime factor table   */
static idx_t     spf_limit = 0;      /* current size of spf array     */

static seq_t  *terms = NULL;         /* cached sequence a(n) values   */
static size_t  terms_cap = 0;        /* allocated capacity in terms   */
static size_t  terms_len = 0;        /* number of terms already stored*/

static idx_t next_k = 1;             /* next k index to process      */

/* Ensure that primes and spf tables are built up to at least 'limit'. */
static void ensure_limit(idx_t limit)
{
    if (limit <= spf_limit) return;

    /* free old tables if they exist */
    if (primes) free(primes);
    if (spf)    free(spf);

    build_primes_upto(limit, &primes, &prime_cnt);
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Append a new term to the cached sequence. */
static void push_term(seq_t v)
{
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    } else if (terms_len == terms_cap) {
        terms_cap *= 2;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
    }
    terms[terms_len++] = v;
}

/* Return true iff n is a prime power with exponent >= 2. */
static bool has_prime_power(idx_t n)
{
    if (n <= 1) return false;
    idx_t p = spf[n];
    if (p == 0 || p == n) return false;  /* prime itself, exponent 1 → not counted */

    int exp = 0;
    while (n % p == 0) {
        n /= p;
        exp++;
    }
    if (n != 1) return false;   /* more than one prime factor */
    return exp >= 2;
}

/* Ensure the cached sequence has at least 'need' terms. */
static void ensure_terms(size_t need)
{
    if (terms_len >= need) return;

    if (!initialized) {
        /* initial sieve limit – roughly enough for 10 000 terms */
        idx_t init_limit = 2000000;
        ensure_limit(init_limit);
        initialized = true;
    }

    /* Process k values until we have enough terms */
    while (terms_len < need) {
        /* Extend sieve if we run out of primes */
        while (next_k + 1 >= prime_cnt) {
            idx_t new_limit = spf_limit * 2;
            ensure_limit(new_limit);
        }

        idx_t pk  = primes[next_k - 1];
        idx_t pk1 = primes[next_k];        /* prime(k+1) */

        bool found = false;
        if (pk1 - pk > 1) {
            for (idx_t v = pk + 1; v < pk1; ++v) {
                if (has_prime_power(v)) {
                    found = true;
                    break;
                }
            }
        }
        if (found) push_term((seq_t)next_k);

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1, behavior below irrelevant */

    ensure_terms((size_t)n);
    return terms[(size_t)n - 1];
}
