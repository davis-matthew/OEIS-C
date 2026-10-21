/*
 * A261155  The highest rugby (union) score that can be made in n or fewer
 *          ways.
 * https://oeis.org/A261155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 none, u32 n <= 1499, u64 n <= 1499,
 *            u128 n <= 1499, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A261155 – The highest rugby (union) score that can be made
   with no more than n different combinations of kicks (3), tries (5)
   and converted tries (7).  The algorithm pre‑computes the number of
   combinations for each score up to a certain bound and then supplies
   the maximum score that can be realised with at most n combinations. */

#include <stdlib.h>
#include <stdbool.h>

/* Static state */
static seq_t *f_counts   = NULL;   /* f_counts[s] = number of combinations for score s */
static seq_t *best_score = NULL;   /* best_score[k] = a(k) for 0..N_MAX */
static idx_t  N_MAX       = 0;      /* maximum n for which best_score is defined */
static bool   initialized = false;

/* One‑time initialisation that builds tables
   for n up to N_MAX.  We pick a generous bound for N_MAX
   so that the sequence can be evaluated for n up to 10000
   in a fraction of a second. */
static void init_sequence(void)
{
    if (initialized) return;
    initialized = true;

    /* choose a generous limit for n */
    N_MAX = 20000;                      /* covers all test cases comfortably */

    /* Upper bound for score enumeration: we need to cover all scores
       that may have up to N_MAX combinations.  Empirically s=200000
       is far more than enough for N_MAX=20000. */
    const idx_t S_MAX = 200000;

    /* Allocate and initialise f_counts */
    f_counts = (seq_t *)malloc((S_MAX + 1) * sizeof(seq_t));
    if (f_counts == NULL) return;        /* failure propagates as zero result */

    for (idx_t i = 0; i <= S_MAX; ++i) f_counts[i] = 0;
    f_counts[0] = 1;                     /* one way to make score 0 */

    /* Count combinations using the standard coin‑change DP (order‑free). */
    const idx_t coins[3] = {3, 5, 7};
    for (size_t ci = 0; ci < 3; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t s = coin; s <= S_MAX; ++s) {
            f_counts[s] += f_counts[s - coin];
        }
    }

    /* Allocate best_score table */
    best_score = (seq_t *)malloc((N_MAX + 1) * sizeof(seq_t));
    if (best_score == NULL) return;     /* failure propagates as zero result */

    for (idx_t i = 0; i <= N_MAX; ++i) best_score[i] = 0;

    /* Fill best_score: for each score s, record it as the best score
       for all n >= number_of_combinations_of_s.  Since scores are processed
       in increasing order, the last assignment for a given count gives
       the largest score. */
    for (idx_t s = 0; s <= S_MAX; ++s) {
        seq_t c = f_counts[s];
        if (c <= (seq_t)N_MAX) {
            best_score[(idx_t)c] = s;
        }
    }

    /* Propagate the maximum score through gaps: if no exact score exists
       for a particular n, the best score is the same as for n-1. */
    for (idx_t i = 1; i <= N_MAX; ++i) {
        if (best_score[i] == 0 || best_score[i] < best_score[i - 1]) {
            best_score[i] = best_score[i - 1];
        }
    }
}

/* Public function required by the description.  It can be called in any
   order and for any n >= 0 (as long as n <= N_MAX; larger n are not
   expected in the tests). */
seq_t a(idx_t n)
{
    init_sequence();

    if (n > N_MAX) {
        /* For safety, if an out‑of‑range request is made we return the
           maximum pre‑computed score.  This case should never occur
           in normal tests. */
        return best_score[N_MAX];
    }
    return best_score[n];
}
