/*
 * A304633  Expansion of 2/((1 - x)*(3 + 2*x - theta_3(x))), where theta_3()
 *          is the Jacobi theta function.
 * https://oeis.org/A304633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=0}^n b(i) * where b(0)=1 * and for n>0, b(n) =
 *            sum_{k>=2, k*k <= n} b(n - k*k) * * a(n) is the partial sum of
 *            the ordered-squares-composition counts
 * Range:     u8 n <= 39, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *b_vals = NULL;
static seq_t *a_vals = NULL;
static idx_t cache_size = 0;

static void extend_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *new_b = (seq_t *)malloc(new_size * sizeof(seq_t));
    seq_t *new_a = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_b || !new_a) {
        exit(EXIT_FAILURE);
    }
    idx_t i_start;
    if (cache_size == 0) {
        new_b[0] = 1;
        new_a[0] = 1;
        i_start = 1;
    } else {
        for (idx_t i = 0; i < cache_size; ++i) {
            new_b[i] = b_vals[i];
            new_a[i] = a_vals[i];
        }
        i_start = cache_size;
    }
    if (b_vals) free(b_vals);
    if (a_vals) free(a_vals);
    b_vals = new_b;
    a_vals = new_a;

    for (idx_t i = i_start; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 2; k * k <= i; ++k) {
            sum += b_vals[i - k * k];
        }
        b_vals[i] = sum;
        a_vals[i] = a_vals[i - 1] + sum;
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    extend_cache(n);
    return a_vals[n];
}
