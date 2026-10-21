/*
 * A243914  Even numbers which are twice the sum of a twin prime pair, but
 *          cannot be expressed as the sum of 2 distinct twin prime pairs.
 * https://oeis.org/A243914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A243914 – computed by generating twin primes up to 2 500 000,
    detecting sums of two distinct twin start primes.
    The sequence is produced on first call and memoised.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t  *g_terms   = NULL;   /* array of sequence values */
static size_t  g_nterms  = 0;      /* number of terms stored   */
static bool    g_built   = false;  /* flag to build once       */

/* --------------------------------------------------------------------- */
static void build_sequence(void)
{
    if (g_built) return;

    /* --------------------------------------------------------------- */
    /* 1.  Generate all primes up to LIMIT (must be >= max twin start) */
    /* --------------------------------------------------------------- */
    const idx_t LIMIT = 2500000;          /* ensures all terms < 10^7   */
    const size_t sz   = (size_t)LIMIT + 1;

    bool *is_prime = (bool*)malloc(sz);
    if (!is_prime) return;
    memset(is_prime, 1, sz);
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i < sz; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j < sz; j += i)
                is_prime[j] = false;
        }
    }

    /* --------------------------------------------------------------- */
    /* 2.  Collect twin–start primes (p such that p and p+2 are prime) */
    /* --------------------------------------------------------------- */
    size_t cap   = 50000;                 /* initial capacity          */
    uint64_t *twin_start = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!twin_start) { free(is_prime); return; }
    size_t twin_cnt = 0;

    for (size_t p = 2; p + 2 < sz; ++p) {
        if (is_prime[p] && is_prime[p + 2]) {
            if (twin_cnt == cap) {               /* grow if needed */
                cap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(twin_start, cap * sizeof(uint64_t));
                if (!tmp) { free(is_prime); free(twin_start); return; }
                twin_start = tmp;
            }
            twin_start[twin_cnt++] = p;
        }
    }

    /* --------------------------------------------------------------- */
    /* 3.  Mark all sums attainable by two *distinct* twin starts     */
    /* --------------------------------------------------------------- */
    const size_t MAX_SUM = 2 * LIMIT;        /* 2*p  (p <= LIMIT)         */
    char *possible = (char*)calloc(MAX_SUM + 1, 1);
    if (!possible) { free(is_prime); free(twin_start); return; }

    for (size_t i = 0; i < twin_cnt; ++i) {
        for (size_t j = i + 1; j < twin_cnt; ++j) {
            uint64_t s = twin_start[i] + twin_start[j];
            if (s > MAX_SUM) break;
            possible[(size_t)s] = 1;
        }
    }

    /* --------------------------------------------------------------- */
    /* 4.  Build sequence terms: 4*(p+1) for eligible twin starts      */
    /* --------------------------------------------------------------- */
    size_t terms_cap = 1024;
    g_terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!g_terms) { free(is_prime); free(twin_start); free(possible); return; }

    for (size_t i = 0; i < twin_cnt; ++i) {
        uint64_t p = twin_start[i];
        if (!possible[(size_t)(2 * p)]) {          /* no representation by two distinct pairs */
            seq_t val = (seq_t)(p + 1ULL) * (seq_t)4;
            if (g_nterms == terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t*)realloc(g_terms, terms_cap * sizeof(seq_t));
                if (!tmp) continue;               /* unlikely, skip this term */
                g_terms = tmp;
            }
            g_terms[g_nterms++] = val;
        }
    }

    /* --------------------------------------------------------------- */
    /* 5.  Clean up temporary arrays                                 */
    /* --------------------------------------------------------------- */
    free(is_prime);
    free(twin_start);
    free(possible);

    g_built = true;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (!g_built) build_sequence();
    if (n > g_nterms) return (seq_t)0;        /* out of known range          */
    return g_terms[n - 1];
}
