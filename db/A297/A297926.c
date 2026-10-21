/*
 * A297926  Number of set partitions of [2n] in which the size of the first
 *          block is n.
 * https://oeis.org/A297926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1, n-1) * Bell(n) (Bell(0) = 1, Bell(n) =
 *            sum_{k=0}^{n-1} binomial(n-1, k) * Bell(k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"

/* Cache for Bell numbers (Bell(0) … Bell(last_computed)) */
static seq_t *bell_cache = NULL;      /* allocated array */
static idx_t  bell_count = 0;         /* number of Bell numbers stored (0 means none) */
static size_t bell_cap   = 0;         /* allocation capacity of the array */

/* Ensure bell_cache contains Bell(0) … Bell(n) */
static void ensure_bell(idx_t n)
{
    if (bell_count > n) return;             /* already cached */

    /* Increase capacity if necessary */
    if (bell_count == 0) {                  /* first use */
        bell_cap = 16;                      /* start with some space */
        bell_cache = (seq_t *)malloc(bell_cap * sizeof(seq_t));
        if (!bell_cache) abort();
        bell_cache[0] = 1;                  /* Bell(0) = 1 */
    } else if (n + 1 > bell_cap) {          /* enlarge array */
        size_t new_cap = bell_cap;
        while (new_cap <= (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(bell_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        bell_cache = tmp;
        bell_cap = new_cap;
    }

    /* Compute missing Bell numbers */
    for (idx_t i = bell_count; i <= n; ++i) {
        if (i == 0) {                     /* already handled above */
            bell_cache[0] = 1;
            continue;
        }
        seq_t sum  = 0;
        seq_t binom = 1;                  /* binomial(i-1,0) */
        for (idx_t k = 0; k < i; ++k) {
            sum += binom * bell_cache[k];
            if (k < i - 1) {
                binom = binom * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
            }
        }
        bell_cache[i] = sum;
    }
    bell_count = n + 1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;                  /* a(0) = 1 by convention */

    /* Make sure Bell(n) is available */
    ensure_bell(n);

    /* Compute binomial(2*n-1, n-1) */
    idx_t two_n_minus1 = 2 * n - 1;
    idx_t n_minus1     = n - 1;
    seq_t binom        = binomial(two_n_minus1, n_minus1);

    return binom * bell_cache[n];
}
