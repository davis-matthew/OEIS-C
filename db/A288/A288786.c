/*
 * A288786  Number of blocks of size >= four in all set partitions of n.
 * https://oeis.org/A288786
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0}^{n-4} binomial(n,j) * Bell(j) (OEIS A288786)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 15, u64 n <= 25, u128 n <= 42,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *bell = NULL;     /* Cached Bell numbers B(0) .. B(m) */
static idx_t bell_size = 0;    /* Largest index computed so far */

/* Ensure bell[0..need] exists and all Bell numbers up to 'need' are computed. */
static void ensure_bell(idx_t need)
{
    /* Allocate at least Bell(0) if it hasn't been done yet. */
    if (bell == NULL) {
        size_t init_cap = 1;
        bell = (seq_t *)malloc(init_cap * sizeof(seq_t));
        bell[0] = (seq_t)1;
        bell_size = 0;
    }

    if (need <= bell_size)
        return;

    /* Determine new capacity (next power of two greater than 'need'). */
    size_t cap = bell_size + 1;
    while (cap <= (size_t)need)
        cap <<= 1;

    /* Reallocate the array. */
    seq_t *tmp = (seq_t *)realloc(bell, cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* In practice we never expect allocation failures here. */
        exit(EXIT_FAILURE);
    }
    bell = tmp;

    /* Compute missing Bell numbers. */
    for (idx_t m = bell_size + 1; m <= need; ++m) {
        seq_t sum = 0;
        idx_t deg = m - 1;
        for (idx_t k = 0; k <= deg; ++k) {
            /* B(m) = Σ_{k=0}^{m-1} C(m-1, k) * B(k) */
            sum += bell[k] * binomial(deg, k);
        }
        bell[m] = sum;
    }
    bell_size = need;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;   /* undefined for indices below the offset */

    /* Ensure Bell numbers up to index n-4 are available. */
    ensure_bell(n - 4);

    seq_t res = 0;
    for (idx_t j = 0; j <= n - 4; ++j)
        res += bell[j] * binomial(n, j);

    return res;
}
