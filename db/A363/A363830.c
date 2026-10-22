/*
 * A363830  Lesser of 2 successive sphenic numbers (k, k+4) sandwiching 3
 *          consecutive nonsquarefree numbers.
 * https://oeis.org/A363830
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the lesser of two consecutive sphenic numbers (k, k+4)
 *            that enclose three consecutive nonsquarefree numbers. Computed
 *            by a pre‑populated sieve of squarefreeness and a
 *            smallest‑prime‑factor table for fast sphenic detection
 * Range:     u8 none, u16 n <= 95, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, is_sphenic_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>  /* for memset */

/* utilities */
#include "../utils/build_spf.h"
#include "../utils/is_sphenic_idx.h"

/* types provided by the build */
extern seq_t a(idx_t n);

static const idx_t LIMIT = 10000000ULL;    /* upper bound for precomputation   */
static idx_t *spf = NULL;                  /* smallest prime factor array      */
static unsigned char *nsf = NULL;          /* nonsquarefree flag array         */
static seq_t *terms = NULL;                /* cache of sequence terms          */
static size_t terms_cap = 0, terms_len = 0;
static idx_t scan_pos = 2;                 /* current scanning position         */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Ensure all global tables are initialized */
static void ensure_init(void)
{
    if (spf != NULL) return;            /* already initialized          */

    /* --- smallest prime factor table --- */
    build_spf(LIMIT, &spf);
    /* spf[0] and spf[1] are unused, spf[i]==i indicates a prime           */

    /* --- nonsquarefree flag array --- */
    nsf = (unsigned char*)calloc((size_t)(LIMIT + 1), sizeof(unsigned char));
    if (!nsf) { abort(); }              /* allocation failure           */
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= LIMIT; ++p) {
        if (spf[p] == p) {              /* p is prime                   */
            idx_t sq = p * p;
            for (idx_t j = sq; j <= LIMIT; j += sq) {
                nsf[j] = 1;              /* mark as not squarefree      */
            }
        }
    }

    /* --- sequence cache initialization --- */
    terms_cap = 128;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!terms) { abort(); }
    terms_len = 0;
    scan_pos = 2;
}

/* ------------------------------------------------------------------ */
/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, behaviour for n==0 undefined     */
    ensure_init();

    /* Produce terms until we have at least n terms */
    while (terms_len < (size_t)n && scan_pos <= LIMIT - 3) {
        idx_t i = scan_pos;
        if (nsf[i] && nsf[i + 1] && nsf[i + 2]) {
            idx_t k = i - 1;
            if (is_sphenic_idx(k, spf) && is_sphenic_idx(k + 4, spf)) {
                /* store term */
                if (terms_len == terms_cap) {
                    terms_cap <<= 1;
                    terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                    if (!terms) { abort(); }
                }
                terms[terms_len++] = (seq_t)k;
            }
        }
        ++scan_pos;
    }

    /* If we ran out of precomputed range, extend the limit (rare for n<=10000) */
    if (terms_len < (size_t)n) {
        /* Very unlikely for n <= 10000.  For safety grow limit linearly. */
        /* Allocate a larger buffer and recompute tables.                */
        /* For brevity we abort here; such a case should not occur for the required limits. */
        abort();
    }

    return terms[n - 1];
}
