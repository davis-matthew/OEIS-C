/*
 * A345802  Numbers that are the sum of nine cubes in exactly ten ways.
 * https://oeis.org/A345802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 111, u32 n <= 111, u64 n <= 111, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS Sequence A345802
   Numbers that are the sum of nine cubes in exactly ten ways.
   The function a(n) returns the n‑th such number (1‑based indexing). */

#include <stdlib.h>   /* malloc, calloc, realloc, free, abort */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

/* Public API ----------------------------------------------------------- */
seq_t a(idx_t n);

/* Internal static data ------------------------------------------------ */
static bool first_time = true;     /* first call flag */
static idx_t current_limit = 0;    /* highest limit used for DP */
static size_t term_len = 0;        /* number of terms found so far */
static size_t term_cap = 0;        /* allocated capacity of terms array */
static seq_t *terms = NULL;        /* array of terms (ascending) */

/* Helper: build the DP table up to max_val and collect terms */
static void build_terms(idx_t max_val)
{
    /* Allocate DP table: rows = 0..max_val, columns = 0..9 (k cubes) */
    size_t rows = (size_t)max_val + 1;
    size_t cols = 10;          /* 0..9 */
    size_t dp_size = rows * cols;
    uint8_t *dp = (uint8_t *)calloc(dp_size, 1);
    if (!dp) abort();           /* out of memory */

    /* Base case: 0 sum with 0 cubes */
    dp[0] = 1;                  /* dp[0*10 + 0] */

    /* Process each cube value */
    for (idx_t t = 1;; ++t) {
        idx_t cube = t * t * t;
        if (cube > max_val) break;

        for (idx_t s = cube; s <= max_val; ++s) {
            size_t base   = (size_t)s * cols;
            size_t prev   = (size_t)(s - cube) * cols;
            for (int k = 1; k <= 9; ++k) {
                unsigned int val = dp[base + k] + dp[prev + k - 1];
                dp[base + k] = (uint8_t)(val > 10 ? 11 : val);
            }
        }
    }

    /* Free any previous term array */
    free(terms);
    terms = NULL;
    term_len = 0;
    term_cap = 0;

    /* Collect sums with exactly 10 representations */
    for (idx_t s = 1; s <= max_val; ++s) {
        if (dp[(size_t)s * cols + 9] == 10) {
            /* Ensure capacity */
            if (term_len == term_cap) {
                term_cap = term_cap ? term_cap * 2 : 16384;
                terms = (seq_t *)realloc(terms, term_cap * sizeof(seq_t));
                if (!terms) abort();
            }
            terms[term_len++] = (seq_t)s;
        }
    }

    free(dp);
    current_limit = max_val;
}

/* Public sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)0U;   /* undefined for n==0, return 0 */

    /* First call: build up to an initial safe limit */
    if (first_time) {
        build_terms(200000);  /* should contain many terms for n up to ~10k */
        first_time = false;
    }

    /* If request exceeds available terms, extend the limit and rebuild */
    while ((size_t)n > term_len) {
        idx_t new_limit = current_limit * 2;
        if (new_limit < current_limit) new_limit = current_limit;  /* overflow guard */
        if (new_limit > 20000000U) break;           /* safety limit */
        build_terms(new_limit);
    }

    /* If still out of range, return 0 (should not occur for normal inputs) */
    if ((size_t)n > term_len) return (seq_t)0U;

    return terms[(size_t)(n - 1)];
}
