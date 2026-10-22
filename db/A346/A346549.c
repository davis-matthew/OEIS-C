/*
 * A346549  Runs (of length > 1) of consecutive squarefree semiprimes.
 * https://oeis.org/A346549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A346549: runs of consecutive squarefree semiprimes.
   The algorithm sieves up to a limit, identifies semiprime numbers
   with two distinct prime factors, marks every consecutive pair
   as belonging to a run, and stores all such numbers in an
   increasing array.  The function a(n) returns the n-th element
   of this array (1‑based index). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t* run_terms = NULL;      /* array of sequence terms */
static size_t run_term_count = 0;    /* number of terms in run_terms   */
static idx_t  run_search_limit = 0;  /* current sieve limit             */

/* --------------------------------------------------------------------- */
/*   Helper: build lists of squarefree semiprimes up to limit            */
/* --------------------------------------------------------------------- */
static void build_terms_up_to(idx_t limit)
{
    /* Allocate arrays */
    uint32_t *spf = (uint32_t*)malloc((limit + 2) * sizeof(uint32_t));
    uint8_t  *semip  = (uint8_t*)malloc((limit + 3) * sizeof(uint8_t));
    uint8_t  *run_mem = (uint8_t*)malloc((limit + 3) * sizeof(uint8_t));

    if (!spf || !semip || !run_mem) {
        /* Memory allocation failure – abort */
        exit(EXIT_FAILURE);
    }

    /* Smallest prime factor sieve */
    memset(spf, 0, (limit + 2) * sizeof(uint32_t));
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if ((uint64_t)i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Identify squarefree semiprimes (p*q, p<q primes) */
    memset(semip, 0, (limit + 3) * sizeof(uint8_t));
    for (idx_t n = 2; n <= limit; ++n) {
        uint32_t p = spf[n];
        if (p == n) continue;                /* prime */
        uint32_t q = (uint32_t)(n / p);
        if (q != p && spf[q] == q)           /* q is prime and distinct */
            semip[n] = 1;
    }

    /* Mark run members: consecutive semiprime pairs */
    memset(run_mem, 0, (limit + 3) * sizeof(uint8_t));
    for (idx_t i = 2; i < limit; ++i) {
        if (semip[i] && semip[i + 1]) {
            run_mem[i]     = 1;
            run_mem[i + 1] = 1;
        }
    }

    /* Count how many run members */
    size_t new_count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (run_mem[i]) ++new_count;

    /* Allocate/replace term array */
    seq_t *new_terms = (seq_t*)malloc(new_count * sizeof(seq_t));
    if (!new_terms) exit(EXIT_FAILURE);

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (run_mem[i]) new_terms[pos++] = (seq_t)i;

    /* Free old array and replace */
    free(run_terms);
    run_terms = new_terms;
    run_term_count = new_count;
    run_search_limit = limit;

    /* Free helpers */
    free(spf);
    free(semip);
    free(run_mem);
}

/* --------------------------------------------------------------------- */
/*   Ensure we have at least n_needed terms in run_terms                 */
/* --------------------------------------------------------------------- */
static void ensure_terms(idx_t n_needed)
{
    while (run_term_count < n_needed) {
        idx_t new_limit = run_search_limit ? run_search_limit * 2 : 500000;
        /* Cap to 10 million to avoid runaway */
        if (new_limit > 10000000) new_limit = 10000000;
        build_terms_up_to(new_limit);
    }
}

/* --------------------------------------------------------------------- */
/*   The required sequence function                                     */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined, but avoid crash */
    ensure_terms(n);
    return run_terms[n - 1];
}
