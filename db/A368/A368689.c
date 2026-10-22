/*
 * A368689  Expansion of g.f. Sum_{n>=1} q^n/(1-q^(2*n)-q^(3*n)).
 * https://oeis.org/A368689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} f(n/d − 1) where f(0) = 1, f(1) = 0, f(k) = f(k −
 *            2) + f(k − 3) for k ≥ 2
 * Range:     u8 n <= 23, u16 n <= 43, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free */

static idx_t max_n = 0;
static seq_t *a_arr = NULL;   /* a_arr[0..max_n], a_arr[0] unused */
static seq_t *f_arr = NULL;   /* f_arr[0..max_n-1] */

/* Ensure arrays are computed up to at least n */
static void compute_up_to(idx_t n)
{
    if (n <= max_n) return;        /* already done */

    /* Allocate new arrays (zeroed) */
    size_t sz_a = (size_t)(n + 1);
    size_t sz_f = (size_t)n;
    seq_t *new_a = (seq_t *)calloc(sz_a, sizeof(seq_t));
    seq_t *new_f = (seq_t *)calloc(sz_f, sizeof(seq_t));
    if (!new_a || !new_f) return;   /* allocation failed, abort */

    /* Compute f[0..n-1] */
    if (n > 0) new_f[0] = 1;            /* f(0) = 1 */
    if (n > 1) new_f[1] = 0;            /* f(1) = 0 */
    for (idx_t i = 2; i < n; ++i) {
        seq_t val = 0;
        if (i >= 2) val += new_f[i - 2];
        if (i >= 3) val += new_f[i - 3];
        new_f[i] = val;
    }

    /* Compute a[1..n] via divisor accumulation */
    for (idx_t d = 1; d <= n; ++d) {
        for (idx_t m = d; m <= n; m += d) {
            idx_t k = m / d - 1;   /* index into f */
            new_a[m] += new_f[k];
        }
    }

    /* Free old arrays if any */
    free(a_arr);
    free(f_arr);

    a_arr = new_a;
    f_arr = new_f;
    max_n = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at index 1 */
    compute_up_to(n);
    return a_arr[n];
}
