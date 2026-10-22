/*
 * A357926  a(n) = Sum_{k=0..floor(n/4)} Stirling2(n - 3*k,n - 4*k).
 * https://oeis.org/A357926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} Stirling2(n - 3*k, n - 4*k)
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 36, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* cache for computed values */
static seq_t *a_cache = NULL;
static idx_t cache_max = 0;

/* compute a(n) for 0 <= n <= max_n and store in a_cache */
static void compute_a_up_to(idx_t max_n)
{
    /* allocate arrays */
    seq_t *new_a = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));
    seq_t *prev   = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));
    seq_t *curr   = (seq_t *)malloc((size_t)(max_n + 1) * sizeof(seq_t));

    if (!new_a || !prev || !curr) {
        /* allocation failure – abort (the test harness guarantees enough memory) */
        exit(1);
    }

    /* initialize a array to 0 */
    for (idx_t i = 0; i <= max_n; ++i) {
        new_a[i] = 0;
    }

    /* k = 0  ->  S_0[N] == 1 for all N */
    for (idx_t N = 0; N <= max_n; ++N) {
        prev[N] = 1;          /* S_0[N] */
        new_a[N] += 1;        /* contribution to a[N] */
    }

    /* process k = 1 .. floor(max_n/4) */
    idx_t max_k = max_n / 4;
    for (idx_t k = 1; k <= max_k; ++k) {
        /* curr[k] = S_k[k] = 0 (since Stirling(N,0)=0 for N>0) */
        curr[k] = 0;

        for (idx_t N = k + 1; N <= max_n; ++N) {
            /* S_k[N] = (N-k)*S_{k-1}[N-1] + S_k[N-1] */
            seq_t term = prev[N - 1] * (seq_t)(N - k);
            curr[N] = curr[N - 1] + term;

            /* add contribution to a[n] with n = N + 3*k */
            idx_t n_val = N + 3 * k;
            if (n_val <= max_n) {
                new_a[n_val] += curr[N];
            }
        }

        /* swap prev and curr for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    /* free temporary arrays not part of cache */
    free(curr);
    free(prev);

    /* replace old cache */
    if (a_cache != NULL) {
        free(a_cache);
    }
    a_cache = new_a;
    cache_max = max_n;
}

seq_t a(idx_t n)
{
    if (a_cache == NULL || n > cache_max) {
        compute_a_up_to(n);
    }
    return a_cache[n];
}
