/*
 * A345587  Numbers that are the sum of nine fourth powers in three or more
 *          ways.
 * https://oeis.org/A345587
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

/* Sequence A345587: Numbers that are the sum of nine fourth powers in three or more ways.
   Computed by dynamic programming over fourth‑power coins. */

#include <stdlib.h>

/* build the list of terms up to a given numeric limit.
   The function fills *out_seq with a dynamically allocated array of all
   sums achievable as the sum of exactly nine fourth powers in at least three
   distinct ways, and sets *out_len to the number of such terms. */
static void build_sequence(idx_t limit, seq_t **out_seq, size_t *out_len)
{
    /* dp[k][s] : number of ways to write s as the sum of exactly k fourth powers
       (capped at 3).  We only need values up to 9.  Use unsigned char for space. */
    unsigned char **dp = (unsigned char**)malloc(10 * sizeof(unsigned char*));
    for (int k = 0; k <= 9; ++k)
        dp[k] = (unsigned char*)calloc((size_t)limit + 1, sizeof(unsigned char));

    dp[0][0] = 1;   /* one way to obtain 0 with 0 terms */

    /* compute all fourth powers <= limit */
    idx_t *powers = NULL;
    size_t pow_cap = 16, pow_cnt = 0;
    powers = (idx_t*)malloc(pow_cap * sizeof(idx_t));

    for (idx_t t = 1;; ++t) {
        idx_t tt = t * t;
        idx_t v  = tt * tt;
        if (v > limit) break;
        if (pow_cnt == pow_cap) {
            pow_cap <<= 1;
            powers = (idx_t*)realloc(powers, pow_cap * sizeof(idx_t));
        }
        powers[pow_cnt++] = v;
    }

    /* dynamic programming: for each coin (fourth power) add its contribution */
    for (size_t pi = 0; pi < pow_cnt; ++pi) {
        idx_t coin = powers[pi];
        for (int k = 1; k <= 9; ++k) {
            unsigned char *dp_k   = dp[k];
            unsigned char *dp_km1 = dp[k-1];
            for (idx_t s = coin; s <= limit; ++s) {
                unsigned char add = dp_km1[s - coin];
                if (!add) continue;
                unsigned char cur = dp_k[s];
                if (cur < 3) {
                    unsigned int sum = cur + add;
                    if (sum > 3) sum = 3;
                    dp_k[s] = (unsigned char)sum;
                }
            }
        }
    }

    /* collect all sums that have at least three representations */
    seq_t *terms = NULL;
    size_t terms_cap = 1024, terms_cnt = 0;
    terms = (seq_t*)malloc(terms_cap * sizeof(seq_t));

    for (idx_t s = 1; s <= limit; ++s) {
        if (dp[9][s] >= 3) {
            if (terms_cnt == terms_cap) {
                terms_cap <<= 1;
                terms = (seq_t*)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_cnt++] = (seq_t)s;
        }
    }

    /* free temporary tables */
    for (int k = 0; k <= 9; ++k) free(dp[k]);
    free(dp);
    free(powers);

    *out_seq = terms;
    *out_len = terms_cnt;
}

/* static storage for the computed terms */
static seq_t *global_terms = NULL;
static size_t global_len   = 0;
static idx_t current_limit = 0;

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* below the offset – behaviour irrelevant */
        return 0;

    /* initialise once with a generous limit that yields >10 000 terms */
    if (!global_terms) {
        current_limit = 5000000;      /* initial numeric limit */
        build_sequence(current_limit, &global_terms, &global_len);
    }

    /* if requested index exceeds cached terms, grow the limit and rebuild */
    while (n > global_len) {
        current_limit <<= 1;          /* double the limit */
        free(global_terms);
        global_terms = NULL;
        global_len   = 0;
        build_sequence(current_limit, &global_terms, &global_len);
    }

    return global_terms[n - 1];
}
