/*
 * A339087  Number of compositions (ordered partitions) of n into distinct
 *          parts congruent to 4 mod 5.
 * https://oeis.org/A339087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A339087: Number of compositions of n into distinct parts
 * congruent to 4 mod 5.
 *
 * Algorithm:
 *   1. For each admissible part w = 4,9,14,… we build a DP table
 *      dp[sum][k] = number of subsets of distinct admissible parts that
 *      sum to 'sum' and contain exactly k parts.
 *   2. After processing all parts, the answer for n is
 *      a(n) = Σ_{k} dp[n][k] * k! .
 *
 * The table is recomputed only when a larger n is requested and is
 * cached for future calls.  The implementation uses only integer
 * arithmetic in ``seq_t`` and ``idx_t`` and works for both fixed‑size
 * and arbitrary‑precision builds.  The code is valid C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *dp      = NULL;   /* flattened DP table [sum*stride + k]   */
static seq_t *ans_arr = NULL;   /* answer for each n  */
static seq_t *fact_arr = NULL;  /* factorials up to current max k */
static size_t precomp_n = (size_t)-1;   /* maximum n that has been computed */
static size_t stride = 0;               /* number of k values stored per sum */

/* Free any previously allocated tables. */
static void free_tables(void)
{
    free(dp);
    free(ans_arr);
    free(fact_arr);
    dp = NULL;
    ans_arr = NULL;
    fact_arr = NULL;
    precomp_n = (size_t)-1;
    stride = 0;
}

/* Pre‑compute all data needed for n <= max_n. */
static void compute_up_to(size_t max_n)
{
    /* If we already have data for a larger n, nothing to do. */
    if (precomp_n != (size_t)-1 && max_n <= precomp_n)
        return;

    /* Free old data if any. */
    free_tables();

    /* Determine maximum number of admissible parts that can fit in max_n.
     * Sum_{j=0}^{k-1} (5j+4) = k*(5k+3)/2.  Find max k with this ≤ max_n. */
    size_t k = 0;
    while ( ((seq_t)k * ((seq_t)5 * k + (seq_t)3)) / 2 <= (seq_t)max_n )
        ++k;
    size_t maxK = (k > 0) ? k - 1 : 0;
    stride = maxK + 1;                     /* indices k = 0 .. maxK */

    /* Allocate DP table: (max_n+1) sums each with 'stride' entries. */
    dp = (seq_t *)calloc((max_n + 1) * stride, sizeof(seq_t));
    if (!dp) exit(EXIT_FAILURE);
    /* Base case: empty subset of size 0 sums to 0. */
    dp[0] = (seq_t)1;

    /* Process each admissible part w = 4 + 5*j, j ≥ 0. */
    for (int w = 4; w <= (int)max_n; w += 5) {
        for (int s = (int)max_n; s >= w; --s) {
            seq_t *dst = dp + (size_t)s * stride;
            seq_t *src = dp + ((size_t)s - (size_t)w) * stride;
            for (size_t kj = 0; kj < maxK; ++kj) {
                seq_t val = src[kj];
                if (val)
                    dst[kj + 1] += val;   /* add part w to a subset of size kj */
            }
        }
    }

    /* Pre‑compute factorials up to maxK. */
    fact_arr = (seq_t *)calloc(stride, sizeof(seq_t));
    if (!fact_arr) exit(EXIT_FAILURE);
    fact_arr[0] = (seq_t)1;
    for (size_t i = 1; i <= maxK; ++i)
        fact_arr[i] = fact_arr[i - 1] * (seq_t)i;

    /* Compute answers for all n up to max_n. */
    ans_arr = (seq_t *)calloc(max_n + 1, sizeof(seq_t));
    if (!ans_arr) exit(EXIT_FAILURE);

    for (size_t n = 0; n <= max_n; ++n) {
        seq_t *dp_base = dp + n * stride;
        seq_t result = (seq_t)0;
        for (size_t i = 0; i <= maxK; ++i) {
            if (dp_base[i])
                result += dp_base[i] * fact_arr[i];
        }
        ans_arr[n] = result;
    }

    precomp_n = max_n;
}

/* Required function */
seq_t a(idx_t n)
{
    compute_up_to((size_t)n);
    return ans_arr[(size_t)n];
}
