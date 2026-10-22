/*
 * A329434  Expansion of Sum_{k>=1} (-1 + Product_{j>=1} (1 + x^(k*(2*j -
 *          1)))).
 * https://oeis.org/A329434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A000700(d)
 * Range:     u8 n <= 61, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
# include <cstddef>
#endif

static seq_t* f_arr = NULL;      /* A000700 values up to current limit */
static seq_t* a_arr = NULL;      /* a(n) values up to current limit */
static idx_t  max_computed = 0;  /* largest index already precomputed */

/* Ensure that f_arr[0..N] and a_arr[0..N] are computed.
   If a larger N is requested, recompute everything from scratch. */
static void ensure_precomputations(idx_t N) {
    if (N <= max_computed) return;

    idx_t size = N + 1;
    seq_t *f_new;
    seq_t *a_new;

#if defined(__cplusplus)
    f_new = new seq_t[size];
    a_new = new seq_t[size];
    for (idx_t i = 0; i <= N; ++i) {
        f_new[i] = 0;
        a_new[i] = 0;
    }
#else
    f_new = (seq_t*)calloc(size, sizeof(seq_t));
    a_new = (seq_t*)calloc(size, sizeof(seq_t));
    /* Zero‑initialize just in case the type is non‑POD */
    for (idx_t i = 0; i <= N; ++i) {
        f_new[i] = 0;
        a_new[i] = 0;
    }
#endif

    /* Compute A000700(k): partitions of k into distinct odd parts. */
    f_new[0] = 1;  /* empty partition */
    for (idx_t part = 1; part <= N; part += 2) {
        for (idx_t sum = N; sum >= part; --sum) {
            f_new[sum] = f_new[sum] + f_new[sum - part];
        }
    }

    /* Compute a(n) = Sum_{d|n} A000700(d) by Dirichlet convolution. */
    for (idx_t d = 1; d <= N; ++d) {
        if (f_new[d] != (seq_t)0) {
            for (idx_t m = d; m <= N; m += d) {
                a_new[m] = a_new[m] + f_new[d];
            }
        }
    }

    /* Free previous arrays */
#if defined(__cplusplus)
    delete[] f_arr;
    delete[] a_arr;
#else
    free(f_arr);
    free(a_arr);
#endif

    f_arr = f_new;
    a_arr = a_new;
    max_computed = N;
}

/* External function required by the problem. */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    ensure_precomputations(n);
    return a_arr[n];
}
