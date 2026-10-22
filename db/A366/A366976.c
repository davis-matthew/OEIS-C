/*
 * A366976  Numbers that cannot be written as sum of two or more consecutive
 *          nonprimes.
 * https://oeis.org/A366976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   alloc_bool
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A366976: numbers that cannot be written as sum of two or more consecutive
   nonprimes. The routine builds the list of nonprime integers up to a sufficient
   bound, marks all sums of length ≥2 as representable, collects the remaining
   numbers and returns the requested term. */
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_bool.h"

/* forward declaration required by the build system */
seq_t a(idx_t n);

/* static state */
static bool          initialized = false;
static idx_t         requiredLen = 0;

static idx_t         limit      = 0;          /* current upper bound of numbers */
static seq_t *       terms      = NULL;      /* array of non‑representable numbers */
static size_t        termsCnt   = 0;          /* size of 'terms' */

/* build the list of non‑representable numbers up to 'limit' */
static void compute_terms(void)
{
    /* 1. sieve for primes */
    bool *prime = alloc_bool(limit + 1);
    if (!prime) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) prime[i] = true;
    prime[0] = prime[1] = false;
    for (idx_t i = 2; i * i <= limit; ++i)
        if (prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                prime[j] = false;

    /* 2. collect nonprime numbers */
    size_t noncnt = 0;
    /* first reserve space */
    size_t noncap = limit > 1000 ? limit : 1000;
    seq_t *nonprimes = (seq_t *)malloc(noncap * sizeof(seq_t));
    if (!nonprimes) exit(EXIT_FAILURE);
    for (idx_t x = 1; x <= limit; ++x) {
        if (!prime[x]) {
            if (noncnt == noncap) {
                noncap <<= 1;
                seq_t *tmp = (seq_t *)realloc(nonprimes, noncap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                nonprimes = tmp;
            }
            nonprimes[noncnt++] = (seq_t)x;
        }
    }
    free(prime);  /* we no longer need the prime sieve */

    /* 3. mark representable sums */
    bool *rep = alloc_bool(limit + 1);
    if (!rep) {
        free(nonprimes);
        exit(EXIT_FAILURE);
    }
    for (idx_t i = 0; i <= limit; ++i) rep[i] = false;

    for (size_t i = 0; i + 1 < noncnt; ++i) {
        seq_t sum = 0;
        for (size_t j = i; j < noncnt; ++j) {
            sum += nonprimes[j];
            if (sum > limit) break;
            if (j - i + 1 >= 2)
                rep[(idx_t)sum] = true;
        }
    }

    /* 4. collect non‑representable numbers */
    size_t cap = 1024;
    seq_t *tmp = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!tmp) {
        free(nonprimes);
        free(rep);
        exit(EXIT_FAILURE);
    }
    size_t sz = 0;
    for (idx_t x = 1; x <= limit; ++x) {
        if (!rep[x]) {
            if (sz == cap) {
                cap <<= 1;
                seq_t *newp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                if (!newp) {
                    free(tmp);
                    free(nonprimes);
                    free(rep);
                    exit(EXIT_FAILURE);
                }
                tmp = newp;
            }
            tmp[sz++] = (seq_t)x;
        }
    }

    /* 5. store the list */
    if (terms) free(terms);
    terms   = tmp;
    termsCnt = sz;

    free(nonprimes);
    free(rep);
}

/* ensure that at least 'req' terms are available */
static void init(idx_t req)
{
    if (!initialized) {
        limit = 10000;          /* initial bound guess */
        initialized = true;
    }
    requiredLen = req;
    while (termsCnt < req) {
        limit <<= 1;            /* double the bound */
        compute_terms();
    }
}

/* public function required by the repository */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; safe return */
    init(n);
    return terms[n - 1];
}
