/*
 * A349939  Terms in A328897 that are not divisible by 3: numbers k > 1 not
 *          divisible by 2 or 3 such that A005179(k-1) > A005179(k) <
 *          A005179(k+1).
 * https://oeis.org/A349939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3345, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A349939 – Numbers k>1 NOT divisible by 2 or 3 such that the smallest
   integer with exactly k divisors is smaller than those with k‑1 or k+1 divisors.
   Computed by generating the minimal n for each divisor count via DFS over
   exponent patterns and then scanning for local minima. */

#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include <math.h>

/* Small list of primes – enough for smallest‑n‑with‑d‑divisors up to several
   millions of divisor counts. */
static const idx_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97
};
static const size_t PRIME_CNT = sizeof(primes) / sizeof(primes[0]);

/* Upper bound for divisor counts we pre‑compute.
   4 000 000 covers more than enough for the first 10 000 terms. */
#define MAX_DIV 4000000
/* Allocate one extra slot to allow look‑ups of d+1. */
#define ARRAY_SIZE (MAX_DIV + 2)

/* Look‑up table holding the logarithm of the minimal number with d divisors. */
static double *min_log   = NULL;
static double *log_prime = NULL;
static bool     ready    = false;

/* Recursive DFS that updates min_log for each divisor count reachable
   with non‑increasing exponents. */
static void dfs(idx_t depth, idx_t max_exp, idx_t d, double log_val)
{
    if (d > MAX_DIV) return;
    if (log_val < min_log[d]) min_log[d] = log_val;
    if (depth + 1 >= PRIME_CNT) return;

    for (idx_t e = 1; e <= max_exp; ++e) {
        idx_t nd = d * (e + 1);
        if (nd > MAX_DIV) break;
        double nlog = log_val + e * log_prime[depth];
        dfs(depth + 1, e, nd, nlog);
    }
}

/* One‑time initialization of tables. */
static void init(void)
{
    min_log = (double *)malloc(ARRAY_SIZE * sizeof(double));
    for (size_t i = 0; i < ARRAY_SIZE; ++i) min_log[i] = DBL_MAX;
    min_log[1] = 0.0;                /* A005179(1) = 1 */

    log_prime = (double *)malloc(PRIME_CNT * sizeof(double));
    for (size_t i = 0; i < PRIME_CNT; ++i)
        log_prime[i] = log((double)primes[i]);

    dfs(0, MAX_DIV, 1, 0.0);
    ready = true;
}

/* Memoised list of sequence terms. */
static idx_t *seq      = NULL;
static size_t seq_cap  = 0;
static size_t seq_len  = 0;
static idx_t next_k = 5;   /* first candidate >1 that is not divisible by 2 or 3 */

/* Grow the seq array when needed. */
static void grow_seq(void)
{
    if (seq_cap == 0) seq_cap = 64;
    else seq_cap <<= 1;
    seq = (idx_t *)realloc(seq, seq_cap * sizeof(idx_t));
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (!ready) init();

    if (n == 0) return (seq_t)0;   /* offset is 1 */

    /* Generate terms until we have at least n entries. */
    while (seq_len < n) {
        while (true) {
            if (next_k % 2 == 0 || next_k % 3 == 0) {
                ++next_k;
                continue;
            }
            /* Candidate is at least 5, so indices exist. */
            double log_prev = min_log[next_k - 1];
            double log_cur  = min_log[next_k];
            double log_next = min_log[next_k + 1];

            if (log_prev > log_cur + 1e-12 && log_next > log_cur + 1e-12) {
                if (seq_len == seq_cap) grow_seq();
                seq[seq_len++] = next_k;
                ++next_k;   /* Move past this candidate for next search. */
                break;
            }
            ++next_k;
        }
    }

    return (seq_t)seq[n - 1];
}
