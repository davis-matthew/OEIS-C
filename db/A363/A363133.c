/*
 * A363133  Numbers > 1 whose prime indices satisfy 2*(minimum) = (mean).
 * https://oeis.org/A363133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A363133 – numbers > 1 whose prime indices satisfy
   2 * (minimum index) = (average of indices)                 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIME_SIEVE_LIMIT ((idx_t)1000000ULL)     /* sieve up to 1 000 000 */

static uint32_t *primes      = NULL;  /* list of primes up to PRIME_SIEVE_LIMIT  */
static size_t   prime_cnt    = 0;     /* number of primes                         */
static uint32_t *prime_index = NULL;  /* prime_index[p] = 1‑based index of prime p */
static bool      primes_built = false;

static seq_t *terms   = NULL;  /* cached terms a(1), a(2), …                */
static size_t  terms_n = 0;    /* number of cached terms                    */
static size_t  terms_cap = 0;  /* capacity of the terms array               */
static idx_t   next_num = 2;   /* next integer to test                      */

/* ------------------------------------------------------------------ */
/* Build the list of primes up to PRIME_SIEVE_LIMIT and fill the       */
/* prime_index table.  This is called once by a().                     */
static void build_primes(void)
{
    if (primes_built) return;
    primes_built = true;

    /* Allocate sieve array */
    uint8_t *mark = (uint8_t*)calloc((size_t)PRIME_SIEVE_LIMIT + 1, sizeof(uint8_t));
    if (!mark) exit(EXIT_FAILURE);

    /* Sieve of Eratosthenes */
    idx_t limit_root = (idx_t)((double)PRIME_SIEVE_LIMIT * 0.5);
    for (idx_t i = 2; i <= limit_root; ++i) {
        if (!mark[i]) {
            for (idx_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i) {
                mark[j] = 1;
            }
        }
    }

    /* Count primes */
    for (idx_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (!mark[i]) ++prime_cnt;

    /* Allocate arrays */
    primes = (uint32_t*)calloc(prime_cnt, sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    prime_index = (uint32_t*)calloc((size_t)PRIME_SIEVE_LIMIT + 1, sizeof(uint32_t));
    if (!prime_index) exit(EXIT_FAILURE);

    /* Populate primes and prime_index */
    size_t idx = 0;
    for (idx_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!mark[i]) {
            primes[idx]      = (uint32_t)i;
            prime_index[i]   = (uint32_t)(idx + 1);  /* 1‑based index */
            ++idx;
        }
    }

    free(mark);
}

/* ------------------------------------------------------------------ */
/* Append a new term to the terms cache, growing the array as needed   */
static void append_term(seq_t val)
{
    if (terms_n == terms_cap) {
        terms_cap = terms_cap ? (terms_cap << 1) : 16;
        terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
        if (!terms) exit(EXIT_FAILURE);
    }
    terms[terms_n++] = val;
}

/* ------------------------------------------------------------------ */
/* Main sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* sequence offset is 1 */

    build_primes();

    /* Generate terms until we have at least n entries */
    while (terms_n < (size_t)n) {
        idx_t x = next_num++;
        idx_t tmp = x;

        seq_t sum_indices = 0;
        seq_t count       = 0;
        uint32_t min_idx  = (uint32_t)-1;  /* max value */

        /* Trial division by small primes up to sqrt(tmp) */
        for (size_t k = 0; k < prime_cnt; ++k) {
            uint32_t p = primes[k];
            idx_t  sq = (idx_t)p * (idx_t)p;
            if (sq > tmp) break;

            if (tmp % p == 0) {
                uint32_t p_idx = prime_index[p];
                /* Count multiplicity */
                do {
                    tmp /= p;
                    count += 1;
                    sum_indices += (seq_t)p_idx;
                } while (tmp % p == 0);

                if (p_idx < min_idx) min_idx = p_idx;
            }
        }

        /* Remaining factor if > 1 (it must be prime) */
        if (tmp > 1) {
            /* tmp is a prime <= PRIME_SIEVE_LIMIT because x is
               small enough for the required n (see analysis).          */
            uint32_t p_idx = prime_index[(uint32_t)tmp];
            count += 1;
            sum_indices += (seq_t)p_idx;
            if (p_idx < min_idx) min_idx = p_idx;
        }

        /* Condition: 2 * (minimum index) == (average of indices) */
        /* i.e. sum_indices == 2 * min_idx * count   */
        if (count > 0 && sum_indices == (seq_t)2 * (seq_t)min_idx * count) {
            append_term((seq_t)x);
        }
    }

    return terms[n - 1];
}
