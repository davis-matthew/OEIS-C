/*
 * A379099  a(n) = Sum_{k=0..n} binomial(2*k, k)*binomial(2*n, n)/(n + 1). Row
 *          sums of A379100.
 * https://oeis.org/A379099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n) * Sum_{k=0..n} binomial(2k, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_realloc.h"  // for seq_realloc

static seq_t *cat   = NULL;   /* Catalan numbers */
static seq_t *binom = NULL;   /* binomial(2k, k)   */
static seq_t *sum   = NULL;   /* cumulative sums   */
static size_t cap   = 0;      /* allocated capacity */
static size_t computed = 0;   /* number of values already computed */

seq_t a(idx_t n)
{
    /* Ensure capacity for indices 0..n */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        cat   = seq_realloc(cat,    cap, need);
        binom = seq_realloc(binom,  cap, need);
        sum   = seq_realloc(sum,    cap, need);
        cap   = need;
    }

    /* Initialize base cases if not yet done */
    if (computed == 0) {
        cat[0]   = 1;
        binom[0] = 1;
        sum[0]   = 1;
        computed = 1;
    }

    /* Incrementally compute values up to n */
    for (size_t i = computed; i <= (size_t)n; ++i) {
        /* Catalan recurrence: C_i = C_{i-1} * (4i-2)/(i+1) */
        seq_t numC = cat[i-1] * ((seq_t)(4 * i - 2));
        cat[i] = numC / ((seq_t)(i + 1));

        /* Binomial recurrence: B_i = B_{i-1} * (4i-2)/i */
        seq_t numB = binom[i-1] * ((seq_t)(4 * i - 2));
        binom[i] = numB / ((seq_t)(i));

        /* Cumulative sum of binomials */
        sum[i] = sum[i-1] + binom[i];
    }
    computed = (size_t)n + 1;

    /* a(n) = Catalan(n) * Sum_{k=0..n} binom(2k,k) */
    return cat[(size_t)n] * sum[(size_t)n];
}
