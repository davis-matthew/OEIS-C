/*
 * A332006  Number of compositions (ordered partitions) of n into distinct
 *          centered square numbers.
 * https://oeis.org/A332006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_k u_k(n) * k! where u_k(n) is the number of subsets of
 *            size k of centered square numbers {1,5,13,25,41,…} that sum to
 *            n. The factor k! accounts for all orderings (compositions) of
 *            that subset
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Types seq_t and idx_t are supplied by the build.  */

static seq_t *cached_A = NULL;    /* cached values of a(n)        */
static idx_t  cached_max_n = 0;   /* largest n for which cached_A is valid */

/* Re‑compute a(0…new_max) and store into cached_A.  */
static void recompute(idx_t new_max)
{
    /* Free previously cached data.  */
    if (cached_A) { free(cached_A); cached_A = NULL; }

    /* 1. Compute all centered square numbers <= new_max.  */
    size_t cs_cap = 16;
    idx_t *centered = (idx_t*)malloc(cs_cap * sizeof(idx_t));
    size_t cs_cnt = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t w = 2 * k * k - 2 * k + 1;    /* 2k^2-2k+1 */
        if (w > new_max) break;
        if (cs_cnt == cs_cap) {
            cs_cap <<= 1;
            centered = (idx_t*)realloc(centered, cs_cap * sizeof(idx_t));
        }
        centered[cs_cnt++] = w;
    }

    /* 2. Pre‑compute factorials up to cs_cnt.  */
    size_t fact_cap = cs_cnt + 1;
    seq_t *fact = (seq_t*)calloc(fact_cap, sizeof(seq_t));
    fact[0] = (seq_t)1;
    for (size_t i = 1; i < fact_cap; ++i)
        fact[i] = fact[i-1] * (seq_t)i;

    /* 3. DP: ways[sum][k] = number of subsets of size k summing to sum.  */
    size_t rows = (size_t)new_max + 1;   /* sums 0..new_max            */
    size_t cols = cs_cnt + 1;            /* sizes 0..cs_cnt            */
    seq_t *ways = (seq_t*)calloc(rows * cols, sizeof(seq_t));
    ways[0] = (seq_t)1;                  /* ways[0][0] = 1             */

    for (size_t idx = 0; idx < cs_cnt; ++idx) {
        idx_t w = centered[idx];
        /* Iterate sums descending to avoid using an item more than once.  */
        for (idx_t sum = new_max; sum >= w; --sum) {
            size_t sumIdx  = (size_t)sum;
            size_t prevIdx = (size_t)(sum - w);
            for (size_t k = 0; k < cs_cnt; ++k) {
                seq_t val = ways[prevIdx * cols + k];
                if (val) {
                    ways[sumIdx * cols + (k+1)] += val;
                }
            }
            if (sum == w) break;          /* prevent underflow of unsigned */
        }
    }

    /* 4. Assemble final a(n) from the subset counts and factorials.  */
    seq_t *A = (seq_t*)calloc(rows, sizeof(seq_t));
    for (size_t n = 0; n <= new_max; ++n) {
        seq_t total = (seq_t)0;
        for (size_t k = 0; k <= cs_cnt; ++k) {
            seq_t cnt = ways[n * cols + k];
            if (cnt) total += cnt * fact[k];
        }
        A[n] = total;
    }

    /* 5. Store results and clean up.  */
    cached_A = A;
    cached_max_n = new_max;

    free(centered);
    free(fact);
    free(ways);
}

/* Public sequence function requested by the build.  */
seq_t a(idx_t n)
{
    /* Compute the table on first call or when asked for a larger index. */
    if (!cached_A || n > cached_max_n) {
        idx_t need = (n > 10000 ? n : 10000);
        recompute(need);
    }
    return cached_A[n];
}
