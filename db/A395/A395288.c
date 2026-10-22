/*
 * A395288  Number of partitions of [2n] where exactly n blocks contain their
 *          own index element.
 * https://oeis.org/A395288
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Σ_{j=0}^{n-1} binomial(n-1,j) * n^j * Bell(n-1-j)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Forward declaration of helper for computing Bell numbers. */
static void compute_bell_upto(idx_t N);

/* Static storage for Bell numbers.  bell[k] = Bell(k)  (k ≥ 0). */
static seq_t *bell = NULL;            /* Array of Bell numbers. */
static idx_t bell_cap = 0;            /* Number of stored Bell numbers
                                        (size of 'bell', i.e., max_k+1). */

/* Compute Bell numbers up to N (inclusive).  After this call bell[0..N] are valid. */
static void compute_bell_upto(idx_t N)
{
    if (N + 1 <= bell_cap)
        return;                /* Already have enough. */

    size_t new_cap = (size_t)(N + 1);
    seq_t *tmp = (seq_t*)realloc(bell, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    bell = tmp;

    /* If we are starting from scratch, initialise Bell(0). */
    if (bell_cap == 0) {
        bell[0] = (seq_t)1;
        bell_cap = 1;
    }

    /* Compute new Bell numbers for k = bell_cap .. N. */
    for (idx_t k = bell_cap; k <= N; ++k) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;          /* C(k-1,0) */
        for (idx_t i = 0; i < k; ++i) {
            sum += bin * bell[i];
            if (i + 1 < k) {
                /* bin = bin * (k-1-i) / (i+1)  (exact division) */
                bin = bin * ((seq_t)(k - 1 - i));
                bin = bin / ((seq_t)(i + 1));
            }
        }
        bell[k] = sum;
    }
    bell_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* Ensure Bell numbers up to n-1 are available. */
    compute_bell_upto(n - 1);

    seq_t n_seq = (seq_t)n;
    seq_t sum   = (seq_t)0;
    seq_t curpow = (seq_t)1;
    seq_t bin   = (seq_t)1;          /* C(n-1,0) */

    for (idx_t j = 0; j < n; ++j) {
        seq_t term = bin * curpow * bell[n - 1 - j];
        sum += term;
        curpow = curpow * n_seq;
        if (j + 1 < n) {
            /* Update bin to C(n-1, j+1) */
            bin = bin * ((seq_t)(n - 1 - j));
            bin = bin / ((seq_t)(j + 1));
        }
    }
    return n_seq * sum;
}
