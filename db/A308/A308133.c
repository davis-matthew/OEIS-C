/*
 * A308133  Sum of the smallest sides of all integer-sided triangles with
 *          squarefree sides and perimeter n.
 * https://oeis.org/A308133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n/3⌋} Σ_{i=k..⌊(n-k)/2⌋}
 *            sign(floor((i+k)/(n-i-k+1))) * μ(i)² * μ(k)² * μ(n-i-k)² * k (μ
 *            is the Möbius function)
 * Range:     u8 n <= 62, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>


static int8_t *mu_cache = NULL;
static idx_t   mu_cache_max = 0;

static void ensure_mu(idx_t n)
{
    if (n <= mu_cache_max)
        return;
    size_t N = (size_t)n;

    int          *cnt     = (int*)calloc(N + 1, sizeof(int));
    unsigned char *sqfree = (unsigned char*)malloc(N + 1);
    if (!cnt || !sqfree) {
        free(cnt);  free(sqfree);
        return;
    }

    for (size_t i = 0; i <= N; ++i)
        sqfree[i] = 1;

    for (size_t p = 2; p <= N; ++p) {
        if (cnt[p] == 0) {                 /* p is prime */
            for (size_t j = p; j <= N; j += p)
                ++cnt[j];
            unsigned long long sq = (unsigned long long)p * p;
            if (sq <= N) {
                for (size_t j = (size_t)sq; j <= N; j += (size_t)sq)
                    sqfree[j] = 0;
            }
        }
    }

    int8_t *new_mu = (int8_t*)realloc(mu_cache,
                                      (N + 1) * sizeof(int8_t));
    if (!new_mu) {
        new_mu = (int8_t*)malloc((N + 1) * sizeof(int8_t));
    }
    mu_cache = new_mu;

    mu_cache[0] = 0;
    mu_cache[1] = 1;
    for (size_t i = 2; i <= N; ++i) {
        if (!sqfree[i])
            mu_cache[i] = 0;  /* not square‑free */
        else
            mu_cache[i] = (cnt[i] % 2 == 0) ? 1 : -1;
    }

    free(cnt);
    free(sqfree);
    mu_cache_max = n;
}

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* all terms below 3 are 0 */

    ensure_mu(n);
    seq_t sum = 0;

    idx_t k_limit = n / 3;
    for (idx_t k = 1; k <= k_limit; ++k) {
        int8_t mu_k = mu_cache[k];
        if (mu_k == 0)
            continue;                /* not square‑free */

        idx_t i_limit = (n - k) / 2;
        for (idx_t i = k; i <= i_limit; ++i) {
            int8_t mu_i = mu_cache[i];
            if (mu_i == 0)
                continue;

            idx_t j = n - i - k;     /* third side */
            int8_t mu_j = mu_cache[j];
            if (mu_j == 0)
                continue;

            /* triangle inequality: i + k >= (n+1)/2 */
            if (((i + k) * 2) < (n + 1))
                continue;

            sum += (seq_t)k;
        }
    }
    return sum;
}
