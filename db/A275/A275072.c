/*
 * A275072  Number of set partitions of [n] such that i-j is a multiple of
 *          five for all i,j belonging to the same block.
 * https://oeis.org/A275072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=0..4} A000110(⌊(n+i)/5⌋) where A000110(k) is the
 *            Bell number B(k) = number of set partitions of [k]. The
 *            implementation computes the required Bell numbers once and
 *            caches them for subsequent calls. The recurrence used is B(n+1)
 *            = Σ_{k=0}^{n} C(n,k)·B(k) (0‑based). The binomial coeffic...
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 26, u64 n <= 41, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of Bell numbers */
static seq_t *bell = NULL;   /* bell[0..size-1] */
static idx_t  bell_size = 0; /* number of computed Bell values */

/* compute Bell numbers up to “up_to” (inclusive) and store them in bell[] */
static void compute_bell_upto(idx_t up_to)
{
    if (up_to + 1 <= bell_size)   /* already computed */
        return;

    /* grow or allocate the array */
    bell = (seq_t*)realloc(bell, (up_to + 1) * sizeof(seq_t));

    /* initialise B(0)=1 if this is the very first call */
    if (bell_size == 0) {
        bell[0] = 1;
        bell_size = 1;
    }

    /* compute missing Bell numbers */
    for (idx_t n = bell_size; n <= up_to; ++n) {
        seq_t comb = 1;          /* C(n-1,0) */
        seq_t sum  = 0;
        for (idx_t k = 0; k < n; ++k) {
            sum += comb * bell[k];
            if (k < n - 1) {    /* update to C(n-1,k+1) */
                comb = comb * (seq_t)(n - 1 - k);
                comb = comb / (seq_t)(k + 1);
            }
        }
        bell[n] = sum;
    }
    bell_size = up_to + 1;
}

/* OEIS sequence A275072 */
seq_t a(idx_t n)
{
    /* largest Bell index required */
    idx_t max_bell = (n + 4) / 5;
    compute_bell_upto(max_bell);

    seq_t prod = 1;
    for (idx_t i = 0; i < 5; ++i) {
        idx_t k = (n + i) / 5;   /* floor((n+i)/5) */
        prod *= bell[k];
    }
    return prod;
}
