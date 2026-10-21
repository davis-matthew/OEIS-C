/*
 * A281809  Expansion of Sum_{i>=1} x^(i^3) / (1 - Sum_{j>=1} x^(j^3))^2.
 * https://oeis.org/A281809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i>=1} x^(i^3) / (1 - Sum_{j>=1} x^(j^3))^2
 *            Implements the recurrence: c[0] = 1, a[0] = 0 c[n] = Σ_{cube≤n}
 *            c[n - cube] a[n] = Σ_{cube≤n} (a[n - cube] + c[n - cube]) where
 *            c[n] is the number of ordered compositions of n into cubes and
 *            a[n] is the sum of parts over all such compositions...
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 92, u64 n <= 194,
 *            u128 n <= 401, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#ifdef __cplusplus
static seq_t* allocate_seq(size_t n) { return new seq_t[n]; }
static void free_seq(seq_t* p) { delete[] p; }
#else
static seq_t* allocate_seq(size_t n) { return (seq_t*)calloc(n, sizeof(seq_t)); }
static void free_seq(seq_t* p) { free(p); }
#endif

static seq_t *c = NULL;
static seq_t *a_val = NULL; /* store sequence values */
static idx_t max_n = 0;

/* Expand the cached arrays to include indices up to n. */
static void expand_to(idx_t n)
{
    if (n <= max_n) return;

    size_t new_size = (size_t)(n) + 1;
    seq_t *new_c = allocate_seq(new_size);
    seq_t *new_a = allocate_seq(new_size);

    if (c && a_val) {
        for (idx_t i = 0; i <= max_n; ++i) {
            new_c[i] = c[i];
            new_a[i] = a_val[i];
        }
    }

    free_seq(c);
    free_seq(a_val);
    c = new_c;
    a_val = new_a;

    if (max_n == 0) {
        c[0] = 1;      /* single empty composition of 0 */
        a_val[0] = 0;  /* no parts in the empty composition */
    }

    for (idx_t k = max_n + 1; k <= n; ++k) {
        seq_t sum_c = 0;
        seq_t sum_a = 0;
        for (idx_t i = 1; ; ++i) {
            idx_t cube = i * i * i;          /* i^3 */
            if (cube > k) break;
            idx_t rem = k - cube;
            sum_c += c[rem];
            sum_a += a_val[rem] + c[rem];   /* add 1 part for the chosen cube */
        }
        c[k] = sum_c;
        a_val[k] = sum_a;
    }
    max_n = n;
}

/* Returns a(n) for the OEIS sequence A281809.  n ≥ 1. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; values below not defined */
    expand_to(n);
    return a_val[n];
}
