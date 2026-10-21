/*
 * A244723  Nonprimes n such that, mu(n) = mu(phi(n)).
 * https://oeis.org/A244723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th composite integer (plus 1 as the first term) *
 *            that satisfies μ(n) = μ(φ(n)). The program builds a linear sieve
 *            * for φ and μ up to a limit sufficient to contain the requested
 *            index * and caches the qualifying numbers for subsequent calls.
 *            No hard‑coded * table is used. The funct...
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Global cache state                                                   */
/* --------------------------------------------------------------------- */
static seq_t   *term_cache   = NULL;   /* stored sequence terms          */
static size_t  term_count   = 0;       /* number of cached terms         */
static size_t  sieve_limit  = 0;       /* current sieve limit            */

/* --------------------------------------------------------------------- */
/*  Linear sieve that also computes φ and μ for all numbers up to
 *  'limit'.  The results are stored in dynamically allocated arrays
 *  that are freed by the caller after use.                           */
/* --------------------------------------------------------------------- */
static void linear_sieve(size_t limit,
                         idx_t *phi, int8_t *mu, uint8_t *is_comp,
                         size_t *prime_cnt, uint32_t **prime_arr)
{
    size_t i, j;
    *prime_cnt = 0;
    *prime_arr = NULL;

    size_t prime_cap = limit / 10 + 10;
    *prime_arr = (uint32_t*)malloc(prime_cap * sizeof(uint32_t));
    if (!*prime_arr) exit(1);

    for (i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {                     /* i is prime          */
            if (*prime_cnt == prime_cap) {
                prime_cap *= 2;
                *prime_arr = (uint32_t*)realloc(*prime_arr,
                                                prime_cap * sizeof(uint32_t));
                if (!*prime_arr) exit(1);
            }
            (*prime_arr)[(*prime_cnt)++] = (uint32_t)i;
            mu[i] = -1;
            phi[i] = (idx_t)i - 1;
        }

        for (j = 0; j < *prime_cnt; ++j) {
            uint32_t p = (*prime_arr)[j];
            size_t ip = i * (size_t)p;
            if (ip > limit) break;

            is_comp[ip] = 1;
            if (i % p == 0) {                     /* p divides i         */
                mu[ip]  = 0;
                phi[ip] = phi[i] * (idx_t)p;
                break;                          /* stop after first hit */
            } else {                              /* p coprime to i      */
                mu[ip]  = -mu[i];
                phi[ip] = phi[i] * (idx_t)(p - 1);
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/*  Build all qualifying numbers up to 'limit' and store them in the
 *  global cache 'term_cache'.  The function frees any previously
 *  allocated cache.                                                    */
/* --------------------------------------------------------------------- */
static void build_terms(size_t limit)
{
    /* Allocate working arrays.  Use idx_t for φ to avoid overflow
       when limit is large; μ fits in int8_t. */
    idx_t   *phi      = (idx_t*)malloc((limit + 1) * sizeof(idx_t));
    int8_t  *mu       = (int8_t*)malloc((limit + 1) * sizeof(int8_t));
    uint8_t *is_comp  = (uint8_t*)calloc((limit + 1), sizeof(uint8_t));

    if (!phi || !mu || !is_comp) { exit(1); }

    size_t prime_cnt = 0;
    uint32_t *primes = NULL;
    linear_sieve(limit, phi, mu, is_comp, &prime_cnt, &primes);

    /* Allocate a workspace for the qualifying numbers. */
    seq_t *terms = (seq_t*)malloc((limit + 1) * sizeof(seq_t));
    if (!terms) exit(1);
    size_t terms_cnt = 0;

    /* a(1) = 1 is always part of the sequence. */
    terms[terms_cnt++] = (seq_t)1;

    for (size_t i = 2; i <= limit; ++i) {
        if (is_comp[i]) {                            /* composite?         */
            idx_t phi_i = phi[i];
            if (mu[i] == mu[phi_i]) {
                terms[terms_cnt++] = (seq_t)i;
            }
        }
    }

    /* Re‑allocate the terms array to its exact size and store it in
       the global cache. */
    terms = (seq_t*)realloc(terms, terms_cnt * sizeof(seq_t));
    if (term_cache) free(term_cache);
    term_cache = terms;
    term_count = terms_cnt;
    sieve_limit = limit;

    /* Clean up temporary arrays. */
    free(phi);
    free(mu);
    free(is_comp);
    free(primes);
}

/* --------------------------------------------------------------------- */
/*  Ensure that the cache contains at least N terms (1‑based).  If
 *  necessary, increase the sieve limit and rebuild the cache.           */
/* --------------------------------------------------------------------- */
static void ensure_terms(idx_t N)
{
    if (N <= term_count) return;       /* already sufficient          */

    /* Start with a modest limit; double it until we have enough
       terms.  The limit grows logarithmically with N. */
    size_t new_limit = sieve_limit ? sieve_limit * 2 : 1000;

    while (1) {
        build_terms(new_limit);
        if (term_count >= (size_t)N) break;
        new_limit *= 2;
    }
}

/* --------------------------------------------------------------------- */
/*  Public entry point:  return the nth term of the sequence.           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined, but safe      */
    ensure_terms(n);
    return term_cache[(size_t)n - 1];
}
