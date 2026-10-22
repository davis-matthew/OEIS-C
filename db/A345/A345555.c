/*
 * A345555  Numbers that are the sum of ten cubes in seven or more ways.
 * https://oeis.org/A345555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345555 – numbers that are the sum of ten cubes in seven or more
   distinct ways.                                  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------------- */
/* Parameters -------------------------------------------------------------- */
#define MAX_K      10            /* exactly ten cubes */
#define THRESHOLD  7             /* need at least seven representations */
#define INIT_LIMIT 1000          /* first DP limit */

/* ------------------------------------------------------------------------- */
/* Static state ------------------------------------------------------------- */
static idx_t  cur_limit        = 0;          /* current DP limit */
static seq_t *terms            = NULL;        /* cached sequence terms */
static size_t term_len         = 0;          /* number of cached terms */
static size_t term_cap         = 0;          /* capacity of 'terms' array */

/* ------------------------------------------------------------------------- */
/* Helper: append a term to the cache ------------------------------------- */
static void append_term(seq_t v) {
    if (term_len == term_cap) {
        size_t new_cap = term_cap == 0 ? 64 : term_cap << 1;
        seq_t *new_arr = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!new_arr) return;                /* allocation failed – ignore */
        terms   = new_arr;
        term_cap = new_cap;
    }
    terms[term_len++] = v;
}

/* ------------------------------------------------------------------------- */
/* Build DP up to 'lim' and collect new terms. ----------------------------- */
static bool build_and_collect(idx_t lim) {
    /* Allocate DP table: (MAX_K+1) × (lim+1) bytes (uint8_t) */
    size_t stride = lim + 1;
    size_t dp_size = (MAX_K + 1) * stride;
    uint8_t *dp = (uint8_t*)calloc(dp_size, 1);
    if (!dp) return false;                       /* allocation failed */

    /* Base case: dp[0][0] = 1 */
    dp[0] = 1;

    /* List cube values ≤ lim */
    uint32_t cubes[200];
    size_t cube_cnt = 0;
    for (uint64_t c = 1;; ++c) {
        uint64_t cube = c * c * c;
        if (cube > (uint64_t)lim) break;
        cubes[cube_cnt++] = (uint32_t)cube;
    }

    /* DP: for each cube value, update counts for all k ≤ MAX_K */
    for (size_t ci = 0; ci < cube_cnt; ++ci) {
        uint32_t val = cubes[ci];
        for (size_t k = 1; k <= MAX_K; ++k) {
            uint8_t *dp_k   = dp + k       * stride;
            uint8_t *dp_km1 = dp + (k - 1) * stride;
            for (uint32_t s = val; s <= lim; ++s) {
                uint8_t add = dp_km1[s - val];
                if (!add) continue;
                uint8_t cur = dp_k[s];
                uint8_t newv = cur + add;
                if (newv > THRESHOLD) newv = THRESHOLD;
                dp_k[s] = newv;
            }
        }
    }

    /* Collect terms with at least THRESHOLD representations using exactly
       MAX_K cubes. Only values beyond the previous limit are new.      */
    idx_t old_limit = cur_limit;
    for (idx_t s = old_limit + 1; s <= lim; ++s) {
        if (dp[MAX_K * stride + s] >= THRESHOLD)
            append_term((seq_t)s);
    }

    free(dp);
    cur_limit = lim;
    return true;
}

/* ------------------------------------------------------------------------- */
/* Ensure that we have at least 'required' terms cached ------------------- */
static bool ensure_terms(idx_t required) {
    if (term_len >= required) return true;

    /* Expand the DP limit until enough terms are collected. */
    idx_t lim = cur_limit == 0 ? INIT_LIMIT : cur_limit * 2;
    if (lim == 0) lim = INIT_LIMIT;          /* for the very first call */

    while (term_len < required) {
        if (!build_and_collect(lim))
            return false;                    /* DP build failed */

        /* If still not enough, increase limit further. */
        if (term_len < required) {
            lim <<= 1;                       /* double the limit */
            if (lim == 0) break;             /* overflow guard */
        }
    }
    return true;
}

/* ------------------------------------------------------------------------- */
/* Public function ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* undefined in OEIS, safe */
    if (!ensure_terms(n)) return (seq_t)0;     /* could not build sequence  */
    return terms[n - 1];                      /* array is 0‑based            */
}
