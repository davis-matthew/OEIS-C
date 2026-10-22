/*
 * A382701  Expansion of e.g.f. -log(1 - x)^3 * exp(3*x) / 6.
 * https://oeis.org/A382701
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A382701
 * Formula: a(n) = Sum_{k=3..n} 3^(n-k) * binomial(n,k) * |Stirling1(k,3)|
 *
 * Uses the unsigned Stirling numbers of the first kind (counting
 * permutations with exactly k cycles).  These are computed
 * recursively with O(n) memory usage.
 */
#include <stdlib.h>

/* cache of unsigned Stirling numbers of the first kind
 * for k=3 (c(n,3)).  Index 0..last_computed inclusive are valid. */
static seq_t *stirling3 = NULL;
static idx_t  last_computed = 2;              /* last n with data in cache */

/* current values for n = last_computed :
 * c1 = c(n,1) , c2 = c(n,2) , c3 = c(n,3) */
static seq_t curr_c1 = 1;   /* c(2,1) = 1! */
static seq_t curr_c2 = 1;   /* c(2,2) = 1   */
static seq_t curr_c3 = 0;   /* c(2,3) = 0   */

/* extend the cache up to n (inclusive) */
static void extend_stirling_cache(idx_t n)
{
    if (n <= last_computed)
        return;

    size_t new_cap = n + 1;                /* indices 0..n inclusive */
    seq_t *tmp = (seq_t *)realloc(stirling3, new_cap * sizeof(seq_t));
    if (!tmp)
        return;   /* allocation failure – unlikely on this platform */
    stirling3 = tmp;

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t new_c1 = curr_c1 * (seq_t)(i - 1);            /* c(i,1) = (i-1)!      */
        seq_t new_c2 = curr_c1 + (seq_t)(i - 1) * curr_c2; /* c(i,2) = c(i-1,1)+ (i-1)*c(i-1,2) */
        seq_t new_c3 = curr_c2 + (seq_t)(i - 1) * curr_c3; /* c(i,3) = c(i-1,2)+ (i-1)*c(i-1,3) */

        stirling3[i] = new_c3;
        curr_c1 = new_c1;
        curr_c2 = new_c2;
        curr_c3 = new_c3;
    }

    last_computed = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* initialise cache if first use */
    if (stirling3 == NULL) {
        stirling3 = (seq_t *)malloc(3 * sizeof(seq_t));    /* indices 0..2 */
        if (!stirling3)
            return 0;   /* unlikely */
        stirling3[0] = stirling3[1] = stirling3[2] = 0;
        curr_c1 = 1;   /* c(2,1) */
        curr_c2 = 1;   /* c(2,2) */
        curr_c3 = 0;   /* c(2,3) */
        last_computed = 2;
    }

    extend_stirling_cache(n);

    /* compute the sum */
    seq_t sum   = 0;
    seq_t binom = 1;  /* C(n,n)          */
    seq_t power = 1;  /* 3^(n-n) = 1     */

    for (idx_t k = n; k >= 3; --k) {
        seq_t term = binom * power;
        term *= stirling3[k];
        sum += term;

        if (k > 3) {
            seq_t numerator   = binom * (seq_t)k;           /* C(n,k)*k          */
            idx_t  denominator = n - k + 1;                 /* divides exactly   */
            binom = numerator / denominator;                /* C(n,k-1)          */
            power *= (seq_t)3;                               /* 3^(n-(k-1))       */
        }
    }

    return sum;
}
