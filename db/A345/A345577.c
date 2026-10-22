/*
 * A345577  Numbers that are the sum of eight fourth powers in two or more
 *          ways.
 * https://oeis.org/A345577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function implements OEIS A345577: numbers that are the sum of
   eight fourth powers in two or more ways.  The implementation
   enumerates all unordered 8‑tuples of fourth powers up to a
   current limit, counts how many times each sum occurs, keeps the
   sums that occur at least twice, and returns the n‑th such sum
   (1‑based).  The limit is increased automatically until enough
   terms have been found for a requested n. */
 
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
 
/* Seq_t and idx_t are supplied by the build system */
extern seq_t a(idx_t n);   /* forward declaration for the build system */
 
/* --- static data ---------------------------------------------------- */
static idx_t limit = 1000000ULL;           /* current search limit */
static idx_t *fourth = NULL;              /* list of k^4 <= limit */
static size_t P = 0;                      /* number of fourth powers */
static uint8_t *cnt = NULL;               /* counter of representations */
static seq_t *terms = NULL;               /* sorted list of results */
static size_t terms_len = 0;               /* number of terms stored */
static size_t terms_cap = 0;               /* capacity of terms array */
static bool initialized = false;           /* initialization flag */
 
/* --- helper: depth‑first search to enumerate unordered 8‑tuples ---- */
static void dfs(idx_t depth, idx_t start, seq_t sum)
{
    if (depth == 8) {
        idx_t idx = (idx_t)sum;
        if (idx <= limit && idx < UINT64_MAX) {            /* guard against overflow */
            if (cnt[idx] < 2)
                cnt[idx]++;                                /* cap at 2 */
        }
        return;
    }
    for (idx_t i = start; i < P; ++i) {
        seq_t ns = sum + fourth[i];
        if (ns > limit)
            break;
        dfs(depth + 1, i, ns);
    }
}
 
/* --- create the list of fourth powers up to the current limit ------- */
static void compute_fourth_powers(void)
{
    if (fourth) free(fourth);
    fourth = NULL;
    P = 0;
    for (idx_t k = 1;; ++k) {
        seq_t p = (seq_t)k * k * k * k;
        if (p > limit) break;
        fourth = (idx_t*)realloc(fourth, (P + 1) * sizeof(idx_t));
        fourth[P++] = (idx_t)p;
    }
}
 
/* --- build the sequence up to the current limit --------------------- */
static void build_sequence(void)
{
    if (!fourth) compute_fourth_powers();
 
    /* allocate counter array */
    if (cnt) free(cnt);
    cnt = (uint8_t*)malloc((size_t)(limit + 1) * sizeof(uint8_t));
    if (!cnt) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= limit; ++i) cnt[i] = 0;
 
    /* enumerate unordered 8‑tuples */
    dfs(0, 0, 0);
 
    /* collect terms with at least two representations */
    if (terms) free(terms);
    terms = NULL;
    terms_cap = 256;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));
    if (!terms) exit(EXIT_FAILURE);
    terms_len = 0;
    for (idx_t s = 1; s <= limit; ++s) {
        if (cnt[s] >= 2) {
            if (terms_len >= terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
                if (!terms) exit(EXIT_FAILURE);
            }
            terms[terms_len++] = (seq_t)s;
        }
    }
 
    /* cleanup counter array */
    free(cnt);
    cnt = NULL;
}
 
/* --- ensure we have at least target terms -------------------------------- */
static void extend_to(idx_t target)
{
    while (terms_len < target) {
        limit <<= 1;                /* double the limit */
        build_sequence();
    }
}
 
/* --- public entry point ---------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized) {
        build_sequence();
        initialized = true;
    }
    if (n == 0 || n > terms_len)
        extend_to(n);
    if (n == 0 || n > terms_len) return 0;  /* still undefined */
    return terms[n - 1];
}
