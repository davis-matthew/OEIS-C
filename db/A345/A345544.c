/*
 * A345544  Numbers that are the sum of nine cubes in five or more ways.
 * https://oeis.org/A345544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer that can be expressed as a sum of nine
 *            integer cubes in at least five distinct ways
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The routine precomputes all terms up to a fairly generous bound
   (MAX_SUM).  It is fast enough for n up to ~10 000 and works for
   seq_t being any unsigned integer type or an arbitrary‑precision
   integer class (casting malloc/realloc is required for C++). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_SUM 500000ULL        /* upper bound on the sums to test */

static seq_t *terms = NULL;     /* array of found terms */
static size_t terms_len = 0;    /* number of terms stored */

/* Precompute the sequence.  Called once on first use. */
static void compute_terms(void)
{
    /* dp[t][s] = number of ways to form sum s using exactly t cubes
       (unordered, each cube >= 1), capped at 5. */
    unsigned char **dp = (unsigned char**)malloc(10 * sizeof(unsigned char*));
    if (!dp) exit(EXIT_FAILURE);
    for (int t = 0; t < 10; ++t) {
        dp[t] = (unsigned char*)calloc(MAX_SUM + 1, sizeof(unsigned char));
        if (!dp[t]) exit(EXIT_FAILURE);
    }
    dp[0][0] = 1;

    /* Enumerate cube values. */
    for (idx_t x = 1; ; ++x) {
        idx_t cube = x * x * x;
        if (cube > MAX_SUM) break;
        for (idx_t t = 1; t <= 9; ++t) {
            for (idx_t s = cube; s <= MAX_SUM; ++s) {
                unsigned char prev = dp[t - 1][s - cube];
                if (prev) {
                    unsigned char new_val = dp[t][s] + prev;
                    dp[t][s] = new_val > 5 ? 5 : new_val;
                }
            }
        }
    }

    /* Copy sums with at least five representations. */
    size_t cap = 128;
    terms = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!terms) exit(EXIT_FAILURE);
    size_t cnt = 0;
    for (idx_t s = 0; s <= MAX_SUM; ++s) {
        if (dp[9][s] >= 5) {
            if (cnt >= cap) {
                cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(terms, cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                terms = tmp;
            }
            terms[cnt++] = (seq_t)s;
        }
    }
    terms_len = cnt;

    /* Free dp tables. */
    for (int t = 0; t < 10; ++t) free(dp[t]);
    free(dp);
}

/* External entry point: returns the nth term (1‑based). */
seq_t a(idx_t n)
{
    static bool init = false;
    if (!init) {
        compute_terms();
        init = true;
    }
    if (n == 0) return 0;               /* undefined for n=0 */
    idx_t idx = n - 1;
    if (idx >= terms_len) return 0;      /* out of computed range */
    return terms[idx];
}
