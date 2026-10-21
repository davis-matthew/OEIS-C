/*
 * A261492  Number of partitions of subsets of {1,...,n}, where consecutive
 *          integers are required to be in the same part and the elements of
 *          {1, n} are required to be in the same part if they are both
 *          members of a subset.
 * https://oeis.org/A261492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * Sum_{j=0..⌊n/2⌋} C(n,2j) * A000110(j)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 72,
 *            big any n
 * Helpers:   binom_acc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A000110(j) is the j-th Bell number.  */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom_acc.h"

/* ------------------------------------------- */
/*  Cache for Bell numbers B_j , j ≥ 0        */
/* ------------------------------------------- */
static seq_t *bell      = NULL;  /* array of Bell numbers          */
static size_t bell_cap   = 0;    /* allocated capacity             */
static size_t bell_cnt   = 0;    /* number of precomputed values   */

/* Ensure that bell[j] is computed for all j ≤ max_index   */
static void ensure_bell(size_t max_index)
{
    /* Grow capacity if necessary */
    if (max_index + 1 > bell_cap) {
        size_t new_cap = bell_cap ? bell_cap * 2 : 8;
        while (new_cap <= max_index)
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(bell, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out of memory */
        bell      = tmp;
        bell_cap  = new_cap;
    }

    /* Initialise B_0 if not done */
    if (bell_cnt == 0) {
        bell[0] = (seq_t)1;
        bell_cnt = 1;
    }

    /* Compute missing Bell numbers up to max_index */
    for (size_t m = bell_cnt; m <= max_index; ++m) {
        seq_t sum = 0;
        seq_t c = 1;            /* C(m-1,0) */
        sum += c * bell[0];     /* term for k=0 */
        for (size_t k = 1; k < m; ++k) {
            /* Compute C(m-1,k) from C(m-1,k-1) */
            c = c * (m - k) / k;
            sum += c * bell[k];
        }
        bell[m] = sum;
        ++bell_cnt;
    }
}

/* ------------------------------------------- */
/*           Main sequence function            */
/* ------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t j_max = n >> 1;            /* floor(n/2) */
    ensure_bell((size_t)j_max);

    seq_t sum = 0;
    for (idx_t j = 0; j <= j_max; ++j) {
        seq_t cmb = binom_acc(n, 2 * j);
        sum += cmb * bell[(size_t)j];
    }
    return sum * ((seq_t)2);
}
