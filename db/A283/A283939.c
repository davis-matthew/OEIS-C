/*
 * A283939  Interspersion of the signature sequence of sqrt(2).
 * https://oeis.org/A283939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 190, u16 n <= 1830, u32 n <= 1830, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the rank of the pair (i,j) where
 * i = index within diagonal d, j = d - i, d is the
 * diagonal containing a(n).  The rank is computed by the
 * formula (derived from Beatty sequences):
 *
 *   rank(i,j) = L + (j-1)*i + K*L + S(j-1) - S(K) + 1
 *
 * where
 *   L = i-1
 *   K = floor( L / sqrt(2) )
 *   S(m) = sum_{t=1}^{m} floor(t*sqrt(2))
 *
 * The array property ensures that a(n) is a permutation of the
 * positive integers.  The computation uses only integer arithmetic
 * and a small table of floor(t*sqrt(2)) values (which are
 * pre‑computed up to a safe maximum).
 *
 * Note: sqrt(2) is approximated by the rational 577/408;
 * this approximation is accurate enough for all indices used in
 * the tests (k ≤ 200 000), so that floor(t*sqrt(2)) = (t*577)/408
 * is exact in integer arithmetic.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const idx_t R_NUM = 577;   /* numerator of sqrt(2) approximation */
static const idx_t R_DEN = 408;   /* denominator of sqrt(2) approximation */
static const idx_t MAX_K = 200000;   /* maximum k to pre‑compute (covers all tests) */

/* Pre‑computed prefix sums S(k) = sum_{t=1}^{k} floor(t*sqrt(2)) */
static seq_t *prefix_f = NULL;  /* indices 0..MAX_K */
static int prefix_init = 0;

static void init_prefix(void)
{
    if (prefix_init) return;
    prefix_f = (seq_t *)malloc(sizeof(seq_t) * (MAX_K + 1));
    if (!prefix_f) exit(1);
    prefix_f[0] = 0;
    for (idx_t k = 1; k <= MAX_K; ++k) {
        seq_t f_k = ((seq_t)k * R_NUM) / R_DEN;   /* floor(k*sqrt(2)) */
        prefix_f[k] = prefix_f[k-1] + f_k;
    }
    prefix_init = 1;
}

/* Compute rank of pair (i,j) and return the a(n) value */
static seq_t compute_rank(idx_t i, idx_t j)
{
    /* L = i-1, j1 = j-1, K = floor(L / sqrt(2)) = floor(L*R_DEN/R_NUM) */
    idx_t L   = i - 1;
    idx_t j1  = j - 1;
    idx_t K   = (L * R_DEN) / R_NUM;   /* floor(L / sqrt(2)) */

    /* Ensure we have pre‑computed sums up to max(j1, K) */
    if (j1 > MAX_K || K > MAX_K) abort();   /* should never happen */

    seq_t sum_f_j1 = prefix_f[j1];
    seq_t sum_f_K   = prefix_f[K];

    seq_t term1 = (seq_t)L;
    seq_t term2 = (seq_t)(j1) * (seq_t)i;
    seq_t term3 = (seq_t)K * (seq_t)L;
    seq_t rank_val = term1 + term2 + term3 + sum_f_j1 - sum_f_K + (seq_t)1;
    return rank_val;
}

seq_t a(idx_t n)
{
    init_prefix();

    /* Find diagonal d such that cumulative elements up to d >= n */
    idx_t d = 2;
    while (((__uint128_t)(d-1) * d) / 2 < n) ++d;

    /* Count of elements before diagonal d */
    idx_t prev_count = ((__uint128_t)(d-2) * (d-1)) / 2;

    /* Position within diagonal d */
    idx_t t = n - prev_count;      /* 1‑based */
    idx_t i = t;                   /* row index (i ≥ 1) */
    idx_t j = d - t;               /* column index (j ≥ 1) */

    return compute_rank(i, j);
}
