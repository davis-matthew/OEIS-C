/*
 * A381572  G.f. A(x) satisfies A(x) = 1/(1 - x*A(x*A(x)))^2.
 * https://oeis.org/A381572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = 1/(1 - x*A(x*A(x)))^2 (OEIS A381572)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   poly_compose, poly_mul_trunc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/poly_compose.h"
#include "../utils/poly_mul_trunc.h"

/* Main sequence generator */
seq_t a(idx_t n)
{
    /* Static cache */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;

    if (n < cache_size) return cache[n];

    idx_t N = n;
    /* Allocate working arrays */
    seq_t *old    = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *new_arr = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *g     = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *f_of_g = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *u      = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *power  = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *temp   = (seq_t *)calloc(N + 1, sizeof(seq_t));

    old[0] = 1;   /* a(0) = 1 */

    bool finished = false;
    while (!finished)
    {
        /* g(x) = x * a(x)  */
        g[0] = 0;
        for (idx_t i = 0; i < N; ++i) g[i + 1] = old[i];

        /* f_of_g = a(g(x)) */
        poly_compose(old, g, f_of_g, N);

        /* u(x) = x * f_of_g(x) */
        u[0] = 0;
        for (idx_t i = 0; i < N; ++i) u[i + 1] = f_of_g[i];

        /* new_arr = (1 - u)^{-2}  */
        for (idx_t i = 0; i <= N; ++i) new_arr[i] = 0;
        for (idx_t i = 0; i <= N; ++i) power[i] = 0;
        power[0] = 1;  /* power = u^0 */

        for (idx_t k = 0; k <= N; ++k)
        {
            seq_t kfat = (seq_t)(k + 1);  /* multiplicative factor k+1 */
            for (idx_t i = 0; i <= N; ++i)
            {
                if (!power[i]) continue;
                new_arr[i] += kfat * power[i];
            }
            if (k == N) break;

            /* power = power * u (truncated) */
            for (idx_t i = 0; i <= N; ++i) temp[i] = 0;
            for (idx_t i = 0; i <= N; ++i)
            {
                if (!power[i]) continue;
                for (idx_t j = 0; j <= N - i; ++j)
                {
                    if (!u[j]) continue;
                    temp[i + j] += power[i] * u[j];
                }
            }
            for (idx_t i = 0; i <= N; ++i) power[i] = temp[i];
        }

        /* Check convergence */
        finished = true;
        for (idx_t i = 0; i <= N; ++i)
        {
            if (new_arr[i] != old[i])
            {
                finished = false;
                break;
            }
        }
        if (finished) break;

        /* Prepare for next iteration */
        seq_t *tmp_ptr = old;
        old = new_arr;
        new_arr = tmp_ptr;
    }

    /* Store result in cache */
    if (cache) free(cache);
    cache = old;
    cache_size = N + 1;

    /* Clean up */
    free(new_arr); free(g); free(f_of_g); free(u); free(power); free(temp);

    return cache[n];
}
